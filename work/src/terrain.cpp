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

  float x = tileSize;
  float y = tileSize;

  float terrain[64][64][3];
  for(int i = 0; i < x; i++){
    for(int j = 0; j < y; j++){
      terrain[i][j][0] = float(i);
      terrain[i][j][1] = rand() % 3;
      terrain[i][j][2] = float(j);
    }
  }
glNormal3f(0.0, 0.0, 1.0);
  for(int j = 0; j < x-1; j++){
    //glBegin(GL_TRIANGLE_STRIP);
    glBegin(GL_QUADS)
    for(int i = 0; i < y-1; i++){
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(terrain[i][j][0], terrain[i][j][1], terrain[i][j][2]);

      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(terrain[i+1][j][0], terrain[i+1][j][1], terrain[i+1][j][2]);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(terrain[i][j+1][0], terrain[i][j+1][1], terrain[i][j+1][2]);

      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(terrain[i+1][j+1][0], terrain[i+1][j+1][1], terrain[i+1][j+1][2]);
    }
    glEnd();
  }
  glEndList();

  float a = 0;
  float b = 0;
  float c = 0;
  float d = 0;

/*
  //glBegin(GL_TRIANGLES);
  glBegin(GL_QUAD_STRIP);
  for(float i = 0.0f; i < x; i++){
    for(float j = 0.0f; j < y; j++){
      glNormal3f(0.0, 0.0, 1.0);

      glTexCoord2f((1.0 / (x-1)) * i, (1.0 / (y-1)) * j);
      glVertex3f(i, 0.0, j);
      //glVertex3f(i, a, j);
      //glVertex3f(i, rand() % 10, j);
      //a += rand() % 3;

      glTexCoord2f((1.0 / (x-1)) * i, (1.0 / (y-1)) * j+1);
      glVertex3f(i, 0.0, j+1);
      //glVertex3f(i, b, j+1);
      //glVertex3f(i, rand() % 10, j+1);
      //b += rand() % 3;



      glTexCoord2f((1.0 / (x-1)) * i+1, (1.0 / (y-1)) * j+1);
      glVertex3f(i+1, 0.0, j+1);
      //glVertex3f(i+1, c, j+1);
      //glVertex3f(i+1, rand() % 10, j+1);
      //c += rand() % 3;



      glTexCoord2f((1.0 / (x-1)) * i+1, (1.0 / (y-1)) * j);
      glVertex3f(i+1, 0.0, j);
      //glVertex3f(i+1, d, j);
      //glVertex3f(i+1, rand() % 10, j);
      //d += rand() % 3;


    }
  }
  glEnd();
  glEndList();
  */

/*
	// TODO
	glBegin(GL_QUADS);
  glNormal3f(0.0, 0.0, 1.0);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-5.0, -0.0, -5.0);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-5.0, 0.0, 5.0);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(5.0, 0.0, 5.0);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(5.0, 0.0, -5.0);
	glEnd();
	glEndList();
*/
	cout << "Finished Creating Terrain Tile" << endl;
}

void Terrain::renderTerrain(){
 glDisable(GL_COLOR_MATERIAL);
 glEnable(GL_TEXTURE_2D);
 glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
