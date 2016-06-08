#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "cgra_math.hpp"
#include "opengl.hpp"

struct vertx {
	float p = 0; // point
	float t = 0; // texture
	float n = 0; // normal
};

struct triangl {
	vertx v[3]; //requires 3 verticies
  // float v[3];
};

class Ocean {
private:
  GLuint m_displayListPoly; // DisplayList for Polygon
  int tileSize = 256;
  int heightMultiplier = 50;
  float seaLevel = 3;


  void createDisplayListTile();

public:
  Ocean(float);
  ~Ocean();

  void renderOcean();
};
