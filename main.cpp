#include <iostream>
#include <string>
#include <vector>
#include "utils/utils.h"
#include "linker/inputfile.h"
#include "linker/input.h"
#include "linker/passes.h"
std::vector<std::string> parseArgs(int argc, char **argv, Context& ctx);

int main(int argc, char** argv) {
    if (argc < 2) {
        Fatal("Wrong Args");
    }

    Context ctx;

    std::vector<std::string> remaining = parseArgs(argc,argv,ctx);


    //now the remaining only contains -l & other object/archive files
    if(ctx.Args.Emulation == MachineTypeNone){
        for(auto& str : remaining){
            if(str[0] == '-'){
                continue;
            }

            //now This means it is a file
            File *file = new File(str);
            ctx.Args.Emulation = GetMachineTypeFromContents(file->contents);
            if(ctx.Args.Emulation != MachineTypeNone){
                break;
            }
        }
    }

//    std::cout << ctx.Args.Output << std::endl;
//    std::cout << MachineTypeStringer(ctx.Args.Emulation).String() << std::endl;
//

    ReadInputFiles(ctx,remaining);

//    for(auto& obj : ctx.Objs){
//        std::cout << obj->inputFile->file->name << std::endl;
//    }

    ResolveSymbols(ctx);

    for(auto& obj : ctx.Objs){
        std::cout << obj->inputFile->file->name << std::endl;
        if(obj->inputFile->file->name == "./tests/main.o"){
            for(auto& sym : obj->inputFile->Symbols){
                if(sym->name == "puts"){
                    std::cout << sym->objectFile->inputFile->file->parent->name << std::endl;
                }
            }
            break;
        }
    }


    for(int i = 0; i < ctx.Args.LibraryPaths.size(); ++i){
        //std::cout << ctx.Args.LibraryPaths[i] << "  ";
        ctx.Args.LibraryPaths[i] = CleanFilePath(ctx.Args.LibraryPaths[i]);
        //std::cout << ctx.Args.LibraryPaths[i] << std::endl;
    }


    std::cout << ctx.Objs.size() << std::endl;
    return 0;
}

std::vector<std::string> parseArgs(int argc, char **argv, Context& ctx){
    std::vector<std::string> args;

    for(int i = 1; i < argc; ++i){
        args.emplace_back(argv[i]);
    }

    auto dashes = [](const std::string& name) -> std::vector<std::string>{
        if(name.size() == 1){
            return {"-" + name};
        }
        return {"-" + name, "--" + name};
    };

    //string -> current parse args
    // -o a.out

    std::string arg = args[0];

    //Intend to parse Compiling flags for example: --help / -static
    //pass dashes arg by reference
    auto readFlag= [&dashes, &args](const std::string& name) -> bool {
        for(const auto& str : dashes(name)){
            if(str == args[0]){
                args = std::vector<std::string>(args.begin() + 1, args.end());
                return true;
            }
        }
        return false;
    };

    //Intend to parse Compiling arg(??) for example: -o [file]
    //for example: input: -o [file]
    auto readArg = [&dashes, &args, &arg](const std::string& name) -> bool{
        for(const auto& str : dashes(name)){
            if(args[0] == str) {
                if (args.size() == 1) {
                    //only one parameter remains for example -o but without the output file name
                    std::string message = "option -" + args[0] + ": argument missing";
                    Fatal(message);
                }

                arg = args[1];
                args = std::vector<std::string>(args.begin() + 2, args.end());
                return true;
            }
            //on only one situation will it be like a -mriscv64/ -L... others are all -[option]=[...]
            auto prefix = str;
            if(name.size() > 1){
                prefix += "=";
            }

            if(args[0].compare(0,prefix.length(),prefix) == 0){
                arg = std::string(args[0].begin() + prefix.size(), args[0].end());
                args = std::vector<std::string>(args.begin() + 1, args.end());
                return true;
            }
        }
        return false;
    };

    //Not in consideration
    std::vector<std::string> UselessArgs = {"sysroot","plugin","plugin-opt","hash-style","build-id","z"};

    std::vector<std::string> UselessFlags = {"static","as-needed","start-group","end-group","s","no-relax"};

    auto IsUselessArgs = [&UselessArgs,&readArg]() -> bool{
        const size_t length = UselessArgs.size();
        for(int i = 0; i < length; ++i){
            if(readArg(UselessArgs[i])){
                return true;
            }
        }
        return false;
    };

    auto IsUselessFlags = [&UselessFlags,&readFlag]() -> bool{
        const size_t length = UselessFlags.size();
        for(int i = 0; i < length; ++i){
            if(readFlag(UselessFlags[i])){
                return true;
            }
        }
        return false;
    };

    std::vector<std::string> remaining;


    while(args.size() != 0){
        for(int i = 0; i < args.size(); ++i){
            if(readFlag("help")){
                std::cout << "Usage: " <<  "./linker" << " [options] file.." << std::endl;
                exit(0);
            }
            if(readArg("o") || readArg("output")) {
                ctx.Args.Output = arg;
            }else if(readArg("v") || readArg("version")){
                std::cout << "ld version: 0.0.1" << std::endl;
                exit(0);
            }else if(readArg("m")) {
                if(arg == "elf64lriscv"){
                    //TODO
                    ctx.Args.Emulation = MachineTypeRISCV64;
                }else {
                    Fatal("unknown -m arch");
                }
            }else if(readArg("L")){
                //Library Paths
                ctx.Args.LibraryPaths.push_back(arg);
            }else if(readArg("l")){
                remaining.push_back("-l" + arg);
            }else if(IsUselessArgs() || IsUselessFlags()){
                //Ignored
            }else{
                //should be object files
                if(args[0][0] == '-'){
                    Fatal("unknown command line option");
                }

                remaining.push_back(args[0]);
                args = std::vector<std::string>(args.begin() + 1, args.end());
            }
        }
    }


    //

    //This is needed
    return remaining;
}