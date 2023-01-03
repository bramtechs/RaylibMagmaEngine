#include "assets.h"

static GameAssets Assets = {0};

bool try_init_assets(const char* folder) {
    if (DirectoryExists(folder)) {
       Assets.folder = folder;
       INFO("Found assets at %s ...",folder);
       return true;
    }
    WARN("Did not find assets at %s, keep searching...",folder);
    return false;
}

void InitAssets(const char* folder){
    INFO("Loading assets...");
    if (try_init_assets(folder)) {
        return;
    }
    // visual studio
    if (try_init_assets("../../assets/")) {
        return;
    }
}

void DisposeAssets(){
    for (auto pair : Assets.textures){
        UnloadTexture(pair.second);
    }
}

const char* GetAssetFolder(){
    return Assets.folder.c_str();
}

Texture RequestTexture(const char* name) {

    // get cached texture
    for (auto pair : Assets.textures){
        if (pair.first == name){
            return pair.second;
        }
    }

    // load texture from disk
    const char* path = TextFormat("%s/%s", Assets.folder.c_str(), name);
    Texture texture = LoadTexture(path);
    if (texture.width == 0) {
        // failed, generate temporary
        Image temp = GenImageChecked(32, 32, 4, 4, RED, WHITE);
        texture = LoadTextureFromImage(temp);
        UnloadImage(temp);
    }

    Assets.textures.insert({name,texture});

    return texture;
}

Model RequestModel(const char* name) {
    const char* path = TextFormat("%s/%s", Assets.folder, name);
    Model model = LoadModel(path);

    // raylib automatically handles if model isn't found
    // TODO memcopying models doesn't seem to work so you'll have to dispose these things manually for now

    return model;
}

Shader RequestShader(const char* name){
    const char* path = TextFormat("%s/%s", Assets.folder, name);
    Shader shader = LoadShader(0, path);
    return shader;
}

FilePathList IndexModels(const char* folder){
    const char* path = TextFormat("%s/%s", Assets.folder, folder);

    INFO("Indexing %s for models", path);
    FilePathList list = LoadDirectoryFilesEx(path, ".obj", true);
    for (int i = 0; i < list.count; i++){
        DEBUG(">>> %s",list.paths[i]);
    }
    INFO("Indexed %d models",list.count);
    // TODO dispose
    return list;
}
