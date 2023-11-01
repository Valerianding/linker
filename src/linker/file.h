//
// Created by Valerian on 2023/9/28.
//

#ifndef LINKER_FILE_H
#define LINKER_FILE_H
#include <iostream>
#include <utility>
#include <vector>
#include <memory>
#include <fstream>
#include "utils/utils.h"
#include "linker/context.h"

class Context;

class File{
public:
    std::string name;
    std::vector<uint8_t> contents;
    File *parent = nullptr; // if is .o // .a ->     else  parent -> file
public:
    File() = default;
    File(const std::string& fileName){
        name = fileName;

        std::ifstream file(fileName,std::ios::binary);
        if(!file){
            std::cout << fileName << std::endl;
            Fatal("open file error");
        }
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        contents.resize(fileSize);
        if (!file.read(reinterpret_cast<char*>(contents.data()), fileSize)) {
            Fatal("failed to read source file");
        }
    }
    File(std::string  fileName,
            std::vector<uint8_t>& fileContents,File *fileParent = nullptr) : name(std::move(fileName)), contents(fileContents), parent(fileParent){
    }
};

File* OpenLibrary(const std::string& filepath);

File* FindLibrary(Context& ctx,const std::string& name);
#endif //LINKER_FILE_H
