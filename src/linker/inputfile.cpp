//
// Created by Valerian on 2023/9/28.
//

#include "inputfile.h"
InputFile *NewInputFile(File *file){
    auto f = new InputFile(file);
    if(file->contents.size() < EhdrSize){
        Fatal("file too small");
    }

    if(!CheckMagic(file->contents)){
        Fatal("Not a ELF file");
    }

    std::vector<uint8_t>& fileContents = file->contents;

    //ELF Header
    Ehdr ehdr = Read<Ehdr>(file->contents);

    //section headers
    std::vector<uint8_t> contents(fileContents.begin() + ehdr.ShOff,fileContents.end());

    //The first section header
    Shdr shdr = Read<Shdr>(contents);

    uint64_t numSections = ehdr.ShNum;


    if(numSections == 0){
        numSections = shdr.Size;
    }

    f->ElfSections.push_back(shdr);
    while(numSections > 1){
        contents = std::vector<uint8_t>(contents.begin() + ShdrSize,contents.end());;
        f->ElfSections.push_back(Read<Shdr>(contents));
        numSections--;
    }

    //find
    uint16_t shstrndx = ehdr.ShStrndx; //notice that this is 16-bit
    if(ehdr.ShStrndx == SHN_XINDEX){
        //if index is in extra table
        shstrndx = shdr.Link;
    }

    //find the section header
    Shdr s = f->ElfSections[shstrndx];
    f->ShStrtab = std::vector<uint8_t>(fileContents.begin() + s.Offset, fileContents.begin() + s.Offset + s.Size);
    return f;
}

std::vector<uint8_t> InputFile::GetBytesFromShdr(Shdr& shdr){
    uint64_t end = shdr.Offset + shdr.Size;

    if(end > file->contents.size()){
        Fatal("Content too small");
    }
    return {file->contents.begin() + shdr.Offset, file->contents.begin()+ end};
}

std::vector<uint8_t> InputFile::GetBytesFromIdx(uint64_t index) {
    //make sure we have this ELF Section
    if(index >= ElfSections.size()){
        Fatal("index out of range");
    }
    //get the section header
    Shdr &shdr = ElfSections[index];
    return GetBytesFromShdr(shdr);
}

Shdr *InputFile::FindSection(uint32_t type){
    for(int i = 0; i < ElfSections.size(); ++i){
        if(ElfSections[i].Type == type){
            return &ElfSections[i];
        }
    }
    return nullptr;
}


void InputFile::FillUpElfSyms(Shdr *shdr){
    Assert(shdr->Type == SHT_SYMTAB);
    //get the byte vector
    std::vector<uint8_t> symbols = GetBytesFromShdr(*shdr);

    unsigned symbolNums = symbols.size() / SymSize;

    while(symbolNums > 0){
        ElfSyms.push_back(Read<Sym>(symbols));
        symbols = std::vector<uint8_t>(symbols.begin() + SymSize, symbols.end());
        symbolNums--;
    }
}
