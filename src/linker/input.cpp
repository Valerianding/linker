//
// Created by Valerian on 2023/10/15.
//
//
#include "input.h"
void ReadInputFiles(Context& ctx,const std::vector<std::string>& remaining){
    for(auto arg : remaining){
        if(arg[0] == '-'){
            //remove prefix -l
            std::string name = RemovePrefix(arg,"-l");
            if(!name.empty()){
                ReadFile(ctx, FindLibrary(ctx,name));
            }
        }else{
            ReadFile(ctx, new File(arg));
        }
    }
}

void ReadFile(Context& ctx, File* file){
    FileType fileType = GetFileType(file->contents);

    switch(fileType){
        case FileTypeObject:
            ctx.Objs.push_back(CreateObjectFile(ctx,file,false));
            break;
        case FileTypeArchive:
            for(auto member : ReadArchiveMembers(file)){
                Assert(GetFileType(member->contents) == FileTypeObject);
                ctx.Objs.push_back(CreateObjectFile(ctx,member,true));
            }
            break;
        default:
            Fatal("unknown file type");
    }
}

ObjectFile* CreateObjectFile(Context& ctx, File* file, bool inLib){
    CheckFileCompatibility(ctx,file);

    //inLib 表示是不是在静态链接库中 是
    ObjectFile *objectFile = NewObjectFile(file,!inLib);
    objectFile->Parse(ctx);
    return objectFile;
}