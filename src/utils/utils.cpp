//
// Created by Valerian on 2023/9/27.
//

#include "utils.h"
void Fatal(const std::string& error){
    std::cout << "ld: \033[0;1;31mfatal:\033[0m " << error << std::endl;
    assert(false);
    std::exit(1);
}

void Assert(bool condition){
    if(!condition){
        Fatal("assert failed");
    }
}


template<typename T>
T Read(std::vector<unsigned char>&& data){
    if(data.size() < sizeof(T)){
        throw std::runtime_error("Insufficient data for reading");
    }
    T val;
    std::memcpy(&val, data.data(), sizeof(T));
    return val;
}

std::string RemovePrefix(const std::string& s, const std::string& prefix){
    if(s.compare(0,prefix.length(),prefix) == 0){
        std::string result = s.substr(prefix.length());
        return result;
    }
    return "";
}

std::string CleanFilePath(const std::string& path){
    std::filesystem::path fspath(path);
    fspath = std::filesystem::canonical(fspath);
    return fspath.string();
}

bool HasPrefix(const std::string& str, const std::string& prefix){
    if(str.length() < prefix.length()){
        return false;
    }
    return str.compare(0,prefix.length(),prefix) == 0;
}

