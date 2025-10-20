#include "raylib.h"
#include "raymath.h"    // Vector2Clamp, Vector2Normalize
#include "math.h"

#include "screen.h"
#include "crosshair.h"
#include "spaceship.h"
#include "bullet.h"
#include "collision.h"
#include "stdlib.h"   // calloc, free

#include "asteroid.h"

void CleanUp( Bullet_t* bulletsArray, 
              Asteroid_t* asteroidsArray, 
              Texture2D* crosshairTexture, 
              Texture2D* spaceshipTexture, 
              Texture2D* bulletTexture,
              Texture2D* asteroidTexture);

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroid 2D Game");
  HideCursor();

  // ------------------------------------------------------
  Texture2D crosshairTexture      =  { 0 };
  Texture2D spaceshipTexture      =  { 0 };
  Texture2D bulletTexture         =  { 0 };
  Texture2D asteroidTexture       =  { 0 };
  
  Vector2 crosshairCenterPos      = { 0 };
  Spaceship_t spaceship           = { 0 };
  
  Bullet_t* bulletsArray          = nullptr;
  Asteroid_t* asteroidsArray      = nullptr;
  size_t currentAsteroidCount     = 0;
  
  float deltaTime                 = 0.0f;
  float asteroid_SpawnTimer       = 0.0f;

  size_t hitCount                 = 0;

  // ------------------------------------------------------
  
  
  bool shouldRestart = true;
  while (shouldRestart)
  {
    shouldRestart       = false;
    hitCount            = 0;
    SetMousePosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    
    crosshairTexture  = LoadTexture("assets/crosshair.png");
    spaceshipTexture  = LoadTexture("assets/asteroids.png");
    bulletTexture     = LoadTexture("assets/asteroids.png");
    asteroidTexture   = LoadTexture("assets/asteroids.png");
    
    spaceship = (Spaceship_t){
      .m_center = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }
    };
    
    bulletsArray = (Bullet_t*)calloc(MAX_ACTIVE_BULLET_COUNT, sizeof(Bullet_t));
    if (bulletsArray == nullptr)
    {
      CloseWindow();
      return 0;
    }
    
    asteroidsArray = (Asteroid_t*)calloc(ASTEROID_SPAWN_COUNT, sizeof(Asteroid_t));
    if( asteroidsArray == NULL)
    {
      CloseWindow();
      return -1;
    }
    currentAsteroidCount = ASTEROID_SPAWN_COUNT;
    for (int i = 0; i < ASTEROID_SPAWN_COUNT; i++)
      Asteroid_SetAsteroidProperties(&asteroidsArray[i]);
    
    
    while (!WindowShouldClose())
    {
      deltaTime = GetFrameTime();

      BeginDrawing();
      ClearBackground(BLACK);
  
      crosshairCenterPos = Crosshair_GetCenterPosition(&crosshairTexture);
  
      Spaceship_UpdateSpaceshipRotation(&spaceship, crosshairCenterPos);
      Spaceship_UpdateSpaceshipPosition(&spaceship, deltaTime);
  
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        Bullet_FindFirstAvailableBulletAndShoot(bulletsArray, spaceship.m_center, crosshairCenterPos);
  
      Bullet_UpdateBulletsLocations(bulletsArray, deltaTime);
  
      shouldRestart = Spaceship_CheckIfSpaceshipIsDead(&spaceship, asteroidsArray, currentAsteroidCount);
      if (shouldRestart)  
        break;
  
      asteroid_SpawnTimer += deltaTime;
      if (asteroid_SpawnTimer >= ASTEROID_SPAWN_TIMER)
      {
        asteroid_SpawnTimer = 0.0f;
        Asteroid_t* p_new_asteroid = Asteroid_GenerateNewAsteroid(&asteroidsArray, &currentAsteroidCount);
        Asteroid_SetAsteroidProperties(p_new_asteroid);
      }
  
      int hits = Collision_CheckCollisionsBetweenBulletsAndAsteroids(bulletsArray, asteroidsArray, currentAsteroidCount);
      hitCount += hits;
      Asteroid_UpdateAsteroids(&asteroidsArray, &currentAsteroidCount, deltaTime);
  
  
      Crosshair_Draw(&crosshairTexture, crosshairCenterPos);
      Spaceship_Draw(&spaceshipTexture, &spaceship);
      Bullet_DrawBullets(&bulletTexture, bulletsArray);
      Asteroid_DrawAsteroids(&asteroidTexture, asteroidsArray, currentAsteroidCount);

      DrawText(TextFormat("Score: %zu", hitCount), SCREEN_WIDTH / 2, 30, 30, WHITE);

      #ifdef DEBUG
      DrawLineEx( (Vector2){ spaceship.m_center.x, spaceship.m_center.y},
                  (Vector2){ crosshairCenterPos.x, crosshairCenterPos.y},
                1.0f, YELLOW);
      #endif
      DrawFPS(10, 10);

      EndDrawing();
    }
  
    CleanUp(bulletsArray, asteroidsArray, &crosshairTexture, &spaceshipTexture, &bulletTexture, &asteroidTexture);
  }
}

void CleanUp( Bullet_t* bulletsArray, 
              Asteroid_t* asteroidsArray, 
              Texture2D* crosshairTexture, 
              Texture2D* spaceshipTexture, 
              Texture2D* bulletTexture,
              Texture2D* asteroidTexture)
{
  free(bulletsArray);
  free(asteroidsArray);
  UnloadTexture(*crosshairTexture);
  UnloadTexture(*spaceshipTexture);
  UnloadTexture(*bulletTexture);
  UnloadTexture(*asteroidTexture);
}