#include <sstream>
#include <string>

#include <gtest/gtest.h>

std::string binaryRepresentation(int number) {
    if (number == 0) {
        return "0";
    }
    constexpr uint8_t BITS_IN_BYTE = 8;
    size_t bitsInByte = sizeof(number) * BITS_IN_BYTE;
    unsigned int unumber = number;

#if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
    if (number > 0) {
        bitsInByte -= __builtin_clz(unumber);
    }
#elif defined(_MSC_VER)
    unsigned long leadingZero = 0;
    if (number > 0 && _BitScanReverse(&leadingZero, unumber)) {
        bitsInByte = leadingZero + 1;
    }
#endif

    std::string ret;
    unsigned int mask = 1u << (bitsInByte - 1);
    for (size_t i = 0; i < bitsInByte; ++i) {
        ret.push_back(((unumber & mask) > 0 ? '1' : '0'));
        mask >>= 1;
    }

#if !defined(__GNUC__) && !defined(__GNUG__) && !defined(__clang__) && !defined(_MSC_VER)
    size_t firstNonNullSymbol = ret.find_first_not_of('0');
    if (firstNonNullSymbol != std::string::npos) {
        return ret.substr(firstNonNullSymbol);
    }
#endif
    return ret;
}

TEST(BinaryRepresentationTest, NegativeTest) {
    ASSERT_EQ(binaryRepresentation(-7), std::string("11111111111111111111111111111001"))
                                << std::endl;
}

TEST(BinaryRepresentationTest, PositiveTest) {
    ASSERT_EQ(binaryRepresentation(7), std::string("111"))
                                << std::endl;
}


TEST(BinaryRepresentationTest, MaxIntTest) {
    ASSERT_EQ(binaryRepresentation(INT32_MAX),
              std::string("1111111111111111111111111111111"))
                                << std::endl;
}

TEST(BinaryRepresentationTest, MinIntTest) {
    ASSERT_EQ(binaryRepresentation(INT32_MIN), std::string
            ("10000000000000000000000000000000"))
                                << std::endl;
}
