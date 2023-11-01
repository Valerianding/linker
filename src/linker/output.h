//
// Created by Valerian on 2023/10/30.
//

#ifndef LINKER_OUTPUT_H
#define LINKER_OUTPUT_H
#include <iostream>
#include <vector>
#include <elf.h>
#include "utils/utils.h"
std::string GetOutputName(std::string input,uint64_t flags);
#endif //LINKER_OUTPUT_H
