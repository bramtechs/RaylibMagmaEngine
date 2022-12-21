#include "player_fps.h"

PlayerFPS SpawnPlayerFPS(float eyeHeight){
    PlayerFPS player = { 0 };
    player.eyeHeight = eyeHeight;

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

Vector3 UpdatePlayerFPS(PlayerFPS* player, EntityGroup* group, float delta){
    Ray ray = { 0 };

    Vector3 offset = {0,player->eyeHeight,0};
    ray.position = Vector3Add(player->camera.position,offset);
    ray.direction = (Vector3) {0,-1,0};

    RayCollision col = GetRayCollisionModels(group, ray);

    // move player to hit point
    // Vector3 dest = Vector3Add(col.point, offset);
    // player->camera.position = dest;
    
    return col.point;
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
