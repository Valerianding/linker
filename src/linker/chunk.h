//
// Created by Valerian on 2023/10/30.
//

#ifndef LINKER_CHUNK_H
#define LINKER_CHUNK_H
#include <iostream>
#include <utility>
#include "linker/elf.h"
class Chunk{
public:
    std::string name;
    Shdr* shdr;
    Chunk(){
        shdr = new Shdr{};
        shdr->AddrAlign = 1;
    }

    Chunk(std::string  n) : name(std::move(n)){
        shdr = new Shdr{};
        shdr->AddrAlign = 1;
    }
};
#endif //LINKER_CHUNK_H
