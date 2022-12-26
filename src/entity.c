#include "entity.h"

Base CreateBase(EntityID id, Vector3 pos, Color tint){
    return (Base) {
        id, pos, Vector3One(), Vector3Zero(), tint
    };
}

Base CreateDefaultBase(EntityID id){
    return (Base) {
        id, Vector3Zero(), Vector3One(), Vector3Zero(), WHITE
    };
}

ModelRenderer CreateModelRendererFromFile(EntityID id, const char* modelPath, Base* base){
    Model model = RequestModel(modelPath);
    return CreateModelRenderer(id, model, base);
}

ModelRenderer CreateModelRenderer(EntityID id, Model model, Base* base){

    // make the base big enough to hold the model
    BoundingBox box = GetModelBoundingBox(model);
    base->pos = box.min;
    base->size = Vector3Subtract(box.max,box.min);

    return (ModelRenderer) {
        id, model
    };
}

BoundingBox GetBaseBounds(Base base){
    return (BoundingBox) {
        base.pos,
        Vector3Add(base.pos,base.size)
    };
}

RayCollision GetRayCollisionGroup(EntityGroup* group, Ray ray){
    ListIterator it = IterateListItemsEx(group->components,COMP_MODEL_RENDERER);

    float closestDistance = 10000000;
    RayCollision hit = { 0 };

    void* renderPtr = NULL;
    while (IterateNextItem(&it,&renderPtr)){
        ModelRenderer *render = (ModelRenderer*) renderPtr;
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
    ListIterator it = IterateListItemsEx(group->components,COMP_BASE);

    void* basePtr = NULL;
    while (IterateNextItem(&it, &basePtr)){
        Base* base = (Base*) basePtr;

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
    g->components = MakeList();
    return g;
}

void DisposeEntityGroup(EntityGroup *group){
    // TODO
}

EntityGroup* ImportEntityGroup(EntityGroup* group, const char* fileName){
    // TODO
}

void ExportEntityGroup(EntityGroup* group, const char* fileName){
    // TODO
}

EntityID AddEntity(EntityGroup* group){
    assert(group != NULL);

    EntityID id = group->entityCount;
    group->entityCount++;
    return id;
}

void AddEntityComponent(EntityGroup* group, ItemType type, EntityID* data, size_t size) {
    PushList(group->components,data,size,type);
}

void* GetEntityComponent(EntityGroup* group, EntityID id, ItemType filter){
    ListIterator it = IterateListItemsEx(group->components,filter);

    void* dataPtr = NULL;
    while (IterateNextItem(&it,&dataPtr)){
        // TODO dirty hack 
        EntityID otherId = *((EntityID*) dataPtr);
        if (otherId == id){
            return dataPtr;
        }
    }
    return NULL;
}


size_t UpdateGroup(EntityGroup* group, float delta){
    assert(group != NULL);

    // IterateArray(group->bases, update_group_entity_bases);
    return group->entityCount;
}

size_t DrawGroup(EntityGroup* group, Camera* camera, bool drawOutlines){
    assert(group != NULL);

    ListIterator it = IterateListItems(group->components);

    void* compPtr = NULL;
    ItemType type = { 0 };
    while (IterateNextItemEx(&it,&type,&compPtr)){
        switch (type){
            case COMP_MODEL_RENDERER:
                {
                    // draw modelrenderers
                    ModelRenderer* renderer = (ModelRenderer*) compPtr;
                    Base* base = GetEntityComponent(group,renderer->id,COMP_BASE);

                    if (base == NULL){
                        assert(false); // model renderer has no base! TODO shouldn't crash
                    }

                    Vector3 rotNorm = Vector3Normalize(base->rotation);
                    float rotAmount = Vector3Length(base->rotation);

                    BoundingBox box = GetModelBoundingBox(renderer->model);
                    DrawModelEx(renderer->model, Vector3Subtract(base->pos,box.min), rotNorm, rotAmount, Vector3One(), base->tint);
                } break;
            case COMP_BASE:
                {
                    if (drawOutlines){
                        Base* base = (Base*) compPtr;
                        BoundingBox box = GetBaseBounds(*base);
                        RayCollision col = GetMouseRayCollisionBase(*base,*camera);
                        DrawBoundingBox(box, col.hit ? WHITE:GRAY);
                    }
                } break;
            default:
                break;
        }
    }
    return group->entityCount;
}

void DrawGroupOutlines(EntityGroup* group, Camera *camera){

    Base* picked = NULL;
    if (GetMousePickedBase(group,camera,&picked)){
        RayCollision col = GetMouseRayCollisionBase(*picked,*camera);
        BoundingBox box = GetBaseBounds(*picked);
        DrawBoundingBox(box, WHITE);
    }
}
