//
// Created by Valerian on 2023/10/13.
//

#ifndef LINKER_ARCHIVE_H
#define LINKER_ARCHIVE_H
#include <cstdint>
#include <iostream>
#include <vector>
#include "utils/utils.h"
#include "linker/file.h"
#include "linker/filetype.h"

class ArHdr{
public:
    uint8_t Name[16];
    uint8_t Date[12];
    uint8_t Uid[6];
    uint8_t Gid[6];
    uint8_t Mode[8];
    uint8_t Size[10];
    uint8_t Fmag[2];
public:
    int GetSize();

    bool HasPrefix(const std::string& s);

    bool IsStrtab();

    bool IsSymtab();

    std::string ReadName(std::vector<uint8_t> strTab);

};
std::vector<File*> ReadArchiveMembers(File* file);

const int ArHdrSize = sizeof(ArHdr);
#endif //LINKER_ARCHIVE_H
