//
// Created by Valerian on 2023/10/10.
//

#include "machinetype.h"
template<typename T>
T Read(std::vector<unsigned char>&& data){
    if(data.size() < sizeof(T)){
        throw std::runtime_error("Insufficient data for reading");
    }
    T val;
    std::memcpy(&val, data.data(), sizeof(T));
    return val;
}


MachineType GetMachineTypeFromContents(std::vector<uint8_t>& contents){
    FileType fileType = GetFileType(contents);

    switch (fileType) {
        case FileTypeObject: {
            //Find Machine Type
            auto machine = Read<uint16_t>(std::vector<uint8_t>(contents.begin() + 18, contents.end()));
            if (machine == EM_RISCV) {
                //read the class
                uint8_t elfClass = contents[4];
                switch (elfClass) {
                    case ELFCLASS64: {
                        return MachineTypeRISCV64;
                    }
                }
            }
        }
    }
    return MachineTypeNone;
}
