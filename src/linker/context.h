//
// Created by Valerian on 2023/10/9.
//

#ifndef LINKER_CONTEXT_H
#define LINKER_CONTEXT_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include "linker/machinetype.h"
#include "linker/objectfile.h"
#include "linker/symbol.h"
#include "linker/mergedsection.h"
class Symbol;
class ObjectFile;
class MergedSection;
enum MachineType : uint8_t;



class ContextArgs{
public:
    std::string Output = "a.out";
    MachineType Emulation;
    std::vector<std::string> LibraryPaths;
};

class Context{
public:
    ContextArgs Args;
    std::vector<ObjectFile*> Objs; //the vector stores the ObjectFile pointer
    std::unordered_map<std::string,Symbol*> SymbolMap;
    std::vector<MergedSection*> mergedSection;
    Context() = default;
};
#endif //LINKER_CONTEXT_H
