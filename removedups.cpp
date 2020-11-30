#include <gtest/gtest.h>

void removeDups(char* str) {
    const size_t size = strlen(str);
    if (size < 2) {
        return;
    }

    size_t resultIdx = 0, currentIdx = 0, nextIdx = 1;
    while (nextIdx < size) {
        if (str[currentIdx] == str[nextIdx]) {
            ++nextIdx;
        } else {
            ++resultIdx;
            str[resultIdx] = str[nextIdx];
            currentIdx = nextIdx++;
        }
    }

    if (resultIdx < size) {
        ++resultIdx;
        str[resultIdx] = '\0';
    }
}

TEST(RemoveDupsTest, smokeTest) {
    char str[] = "aaabc   aa";
    char expectedStr[] = "abc a";
    removeDups(str);

    ASSERT_STREQ(str, expectedStr);
}

TEST(RemoveDupsTest, emptyTest) {
    char str[] = "";
    char expectedStr[] = "";
    removeDups(str);

    ASSERT_STREQ(str, expectedStr);
}

TEST(RemoveDupsTest, oneLetterTest) {
    char str[] = "a";
    char expectedStr[] = "a";
    removeDups(str);

    ASSERT_STREQ(str, expectedStr);
}