//
// Created by Valerian on 2023/10/10.
//

#ifndef LINKER_FILETYPE_H
#define LINKER_FILETYPE_H
#include <cstdint>
#include <vector>
#include <elf.h>
#include "utils/utils.h"
#include "linker/magic.h"
#include "linker/machinetype.h"
#include "linker/file.h"
class ContextArgs;
class File;
class Context;

typedef enum FileType : uint8_t {
    FileTypeUnknown,
    FileTypeEmpty,
    FileTypeObject,
    FileTypeArchive,
}FileType;

FileType GetFileType(std::vector<uint8_t>& contents);
void CheckFileCompatibility(Context& ctx, File* file);
#endif //LINKER_FILETYPE_H
