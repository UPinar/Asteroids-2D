#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "raylib.h"

#define   CROSSHAIR_SCALE   2.0F

Vector2 Crosshair_GetCenterPosition(const Texture2D* p_crosshairTextrure);
void Crosshair_Draw(const Texture2D* p_crosshairTexture, Vector2 crosshairCenterPos);

#endif  // CROSSHAIR_H