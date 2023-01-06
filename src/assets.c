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
    assert(Assets);

    for (int i = 0; i < Assets->textureCount; i++){
        UnloadTexture(Assets->textures[i].texture);
    }
    for (int i = 0; i < Assets->modelCount; i++){
        UnloadModel(Assets->models[i].model);
    }

    M_MemFree(Assets);
}

const char* GetAssetFolder(){
    assert(Assets);
    return Assets->folder;
}

Texture RequestTexture(const char* name) {
    const char* path = TextFormat("%s/%s", Assets->folder, name);

    // get cached texture
    for (int i = 0; i < Assets->textureCount; i++){
        TextureContainer cont = Assets->textures[i];
        if (strcmp(cont.name,name) == 0){
            return cont.texture;
        }
    }

    // load texture from disk
    Texture texture = LoadTexture(path);
    if (texture.width == 0) {
        // failed, generate placeholder instead
        Image temp = GenImageChecked(32, 32, 4, 4, RED, WHITE);
        texture = LoadTextureFromImage(temp);
        UnloadImage(temp);
    }

    // push into texture array
    TextureContainer* cont = &Assets->textures[Assets->textureCount];
    strcpy(cont->name, name);
    cont->texture = texture;

    Assets->textureCount++;
    return texture;
}

Model RequestModel(const char* name) {
    const char* path = TextFormat("%s/%s", Assets->folder, name);

    // get cached model
    for (int i = 0; i < Assets->modelCount; i++){
        ModelContainer cont = Assets->models[i];
        if (strcmp(cont.name,name) == 0){
            return cont.model;
        }
    }

    Model model = LoadModel(path);

    // raylib automatically handles if model isn't found
    // NOTE memcopying models doesn't seem to work so you'll have to dispose these things manually for now
    
    // push into model array
    ModelContainer* cont = &Assets->models[Assets->modelCount];
    strcpy(cont->name, name);
    cont->model = model;

    Assets->modelCount++;
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
