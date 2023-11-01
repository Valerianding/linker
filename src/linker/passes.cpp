//
// Created by Valerian on 2023/10/19.
//

#include "passes.h"
//mark live object files
void MarkLiveObjects(Context& ctx){
    std::vector<ObjectFile *> roots;
    for(auto file : ctx.Objs){
        if(file->inputFile->IsAlive){
            roots.push_back(file);
        }
    }

    Assert(roots.size() > 0);

    while(roots.size() > 0){
        auto file = roots[0];

        if(!file->inputFile->IsAlive) continue;

        auto func = [&roots](ObjectFile* file){
            roots.push_back(file);
        };

        file->MarkLiveObjects(ctx,func);

        roots.erase(roots.begin());
    }
}


//find where the symbol come from
void ResolveSymbols(Context& ctx){
    for(auto objs : ctx.Objs){
        objs->ResolveSymbols();
    }

    MarkLiveObjects(ctx);

    for(auto objs : ctx.Objs){
        if(!objs->inputFile->IsAlive){
            //clear symbols
            objs->ClearSymbols();
        }
    }

    auto func = [](ObjectFile* file){
        return !file->inputFile->IsAlive;
    };

    //remove not alive files
    ctx.Objs = RemoveIf<ObjectFile*>(ctx.Objs,func);
}