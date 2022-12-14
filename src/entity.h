#pragma once

#include "magma.h"

#define COMP_BASE (1 << 1)

typedef void(*UPDATE_FUNC)(void*,float);
typedef void(*DRAW_FUNC)(void*);

typedef unsigned long Components;

struct Entity;
typedef struct Entity Entity;

struct Entity {
    Components components;
    UPDATE_FUNC updateFunc;
    DRAW_FUNC drawFunc;
    Entity* next;
    void* content;
};

typedef struct {
    Entity* root;
} EntityGroup;

typedef void(*POLL_FUNC)(void*,EntityGroup* group);

typedef struct {
    Vector3 pos;
    Vector3 size;
    Vector3 rotation;

    Color tint;
} Base;

Base CreateBase(Vector3 pos, Color tint);

Base CreateDefaultBase();

Base CreateRandomBase();

BoundingBox GetBaseBounds(Base base);

RayCollision GetRayCollisionBase(Base base, Ray ray);
RayCollision GetMouseRayCollisionBase(Base base, Camera camera);

EntityGroup* CreateEntityGroup();

void AddGroupEntity(EntityGroup* group, void* data, size_t size, Components comps,
                    UPDATE_FUNC updateFunc, DRAW_FUNC drawFunc);

bool CheckEntityComponents(Entity* entity, Components filter); // Checks if an entity has a tag/component

size_t PollEntities(EntityGroup* group, Components filter, POLL_FUNC func);

size_t UpdateGroup(EntityGroup* group, float delta);

size_t DrawGroup(EntityGroup* group);
