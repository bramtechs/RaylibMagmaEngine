#pragma once

#include "raylib.h"
#include "raymath.h"
#include "window.h"
#include "memory.h"
#include "array.h"

// poor man's ECS imitation, it's probably slow

typedef void(*UPDATE_FUNC)(void*,float);
typedef void(*DRAW_FUNC)(void*);

typedef size_t EntityID;

typedef struct {
    EntityID id;
    Vector3 pos;
    Vector3 size;
    Vector3 rotation;

    Color tint;
} Base;

typedef struct {
    EntityID entityCount; 
    Array* bases;
} EntityGroup;

Base CreateBase(Vector3 pos, Color tint);

Base CreateDefaultBase();

Base CreateRandomBase();

BoundingBox GetBaseBounds(Base base);

RayCollision GetRayCollisionBase(Base base, Ray ray);
RayCollision GetMouseRayCollisionBase(Base base, Camera camera);

EntityGroup* CreateEntityGroup();

EntityID AddEntity(EntityGroup* group);

// nested macro's, seems like a good idea!
#define AddEntityComponent(ARRAY_PTR, TYPE, DATA_PTR, ID) \
    DATA_PTR->id = ID; \
    PushArray(ARRAY_PTR,TYPE,DATA_PTR)

size_t UpdateGroup(EntityGroup* group, float delta);

size_t DrawGroup(EntityGroup* group);
