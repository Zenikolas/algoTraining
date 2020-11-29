#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <netinet/in.h>
#include <assert.h>
#include <vector>

#include "util.h"

#include <gtest/gtest.h>

void write(FILE* file, const void* value, size_t size) {
    const std::string errorPrefix = "Failed to write data: ";
    if (!file) {
        throw std::runtime_error(errorPrefix + "bad file descriptor");
    }

    if (fwrite(value, size, 1, file) != 1) {
        throw std::runtime_error(errorPrefix + "couldn't write requested bytes of data");
    }
}

void write(FILE* file, uint8_t value) {
    write(file, &value, sizeof(value));
}

void write(FILE* file, size_t value) {
    htonT(&value);
    write(file, &value, sizeof(value));
}

void write(FILE* file, std::string_view value) {
    size_t desSize = value.size();
    write(file, desSize);
    write(file, value.data(), value.size());
}

void read(FILE* file, void* value, size_t size) {
    const std::string errorPrefix = "Failed to read data: ";
    if (!file) {
        throw std::runtime_error(errorPrefix + "bad file descriptor");
    }

    if (fread(value, size, 1, file) != 1) {
        throw std::runtime_error(errorPrefix + "couldn't read requested bytes of data");
    }
}

void read(FILE* file, uint8_t& value) {
    read(file, &value, sizeof(value));
}

void read(FILE* file, size_t& value) {
    read(file, &value, sizeof(value));
    htonT(&value);
}

void read(FILE* file, std::string& value) {
    size_t size;
    read(file, size);
    value.resize(size);
    read(file, value.data(), size);
}

// структуру ListNode модифицровать нельзя
struct ListNode {
    ListNode* prev;
    ListNode* next;
    ListNode* rand; // указатель на произвольный элемент данного списка, либо NULL
    std::string data;
};

class List {
public:
    List() : m_head(nullptr), m_tail(nullptr), m_count(0) {}

    ~List() { destroy(); }

    size_t size() const { return m_count; }

    void pushFront(std::string_view data = "", ListNode* rand = nullptr);

    void pushBack(std::string_view data = "", ListNode* rand = nullptr);

    ListNode* getNElement(size_t idx);

    const ListNode* getNElement(size_t idx) const;

    void serialize(FILE* file);

    void deserialize(FILE* file);

    void destroy();

private:
    ListNode* m_head;
    ListNode* m_tail;
    size_t m_count;

    bool initFirst(std::string_view data, ListNode* rand);

    enum RandPresence : uint8_t {
        NULL_VALUE = 0,
        NOT_NULL_VALUE = 1
    };

    using RandNodePresence = std::tuple<RandPresence, size_t, ListNode*>;

    RandNodePresence getRandNodePresence(FILE* file);
};

void List::pushFront(std::string_view data, ListNode* rand) {
    if (initFirst(data, rand)) {
        return;
    }

    m_head->prev = new ListNode();
    m_head->prev->next = m_head;
    m_head->prev->prev = nullptr;
    m_head = m_head->prev;
    m_head->data = data;
    m_head->rand = rand;

    ++m_count;
}

void List::pushBack(std::string_view data, ListNode* rand) {
    if (initFirst(data, rand)) {
        return;
    }

    m_tail->next = new ListNode();
    m_tail->next->prev = m_tail;
    m_tail->next->next = nullptr;
    m_tail = m_tail->next;
    m_tail->data = data;
    m_tail->rand = rand;
    ++m_count;
}

ListNode* List::getNElement(size_t idx) {
    if (!m_head) {
        assert(m_tail && "TAIL MUST BE NULL DUE TO HEAD IS NULL");
        assert(m_count == 0);
        return nullptr;
    }

    if (idx >= m_count) {
        throw std::runtime_error("Bad idx to retrieve");
    }


    const size_t lenghtFromTail = m_count - idx;
    const size_t lengthHalf = m_count / 2;
    if (lenghtFromTail <= lengthHalf) {
        size_t i = 0;
        ListNode* current = m_tail;
        idx = lenghtFromTail - 1;
        while (i != idx) {
            ++i;
            current = current->prev;
        }

        return current;
    }

    size_t i = 0;
    ListNode* current = m_head;
    while (i != idx) {
        ++i;
        current = current->next;
    }

    return current;
}

const ListNode* List::getNElement(size_t idx) const {
    return const_cast<List*>(this)->getNElement(idx);
}

void List::serialize(FILE* file) {
    const std::string errorPrefix = "Failed to serialise list: ";
    std::unordered_map<ListNode*, size_t> nodeIndexes;
    size_t idx = 0;
    ListNode* currentNode = m_head;
    while (currentNode) {
        nodeIndexes.emplace(currentNode, idx++);
        currentNode = currentNode->next;
    }

    write(file, m_count);
    currentNode = m_head;
    while (currentNode) {
        if (!currentNode->rand) {
            write(file, static_cast<uint8_t>(NULL_VALUE));
        } else {
            auto idxIter = nodeIndexes.find(currentNode->rand);
            if (idxIter == nodeIndexes.end()) {
                throw std::runtime_error(errorPrefix + "failed to find rand node");
            }

            write(file, static_cast<uint8_t>(NOT_NULL_VALUE));
            write(file, idxIter->second);
        }

        write(file, currentNode->data);
        currentNode = currentNode->next;
    }
}

void List::deserialize(FILE* file) {
    destroy();

    size_t listSize{};
    read(file, listSize);
    std::vector<RandNodePresence> randNodeIndexes;
    randNodeIndexes.reserve(listSize);

    for (size_t i = 0; i < listSize; ++i) {
        randNodeIndexes.emplace_back(getRandNodePresence(file));
    }

    ListNode* current = m_head;
    for (auto& node : randNodeIndexes) {
        RandPresence randPresence = std::get<0>(node);
        size_t randIdx = std::get<1>(node);
        if (randPresence != NULL_VALUE && randIdx < listSize) {
            current->rand = std::get<2>(randNodeIndexes[randIdx]);
        }

        current = current->next;
    }

    m_count = listSize;
}

void List::destroy() {
    if (!m_head) {
        assert(!m_tail && "TAIL MUST BE NULL DUE TO HEAD IS NULL");
        assert(m_count == 0);
        return;
    }

    ListNode* current = m_head;
    while (true) {
        if (!current->next) {
            delete current;
            break;
        }

        current = current->next;
        delete current->prev;
    }

    m_head = m_tail = nullptr;
    m_count = 0;
}

bool List::initFirst(std::string_view data, ListNode* rand) {
    if (m_head) {
        return false;
    }

    assert(!m_tail && "TAIL MUST BE NULL DUE TO HEAD IS NULL");
    assert(m_count == 0);

    m_head = new ListNode();
    m_head->prev = nullptr;
    m_head->next = nullptr;
    m_head->rand = rand;
    m_head->data = data;
    m_tail = m_head;
    ++m_count;

    return true;
}

List::RandNodePresence List::getRandNodePresence(FILE* file) {
    pushBack();
    uint8_t randPresenceInt{};
    read(file, randPresenceInt);
    auto randPresence = static_cast<RandPresence>(randPresenceInt);
    size_t idx{};
    if (randPresence != NULL_VALUE) {
        read(file, idx);
    }

    read(file, m_tail->data);
    return {randPresence, idx, m_tail};
}

bool operator==(const List& lhs, const List& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    const ListNode* lhsCurrent = lhs.getNElement(0);
    const ListNode* rhsCurrent = rhs.getNElement(0);
    while (lhsCurrent) {
        if (lhsCurrent->data != rhsCurrent->data) {
            return false;
        }
        lhsCurrent = lhsCurrent->next;
        rhsCurrent = rhsCurrent->next;
    }

    return !rhsCurrent;
}


List createList() {
    List list;

    list.pushBack("element_1");
    list.pushBack("element_2");
    list.pushBack("element_3");

    list.pushFront("element_0", list.getNElement(2));
    list.pushBack("element_4", list.getNElement(1));
    list.pushBack("element_5", list.getNElement(2));

    return list;
}

void printList(const List& list) {
    const ListNode* current = list.getNElement(0);
    std::cout << "+++++++++++++++++++++" << std::endl;
    while (current) {
        std::cout << current->data << " -> ";
        std::cout << (current->rand ? current->rand->data : "NULL") << std::endl;
        current = current->next;
    }
    std::cout << "+++++++++++++++++++++" << std::endl;
}

class RAIIFile {
    FILE* m_file;
    std::string m_filenameToRemove;
public:
    RAIIFile() = default;

    RAIIFile(FILE* file, std::string_view fileNameToRemove = "")
    : m_file(file)
    , m_filenameToRemove(fileNameToRemove) {}

    ~RAIIFile() {
        if (!m_file) {
            return;
        }
        fclose(m_file);

        if (!m_filenameToRemove.empty()) {
            remove(m_filenameToRemove.c_str());
        }
    }

    FILE* get() { return m_file; }

    const FILE* get() const { return m_file; }
};

TEST(ListSerializationTest, SmokeTest) {
    std::string_view filename = tmpnam(nullptr);;
    List list = createList();
    List deserializedList;

    ASSERT_NO_THROW({
        {
            RAIIFile writeFile(fopen(filename.data(), "wb"));
            ASSERT_TRUE(writeFile.get());
            printList(list);
            list.serialize(writeFile.get());
        }

        {
            RAIIFile readFile(fopen(filename.data(), "rb"), filename.data());
            ASSERT_TRUE(readFile.get());

            deserializedList.deserialize(readFile.get());
            printList(deserializedList);
        }
    });

    ASSERT_EQ(list, deserializedList);
}
