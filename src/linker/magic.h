//
// Created by Valerian on 2023/10/7.
//

#ifndef LINKER_MAGIC_H
#define LINKER_MAGIC_H
//
// Created by Valerian on 2023/9/27.
//

#include <vector>
#include <cstdint>
#include <cstring>
bool CheckMagic(std::vector<uint8_t>& contents);
bool CheckArchive(std::vector<uint8_t>& contents);
#endif //LINKER_MAGIC_H
