#pragma once

#include "raylib.h"
#include "raymath.h"
#include "window.h"
#include "memory.h"
#include "assets.h"
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
    EntityID id;
    Model model;
} ModelRenderer;

typedef struct {
    EntityID entityCount; 
    Array* bases;
    Array* modelRenderers;
} EntityGroup;

Base CreateBase(Vector3 pos, Color tint);
Base CreateDefaultBase();

ModelRenderer CreateModelRendererFromFile(const char* modelPath, Base* base);
ModelRenderer CreateModelRenderer(Model model, Base* base);

BoundingBox GetBaseBounds(EntityID id);

RayCollision GetRayCollisionGroup(EntityGroup* groups, Ray ray);
RayCollision GetRayCollisionBase(Base base, Ray ray);
RayCollision GetMouseRayCollisionBase(Base base, Camera camera);

EntityGroup* CreateEntityGroup();
void DisposeEntityGroup(EntityGroup *group); // NOTE: custom component arrays need to be disposed manually!

EntityID AddEntity(EntityGroup* group);

// nested macro's, seems like a good idea!
#define AddEntityComponent(ARRAY_PTR, TYPE, DATA_PTR, ID) \
    (DATA_PTR)->id = ID; \
    PushArray(ARRAY_PTR,TYPE,DATA_PTR)

void* GetEntityComponentRaw(Array* array, EntityID id);

#define GetEntityComponent(ARRAY_PTR, TYPE, ID) \
    (TYPE*) GetEntityComponentRaw(ARRAY_PTR,ID)

size_t UpdateGroup(EntityGroup* group, float delta);

size_t DrawGroup(EntityGroup* group);

void DrawGroupOutlines(EntityGroup* group, Camera *camera);
