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
  float ridgedMultifractal(cgra::vec3, float, float, float, float, float);
  float noise3(cgra::vec3);

public:
  Terrain();
};
