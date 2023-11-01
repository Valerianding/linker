//
// Created by Valerian on 2023/10/18.
//

#include "inputsection.h"
Shdr* InputSection::GetShdr(){
    Assert(Shndx < objectFile->inputFile->ElfSections.size());
    return &objectFile->inputFile->ElfSections[Shndx];
}

InputSection::InputSection(ObjectFile *file, uint32_t shndx) : objectFile(file), Shndx(shndx){

    //section header
    struct Shdr *shdr = GetShdr();
    contents = std::vector<uint8_t>(file->inputFile->file->contents.begin()
                                    + shdr->Offset, file->inputFile->file->contents.begin() + shdr->Offset + shdr->Size);
    Assert((shdr->Flags & SHF_COMPRESSED) == 0);
    ShSize = shdr->Size;


    //1 2, 4, 8, 16, 32
    auto toP2Align = [](uint64_t align) -> uint8_t {
        if(align == 0) return 0;
        int count = 0;
        while((align & 1) == 0){
            align >>= 1;
            count++;
        }
        return count;
    };

    P2Align = toP2Align(shdr->AddrAlign);
}

std::string InputSection::Name(){
    return ElfGetName(objectFile->inputFile->ShStrtab,GetShdr()->Name);
}


