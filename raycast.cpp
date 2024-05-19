#include <iostream>
#include <vector>

std::vector<int> get_lines()
{

/*
* 1. Init positions, directions and camera plane
* 2. For each ray:
*   1. determine its cameraX, or the point at which it intersects the camera plane
*   2. determine its x and y components using the player's position, direction and the cameraX value found earlier
*   3. calculate delta distance between x and y grid squares using delta distance = abs(1/ray direction)
*   4. calculate stepX and stepY (-1 if direction is negative, 1 if direction is positive)
*   5. calculate distance from current position to closest x and y grid squares  
*   6. perform DDA: while the ray has not hit anything, increment the x or y pos by delta distance depending on which one 
       is currently lower. increment the map coord x and y using step and set side = 0 if x, and side = 1 if y.
*   7. get distance from camera plane to wall sideDist - delta distance
*   8. line height = h / distance to wall
*/

  // int* lines = (int*)malloc(sizeof(int) * screenWidth);
  std::vector<int> lines;

  double posX = 22.0, posY = 12.0;
  double dirX = -1.0, dirY = 0.0;
  double planeX = 0.0, planeY = 0.66;

  double cameraX;
  double rayDirX, rayDirY;
  double deltaDistX, deltaDistY;
  double sideDistX, sideDistY;
  int mapX, mapY;
  int stepX, stepY;
  int hit, side;
  double perpWallDist;
  int lineHeight;

  for (int i = 0; i < screenWidth; i++) {
    cameraX = 2 * i / (double)screenWidth - 1;
    rayDirX = posX + dirX * cameraX;
    rayDirY = posY + dirY * cameraX;

    deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
    deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

    mapX = (int)posX;
    mapY = (int)posY;

    //calculate step and initial sideDist
    if (rayDirX < 0) {
      stepX = -1;
      sideDistX = (posX - mapX) * deltaDistX;
    } else {
      stepX = 1;
      sideDistX = (mapX + 1.0 - posX) * deltaDistX;
    } if (rayDirY < 0) {
      stepY = -1;
      sideDistY = (posY - mapY) * deltaDistY;
    } else {
      stepY = 1;
      sideDistY = (mapY + 1.0 - posY) * deltaDistY;
    }

    hit = 0;
    side = 0;

    while (hit==0) {
      if (sideDistX < sideDistY) {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      } else {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }
      if (worldMap[mapX][mapY] > 0) hit = 1;
    }
    if(side == 0) perpWallDist = (sideDistX - deltaDistX);
    else          perpWallDist = (sideDistY - deltaDistY);

    lineHeight = (int)(screenHeight / perpWallDist);
    lines.push_back(lineHeight);
  }
  return lines;
}
