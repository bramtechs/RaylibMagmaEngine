#include "entity.h"

Base CreateBase(Vector3 pos, Color tint){
    return (Base) {
        -1, pos, Vector3One(), Vector3Zero(), tint
    };
}

Base CreateDefaultBase(){
    return (Base) {
        -1, Vector3Zero(), Vector3One(), Vector3Zero(), WHITE
    };
}

ModelRenderer CreateModelRendererFromFile(const char* modelPath, Base* base){
    Model model = RequestModel(modelPath);
    return CreateModelRenderer(model, base);
}

ModelRenderer CreateModelRenderer(Model model, Base* base){

    // make the base big enough to hold the model
    BoundingBox box = GetModelBoundingBox(model);
    base->pos = box.min;
    base->size = Vector3Subtract(box.max,box.min);

    return (ModelRenderer) {
        -1, model
    };
}

BoundingBox GetBaseBounds(Base base){
    return (BoundingBox) {
        base.pos,
        Vector3Add(base.pos,base.size)
    };
}

RayCollision GetRayCollisionGroup(EntityGroup* groups, Ray ray){
    Array* models = groups->modelRenderers;

    float closestDistance = 10000000;
    RayCollision hit = { 0 };

    for (int i = 0; i < models->count; i++){
        ModelRenderer *render = GetArrayItem(models,i,ModelRenderer);
        Model model = render->model;
        for (int j = 0; j < model.meshCount; j++){
            RayCollision col = GetRayCollisionMesh(ray, model.meshes[j], model.transform);
            if (col.hit && col.distance < closestDistance){
                closestDistance = col.distance;
                hit = col;
            }
        }
    }

    return hit;
}

RayCollision GetRayCollisionBase(Base base, Ray ray){
    BoundingBox box = GetBaseBounds(base);
    return GetRayCollisionBox(ray, box);
}

RayCollision GetMouseRayCollisionBase(Base base, Camera camera){
    Vector2 mouse = GetScaledMousePosition();

    // TODO do some terribleness for this to work with letterboxing
    // TODO turn into own api function
    mouse = Vector2Scale(mouse,GetMagmaScaleFactor());
    mouse.x += GetLeftMagmaWindowOffset();
    mouse.y += GetTopMagmaWindowOffset();

    Ray ray = GetMouseRay(mouse,camera);
    return GetRayCollisionBase(base, ray);
}

bool GetMousePickedBase(EntityGroup* group, Camera* camera, Base** result){
    for (int i = 0; i < group->bases->count; i++) {
        Base* base = GetArrayItem(group->bases, i, Base);

        BoundingBox box = GetBaseBounds(*base);
        RayCollision col = GetMouseRayCollisionBase(*base,*camera);

        if (col.hit){
            *result = base;
            return true;
        }
    }
    *result = NULL;
    return false;
}

EntityGroup* CreateEntityGroup() {
    EntityGroup *g = new(EntityGroup);
    g->entityCount = 0;
    g->bases = MakeArray(sizeof(Base));
    g->modelRenderers = MakeArray(sizeof(ModelRenderer));
    return g;
}

void DisposeEntityGroup(EntityGroup *group){
    DisposeArray(group->bases);
    DisposeArray(group->modelRenderers);
}

//typedef struct {
//    
//} SavedEntityGroup;
//
EntityGroup* ImportEntityGroup(EntityGroup* group, const char* fileName){

}

void ExportEntityGroup(EntityGroup* group, const char* fileName){

}

EntityID AddEntity(EntityGroup* group){
    assert(group != NULL);

    EntityID id = group->entityCount;
    group->entityCount++;
    return id;
}

void* GetEntityComponentRaw(Array* array, EntityID id){
    for(int i = 0; i < array->count; i++){
        void* data = GetArrayItemRaw(array,i);
        // TODO dirty hack 
        EntityID otherId = *((EntityID*) data);
        if (otherId == id){
            return data;
        }
    }
    return NULL;
}


size_t UpdateGroup(EntityGroup* group, float delta){
    assert(group != NULL);

    // IterateArray(group->bases, update_group_entity_bases);
    return group->entityCount;
}

size_t DrawGroup(EntityGroup* group){
    assert(group != NULL);

    // draw modelrenderers
    for (int i = 0; i < group->modelRenderers->count; i++){
        ModelRenderer* renderer = GetArrayItem(group->modelRenderers,i,Base);
        Base* base = GetEntityComponent(group->bases,Base,renderer->id);

        Vector3 rotNorm = Vector3Normalize(base->rotation);
        float rotAmount = Vector3Length(base->rotation);

        BoundingBox box = GetModelBoundingBox(renderer->model);
        DrawModelEx(renderer->model, Vector3Subtract(base->pos,box.min), rotNorm, rotAmount, Vector3One(), base->tint);
    }

    return group->entityCount;
}

void DrawGroupOutlines(EntityGroup* group, Camera *camera){
    for (int i = 0; i < group->bases->count; i++) {
        Base* base = GetArrayItem(group->bases, i, Base);

        BoundingBox box = GetBaseBounds(*base);
        RayCollision col = GetMouseRayCollisionBase(*base,*camera);
        DrawBoundingBox(box, GRAY);
    }

    Base* picked = NULL;
    if (GetMousePickedBase(group,camera,&picked)){
        RayCollision col = GetMouseRayCollisionBase(*picked,*camera);
        BoundingBox box = GetBaseBounds(*picked);
        DrawBoundingBox(box, WHITE);
    }
}
