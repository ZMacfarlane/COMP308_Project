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

class Terrain {
private:
  GLuint m_displayListPoly; // DisplayList for Polygon
  int tileSize = 64;

  float ridgedMultifractal(cgra::vec3, float, float, float, float, float);
  float noise3(cgra::vec3);
  void createDisplayListTile();

public:
  Terrain(cgra::vec2);
  ~Terrain();

  void renderTerrain();
};
