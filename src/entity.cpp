#include "entity.h"

static EntityGroup* Group = nullptr;

// abstract component 
Component::Component(ComponentType type){
    this->type = type;
}

// component base
Base::Base(RVector3 pos, Color tint) : Component(COMP_BASE){
    MBoundingBox box = {
        pos,
        pos + RVector3::One()
    };

    this->bounds = box;
    this->tint = tint;
}

Base::Base() : Base(RVector3::Zero(), WHITE) {
}

void Base::Translate(RVector3 offset){
    bounds.min += offset;
    bounds.max += offset;
}

inline void Base::Translate(float x, float y, float z){
    Translate({ x,y,z });
}

inline void Base::TranslateX(float v){
    Translate({v,0.f,0.f});
}

inline void Base::TranslateY(float v){
    Translate({0.f,v,0.f});
}

inline void Base::TranslateZ(float v){
    Translate({0.f,0.f,v});
}

void Base::SetCenter(RVector3 pos){
    bounds.min = pos - halfSize;
    bounds.max = pos + halfSize;
}

inline void Base::ResetTranslation(){
    SetCenter(RVector3::Zero());
}

bool Base::IsPickedByMouse() {
    
}

RRayCollision Base::GetMouseRayCollision(Base base, Camera camera)
{
    RVector2 mouse = GetScaledMousePosition();

    // TODO do some terribleness for this to work with letterboxing
    // TODO turn into own api function
    mouse = Vector2Scale(mouse,GetMagmaScaleFactor());
    mouse.x += GetLeftMagmaWindowOffset();
    mouse.y += GetTopMagmaWindowOffset();

    RRay ray = RRay::GetMouse(camera);
    return RRayCollision(ray, base.bounds);
}

// component model renderer
ModelRenderer::ModelRenderer(Base* base, RModel model) : Component(COMP_MODEL_RENDERER) {
    // make the base big enough to hold the model
    MBoundingBox modelBox = MBoundingBox(model.GetBoundingBox());
    
    RVector3 size = modelBox.max - modelBox.min;
    base->bounds.max = base->bounds.min + size;

    RVector3 modelCenter = modelBox.min + (size * 0.5f);
    RVector3 offset = base->center - modelCenter;

    this->model = model;
    this->accurate = false;
    this->offset = offset;
}

void ModelRenderer::Update(float delta) {

}

void ModelRenderer::Draw(Camera* camera, bool drawOutlines = false) {
	Base* base = (Base*) GetEntityComponent(group,renderer->id,COMP_BASE);

	if (base == NULL){
		assert(false); // model renderer has no base! TODO shouldn't crash
	}

	DrawModelEx(renderer->model, Vector3Add(base->center,renderer->offset), Vector3Zero(), 0, Vector3One(), base->tint);
}

// entity core 
Component* EntityGroup::GetComponent(EntityID id, ComponentType type) {
    for (const auto& item : components) {
        if (item.first == type && item.first == id) {
            return item.second;
        }
    }
    assert(false);
}

std::multimap<EntityID, Component*> EntityGroup::GetComponents(ComponentType type)
{
    auto results = std::multimap<EntityID, Component*>();
    for (const auto& comp : components) {
        if (comp.second->type == type) {
            results.insert({ comp.first,comp.second });
        }
    }
    return results;
}

RRayCollision EntityGroup::GetRayCollision(RRay ray){

    float closestDistance = 10000000;
    RayCollision hit = { 0 };

    for (const auto& comp : GetComponents(COMP_MODEL_RENDERER)){
        auto render = (ModelRenderer*) comp.second;
        auto base = (Base*) GetComponent(comp.first,COMP_BASE);

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

EntityGroup::EntityGroup() {
    this->count = 0;
    this->components = std::multimap<ComponentType,Component*>();
}

EntityID EntityGroup::AddEntity() {
    EntityID id = count;
    count++;
    return id;
}

template <class T> T* EntityGroup::AddComponent(EntityID id, T comp) {
    T* heapComp = new T;
    memcpy(headComp, &comp, sizeof(T));
    components.insert({ id, heapComp });
}

void EntityGroup::Update(float delta){
    for (const auto& comp : components) {
        comp.second->Update(delta);
    }
}

void EntityGroup::Draw(Camera* camera, bool drawOutlines = false){
    for (const auto& comp : components) {
        comp.second->Draw(camera,drawOutlines);
    }
}

void DrawGroup(Camera* camera, bool drawOutlines = false){
    ListIterator it = IterateListItems(group->components);

    void* compPtr = NULL;
    ItemType type = { 0 };
    while (IterateNextItemEx(&it,&type,&compPtr)){
        switch (type){
            case COMP_MODEL_RENDERER:
                {
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

void EntityGroup::ImportFromDisk(const char* fileName) {

}

void EntityGroup::ExportToDisk(const char* fileName){
    const char* path = TextFormat("%s/%s.comps",GetAssetFolder(),fileName);
    //ExportList(group->components, path);
    INFO("Exported entity component to %s",path);
}
