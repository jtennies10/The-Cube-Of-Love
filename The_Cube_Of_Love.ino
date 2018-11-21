#include "beta-cube-library.h" 
#include "L3D_CubePixel.h" 
#include <math.h> 

//Cube cube = Cube(6,50);
Cube cube(6,50);
void drawHeart(Color);

void setup() {
   cube.begin();
   }

void loop() {
  Color voxelColor(0, 100, 255);

  drawHeart(voxelColor, 0);
  drawHeart(voxelColor, 1);
  drawHeart(voxelColor, 2);
  drawHeart(voxelColor, 3);
  drawHeart(voxelColor, 4);
  drawHeart(voxelColor, 5);

  cube.show();
            
    }

//draws a heart in the front vertical plane
void drawHeart(Color voxelColor, int zPlane) {
  cube.setVoxel(2, 0, zPlane, voxelColor);
  cube.setVoxel(1, 1, zPlane, voxelColor);
  cube.setVoxel(3, 1, zPlane, voxelColor);
  cube.setVoxel(0, 2, zPlane, voxelColor);
  cube.setVoxel(4, 2, zPlane, voxelColor);
  cube.setVoxel(0, 3, zPlane, voxelColor);
  cube.setVoxel(4, 3, zPlane, voxelColor);
  cube.setVoxel(1, 4, zPlane, voxelColor);
  cube.setVoxel(3, 4, zPlane, voxelColor);
  cube.setVoxel(2, 3, zPlane, voxelColor);
}
