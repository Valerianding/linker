//
// Created by Valerian on 2023/10/10.
//

#ifndef LINKER_MACHINETYPE_H
#define LINKER_MACHINETYPE_H

#include <cstdint>
#include <iostream>
#include "utils/utils.h"
#include "linker/filetype.h"
typedef enum MachineType : uint8_t{
    MachineTypeNone,
    MachineTypeRISCV64,
}MachineType;


class MachineTypeStringer{
public:
    MachineType type;
    [[nodiscard]] std::string String() const{
        switch (type) {
            case MachineTypeRISCV64:
                return "riscv64";
        }
        Assert(type == MachineTypeNone);
        return "none";
    }
    MachineTypeStringer() = default;
    MachineTypeStringer(MachineType tp) : type(tp){};
};


MachineType GetMachineTypeFromContents(std::vector<uint8_t>& contents);
#endif //LINKER_MACHINETYPE_H
