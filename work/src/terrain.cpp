#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "cgra_math.hpp"
#include "terrain.hpp"
#include "imageLoader.hpp"
#include "opengl.hpp"

using namespace std;
using namespace cgra;

Terrain::Terrain(vec2 tile){
  createDisplayListTile();
}

Terrain::~Terrain() {}

void Terrain::createDisplayListTile() {

	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);

	// Create a new list
	cout << "Creating Terrain Tile" << endl;
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);

  int x = tileSize;
  int y = tileSize;
  float height;

  triangle verts[x][y];

  for(int i = 0; i < x; i++){
    for(int j = 0; j < y; j++){
      /*
      if(j <= 10)
        height = 0;
      else if(j <= 20)
        height = 0.5;
      else if(j <= 30)
        height = 0.8;
      else if(j <= 40)
        height = 1;
      else if(j <= 50)
        height = 2;
      else if(j <= 60)
        height = 1.6;
      else if(j <= 70)
        height = 2.3;
        if(j <= 80)
          height = 2.4;
        else if(j <= 90)
          height = 2.8;
        else if(j <= 100)
          height = 3;

      else
      */
      height = ((float)rand()/(float)(RAND_MAX)) * 1;


      triangle tri;
      tri.v[0] = float(i);
      tri.v[1] = height;
      tri.v[2] = float(j);
      // height = ridgedMultifractal(tri, 2, 2.5, 8, 1.0, 2.0);
      verts[i][j] = tri;

    }
  }
vec3 u;
vec3 v;
vec3 normal;
glBegin(GL_TRIANGLES);
  for(int j = 0; j < x-1; j++){
    for(int i = 0; i < y-1; i++){
      //tri 1
      //calcNormals
      u = vec3(verts[i+1][j].v[0] - verts[i][j].v[0],
        verts[i+1][j].v[1] - verts[i][j].v[1],
        verts[i+1][j].v[2] - verts[i][j].v[2]);
      v = vec3(verts[i][j+1].v[0] - verts[i][j].v[0],
        verts[i][j+1].v[1] - verts[i][j].v[1],
        verts[i][j+1].v[2] - verts[i][j].v[2]);

      normal = vec3((u.y * v.z) - (u.z * v.y),
        (u.z * v.x) - (u.x * v.z),
        (u.x * v.y) - (u.y - v.x)
      );

      glNormal3f(-normal.x, -normal.y, -normal.z);

      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(verts[i][j].v[0], verts[i][j].v[1], verts[i][j].v[2]);

      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(verts[i+1][j].v[0], verts[i+1][j].v[1], verts[i+1][j].v[2]);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(verts[i][j+1].v[0], verts[i][j+1].v[1], verts[i][j+1].v[2]);

      //tri 2
      //calcNormals
      u = vec3(verts[i][j+1].v[0] - verts[i+1][j].v[0],
        verts[i][j+1].v[1] - verts[i+1][j].v[1],
        verts[i][j+1].v[2] - verts[i+1][j].v[2]);
      v = vec3(verts[i+1][j+1].v[0] - verts[i+1][j].v[0],
        verts[i+1][j+1].v[1] - verts[i+1][j].v[1],
        verts[i+1][j+1].v[2] - verts[i+1][j].v[2]);

      normal = vec3((u.y * v.z) - (u.z * v.y),
        (u.z * v.x) - (u.x * v.z),
        (u.x * v.y) - (u.y - v.x)
      );

      glNormal3f(normal.x, normal.y, normal.z);

      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(verts[i+1][j].v[0], verts[i+1][j].v[1], verts[i+1][j].v[2]);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(verts[i][j+1].v[0], verts[i][j+1].v[1], verts[i][j+1].v[2]);

      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(verts[i+1][j+1].v[0], verts[i+1][j+1].v[1], verts[i+1][j+1].v[2]);
    }
  }
  glEnd();
  glEndList();

  float a = 0;
  float b = 0;
  float c = 0;
  float d = 0;

	cout << "Finished Creating Terrain Tile" << endl;
}

void Terrain::renderTerrain(){
 //glDisable(GL_COLOR_MATERIAL);
 glEnable(GL_TEXTURE_2D);
 glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
 glShadeModel(GL_SMOOTH);
 glCallList(m_displayListPoly);
 glDisable(GL_TEXTURE_2D);
 glEnable(GL_COLOR_MATERIAL);
}

float Terrain::noise3(vec3 point){
  return 0;
}

/* Ridged multifractal terrain model.
 *
 * Some good parameter values to start with:
 *
 *  H:      1.0
 *  offset: 1.0
 *  gain:   2.0
 *
 *  H: the fractal dimension of the roughest areas
 *  Lacuarity: the gap between successive frequencies
 * octaves: number of frequencies in the fBm
 *  offset: raises the terrain from sea level
 *
 * Taken from the book "Texturing and modeling: aprocedural approach"
 * by D.S.Ebert, F K. Musgrave, D. Peachey, K. Perlin, S. Worley
 */

 float Terrain::ridgedMultifractal(vec3 point, float H, float lacunarity,
                          float octaves, float offset, float gain )
 {
   float result, frequency, sig, weight;
    //Terrain::noise3();
   static int first = true;
   static float *exponent_array;

   /* precompute and store spectral weights */
   if(first){
     /* seize required memory for exponent_array */
     exponent_array = (float *)malloc((octaves+1) * sizeof(float));
     frequency = 1.0;
     for(int i = 0; i<=octaves; i++){
       /* computer wight for each frequency */
       exponent_array[i] = pow(frequency, -H);
       frequency *= lacunarity;
     }
     first = false;
   }

   /* get first octave */
   sig = Terrain::noise3(point);
   /* get absolute value of signal (this creates the  ridges) */
   if(sig <= 0.0) sig = -sig;
   /* invert and translate (note that "offset" should be ~= 1.0) */
   sig = offset - sig;
   /* square the signal, to increase "sharpness" of ridges */
   sig *= sig;
   /* assign initial values */
   result = sig;
   weight = 1.0;

   for(int i = 1; i<octaves; i++){
     /* increase the frequency */
     point.x *= lacunarity;
     point.y *= lacunarity;
     point.z *= lacunarity;

     /* weight successive contributions by previous signal */
     weight = sig * gain;
     if(weight > 1.0) weight = 1.0;
     if(weight < 0.0) weight = 0.0;
     sig = Terrain::noise3(point);
     if(sig < 0.0) sig = -sig;
     sig = offset - sig;
     sig *= sig;
     /* weight the contribution */
     sig *= weight;
     result += sig * exponent_array[i];
   }
   return result;
 } /* RidgedMultifractal() */
