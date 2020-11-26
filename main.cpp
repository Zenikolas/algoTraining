#include <iostream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <unordered_map>

void removeDups(char* str) {
    const size_t N = strlen(str);
    if (N < 2) {
        return;
    }

    size_t w = 0, c = 0, n = 1;
    while (n < N) {
        if (str[c] == str[n]) {
            ++n;
        } else {
            ++w;
            str[w] = str[n];
            c = n++;
        }
    }

    if (w < N) {
        ++w;
        str[w] = '\0';
    }
}

std::string binaryRepresentation(int number) {
    constexpr uint8_t BITS_IN_BYTE = 8;
    constexpr size_t bitsInByte = sizeof(number) * BITS_IN_BYTE;

    std::stringstream binaryReprStream;
    for (size_t i = 0; i < bitsInByte; ++i) {
        binaryReprStream << (number & 1);
        number >>= 1;
    }

    auto ret = binaryReprStream.str();
    return std::string(ret.rbegin(), ret.rend());
}

// структуру ListNode модифицровать нельзя
struct ListNode {
    ListNode *      prev;
    ListNode *      next;
    ListNode *      rand; // указатель на произвольный элемент данного списка, либо NULL
    std::string     data;
};

class List {
public:
    void Serialize   (FILE * file);  // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
    void Deserialize (FILE * file);  // загрузка из файла (файл открыт с помощью fopen(path, "rb"))

private:
    ListNode *      head;
    ListNode *      tail;
    int       count;
};

void List::Serialize(FILE* file) {

    std::unordered_map<ListNode*, size_t> nodeIndexes;
    ListNode* currentNode = head;
    while (currentNode) {
        auto idxIter = nodeIndexes.find(currentNode);
        if (idxIter == nodeIndexes.end()) {
            nodeIndexes.
        }

        currentNode = currentNode->next;
    }
}

int main() {
//    char str[] = "aaabc   aa";
//    std::cout << std::string(str) << std::endl;
//    removeDups(str);
//    std::cout << std::string(str) << std::endl;

    std::cout << binaryRepresentation(-7) << std::endl;

    return 0;
}
