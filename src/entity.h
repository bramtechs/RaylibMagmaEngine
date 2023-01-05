#pragma once

#include <memory>
#include <map> 

#include "engine.h"
#include "window.h"
#include "memory.h"
#include "assets.h"

// poor man's ECS imitation, it's probably slow

#define COMP_ALL                0
#define COMP_BASE               1
#define COMP_MODEL_RENDERER     2

typedef size_t EntityID;
typedef size_t ComponentType;

struct Component {
    ComponentType type;
    Component(ComponentType type);

    virtual void Update(float delta) = 0;
    virtual void Draw(Camera* camera, bool drawOutlines = false) = 0;
};

struct Base : Component {
    MBoundingBox bounds;
    RColor tint;

    Base(RVector3 pos, Color tint);
    Base();

    void Translate(RVector3 offset);
    inline void Translate(float x, float y, float z);
    inline void TranslateX(float v);
    inline void TranslateY(float v);
    inline void TranslateZ(float v);

    void SetCenter(RVector3 pos);
    inline void ResetTranslation();
    
    bool IsPickedByMouse();
    RRayCollision GetMouseRayCollision(Base base, Camera camera);

    // read only
    RVector3 center;
    RVector3 size;
    RVector3 halfSize;
};

struct ModelRenderer : Component {
    Model model;
    bool accurate;
    Vector3 offset; //from base center

    ModelRenderer(Base* base, RModel model);

    void Update(float delta);
    void Draw(Camera* camera, bool drawOutlines = false);
};

struct EntityGroup {
    size_t count; 
    std::multimap<EntityID,Component*> components;

    EntityID AddEntity();
    template <class T> T* AddComponent(EntityID id, T comp);

    void Update(float delta);
    void Draw(Camera* camera, bool drawOutlines = false);

    void ImportFromDisk(const char* fileName);
    void ExportToDisk(const char* fileName);

    bool MousePickBase(Camera camera, Base** result);
    bool MousePickBase(Camera camera, Base** result, RayCollision* col);

    Component* GetComponent(EntityID id, ComponentType type);
    std::multimap<EntityID, Component*> GetComponents(ComponentType type);
    RRayCollision GetRayCollision(RRay ray);
};
