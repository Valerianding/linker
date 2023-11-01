//
// Created by Valerian on 2023/9/27.
//

#ifndef LINKER_UTILS_H
#define LINKER_UTILS_H
#include <iostream>
#include <vector>
#include <fstream>
#include <functional>
#include <cstring>
#include <cassert>
#include <filesystem>
void Fatal(const std::string& error);
void Assert(bool condition);

template<typename T>
T Read(std::vector<unsigned char>& data){
    if(data.size() < sizeof(T)){
        throw std::runtime_error("Insufficient data for reading");
    }
    T val;
    std::memcpy(&val, data.data(), sizeof(T));
    return val;
}


std::string RemovePrefix(const std::string& s, const std::string& prefix);


template<typename T>
std::vector<T> RemoveIf(std::vector<T> elems,std::function<bool(T)> condition){
    int index = 0;
    for(int i = 0; i < elems.size(); ++i){
        if(condition(elems[i])){
            continue;
        }
        elems[index] = elems[i];
        index++;
    }
    return std::vector<T>{elems.begin(), elems.begin() + index};
}

std::string CleanFilePath(const std::string& path);


//template function should define in .h files
//vector data is passed by value
template<typename T>
std::vector<T> ReadVector(std::vector<std::uint8_t> data, int size){
    unsigned nums = data.size() / size;
    std::vector<T> res(nums);
    while(nums > 0){
        res.push_back(Read<T>(data));
        data.erase(data.begin(),data.begin() + size);
        nums--;
    }
    return res;
}

bool HasPrefix(const std::string& str, const std::string& prefix);
#endif //LINKER_UTILS_H
