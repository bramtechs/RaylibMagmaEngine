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
    Assets->textures = MakeArray(sizeof(TextureContainer));

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
    Texture texture = ((TextureContainer*) ptr)->texture;
    UnloadTexture(texture);
}

void dispose_model(int i, void* ptr){
    Model model = *(Model*) ptr;
    UnloadModel(model);
}

void DisposeAssets(){
    assert(Assets);

    IterateArray(Assets->textures, dispose_texture);
    DisposeArray(Assets->textures);

    M_MemFree(Assets);
}

const char* GetAssetFolder(){
    assert(Assets);
    return Assets->folder;
}

Texture RequestTexture(const char* name) {
    const char* path = TextFormat("%s/%s", Assets->folder, name);

    // get cached texture
    for (int i = 0; i < Assets->textures->count; i++){
        TextureContainer cont = *GetArrayItem(Assets->textures,i,TextureContainer);
        if (strcmp(cont.name,name) == 0){
            return cont.texture;
        }
    }

    // load texture from disk
    Texture texture = LoadTexture(path);
    if (texture.width == 0) {
        // failed, generate temporary
        Image temp = GenImageChecked(32, 32, 4, 4, RED, WHITE);
        texture = LoadTextureFromImage(temp);
        UnloadImage(temp);
    }

    // push into texture array
    TextureContainer cont = { 0 };
    strcpy(cont.name, name);
    cont.texture = texture;

    PushArray(Assets->textures, TextureContainer, &cont);

    return texture;
}

Model RequestModel(const char* name) {
    const char* path = TextFormat("%s/%s", Assets->folder, name);
    Model model = LoadModel(path);

    // raylib automatically handles if model isn't found
    // TODO memcopying models doesn't seem to work so you'll have to dispose these things manually for now

    return model;
}

Shader RequestShader(const char* name){
    const char* path = TextFormat("%s/%s", Assets->folder, name);
    Shader shader = LoadShader(0, path);
    return shader;
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
