//
// Created by Valerian on 2023/9/28.
//

#ifndef LINKER_INPUTFILE_H
#define LINKER_INPUTFILE_H
#include <vector>
#include <elf.h>
#include "linker/elf.h"
#include "linker/file.h"
#include "linker/magic.h"
#include "utils/utils.h"
class File;
class Symbol;

class InputFile{
public:
    File *file;
    std::vector<Shdr> ElfSections;
    std::vector<Sym> ElfSyms;
    uint64_t FirstGlobal = 0; //the position of the first global symbol
    std::vector<uint8_t> ShStrtab;
    std::vector<uint8_t> SymbolStrtab;
    bool IsAlive;
    std::vector<Symbol*> Symbols; // can be context symbol also global symbols -> all the symbols
    std::vector<Symbol*> LocalSymbols; // all the local symbols except for the global symbols
    // why this is not Symbol*
    // -> cause local Symbol only used in this file -> let's make it ptr to be nice
public:
    InputFile() = default;
    InputFile(File *file) : file(file){}
    std::vector<uint8_t> GetBytesFromShdr(Shdr& shdr);
    std::vector<uint8_t> GetBytesFromIdx(uint64_t index);

    //find the section according to type
    Shdr *FindSection(uint32_t type);

    //fill the symbol table
    void FillUpElfSyms(Shdr *shdr);
};

InputFile *NewInputFile(File *file);

#endif //LINKER_INPUTFILE_H
