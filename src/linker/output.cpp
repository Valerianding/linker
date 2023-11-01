//
// Created by Valerian on 2023/10/30.
//

#include "output.h"

std::vector<std::string> prefixes = {".text.",".data.rel.ro.",};
std::string GetOutputName(std::string input,uint64_t flags){
    if((input == ".rodata" || HasPrefix(input,".rodata."))
            && (flags & SHF_MERGE) != 0){
        if((flags & SHF_STRINGS) != 0){
            return ".rodata.str";
        }else{
            return ".rodata.cst";
        }
    }

    for(auto & prefix : prefixes){
        std::string stem = std::string(prefix.begin(),prefix.end() - 1);
        if(input == stem || HasPrefix(input,prefix)){
            return stem;
        }
    }
    return input;
}
