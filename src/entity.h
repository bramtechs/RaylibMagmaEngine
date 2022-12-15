#pragma once

#include "common.h"
#include "raylib.h"
#include "raymath.h"
#include "window.h"
#include "memory.h"

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

typedef void(*POLL_FUNC)(Entity,EntityGroup* group);

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

void AddGroupEntity(EntityGroup* group, void* data, uint size, Components comps,
                    UPDATE_FUNC updateFunc, DRAW_FUNC drawFunc);

bool CheckEntityComponents(Entity* entity, Components filter); // Checks if an entity has a tag/component

uint PollEntities(EntityGroup* group, Components filter, POLL_FUNC func);

uint UpdateGroup(EntityGroup* group, float delta);

uint DrawGroup(EntityGroup* group);
