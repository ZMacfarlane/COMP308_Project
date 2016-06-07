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

/*
struct vertex {
	int p = 0; // index for point in m_points
	int t = 0; // index for uv in m_uvs
	int n = 0; // index for normal in m_normals
};
*/

struct triangle {
	// vertex v[3]; //requires 3 verticies
  float v[3];
};

class Terrain {
private:
  GLuint m_displayListPoly; // DisplayList for Polygon
  int tileSize = 128;

  float ridgedMultifractal(cgra::vec3, float, float, float, float, float);
  float noise3(cgra::vec3);
  void createDisplayListTile();
  static void noiseInit();

public:
  Terrain(cgra::vec2);
  ~Terrain();

  void renderTerrain();
};
