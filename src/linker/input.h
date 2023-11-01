//
// Created by Valerian on 2023/10/15.
//

#ifndef LINKER_INPUT_H
#define LINKER_INPUT_H
#include "utils/utils.h"
#include "linker/file.h"
#include "linker/context.h"
#include "linker/archive.h"
void ReadInputFiles(Context& ctx,const std::vector<std::string>& remaining);

void ReadFile(Context& ctx, File* file);

ObjectFile* CreateObjectFile(Context& ctx, File* file, bool inLib);
#endif //LINKER_INPUT_H
