#include "assets.h"

// TODO way to complicated, don't

static Assets* LoadedAssets = NULL;

static Asset* create_asset(const char* name, size_t size, AssetType type){
    Asset* asset = &LoadedAssets->assets[LoadedAssets->count];

    LoadedAssets->count++;
    assert(LoadedAssets->count < MAX_ASSETS);

    asset->type = type;
    asset->memory = M_MemAlloc(size);

    // strip prefix from name
    // TODO: unsafe

    int prefixLength = strlen(LoadedAssets->folder);
    char *shortName = strncmp(name, LoadedAssets->folder, prefixLength) ? name : name + prefixLength;

    if (shortName[0] == '/'){
        memmove(shortName, shortName + 1, strlen(shortName));
    }

    strcpy(asset->name,shortName);

    return asset;
}

static void load_file(const char* name){
    assert(LoadedAssets);

    if (IsFileExtension(name,".png")){
        Asset* asset = create_asset(name,sizeof(Texture),AssetTexture);
        Texture texture = LoadTexture(name);
        // TODO add placeholder
        memcpy(asset->memory,&texture,sizeof(Texture));
    }
    else if (IsFileExtension(name,".obj")){
        Asset* asset = create_asset(name,sizeof(Model),AssetModel);
        Model model = LoadModel(name);
        Image image = GenImageChecked(128, 128, 10, 10, RED, WHITE);
        //model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(image);
        memcpy(asset->memory,&model,sizeof(Model));
    }

    INFO("Loaded %s",name);
}

// TODO macro
Model* GetModel(const char* name){
    assert(LoadedAssets);

    for (int i = 0; i < LoadedAssets->count; i++){
        Asset asset = LoadedAssets->assets[i];
        if (strcmp(asset.name, name)){
            Model* model = asset.memory;
            return model;
        }
    }
    return NULL;
}

static int Attemps = 0;

// TODO add better hack to replace this hack
void LoadAssets(const char* folder){
    LoadedAssets = new(Assets);
    strcpy(LoadedAssets->folder,folder);

    INFO("Loading assets...");

    if (DirectoryExists(folder)){
        FilePathList list = LoadDirectoryFilesEx(folder, NULL, true);
        for (int i = 0; i < list.count; i++){
            const char *name = list.paths[i];
            load_file(name);
        }
        UnloadDirectoryFiles(list);
    }else if (Attemps < 2) {
        ERROR("Asset directory not found!");
        // TODO visual studio hack
        Attemps++;
        LoadAssets("../../assets");
    }
}

void UnloadAssets(){
    assert(LoadedAssets);
    M_MemFree(LoadedAssets);
    //INFO("Disposed assets!");
    WARN("TODO dispose assets!");
}

AssetList GetLoadedAssetList(){
    assert(LoadedAssets);

    AssetList list = { 0 };
    list.count = LoadedAssets->count;
    list.names = (char**) M_MemAlloc(LoadedAssets->count*sizeof(char)*256);
    for (int i = 0; i < LoadedAssets->count; i++){
       list.names[i] = LoadedAssets->assets[i].name;
       INFO(list.names[i]);
    }
    return list;
}

void UnloadAssetList(AssetList list){
    M_MemFree(list.names);
}
