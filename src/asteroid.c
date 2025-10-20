#include "asteroid.h"
#include "screen.h"

#include "raymath.h"
#include "stdlib.h"   // realloc, free

static Vector2 Asteroid_GetRandomAsteroidSpawnLocation(void)
{
  float x = 0.0f;
  float y = 0.0f;

  switch (GetRandomValue(0, 3))
  {
    case 0: // left side
      x = GetRandomValue(-200, 0);
      y = GetRandomValue(-200, SCREEN_HEIGHT + 200);
      break;
    case 1: // right side
      x = GetRandomValue(SCREEN_WIDTH, SCREEN_WIDTH + 200);
      y = GetRandomValue(-200, SCREEN_HEIGHT + 200);
      break;
    case 2: // top side
      x = GetRandomValue(-200, SCREEN_WIDTH + 200);
      y = GetRandomValue(-200, 0);
      break;
    case 3: // bottom side
      x = GetRandomValue(-200, SCREEN_WIDTH + 200);
      y = GetRandomValue(SCREEN_HEIGHT, SCREEN_HEIGHT + 200);
      break;
  }

  return (Vector2){ x, y };
}

static Vector2 Asteroid_GetRandomAsteroidDirection(void)
{
  float x = (float)GetRandomValue(-1000, 1000);
  float y = (float)GetRandomValue(-1000, 1000);
  return Vector2Normalize((Vector2){ x, y });
}


Asteroid_t* Asteroid_GenerateNewAsteroid(Asteroid_t** p_asteroidsArray, size_t* p_currentAsteroidCount)
{
  Asteroid_t* tempAsteroidArray = realloc(*p_asteroidsArray, (*p_currentAsteroidCount + 1) * sizeof(Asteroid_t));
  if (tempAsteroidArray == nullptr)
  {
    free(*p_asteroidsArray);
    CloseWindow();
    exit(-1);
  }
  *p_asteroidsArray = tempAsteroidArray;

  (*p_currentAsteroidCount)++;
  return &(*p_asteroidsArray)[*p_currentAsteroidCount - 1];
}

void Asteroid_SetAsteroidProperties(Asteroid_t* p_asteroid)
{
  // 0: Large, 1: Medium, 2: Small
  switch (GetRandomValue(0, 2))
  {
  case ASTEROID_SIZE_LARGE:
    p_asteroid->m_size                = ASTEROID_SIZE_LARGE;
    p_asteroid->m_image_location_x_y  = (Vector2){ ASTEROID_LARGE_TOP_LEFT_X, ASTEROID_LARGE_TOP_LEFT_Y};
    p_asteroid->m_width               = ASTEROID_LARGE_WIDTH;
    p_asteroid->m_height              = ASTEROID_LARGE_HEIGHT;
    p_asteroid->m_speed               = (float)GetRandomValue((int)ASTEROID_LARGE_MIN_SPEED, (int)ASTEROID_LARGE_MAX_SPEED);
    break;
  case ASTEROID_SIZE_MEDIUM:
    p_asteroid->m_size                = ASTEROID_SIZE_MEDIUM;
    p_asteroid->m_image_location_x_y  = (Vector2){ ASTEROID_MEDIUM_TOP_LEFT_X, ASTEROID_MEDIUM_TOP_LEFT_Y};
    p_asteroid->m_width               = ASTEROID_MEDIUM_WIDTH;
    p_asteroid->m_height              = ASTEROID_MEDIUM_HEIGHT;
    p_asteroid->m_speed               = (float)GetRandomValue((int)ASTEROID_MEDIUM_MIN_SPEED, (int)ASTEROID_MEDIUM_MAX_SPEED);
    break;
  case ASTEROID_SIZE_SMALL:
    p_asteroid->m_size                = ASTEROID_SIZE_SMALL;
    p_asteroid->m_image_location_x_y  = (Vector2){ ASTEROID_SMALL_TOP_LEFT_X, ASTEROID_SMALL_TOP_LEFT_Y};
    p_asteroid->m_width               = ASTEROID_SMALL_WIDTH;
    p_asteroid->m_height              = ASTEROID_SMALL_HEIGHT;
    p_asteroid->m_speed               = (float)GetRandomValue((int)ASTEROID_SMALL_MIN_SPEED, (int)ASTEROID_SMALL_MAX_SPEED);
    break;
  }

  p_asteroid->m_active              = true;
  p_asteroid->m_position            = Asteroid_GetRandomAsteroidSpawnLocation();
  p_asteroid->m_direction           = Asteroid_GetRandomAsteroidDirection();
  p_asteroid->m_lifetime            = 0.0f;
  p_asteroid->m_is_splitted         = false;
  p_asteroid->m_rotation_angle      = (float)GetRandomValue(0, 360);
  p_asteroid->m_rotation_speed      = (float)GetRandomValue(-180, 180);
}

static void Asteroid_SpawnNewAsteroidsIfThereAreInactive(Asteroid_t* asteroidsArray, size_t currentAsteroidCount)
{
  Asteroid_t* p_asteroid = nullptr;

  for (int i = 0; i < currentAsteroidCount; i++)
  {
    if (!asteroidsArray[i].m_active)
    {
      p_asteroid = &asteroidsArray[i];

      p_asteroid->m_active            = true;
      p_asteroid->m_position          = Asteroid_GetRandomAsteroidSpawnLocation();
      p_asteroid->m_direction         = Asteroid_GetRandomAsteroidDirection();
      p_asteroid->m_lifetime          = 0.0f;
      p_asteroid->m_is_splitted      = false;
      p_asteroid->m_rotation_angle   = (float)GetRandomValue(0, 360);
      p_asteroid->m_rotation_speed   = (float)GetRandomValue(-180, 180);

      int randomSize = GetRandomValue(0, 2);
      switch (randomSize)
      {
        case 0:
          p_asteroid->m_size                = ASTEROID_SIZE_LARGE;
          p_asteroid->m_image_location_x_y  = (Vector2){ ASTEROID_LARGE_TOP_LEFT_X, ASTEROID_LARGE_TOP_LEFT_Y};
          p_asteroid->m_width               = ASTEROID_LARGE_WIDTH;
          p_asteroid->m_height              = ASTEROID_LARGE_HEIGHT;
          p_asteroid->m_speed               = (float)GetRandomValue((int)ASTEROID_LARGE_MIN_SPEED, (int)ASTEROID_LARGE_MAX_SPEED);
          break;
        case 1:
          p_asteroid->m_size = ASTEROID_SIZE_MEDIUM;
          p_asteroid->m_image_location_x_y  = (Vector2){ ASTEROID_MEDIUM_TOP_LEFT_X, ASTEROID_MEDIUM_TOP_LEFT_Y};
          p_asteroid->m_width               = ASTEROID_MEDIUM_WIDTH;
          p_asteroid->m_height              = ASTEROID_MEDIUM_HEIGHT;
          p_asteroid->m_speed               = (float)GetRandomValue((int)ASTEROID_MEDIUM_MIN_SPEED, (int)ASTEROID_MEDIUM_MAX_SPEED);
          break;
        case 2:
          p_asteroid->m_size = ASTEROID_SIZE_SMALL;
          p_asteroid->m_image_location_x_y  = (Vector2){ ASTEROID_SMALL_TOP_LEFT_X, ASTEROID_SMALL_TOP_LEFT_Y};
          p_asteroid->m_width               = ASTEROID_SMALL_WIDTH;
          p_asteroid->m_height              = ASTEROID_SMALL_HEIGHT;
          p_asteroid->m_speed               = (float)GetRandomValue((int)ASTEROID_SMALL_MIN_SPEED, (int)ASTEROID_SMALL_MAX_SPEED);
          break;
      }
    }
  }
}



static void Asteroid_GenerateTwoMediumAsteroidsFromLarge(Asteroid_t** p_asteroidsArray,
                                                  size_t* p_currentAsteroidCount,
                                                  Vector2 splitPosition)
{
  Asteroid_t* tempAsteroidArray = realloc(*p_asteroidsArray, (*p_currentAsteroidCount + 2) * sizeof(Asteroid_t));
  if (tempAsteroidArray == nullptr)
  {
    CloseWindow();
    exit(-1);
  }
  *p_asteroidsArray = tempAsteroidArray;


  Asteroid_t* newAsteroidArray[2] = {
    &(*p_asteroidsArray)[*p_currentAsteroidCount],
    &(*p_asteroidsArray)[*p_currentAsteroidCount + 1]
  };
  
  for (int i = 0; i < 2; ++i)
  {
    newAsteroidArray[i]->m_active              = true;
    newAsteroidArray[i]->m_size                = ASTEROID_SIZE_MEDIUM;
    newAsteroidArray[i]->m_image_location_x_y  = (Vector2){ ASTEROID_MEDIUM_TOP_LEFT_X, ASTEROID_MEDIUM_TOP_LEFT_Y};
    newAsteroidArray[i]->m_width               = ASTEROID_MEDIUM_WIDTH;
    newAsteroidArray[i]->m_height              = ASTEROID_MEDIUM_HEIGHT;
    newAsteroidArray[i]->m_position            = splitPosition;
    newAsteroidArray[i]->m_direction           = Asteroid_GetRandomAsteroidDirection();
    newAsteroidArray[i]->m_speed               = (float)GetRandomValue((int)ASTEROID_MEDIUM_MIN_SPEED, (int)ASTEROID_MEDIUM_MAX_SPEED);
    newAsteroidArray[i]->m_lifetime            = 0.0f;
    newAsteroidArray[i]->m_is_splitted         = false;
    newAsteroidArray[i]->m_rotation_angle      = (float)GetRandomValue(0, 360);
    newAsteroidArray[i]->m_rotation_speed      = (float)GetRandomValue(-180, 180);
  }

  *p_currentAsteroidCount += 2;
}

static void Asteroid_GenerateTwoSmallAsteroidsFromMedium(Asteroid_t** p_asteroidsArray, 
                                                  size_t* p_currentAsteroidCount,
                                                  Vector2 splitPosition)
{
  Asteroid_t* tempAsteroidArray = realloc(*p_asteroidsArray, (*p_currentAsteroidCount + 2) * sizeof(Asteroid_t));
  if (tempAsteroidArray == NULL)
  {
    free(*p_asteroidsArray);
    CloseWindow();
    exit(-1);
  }
  *p_asteroidsArray = tempAsteroidArray;

  Asteroid_t* newAsteroidArray[2] = {
    &(*p_asteroidsArray)[*p_currentAsteroidCount],
    &(*p_asteroidsArray)[*p_currentAsteroidCount + 1]
  };
  
  for (int i = 0; i < 2; ++i)
  {
    newAsteroidArray[i]->m_active              = true;
    newAsteroidArray[i]->m_size                = ASTEROID_SIZE_SMALL;
    newAsteroidArray[i]->m_image_location_x_y  = (Vector2){ ASTEROID_SMALL_TOP_LEFT_X, ASTEROID_SMALL_TOP_LEFT_Y};
    newAsteroidArray[i]->m_width               = ASTEROID_SMALL_WIDTH;
    newAsteroidArray[i]->m_height              = ASTEROID_SMALL_HEIGHT;
    newAsteroidArray[i]->m_position            = splitPosition;
    newAsteroidArray[i]->m_direction           = Asteroid_GetRandomAsteroidDirection();
    newAsteroidArray[i]->m_speed               = (float)GetRandomValue((int)ASTEROID_SMALL_MIN_SPEED, (int)ASTEROID_SMALL_MAX_SPEED);
    newAsteroidArray[i]->m_lifetime            = 0.0f;
    newAsteroidArray[i]->m_is_splitted         = false;
    newAsteroidArray[i]->m_rotation_angle      = (float)GetRandomValue(0, 360);
    newAsteroidArray[i]->m_rotation_speed      = (float)GetRandomValue(-180, 180);
  }
  
  *p_currentAsteroidCount += 2;
}     



static void Asteroid_CheckForSplittedAsteroids(Asteroid_t** p_asteroidsArray, size_t* p_currentAsteroidCount)
{
  // Save the original count to avoid checking newly created asteroids in this iteration
  size_t originalCount = *p_currentAsteroidCount;
  
  for (int i = 0; i < originalCount; i++)
    if ((*p_asteroidsArray)[i].m_is_splitted)
    {
      Asteroid_t* asteroid = &(*p_asteroidsArray)[i];
      asteroid->m_is_splitted = false;
      
      // Save the position before any realloc happens
      Vector2 splitPosition = asteroid->m_position;
      switch (asteroid->m_size)
      {
        case ASTEROID_SIZE_LARGE:
          Asteroid_GenerateTwoMediumAsteroidsFromLarge(p_asteroidsArray, p_currentAsteroidCount, splitPosition);
          break;
        case ASTEROID_SIZE_MEDIUM:
          Asteroid_GenerateTwoSmallAsteroidsFromMedium(p_asteroidsArray, p_currentAsteroidCount, splitPosition);
          break;
        case ASTEROID_SIZE_SMALL:
          // Small asteroids do not split further
          break;
      }
    }
}

static bool Asteroid_CheckIfAsteroidIsOutOfScreen(Asteroid_t* p_asteroid)
{
  float halfWidth = p_asteroid->m_width / 2.0f;
  float halfHeight = p_asteroid->m_height / 2.0f;

  return (p_asteroid->m_position.x + halfWidth < 0            ||
          p_asteroid->m_position.x - halfWidth > SCREEN_WIDTH ||
          p_asteroid->m_position.y + halfHeight < 0           ||
          p_asteroid->m_position.y - halfHeight > SCREEN_HEIGHT
    );
}



static void Asteroid_DeactivateAsteroidsIfOutOfScreen(Asteroid_t* asteroidsArray, size_t currentAsteroidCount, float deltaTime)
{
  Asteroid_t* p_asteroid = nullptr;
  
  for (int i = 0; i < currentAsteroidCount; i++)
  {
    if (asteroidsArray[i].m_active)
    {
      p_asteroid = &asteroidsArray[i];

      p_asteroid->m_position.x      += p_asteroid->m_direction.x * p_asteroid->m_speed * deltaTime;
      p_asteroid->m_position.y      += p_asteroid->m_direction.y * p_asteroid->m_speed * deltaTime;
      p_asteroid->m_rotation_angle  += p_asteroid->m_rotation_speed * deltaTime;

      bool asteroidIsOutOfScreen = Asteroid_CheckIfAsteroidIsOutOfScreen(p_asteroid);

      if (asteroidIsOutOfScreen && p_asteroid->m_lifetime < 2.0f)
        p_asteroid->m_lifetime += deltaTime;
      else if (asteroidIsOutOfScreen && p_asteroid->m_lifetime >= 2.0f)
        p_asteroid->m_active = false;
    }
  }
}

void Asteroid_UpdateAsteroids(Asteroid_t** p_asteroidsArray, size_t* p_currentAsteroidCount, float deltaTime)
{
  Asteroid_DeactivateAsteroidsIfOutOfScreen(*p_asteroidsArray, *p_currentAsteroidCount, deltaTime);
  Asteroid_CheckForSplittedAsteroids(p_asteroidsArray, p_currentAsteroidCount);
  Asteroid_SpawnNewAsteroidsIfThereAreInactive(*p_asteroidsArray, *p_currentAsteroidCount);
}

void Asteroid_DrawAsteroids(const Texture2D* p_asteroidTexture, Asteroid_t* asteroidsArray, size_t currentAsteroidCount)
{
  for (int i = 0; i < currentAsteroidCount; i++)
  {
    if (asteroidsArray[i].m_active)
    {
      Asteroid_t* p_asteroid = &asteroidsArray[i];

      DrawTexturePro( *p_asteroidTexture,
        (Rectangle){ p_asteroid->m_image_location_x_y.x, 
                     p_asteroid->m_image_location_x_y.y, 
                     p_asteroid->m_width,
                     p_asteroid->m_height },
        (Rectangle){
          p_asteroid->m_position.x,
          p_asteroid->m_position.y,
          p_asteroid->m_width,
          p_asteroid->m_height},
        (Vector2){ p_asteroid->m_width / 2, p_asteroid->m_height / 2},
        p_asteroid->m_rotation_angle,
        (Color){ 162, 242, 16, 255 });

      #ifdef DEBUG
      DrawRectangleLines(p_asteroid->m_position.x - p_asteroid->m_width / 2.0f,
                        p_asteroid->m_position.y - p_asteroid->m_height / 2.0f,
                        p_asteroid->m_width,
                        p_asteroid->m_height,
                        PURPLE);
      #endif
    }
  }
}