#include "player_fps.h"

PlayerFPS SpawnPlayerFPS(EntityGroup* group, float eyeHeight){
    PlayerFPS player = { 0 };

    EntityID id = player.floorCasterID = AddEntity(group);
    FloorCaster caster = CreateFloorCaster(eyeHeight);
    AddEntityComponent(group->floorCasters,FloorCaster,&caster,id);

    Camera* cam = &player.camera;
    cam->projection = CAMERA_PERSPECTIVE;
    cam->up = (Vector3){ 0.0f, 1.0f, 0.f };
    SetPlayerFPSAngle(&player,0);
    SetPlayerFPSFov(&player,80);

    FocusPlayerFPS(&player);
    return player;
}

void SetPlayerFPSAngle(PlayerFPS *player, int lookAtDeg){
    Vector3 pos = player->camera.position;
    Vector3 offset = {
        cosf(lookAtDeg*DEG2RAD),
        sinf(lookAtDeg*DEG2RAD),
        0
    };
    player->camera.target = Vector3Add(pos,offset);
}

void SetPlayerFPSFov(PlayerFPS *player, int fovDeg){
    player->camera.fovy = fovDeg;
}

void FocusPlayerFPS(PlayerFPS *player){
    SetCameraMode(player->camera,CAMERA_FIRST_PERSON);
}

void UnfocusPlayerFPS(PlayerFPS *player){
    SetCameraMode(player->camera,CAMERA_CUSTOM);
}

void TeleportPlayerFPS(PlayerFPS *player, Vector3 pos){
    player->camera.position = pos;
}
