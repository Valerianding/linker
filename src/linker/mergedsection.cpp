//
// Created by Valerian on 2023/10/30.
//

#include "mergedsection.h"
MergedSection* GetMergedSectionInstance(Context& ctx, const std::string& name, uint32_t type, uint64_t flags){
    std::string output = GetOutputName(name,flags);
//    flags = flags & !SHF_GROUP & !SHF_MERGE & !SHF_STRINGS & !SHF_COMPRESSED;

    auto find = [&]() -> MergedSection*{
        for(auto& osec :ctx.mergedSection){
            if(output == osec->name && flags == osec->shdr->Flags && type == osec->shdr->Type){
                //find the section
                return osec;
            }
            return nullptr;
        }
    };

    auto osec = find();
    if(osec != nullptr){
       return osec;
    }
    //else
    osec = new MergedSection(name,flags,type);
    ctx.mergedSection.push_back(osec);
}


SectionFragment* MergedSection::Insert(const std::string& key, uint32_t p2align){
    SectionFragment* frag = nullptr;
    if(!map.count(key)){
        frag = new SectionFragment(this);
        map[key] = frag;
    }

    if(frag->P2Align < p2align){
        frag->P2Align = p2align;
    }
    return frag;
}