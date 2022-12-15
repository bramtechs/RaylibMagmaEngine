#include "assets.h"

static Assets* LoadedAssets = NULL;

bool try_load_assets(const char* folder){
    if (DirectoryExists(folder)){
        strcpy(LoadedAssets->folder,folder);
        return true;
    }
    WARN("Asset directory %s not found, keep searching...",folder);
    return false;
}

void LoadAssets(const char* folder){
    LoadedAssets = new(Assets);

    INFO("Loading assets...");

    if (try_load_assets(folder)){
        return;
    }

    // visual studio fallback folder
    if (try_load_assets("../../assets")){
        return;
    }

    ERROR("Asset directory not found!");
}

void UnloadAssets(){
    assert(LoadedAssets);
    M_MemFree(LoadedAssets);
    //INFO("Disposed assets!");
    WARN("TODO dispose assets!");
}

Model RequestModel(const char* name){
    assert(LoadedAssets);
    const char* path = TextFormat("%s/%s",LoadedAssets->folder,name);
    Model model = LoadModel(path);
    return model;
}

Texture RequestTexture(const char* name){
    assert(LoadedAssets);
    const char* path = TextFormat("%s/%s",LoadedAssets->folder,name);
    Texture texture = LoadTexture(path);
    return texture;
}
