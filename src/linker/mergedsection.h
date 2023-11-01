//
// Created by Valerian on 2023/10/30.
//

#ifndef LINKER_MERGEDSECTION_H
#define LINKER_MERGEDSECTION_H
#include <unordered_map>
#include <elf.h>
#include "linker/context.h"
#include "linker/chunk.h"
#include "linker/output.h"
#include "linker/sectionfragment.h"

class SectionFragment;
//write to the ELF
class MergedSection : public Chunk{
public:
    std::unordered_map<std::string,SectionFragment*> map;
    MergedSection() = default;
    MergedSection(const std::string& name, uint64_t flags, uint32_t type) : Chunk(name){
        shdr->Flags = flags;
        shdr->Type = type;
    }

    SectionFragment* Insert(const std::string& key, uint32_t p2align);
};
MergedSection* GetMergedSectionInstance(Context& ctx, const std::string& name, uint32_t type, uint64_t flags);
#endif //LINKER_MERGEDSECTION_H
