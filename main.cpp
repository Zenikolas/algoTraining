#include <iostream>
#include <cstring>
#include <sstream>
#include <algorithm>

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

int main() {
//    char str[] = "aaabc   aa";
//    std::cout << std::string(str) << std::endl;
//    removeDups(str);
//    std::cout << std::string(str) << std::endl;

    std::cout << binaryRepresentation(-7) << std::endl;

    return 0;
}
