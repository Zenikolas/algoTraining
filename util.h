#pragma once

#include <utility>

template<typename T>
void htonT(T* input) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    char* ptr = reinterpret_cast<char*>(input);

    for (std::size_t i = 0; i < sizeof(T) / 2; ++i) {
        std::swap(ptr[i], ptr[sizeof(T) - 1 - i]);
    }
#endif
}

