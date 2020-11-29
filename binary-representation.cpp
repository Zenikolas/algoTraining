#include <sstream>
#include <string>

#include <gtest/gtest.h>

std::string binaryRepresentation(int number) {
    constexpr uint8_t BITS_IN_BYTE = 8;
    constexpr size_t bitsInByte = sizeof(number) * BITS_IN_BYTE;

    std::stringstream binaryReprStream;
    // Can be changed to iterate from the highest byte, but I decided this version
    // looks neater
    for (size_t i = 0; i < bitsInByte; ++i) {
        binaryReprStream << (number & 1);
        number >>= 1;
    }

    auto ret = binaryReprStream.str();
    return std::string(ret.rbegin(), ret.rend());
}

TEST(BinaryRepresentationTest, SmokeTest) {
    ASSERT_EQ(binaryRepresentation(-7), std::string("11111111111111111111111111111001")) << std::endl;
}
