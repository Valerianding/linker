//
// Created by Valerian on 2023/9/28.
//

#include "file.h"
File* OpenLibrary(const std::string& filepath){
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if(!file.is_open()){
        return nullptr;
    }

    std::streampos fileSize = file.tellg();

    std::vector<uint8_t> contents(fileSize);

    file.seekg(0,std::ios::beg);


    if(!file.read(reinterpret_cast<char *>(contents.data()),fileSize)){
        return nullptr;
    }

    return new File(filepath,contents);
}

File* FindLibrary(Context& ctx,const std::string& name){
    for(const auto& dir : ctx.Args.LibraryPaths){
        std::string stem = dir + "/lib" + name + ".a";
        File *file;
        if((file = OpenLibrary(stem)) != nullptr){
            return file;
        }
    }
    return nullptr;
}