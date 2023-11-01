//
// Created by Valerian on 2023/10/13.
//

#include "archive.h"
int ArHdr::GetSize(){
    std::string str(reinterpret_cast<char*>(Size),sizeof(Size));

    if(str.size() == 0) return 0;

    int size = std::atoi(str.c_str());

    return size;
}

bool ArHdr::HasPrefix(const std::string& s) {
    std::string str(reinterpret_cast<char *>(Name), sizeof(Name));
    return str.compare(0, s.length(), s) == 0;
}

bool ArHdr::IsStrtab(){
    return HasPrefix("// ");
}

bool ArHdr::IsSymtab(){
    return HasPrefix("/ ") || HasPrefix("/SYM64/ ");
}

std::string ArHdr::ReadName(std::vector<uint8_t> strTab){
    //long filename
    //'/123 /\n '
    if(HasPrefix("/")){
        std::string str( reinterpret_cast<const char*>(Name + 1));
        int start = std::atoi(str.c_str());
        int end = start + std::distance(strTab.begin() + start, std::find(strTab.begin() + start, strTab.begin() + start + str.size(), '/'));
        return std::string(strTab.begin() + start, strTab.begin() + end);
    }

    //short filename
    //test.o
    int end = -1;
    for(int i = 0; i < 16; ++i){
        if(Name[i] == '/'){
            end = i;
        }
    }
    Assert(end != -1);
    return std::string(reinterpret_cast<const char *>(Name),end);
}

std::vector<File*> ReadArchiveMembers(File* file){
    Assert(GetFileType(file->contents) == FileTypeArchive);

    int pos = 8;
    std::vector<uint8_t> strTab;

    std::vector<File*> files;

    //notice that the data section is 2 byte aligned
    while(file->contents.size() - pos > 1){

        //since it is 2 byte aligned we must process from 2
        if(pos % 2 == 1){
            pos++;
        }
        //read ArHdr from file contents pos position

        std::vector<uint8_t> part(file->contents.begin() + pos, file->contents.end());
        auto arHdr = Read<ArHdr>(part);

        //find the datastart
        int dataStart = pos + ArHdrSize;
        int dataEnd = dataStart +  arHdr.GetSize();
        pos = dataEnd;

        //pos = dataend

        //calc dataend
        //contents
        std::vector<uint8_t> contents = std::vector<uint8_t>(file->contents.begin() + dataStart, file->contents.begin() + dataEnd);

        //if is Symbtab continue
        if(arHdr.IsStrtab()){
            strTab = contents;
            continue;
        }else if(arHdr.IsSymtab()){
            continue;
        }

        files.emplace_back(new File{arHdr.ReadName(strTab),contents,file});

    }
    return files;
}