#include "entity.h"


Base CreateBase(Vector3 pos, Color tint){
    return (Base) {
        -1, pos, Vector3One(), Vector3One(), tint
    };
}

Base CreateDefaultBase(){
    return (Base) {
        -1, Vector3Zero(), Vector3One(), Vector3Zero(), WHITE
    };
}

ModelRenderer CreateModelRenderer(const char* modelPath, Base* base){
    Model model = RequestModel(modelPath);

    // offset the Base with the loaded geometry
    BoundingBox box = GetModelBoundingBox(model);
    base->pos = Vector3Subtract(base->pos,box.min);
    base->size = Vector3Subtract(box.max,box.min);

    return (ModelRenderer) {
        -1,
        model
    };
}

BoundingBox GetBaseBounds(Base base){
    Vector3 halfSize = Vector3Scale(base.size, 0.5f);
    Vector3 startCorner = Vector3Subtract(base.pos, halfSize);

    return (BoundingBox) {
        startCorner,
        Vector3Add(startCorner,base.size)
    };
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

EntityGroup* CreateEntityGroup() {
    EntityGroup *g = new(EntityGroup);
    g->entityCount = 0;
    g->bases = MakeArray(sizeof(Base));
    g->modelRenderers = MakeArray(sizeof(ModelRenderer));
    return g;
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

void draw_group_entity_renderers(int _, void* ptr){
}

size_t DrawGroup(EntityGroup* group){
    assert(group != NULL);

    for (int i = 0; i < group->bases->count; i++){
        Base* base = GetArrayItem(group->bases,i,Base);
        DrawCubeWires(base->pos, base->size.x, base->size.y, base->size.z, base->tint);
    }

    for (int i = 0; i < group->modelRenderers->count; i++){
        ModelRenderer* renderer = GetArrayItem(group->bases,i,Base);
        Base* base = (Base*) GetEntityComponent(group->bases,Base,renderer->id);
        DrawModel(renderer->model, base->pos, 1.f, base->tint);
    }

    IterateArray(group->modelRenderers, draw_group_entity_renderers);
    return group->entityCount;
}
