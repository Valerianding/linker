//
// Created by Valerian on 2023/10/7.
//

#include "magic.h"
bool CheckMagic(std::vector<uint8_t>& contents){
    const char* magic = "\x7F""ELF";
    const size_t magicSize = 4;
    if(contents.size() < magicSize){
        return false;
    }
    return std::memcmp(contents.data(),magic,magicSize) == 0;
}

bool CheckArchive(std::vector<uint8_t>& contents){
    const char* format = "!<arch>\n";
    const size_t length = strlen(format);
    if(contents.size() < length){
        return false;
    }
    return std::memcmp(contents.data(),format,length) == 0;
}