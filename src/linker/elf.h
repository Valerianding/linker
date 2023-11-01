//
// Created by Valerian on 2023/9/28.
//

#ifndef LINKER_ELF_H
#define LINKER_ELF_H
#include <cstdint>
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <elf.h>
typedef struct {
    uint8_t Ident[16];
    uint16_t Type;
    uint16_t Machine;
    uint32_t Version;
    uint64_t Entry; //entry point address
    uint64_t PhOff; //start of program headers
    uint64_t ShOff; //start of section headers(Offset)
    uint32_t Flags; //
    uint16_t EhSize;
    uint16_t PhEntSize; //size of program headers
    uint16_t PhNum;
    uint16_t ShEntSize;
    uint16_t ShNum; //number of section headers
    uint16_t ShStrndx; // section string table index
}Ehdr;

const int EhdrSize = sizeof(Ehdr);

typedef struct Shdr {
    uint32_t Name; // section name(string index in .shstrtab)
    uint32_t Type; // section type
    uint64_t Flags;
    uint64_t Addr;
    uint64_t Offset; // section offset
    uint64_t Size; // section size
    uint32_t Link;
    uint32_t Info;
    uint64_t AddrAlign;
    uint64_t EntSize; // section entry size
}Shdr;

const int ShdrSize = sizeof(Shdr);

typedef struct Sym{
    uint32_t Name; //string table offset
    uint8_t Info; //
    uint8_t Other; // Symbol visibility
    uint16_t Shndx; // Section index
    uint64_t Value; // Section offset or absolute address
    uint64_t Size; // Object size in bytes

    //符号是不是绝对符号
    bool IsAbs() const{
        return Shndx == SHN_ABS;
    }

    bool IsUndef() const{
        return Shndx == SHN_UNDEF;
    }

    bool IsCommon() const{
        return Shndx == SHN_COMMON;
    }
}Sym;

const int SymSize = sizeof(Sym);


std::string ElfGetName(std::vector<uint8_t>& strTab, uint32_t offset);

#endif //LINKER_ELF_H
