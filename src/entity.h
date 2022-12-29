#pragma once

#include "raylib.h"
#include "raymath.h"
#include "list.h"
#include "window.h"
#include "memory.h"
#include "assets.h"

// poor man's ECS imitation, it's probably slow

#define COMP_ALL                0
#define COMP_BASE               1
#define COMP_MODEL_RENDERER     2

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
    size_t entityCount; 
    List* components;
} EntityGroup;

Base CreateBase(EntityID id, Vector3 pos, Color tint);
Base CreateDefaultBase(EntityID id);

ModelRenderer CreateModelRendererFromFile(EntityID id, const char* modelPath, Base* base);
ModelRenderer CreateModelRenderer(EntityID id, Model model, Base* base);

BoundingBox GetBaseBounds(Base base);

RayCollision GetRayCollisionGroup(EntityGroup* groups, Ray ray);
RayCollision GetRayCollisionBase(Base base, Ray ray);
RayCollision GetMouseRayCollisionBase(Base base, Camera camera);

bool GetMousePickedBase(EntityGroup* group, Camera camera, Base** result);
bool GetMousePickedBaseEx(EntityGroup* group, Camera camera, Base** result, RayCollision* col);

EntityGroup* CreateEntityGroup();
void DisposeEntityGroup(EntityGroup *group); // NOTE: custom component arrays need to be disposed manually!

EntityGroup* ImportEntityGroup(EntityGroup* group, const char* fileName);
void ExportEntityGroup(EntityGroup* group, const char* fileName);

EntityID AddEntity(EntityGroup* group);

void AddEntityComponent(EntityGroup* group, ItemType type, EntityID* data, size_t size);

void* GetEntityComponent(EntityGroup* group, EntityID id, ItemType filter);

size_t UpdateGroup(EntityGroup* group, float delta);

size_t DrawGroup(EntityGroup* group, Camera* camera, bool drawOutlines);
