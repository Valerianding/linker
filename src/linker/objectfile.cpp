//
// Created by Valerian on 2023/10/7.
//

#include "objectfile.h"



ObjectFile *NewObjectFile(File *file, bool isAlive){
    auto objectFile = new ObjectFile();
    objectFile->inputFile = NewInputFile(file);
    objectFile->inputFile->IsAlive = isAlive;
    return objectFile;
}

void ObjectFile::Parse(Context& ctx){
    SymtabSec = inputFile->FindSection(SHT_SYMTAB);
    if(SymtabSec != nullptr){
        //Set the global
        inputFile->FirstGlobal = SymtabSec->Info;
        inputFile->FillUpElfSyms(SymtabSec);
        inputFile->SymbolStrtab = inputFile->GetBytesFromIdx(SymtabSec->Link);
    }

    //init inputSection
    InitializeSections();

    //init symbols
    InitializeSymbols(ctx);

    //initialize mergeable sections

}

void ObjectFile::InitializeSections(){
    //arrange space for inputSection
    inputSection.resize(inputFile->ElfSections.size());

    for(int i = 0; i < inputFile->ElfSections.size(); ++i){
        Shdr* shdr = &inputFile->ElfSections[i];
        switch (shdr->Type) {
            case SHT_SYMTAB_SHNDX:
                FillUpSymtabShndxSec(shdr);
                break;
            case SHT_GROUP: case SHT_SYMTAB:case SHT_STRTAB:
            case SHT_REL: case SHT_RELA:case SHT_NULL:
                break;
            default:
                inputSection[i] = new InputSection(this,uint32_t(i));
                break;
        }
    }
}

void ObjectFile::FillUpSymtabShndxSec(Shdr *s){
    //
    Assert(s->Type == SHT_SYMTAB_SHNDX); // only this section should enter
    std::vector<uint8_t> bytes = inputFile->GetBytesFromShdr(*s);
    int nums = bytes.size() / 4;

    std::cout << nums << std::endl;

    while(nums > 0){
        SymtabShndxSec.push_back(Read<uint32_t>(bytes));
        bytes = std::vector<uint8_t>(bytes.begin() + 4, bytes.end());
        nums--;
    }
}

void ObjectFile::InitializeSymbols(Context& ctx){
    if(SymtabSec == nullptr){
        return;
    }

    int localNums = inputFile->FirstGlobal;

    //local symbols has exactly the same length of FirstGlobal
    //the first symbol is special
    inputFile->LocalSymbols.resize(localNums);

    for(int i = 0; i < localNums; ++i){
        inputFile->LocalSymbols[i] = new Symbol("");
    }


    //
    inputFile->LocalSymbols[0]->objectFile = this;

    //
    for(int i = 1; i < localNums; ++i){
        auto elfSym = &inputFile->ElfSyms[i]; // elf symbol
        auto sym = inputFile->LocalSymbols[i]; //
        sym->name = ElfGetName(inputFile->SymbolStrtab,elfSym->Name);
        sym->symIdx = i;
        sym->value = elfSym->Value;
        sym->objectFile = this;

        //only consider not absolute symbol -> function etc..
        //connect symbol to section
        if(!elfSym->IsAbs()){
            sym->SetInputSection(inputSection[GetShndx(elfSym,i)]);
        }
    }

    //init all Symbols
    inputFile->Symbols.resize(inputFile->ElfSyms.size());

    for(int i = 0; i < localNums; ++i){
        inputFile->Symbols[i] = inputFile->LocalSymbols[i];
    }

    for(int i = localNums; i < inputFile->ElfSyms.size(); ++i){
        Sym* elfSym = &inputFile->ElfSyms[i];
        std::string name = ElfGetName(inputFile->SymbolStrtab, elfSym->Name);
        inputFile->Symbols[i] = Symbol::GetSymbolByName(ctx,name);
    }
}

uint64_t ObjectFile::GetShndx(Sym *sym, int idx){
    Assert(idx >= 0 && idx < inputFile->ElfSyms.size());
    if(sym->Shndx == SHN_XINDEX){
        return uint64_t(SymtabShndxSec[idx]);
    }
    return uint64_t(sym->Shndx);
}

void ObjectFile::ResolveSymbols(){
    //only resolve global
    for(int i = inputFile->FirstGlobal; i < inputFile->ElfSyms.size(); ++i){
        auto sym = inputFile->Symbols[i];
        auto elfSym = &inputFile->ElfSyms[i];


        //skip undef symbols
        if(elfSym->IsUndef()){
            continue;
        }

        //
        InputSection *isec;
        if(!elfSym->IsAbs()){
            isec = GetSection(elfSym,i);
            if(isec == nullptr){
                continue;
            }
        }

        //important!! it is defined
        if(sym->objectFile == nullptr){
            sym->objectFile = this;
            sym->SetInputSection(isec) ;
            sym->value = elfSym->Value;
            sym->symIdx = i; //
        }
    }
}

//get inputsection according to idx
InputSection* ObjectFile::GetSection(Sym* esym, int idx){
    if(esym->Shndx == SHN_COMMON){
//        std::cout << inputFile->FirstGlobal << std::endl;
//        std::cout << ElfGetName(inputFile->SymbolStrtab, esym->Name) << std::endl;
        return nullptr;
    }
    return inputSection[GetShndx(esym,idx)];
}

void ObjectFile::ClearSymbols(){
    for(int i =inputFile->FirstGlobal ; i < inputFile->Symbols.size(); ++i){
        auto sym = inputFile->Symbols[i];
        if(sym->objectFile == this){
            sym->Clear();
        }
    }
}

void ObjectFile::MarkLiveObjects(Context& ctx, std::function<void(ObjectFile* )> feeder){
    Assert(inputFile->IsAlive);

    for(int i = inputFile->FirstGlobal; i < inputFile->ElfSyms.size(); ++i){
        auto sym = inputFile->Symbols[i];
        auto elfSym = &inputFile->ElfSyms[i];

        if(sym->objectFile == nullptr){
            continue;
        }

        //something like puts
        if(elfSym->IsUndef() && !sym->objectFile->inputFile->IsAlive){
            //
            sym->objectFile->inputFile->IsAlive = true;
            feeder(sym->objectFile);
        }
    }
}

void ObjectFile::InitializeMergeableSections(Context& ctx){
    mergeableSections.resize(inputSection.size());


}