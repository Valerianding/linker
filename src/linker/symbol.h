//
// Created by Valerian on 2023/10/18.
//

#ifndef LINKER_SYMBOL_H
#define LINKER_SYMBOL_H

#include <utility>

#include "linker/objectfile.h"
#include "linker/inputsection.h"

class ObjectFile;
class InputSection;
class Context;
class Sym;
class SectionFragment;

class Symbol{
public:
    ObjectFile* objectFile = nullptr;
    InputSection* inputSection = nullptr;
    SectionFragment* sectionFragment = nullptr; // "Hello World" will be a symbol
                                                // but belongs to a sectionFragment

    std::string name;
    uint64_t value;
    int symIdx = 0; //default is undef section
    Symbol() = default;
    Symbol(std::string  symbolName) : name(std::move(symbolName)){}

    void SetInputSection(InputSection* isec);

    void SetSectionFragment(SectionFragment *fragment);

    static Symbol* GetSymbolByName(Context& ctx, const std::string& symbolName);

    [[nodiscard]] Sym* GetElfSym() const;

    void Clear();
};
#endif //LINKER_SYMBOL_H
