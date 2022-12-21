#pragma once

#include "raylib.h"
#include "raymath.h"
#include "entity.h"

typedef struct {
    Camera camera;
    EntityID floorCasterID;
} PlayerFPS;

PlayerFPS SpawnPlayerFPS(EntityGroup* group);

void SetPlayerFPSAngle(PlayerFPS *player, int lookAtDeg);
void SetPlayerFPSFov(PlayerFPS *player, int fovDeg);
void FocusPlayerFPS(PlayerFPS *player);
void UnfocusPlayerFPS(PlayerFPS *player);
void TeleportPlayerFPS(PlayerFPS *player, Vector3 position);
