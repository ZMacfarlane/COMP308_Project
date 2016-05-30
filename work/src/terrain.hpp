#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "cgra_math.hpp"
#include "opengl.hpp"


struct tile{
  int size = 64;
};

class Terrain {
private:
  GLuint m_displayListPoly; // DisplayList for Polygon

  float ridgedMultifractal(cgra::vec3, float, float, float, float, float);
  float noise3(cgra::vec3);
  void createDisplayListTile();

public:
  Terrain(std::string);
  ~Terrain();

  void renderTerrain();
};
