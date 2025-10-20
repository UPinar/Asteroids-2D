#ifndef   COLLISION_H
#define   COLLISION_H

#include "stddef.h"
#include "bullet.h"
#include "asteroid.h"

size_t Collision_CheckCollisionsBetweenBulletsAndAsteroids( Bullet_t* bulletsArray, 
                                                         Asteroid_t* asteroidsArray, 
                                                         size_t currentAsteroidCount);

#endif    // COLLISION_H