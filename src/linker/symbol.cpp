//
// Created by Valerian on 2023/10/18.
//

#include "symbol.h"
void Symbol::SetInputSection(InputSection* isec){
    inputSection = isec;
    assert(sectionFragment == nullptr);
}

void Symbol::SetSectionFragment(SectionFragment *fragment){
    sectionFragment = fragment;
    assert(inputSection == nullptr);
}


Symbol* Symbol::GetSymbolByName(Context& ctx, const std::string& symbolName){
    if(ctx.SymbolMap[symbolName] != nullptr){
        return ctx.SymbolMap[symbolName];
    }

    ctx.SymbolMap[symbolName] = new Symbol(symbolName);
    return ctx.SymbolMap[symbolName];
}

Sym* Symbol::GetElfSym() const{
    Assert(symIdx < objectFile->inputFile->ElfSyms.size());
    return &objectFile->inputFile->ElfSyms[symIdx];
}

void Symbol::Clear(){
    objectFile = nullptr;
    inputSection = nullptr;
    symIdx = -1;
}

