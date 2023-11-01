//
// Created by Valerian on 2023/10/7.
//

#ifndef LINKER_OBJECTFILE_H
#define LINKER_OBJECTFILE_H
#include <functional>
#include "linker/symbol.h"
#include "utils/utils.h"
#include "linker/elf.h"
#include "linker/inputfile.h"
#include "linker/inputsection.h"
#include "linker/mergeablesection.h"

class Context;
class InputFile;
class File;
class MergeableSection;

class ObjectFile{
public:
    InputFile *inputFile;
    Shdr* SymtabSec;
    std::vector<InputSection*> inputSection;
    std::vector<uint32_t> SymtabShndxSec; //if there is too much sections
    std::vector<MergeableSection *> mergeableSections; // mergeableSections
public:
    ObjectFile() = default;
    void Parse(Context& ctx);
    void InitializeSections();
    void FillUpSymtabShndxSec(Shdr *s);
    void InitializeSymbols(Context& ctx);
    void InitializeMergeableSections(Context& ctx);
    uint64_t GetShndx(Sym *esym, int idx);
    void ResolveSymbols();
    InputSection* GetSection(Sym* esym, int idx);
    void MarkLiveObjects(Context& ctx, std::function<void(ObjectFile* )> feeder);
    void ClearSymbols();
};

ObjectFile *NewObjectFile(File *file, bool isAlive);
ObjectFile* CreateObjectFile(Context& ctx, File* file, bool inLib);
#endif //LINKER_OBJECTFILE_H
