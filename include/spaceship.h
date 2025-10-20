#ifndef   SPACESHIP_H
#define   SPACESHIP_H

#include "raylib.h"
#include "asteroid.h"

#define   SPACESHIP_TOP_LEFT_X    196.0F
#define   SPACESHIP_TOP_LEFT_Y    256.0F

#define   SPACESHIP_WIDTH         96.0F
#define   SPACESHIP_HEIGHT        64.0F

#define   SPACESHIP_SPEED         500.0F


typedef struct {
  Vector2   m_center;
  Vector2   m_direction;
  float     m_rotation_angle;
} Spaceship_t;

void Spaceship_UpdateSpaceshipRotation(Spaceship_t* spaceship, Vector2 crosshairCenter);
void Spaceship_UpdateSpaceshipPosition(Spaceship_t* spaceship, float deltaTime);

bool Spaceship_CheckIfSpaceshipIsDead(Spaceship_t* spaceship, 
                                      Asteroid_t* asteroidsArray, 
                                      size_t currentAsteroidCount);

void Spaceship_Draw(const Texture2D* p_spaceshipTexture, Spaceship_t* p_spaceship);

#endif