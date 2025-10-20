#include "spaceship.h"
#include "screen.h"

#include "raymath.h"

void Spaceship_UpdateSpaceshipRotation(Spaceship_t* spaceship, Vector2 crosshairCenter)
{
  Vector2 centerOfTheScreen = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
  Vector2 spaceshipDirection = Vector2Subtract(crosshairCenter, centerOfTheScreen);

  // atan2f returns radians; convert to degrees for DrawTexturePro
  float rotationAngle = atan2f(spaceshipDirection.y, spaceshipDirection.x) * (180.0f / PI);

  // Raylib's rotation is clockwise-positive when using DrawTexturePro angles in degrees
  spaceship->m_rotation_angle = rotationAngle;
}

static void Spaceship_CheckAndLimitBounds(Spaceship_t* spaceship)
{
  float halfWidth = SPACESHIP_WIDTH / 2.0f;
  float halfHeight = SPACESHIP_HEIGHT / 2.0f;

  if (spaceship->m_center.x + halfWidth > SCREEN_WIDTH)
    spaceship->m_center.x = SCREEN_WIDTH - halfWidth;
  else if (spaceship->m_center.x - halfWidth < 0)
    spaceship->m_center.x = halfWidth;

  if (spaceship->m_center.y + halfHeight > SCREEN_HEIGHT)
    spaceship->m_center.y = SCREEN_HEIGHT - halfHeight;
  else if (spaceship->m_center.y - halfHeight < 0)
    spaceship->m_center.y = halfHeight;
}

static void Spaceship_GoLeft(Spaceship_t* p_spaceship, float deltaTime)
{
  p_spaceship->m_direction.x = -1.0f;
  p_spaceship->m_direction = Vector2Normalize(p_spaceship->m_direction);
  p_spaceship->m_center.x += p_spaceship->m_direction.x * deltaTime * SPACESHIP_SPEED;
  Spaceship_CheckAndLimitBounds(p_spaceship);
}
static void Spaceship_GoUp(Spaceship_t* p_spaceship, float deltaTime)
{
  p_spaceship->m_direction.y = -1.0f;
  p_spaceship->m_direction = Vector2Normalize(p_spaceship->m_direction);
  p_spaceship->m_center.y += p_spaceship->m_direction.y * deltaTime * SPACESHIP_SPEED;
  Spaceship_CheckAndLimitBounds(p_spaceship);
}
static void Spaceship_GoDown(Spaceship_t* p_spaceship, float deltaTime)
{
  p_spaceship->m_direction.y = 1.0f;
  p_spaceship->m_direction = Vector2Normalize(p_spaceship->m_direction);
  p_spaceship->m_center.y += p_spaceship->m_direction.y * deltaTime * SPACESHIP_SPEED;
  Spaceship_CheckAndLimitBounds(p_spaceship);
}
static void Spaceship_GoRight(Spaceship_t* p_spaceship, float deltaTime)
{
  p_spaceship->m_direction.x = 1.0f;
  p_spaceship->m_direction = Vector2Normalize(p_spaceship->m_direction);
  p_spaceship->m_center.x += p_spaceship->m_direction.x * deltaTime * SPACESHIP_SPEED;
  Spaceship_CheckAndLimitBounds(p_spaceship);
}

void Spaceship_UpdateSpaceshipPosition(Spaceship_t* spaceship, float deltaTime)
{
  if (IsKeyDown(KEY_D))
    Spaceship_GoRight(spaceship, deltaTime);
  if (IsKeyDown(KEY_A))
    Spaceship_GoLeft(spaceship, deltaTime);
  if (IsKeyDown(KEY_W))
    Spaceship_GoUp(spaceship, deltaTime);
  if (IsKeyDown(KEY_S))
    Spaceship_GoDown(spaceship, deltaTime);
}

bool Spaceship_CheckIfSpaceshipIsDead(Spaceship_t* spaceship, 
                                      Asteroid_t* asteroidsArray, 
                                      size_t currentAsteroidCount)
{
  Rectangle spaceshipRect = {
    spaceship->m_center.x - SPACESHIP_WIDTH / 2.0f,
    spaceship->m_center.y - SPACESHIP_HEIGHT / 2.0f,
    SPACESHIP_WIDTH,
    SPACESHIP_HEIGHT
  };

  Asteroid_t* asteroid = nullptr;
  Rectangle asteroidRect = { 0 };
  for (int i = 0; i < currentAsteroidCount; i++)
  {
    asteroid = &asteroidsArray[i];
    if (asteroid->m_active)
    {
      asteroidRect = (Rectangle){
        asteroid->m_position.x - asteroid->m_width / 2.0f,
        asteroid->m_position.y - asteroid->m_height / 2.0f,
        asteroid->m_width,
        asteroid->m_height
      };

      if (CheckCollisionRecs(spaceshipRect, asteroidRect))
        return true;
    }
  }
  return false;
}

void Spaceship_Draw(const Texture2D* p_spaceshipTexture, Spaceship_t* p_spaceship)
{
  DrawTexturePro(
    *p_spaceshipTexture,
    (Rectangle){SPACESHIP_TOP_LEFT_X,
                SPACESHIP_TOP_LEFT_Y,
                SPACESHIP_WIDTH,
                SPACESHIP_HEIGHT},
    (Rectangle){p_spaceship->m_center.x,
                p_spaceship->m_center.y,
                SPACESHIP_WIDTH,
                SPACESHIP_HEIGHT},
    (Vector2){ SPACESHIP_WIDTH / 2.0f, SPACESHIP_HEIGHT / 2.0f },
    p_spaceship->m_rotation_angle,
    (Color){ 247, 102, 54, 255 }
  );

  #ifdef DEBUG
  DrawRectangleLines( (int)(p_spaceship->m_center.x - SPACESHIP_WIDTH / 2.0f),
                      (int)(p_spaceship->m_center.y - SPACESHIP_HEIGHT / 2.0f),
                      (int)SPACESHIP_WIDTH,
                      (int)SPACESHIP_HEIGHT,
                      GREEN);
  #endif
}