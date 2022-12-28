#include "assets.h"

static GameAssets* Assets = NULL;

bool try_init_assets(const char* folder) {
    if (DirectoryExists(folder)) {
       strcpy(Assets->folder,folder);
       INFO("Found assets at %s ...",folder);
       return true;
    }
    WARN("Did not find assets at %s, keep searching...",folder);
    return false;
}

void InitAssets(const char* folder){
    Assets = new(GameAssets);
    Assets->textures = MakeArray(sizeof(Texture));
    Assets->texturePaths = MakeArray(sizeof(char[128]));

    INFO("Loading assets...");

    if (try_init_assets(folder)) {
        return;
    }

    // visual studio
    if (try_init_assets("../../assets/")) {
        return;
    }
}

void dispose_texture(int i, void* ptr){
    Texture* texture = (Texture*) ptr;
    UnloadTexture(*texture);
}

void dispose_model(int i, void* ptr){
    Model* model = (Model*) ptr;
    UnloadModel(*model);
}

void DisposeAssets(){
    assert(Assets);

    IterateArray(Assets->textures, dispose_texture);

    DisposeArray(Assets->textures);
    DisposeArray(Assets->texturePaths);

    M_MemFree(Assets);
}

Texture RequestTexture(const char* name) {
    const char* path = TextFormat("%s/%s", Assets->folder, name);
    Texture texture = LoadTexture(path);
    if (texture.width == 0) {
        // failed, generate temporary
        Image temp = GenImageChecked(32, 32, 4, 4, RED, WHITE);
        texture = LoadTextureFromImage(temp);
        UnloadImage(temp);
    }
    PushArray(Assets->textures,Texture,&texture);
    return texture;
}

Model RequestModel(const char* name) {
    const char* path = TextFormat("%s/%s", Assets->folder, name);
    Model model = LoadModel(path);

    // raylib automatically handles if model isn't found
    // TODO memcopying models doesn't seem to work so you'll have to dispose these things manually for now

    return model;
}

FilePathList IndexModels(const char* folder){
    const char* path = TextFormat("%s/%s", Assets->folder, folder);

    INFO("Indexing %s for models", path);
    FilePathList list =  LoadDirectoryFilesEx(path, ".obj", true);
    for (int i = 0; i < list.count; i++){
        DEBUG(">>> %s",list.paths[i]);
    }
    INFO("Indexed %d models",list.count);
    // TODO dispose
    return list;
}
