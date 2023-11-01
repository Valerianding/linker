//
// Created by Valerian on 2023/10/19.
//

#ifndef LINKER_PASSES_H
#define LINKER_PASSES_H
//main function of linker
#include "utils/utils.h"
#include "linker/context.h"
void MarkLiveObjects(Context& ctx);
void ResolveSymbols(Context& ctx);
#endif //LINKER_PASSES_H
