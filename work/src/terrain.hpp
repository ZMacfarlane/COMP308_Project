#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "cgra_math.hpp"
#include "opengl.hpp"


struct tile{
  int size = 64;
  int x = 0; //positiion of tile along x-axis
  int y = 0; //positiion of tile along y-axis
};


struct vertex {
	float p = 0; // point
	float t = 0; // texture
	float n = 0; // normal
};


struct triangle {
	vertex v[3]; //requires 3 verticies
  // float v[3];
};

class Terrain {
private:
  GLuint m_displayListPoly; // DisplayList for Polygon
  int tileSize = 256;
  // int heightMultiplier = 50;

  float ridgedMultifractal(cgra::vec3, float, float, float, float, float);
  float noise3(cgra::vec3);
  void createDisplayListTile();
  static void noiseInit();

public:
  Terrain(float, float, float);
  ~Terrain();

  void renderTerrain();
};
