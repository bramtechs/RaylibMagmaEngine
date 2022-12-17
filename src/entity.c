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

EntityGroup* CreateEntityGroup() {
    EntityGroup *g = new(EntityGroup);
    g->entityCount = 0;
    g->bases = MakeArray(sizeof(Base));
    return g;
}

EntityID AddEntity(EntityGroup* group){
    assert(group != NULL);

    EntityID id = group->entityCount;
    group->entityCount++;
    return id;
}

void update_group_entity(int i, void* ptr){
    // TODO add velocity to position
}

size_t UpdateGroup(EntityGroup* group, float delta){
    assert(group != NULL);

    IterateArray(group->bases, update_group_entity);
    return group->entityCount;
}

void draw_group_entity(int i, void* ptr){
    // TODO draw bounding box
}

size_t DrawGroup(EntityGroup* group){
    assert(group != NULL);

    IterateArray(group->bases, update_group_entity);
    return group->entityCount;
}
