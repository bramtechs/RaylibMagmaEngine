#include "entity.h"

Base CreateBase(EntityID id, Vector3 pos, Color tint){
    BoundingBox box = {
        pos,
        Vector3Add(pos,Vector3One())
    };

    return (Base) {
        id, box, tint
    };
}

void TranslateBase(Base* base, Vector3 offset){
    base->bounds.min = Vector3Add(base->bounds.min,offset);
    base->bounds.max = Vector3Add(base->bounds.max,offset);
}

void SetBaseCenter(Base* base, Vector3 pos){
    base->bounds.min = Vector3Subtract(pos,base->halfSize);
    base->bounds.max = Vector3Add(pos,base->halfSize);
}

ModelRenderer CreateModelRendererFromFile(EntityID id, const char* modelPath, Base* base){
    Model model = RequestModel(modelPath);
    return CreateModelRenderer(id, model, base);
}

ModelRenderer CreateModelRenderer(EntityID id, Model model, Base* base){

    // make the base big enough to hold the model
    BoundingBox modelBox = GetModelBoundingBox(model);
    
    Vector3 size = Vector3Subtract(modelBox.max, modelBox.min);
    base->bounds.max = Vector3Add(base->bounds.min,size);

    Vector3 modelCenter = Vector3Add(modelBox.min,Vector3Scale(size,0.5f));
    Vector3 offset = Vector3Subtract(base->center,modelCenter);

    ModelRenderer render;
    render.id = id;
    render.model = model;
    render.accurate = false;
    render.offset = offset;

    return render;
}

RayCollision GetRayCollisionGroup(EntityGroup* group, Ray ray){
    ListIterator it = IterateListItemsEx(group->components,COMP_MODEL_RENDERER);

    float closestDistance = 10000000;
    RayCollision hit = { 0 };

    void* renderPtr = NULL;
    while (IterateNextItem(&it,&renderPtr)){
        ModelRenderer *render = (ModelRenderer*) renderPtr;
        Base* base = GetEntityComponent(group,render->id,COMP_BASE);
        Model model = render->model;

        if (render->accurate){ // do per triangle collisions

            Vector3 offset = Vector3Add(base->center,render->offset);
            for (int j = 0; j < model.meshCount; j++){
                RayCollision col = GetRayCollisionMesh(ray, model.meshes[j],
                                                       MatrixTranslate(offset.x,offset.y,offset.z));

                if (col.hit && col.distance < closestDistance){
                    closestDistance = col.distance;
                    hit = col;
                }
            }
        } else { // do bounds collision
            RayCollision col = GetRayCollisionBox(ray,base->bounds);
            if (col.hit && col.distance < closestDistance){
                closestDistance = col.distance;
                hit = col;
            }
        }
    }

    return hit;
}

RayCollision GetMouseRayCollisionBase(Base base, Camera camera){
    Vector2 mouse = GetScaledMousePosition();

    // TODO do some terribleness for this to work with letterboxing
    // TODO turn into own api function
    mouse = Vector2Scale(mouse,GetMagmaScaleFactor());
    mouse.x += GetLeftMagmaWindowOffset();
    mouse.y += GetTopMagmaWindowOffset();

    Ray ray = GetMouseRay(mouse,camera);
    return GetRayCollisionBox(ray, base.bounds);
}

bool GetMousePickedBase(EntityGroup* group, Camera camera, Base** result){
    RayCollision col = { 0 };
    return GetMousePickedBaseEx(group,camera,result,&col);
}

bool GetMousePickedBaseEx(EntityGroup* group, Camera camera, Base** result, RayCollision* col){
    ListIterator it = IterateListItemsEx(group->components,COMP_BASE);

    void* basePtr = NULL;
    while (IterateNextItem(&it, &basePtr)){
        Base* base = (Base*) basePtr;

        RayCollision rayCol = GetMouseRayCollisionBase(*base,camera);

        if (rayCol.hit){
            *result = base;
            *col = rayCol;
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

    ListIterator it = IterateListItems(group->components);

    void* compPtr = NULL;
    ItemType type = { 0 };
    while (IterateNextItemEx(&it,&type,&compPtr)){
        switch (type){
            case COMP_BASE:
                {
                    Base* base = (Base*) compPtr;
                    base->size = Vector3Subtract(base->bounds.max, base->bounds.min);
                    base->halfSize = Vector3Scale(base->size, 0.5f);
                    base->center = Vector3Add(base->bounds.min, base->halfSize);
                } break;
            default:
                break;
        }
    }
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

                    DrawModelEx(renderer->model, Vector3Add(base->center,renderer->offset), Vector3Zero(), 0, Vector3One(), base->tint);
                } break;
            case COMP_BASE:
                {
                    Base* base = (Base*) compPtr;
                    RayCollision col = GetMouseRayCollisionBase(*base,*camera);
                    Color tint = col.hit ? WHITE:GRAY;
                    DrawBoundingBox(base->bounds, tint);
                    DrawPoint3D(base->center, col.hit ? WHITE:GRAY);
                } break;
            default:
                break;
        }
    }
    return group->entityCount;
}

void DrawGroupOutlines(EntityGroup* group, Camera camera){

    Base* picked = NULL;
    if (GetMousePickedBase(group,camera,&picked)){
        RayCollision col = GetMouseRayCollisionBase(*picked,camera);
        DrawBoundingBox(picked->bounds, WHITE);
    }
}
