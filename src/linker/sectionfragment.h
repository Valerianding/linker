//
// Created by Valerian on 2023/10/30.
//

#ifndef LINKER_SECTIONFRAGMENT_H
#define LINKER_SECTIONFRAGMENT_H
#include <iostream>
#include "linker/mergedsection.h"
class MergedSection;
//MergedSection has many SectionFragment
class SectionFragment{
public:
    MergedSection* outputSection;
    uint32_t Offset = UINT32_MAX;
    uint32_t  P2Align;
    bool IsAlive;
    SectionFragment(MergedSection* mergedSection) : outputSection(mergedSection){}
};
#endif //LINKER_SECTIONFRAGMENT_H
