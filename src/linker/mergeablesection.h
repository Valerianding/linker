//
// Created by Valerian on 2023/10/31.
//

#ifndef LINKER_MERGEABLESECTION_H
#define LINKER_MERGEABLESECTION_H
#include <iostream>
#include <algorithm>
#include <functional>
#include "linker/mergedsection.h"
class MergedSection;

class MergeableSection{
public:
    MergedSection *parent;
    uint8_t p2Align;
    std::vector<std::string> strs;
    std::vector<uint32_t> fragOffsets;
    std::vector<SectionFragment*>fragments;
    std::pair<SectionFragment*,uint32_t> GetFragment(uint32_t offset);
};
#endif //LINKER_MERGEABLESECTION_H
