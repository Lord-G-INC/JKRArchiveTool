#pragma once

#include <array>
#include <algorithm>
#include <string>
#include "types.h"
#include <fstream>
#include <vector>
#include <iostream>

// MSVC/Clang checks
#ifdef __GNUC__
#include <strings.h>
#elif _MSC_VER
#include <string.h>
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

namespace File {
    void writeAllBytes(const std::string &, const u8*, u32);
    u8* readAllBytes(const std::string &, u32*);

    bool FileExists(const std::string &filePath);
};

namespace Util {
    template<typename T>
    s32 getVectorIndex(std::vector<T> vector, T val) {
        auto iter = find(vector.begin(), vector.end(), val);
 
        if (iter != vector.end()) {  
            s32 index = iter - vector.begin();
            return index;
        }
        else {
            return -1;
        }
    }
    template<typename T>
    T align32(T val) {
        return (val + 0x1F) & ~0x1F;
    }
};