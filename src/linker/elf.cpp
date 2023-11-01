//
// Created by Valerian on 2023/9/28.
//

#include "elf.h"
std::string ElfGetName(std::vector<uint8_t>& strTab, uint32_t offset){
    if(strTab.empty()) return "";
    auto itr = std::find(strTab.begin() + offset, strTab.end(), 0);
    uint32_t length = static_cast<uint32_t>(std::distance(strTab.begin() + offset, itr));
    return {strTab.begin() + offset, strTab.begin() + offset + length};
}
