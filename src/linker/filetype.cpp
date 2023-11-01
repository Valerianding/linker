//
// Created by Valerian on 2023/10/10.
//

#include "filetype.h"
FileType GetFileType(std::vector<uint8_t>& contents){
    if(contents.empty()){
        return FileTypeEmpty;
    }

    if(CheckMagic(contents)){
        //read from the 16 bytes;
        std::vector<uint8_t> rest(contents.begin() + 16, contents.end());
        uint8_t fileType = Read<uint16_t>(rest);
        switch (fileType) {
            case ET_REL:
                return FileTypeObject;
        }
        return FileTypeUnknown;
    }


    if(CheckArchive(contents)){
        return FileTypeArchive;
    }

    return FileTypeUnknown;
}


void CheckFileCompatibility(Context& ctx, File* file){
    MachineType machineType = GetMachineTypeFromContents(file->contents);
    if(machineType != ctx.Args.Emulation){
        Fatal("incompatible file type");
    }
}