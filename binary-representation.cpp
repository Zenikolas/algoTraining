#include <sstream>
#include <string>

#include <gtest/gtest.h>

std::string binaryRepresentation(int number) {
    constexpr uint8_t BITS_IN_BYTE = 8;
    constexpr size_t bitsInByte = sizeof(number) * BITS_IN_BYTE;
    unsigned int unumber = number;

    std::stringstream binaryReprStream;
    for (size_t i = 0; i < bitsInByte; ++i) {
        binaryReprStream << (unumber & 1);
        unumber >>= 1;
    }

    auto ret = binaryReprStream.str();

    size_t lastNonNullSymbol = ret.find_last_not_of('0');
    size_t rightBorder = 0;
    if (lastNonNullSymbol != std::string::npos) {
        rightBorder = ret.size() - lastNonNullSymbol - 1;
    }
    return std::string(ret.rbegin() + rightBorder, ret.rend());
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
