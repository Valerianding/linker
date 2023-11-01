//
// Created by Valerian on 2023/10/31.
//

#include "mergeablesection.h"

//why is this happens?
std::pair<SectionFragment*,uint32_t> MergeableSection::GetFragment(uint32_t offset){
    auto itr = std::lower_bound(fragOffsets.begin(), fragOffsets.end(), offset);
    int pos = std::distance(fragOffsets.begin(), itr);
    if(pos == 0){
        return {nullptr,0};
    }

    //
    return {fragments[pos - 1], offset - fragOffsets[pos - 1]};
}