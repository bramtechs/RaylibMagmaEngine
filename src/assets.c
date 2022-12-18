#include "assets.h"

// TODO way to complicated, don't

static Assets* LoadedAssets = NULL;

bool try_init_assets(const char* folder) {
    if (DirectoryExists(folder)) {
       strcpy(LoadedAssets->folder,folder);
       INFO("Found assets at %s ...",folder);
       return true;
    }
    WARN("Did not find assets at %s, keep searching...",folder);
    return false;
}

void InitAssets(const char* folder){
    LoadedAssets = new(Assets);

    INFO("Loading assets...");

    if (try_init_assets(folder)) {
        return;
    }

    // visual studio
    if (try_init_assets("../../assets/")) {
        return;
    }
}

void UnloadAssets(){
    assert(LoadedAssets);
    M_MemFree(LoadedAssets);
    //INFO("Disposed assets!");
    WARN("TODO dispose assets!");
}

#define REGISTER(X) LoadedAssets->X##s[LoadedAssets->X##Count] = X; \
                    LoadedAssets->X##Count++

Texture RequestTexture(const char* name) {
    const char* path = TextFormat("%s/%s", LoadedAssets->folder, name);
    Texture texture = LoadTexture(path);
    if (texture.width == 0) {
        // failed, generate temporary
        Image temp = GenImageChecked(32, 32, 4, 4, RED, WHITE);
        texture = LoadTextureFromImage(temp);
        UnloadImage(temp);
    }
    
    REGISTER(texture);
    LoadedAssets->textures[LoadedAssets->textureCount] = texture;
    return texture;
}

Model RequestModel(const char* name) {
    const char* path = TextFormat("%s/%s", LoadedAssets->folder, name);
    Model model = LoadModel(path);
    // raylib automatically handles not found
    REGISTER(model);
    return model;
}
