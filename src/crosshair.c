#include "crosshair.h"
#include "screen.h"

#include "raylib.h"
#include "raymath.h"

Vector2 Crosshair_GetCenterPosition(const Texture2D* p_crosshairTextrure)
{
  float crosshairHalfWidth = p_crosshairTextrure->width * CROSSHAIR_SCALE / 2.0f;
  float crosshairHalfHeight = p_crosshairTextrure->height * CROSSHAIR_SCALE / 2.0f;

  Vector2 noProblemPosition = { (float)GetMouseX(), (float)GetMouseY() };
  Vector2 minBounds = { crosshairHalfWidth, crosshairHalfHeight };
  Vector2 maxBounds = { (float)SCREEN_WIDTH - crosshairHalfWidth, (float)SCREEN_HEIGHT - crosshairHalfHeight};

  return Vector2Clamp(noProblemPosition, minBounds, maxBounds);
}

void Crosshair_Draw(const Texture2D* p_crosshairTexture, Vector2 crosshairCenterPos)
{
  float crosshairWidth = p_crosshairTexture->width * CROSSHAIR_SCALE;
  float crosshairHeight = p_crosshairTexture->height * CROSSHAIR_SCALE;

  DrawTexturePro(
    *p_crosshairTexture, 
    (Rectangle){0.0f, 0.0f, p_crosshairTexture->width, p_crosshairTexture->height},
    (Rectangle){crosshairCenterPos.x, crosshairCenterPos.y, crosshairWidth, crosshairHeight },
    (Vector2){ crosshairWidth / 2, crosshairHeight / 2 },
    0.0f,
    WHITE
    );

  #ifdef DEBUG
  DrawRectangleLines((int)(crosshairCenterPos.x - crosshairWidth / 2.0f),
                    (int)(crosshairCenterPos.y - crosshairHeight / 2.0f),
                    (int)crosshairWidth,
                    (int)crosshairHeight,
                    GREEN);
  #endif
}