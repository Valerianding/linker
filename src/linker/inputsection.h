//
// Created by Valerian on 2023/10/18.
//

#ifndef LINKER_INPUTSECTION_H
#define LINKER_INPUTSECTION_H

#include <iostream>
#include <vector>
#include "linker/objectfile.h"
#include "linker/elf.h"
class InputFile;
class ObjectFile;

class InputSection{
public:
    std::vector<uint8_t> contents;
    ObjectFile* objectFile;
    uint32_t Shndx;
    uint32_t  ShSize; // section header -> size
    bool IsAlive = true;
    uint8_t P2Align; // power of 2

    InputSection() = default;

    InputSection(ObjectFile *file, uint32_t shndx);

    Shdr* GetShdr();

    std::string Name();
};
#endif //LINKER_INPUTSECTION_H
