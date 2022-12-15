#include "entity.h"

Base CreateBase(Vector3 pos, Color tint){
    return (Base) {
        pos, Vector3One(), Vector3One(), tint
    };
}

Base CreateDefaultBase(){
    return (Base) {
        Vector3Zero(), Vector3One(), Vector3Zero(), WHITE
    };
}

Base CreateRandomBase(){
    int MAX_RANGE = 100;

    Vector3 pos = {
        GetRandomValue(-MAX_RANGE,MAX_RANGE),
        GetRandomValue(-MAX_RANGE,MAX_RANGE),
        GetRandomValue(-MAX_RANGE,MAX_RANGE)
    };

    Color col = {
        GetRandomValue(100,255),
        GetRandomValue(100,255),
        GetRandomValue(100,255),
        255
    };

    return CreateBase(pos,col);
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

EntityGroup* CreateEntityGroup(){
    EntityGroup *g = new(EntityGroup);
    g->root = new(Entity);
    return g;
}

// TODO horrible name
void add_group_entity_child(Entity* root, void* data, uint size, Components comps,
                            UPDATE_FUNC updateFunc, DRAW_FUNC drawFunc){
    assert(root != NULL);

    if (root->next != NULL){
        add_group_entity_child(root->next,data,size,comps,updateFunc,drawFunc);
        return;
    }

    root->next = MemAlloc(sizeof(Entity));
    root->next->components = comps;
    root->next->updateFunc = updateFunc;
    root->next->drawFunc = drawFunc;

    root->next->content = MemAlloc(size);
    memcpy(root->next->content,data,size);
}

void AddGroupEntity(EntityGroup* group, void* data, uint size, Components comps,
                    UPDATE_FUNC updateFunc, DRAW_FUNC drawFunc){
    assert(group != NULL);

    add_group_entity_child(group->root,data,size,comps,updateFunc,drawFunc);
}

bool CheckEntityComponents(Entity* entity, Components filter){
    return (entity->components & filter) > 0;
}

uint PollEntities(EntityGroup* group, Components filter, POLL_FUNC func){
    Entity *next = group->root->next;
    uint counter = 0;
    while (next != NULL){
        if (CheckEntityComponents(next,filter)){
            if (func != NULL){
                (*func)(*next,group);
            }
            counter ++;
        }
        next = next->next;
    }
    return counter;
}

uint UpdateGroup(EntityGroup* group, float delta){
    assert(group != NULL);

    Entity *next = group->root->next;
    uint counter = 0;
    while (next != NULL){
        UPDATE_FUNC func = next->updateFunc;
        if (func != NULL){
            (*func)(next->content,delta);
        }
        next = next->next;
        counter ++;
    }
    return counter;
}

uint DrawGroup(EntityGroup* group){
    assert(group != NULL);

    Entity *next = group->root->next;
    uint counter = 0;
    while (next != NULL){
        DRAW_FUNC func = next->drawFunc;
        if (func != NULL){
            (*func)(next->content);
        }
        next = next->next;
        counter++;
    }
    return counter;
}
