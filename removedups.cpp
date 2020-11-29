#include <gtest/gtest.h>

void removeDups(char* str) {
    const size_t size = strlen(str);
    if (size < 2) {
        return;
    }

    size_t writeIdx = 0, currentIdx = 0, nextIdx = 1;
    while (nextIdx < size) {
        if (str[currentIdx] == str[nextIdx]) {
            ++nextIdx;
        } else {
            ++writeIdx;
            str[writeIdx] = str[nextIdx];
            currentIdx = nextIdx++;
        }
    }

    if (writeIdx < size) {
        ++writeIdx;
        str[writeIdx] = '\0';
    }
}

TEST(RemoveDupsTest, smokeTest) {
    char str[] = "aaabc   aa";
    char expectedStr[] = "abc a";
    removeDups(str);

    ASSERT_STREQ(str, expectedStr);
}