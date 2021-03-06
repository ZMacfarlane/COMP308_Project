#include <cmath>
#include <ctime>
#include <cstring>
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

// Noise Variales
const int B = 256;


static int p[B + B + 2];
static float g[B + B + 2][3];
static int start = 1;
static int first = true;
static float *exponent_array;
// endNoise
float fracDim;
float seed;
float heightMultiplier = 50;

#define noiseSetup(i, b0, b1, r0, r1) \
  t = i + 10000.; \
  b0 = ((int)t) & (B-1); \
  b1 = (b0+1) & (B-1); \
  r0 = t - (int)t; \
  r1 = r0 -1.;


Terrain::Terrain(float h, float s, float hm){
  fracDim = h;
  seed = s;
  heightMultiplier = hm;
  start = 1;
  int P[B + B + 2];
  float G[B + B + 2][3];
  memcpy (&p, &P, sizeof(P));
  memcpy (&g, &G, sizeof(G));
  first = true;
  createDisplayListTile();
}

Terrain::~Terrain() {}

void Terrain::createDisplayListTile() {

// glShadeModel(GL_SMOOTH);
	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);

	// Create a new list
	cout << "Creating Terrain Tile" << endl;
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);

  const int x = tileSize;
  const int y = tileSize;
  float height;

  triangle verts[x][y];

  for(int i = 0; i < x; i++){
    for(int j = 0; j < y; j++){

      height = 1;
      height = ridgedMultifractal(vec3(float(i)/x, height, float(j)/y), fracDim, 2.5, 8, 1.0, 2.0);
      // cout << "dim " << fracDim << endl;

      triangle tri;
      tri.v[0].p = float(i);
      tri.v[1].p = height*heightMultiplier;
      tri.v[2].p = float(j);

      if(i == 0 || j == 0 || i == x-1 || j == y-1)
        tri.v[1].p = 0;

      tri.v[0].n = 0;
      tri.v[1].n = 0;
      tri.v[2].n = 0;

      verts[i][j] = tri;

    }
  }
  vec3 u;
  vec3 v;
  vec3 normal;

  for(int i = 0; i < x-1; i++){
    for(int j = 0; j < y-1; j++){
      //tri 1
      u = vec3(verts[i+1][j].v[0].p - verts[i][j].v[0].p,
        verts[i+1][j].v[1].p/heightMultiplier - verts[i][j].v[1].p/heightMultiplier,
        verts[i+1][j].v[2].p - verts[i][j].v[2].p);

      v = vec3(verts[i][j+1].v[0].p - verts[i][j].v[0].p,
        verts[i][j+1].v[1].p/heightMultiplier - verts[i][j].v[1].p/heightMultiplier,
        verts[i][j+1].v[2].p - verts[i][j].v[2].p);

      normal = cross(u,v);

      verts[i][j].v[0].n = verts[i][j].v[0].n + normal.x;
      verts[i][j].v[1].n = verts[i][j].v[1].n + normal.y;
      verts[i][j].v[2].n = verts[i][j].v[2].n + normal.z;

      verts[i+1][j].v[0].n = verts[i+1][j].v[0].n + normal.x;
      verts[i+1][j].v[1].n = verts[i+1][j].v[1].n + normal.y;
      verts[i+1][j].v[2].n = verts[i+1][j].v[2].n + normal.z;

      verts[i][j+1].v[0].n = verts[i][j+1].v[0].n + normal.x;
      verts[i][j+1].v[1].n = verts[i][j+1].v[1].n + normal.y;
      verts[i][j+1].v[2].n = verts[i][j+1].v[2].n + normal.z;

      //tri 2

      u = vec3(verts[i][j+1].v[0].p - verts[i+1][j].v[0].p,
        verts[i][j+1].v[1].p - verts[i+1][j].v[1].p,
        verts[i][j+1].v[2].p - verts[i+1][j].v[2].p);
      v = vec3(verts[i+1][j+1].v[0].p - verts[i+1][j].v[0].p,
        verts[i+1][j+1].v[1].p - verts[i+1][j].v[1].p,
        verts[i+1][j+1].v[2].p - verts[i+1][j].v[2].p);

      normal = cross(u,v);

      verts[i+1][j].v[0].n = verts[i+1][j].v[0].n + normal.x;
      verts[i+1][j].v[1].n = verts[i+1][j].v[1].n + normal.y;
      verts[i+1][j].v[2].n = verts[i+1][j].v[2].n + normal.z;

      verts[i][j+1].v[0].n = verts[i][j+1].v[0].n + normal.x;
      verts[i][j+1].v[1].n = verts[i][j+1].v[1].n + normal.y;
      verts[i][j+1].v[2].n = verts[i][j+1].v[2].n + normal.z;

      verts[i+1][j+1].v[0].n = verts[i+1][j+1].v[0].n + normal.x;
      verts[i+1][j+1].v[1].n = verts[i+1][j+1].v[1].n + normal.y;
      verts[i+1][j+1].v[2].n = verts[i+1][j+1].v[2].n + normal.z;
    }
  }


  for(int i = 0; i < x; i++){
    for(int j = 0; j < y; j++){
      vec3 temp;

      temp = vec3(verts[i][j].v[0].n, verts[i][j].v[1].n, verts[i][j].v[2].n);
      temp = normalize(temp);
      verts[i][j].v[0].n = temp.x;
      verts[i][j].v[1].n = temp.y;
      verts[i][j].v[2].n = temp.z;

    }
  }

 float texScale = 0.05;
glBegin(GL_TRIANGLES);
      float ambient[] = { 1.0, 1.0, 1.0 ,1.0 };
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			float diffuse[] = { 1.0, 1.0, 1.0 ,1.0 };
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
			float specular[] = { 1.0, 1.0, 1.0, 9800.0 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			float shininess[] = { 0.0078*128.0 };
			glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

  for(int j = 0; j < x-1; j++){
    for(int i = 0; i < y-1; i++){
      //tri 1

      glNormal3f(verts[i][j].v[0].n, verts[i][j].v[1].n, verts[i][j].v[2].n);
      glTexCoord2f(i*texScale, j*texScale);
      glVertex3f(verts[i][j].v[0].p, verts[i][j].v[1].p, verts[i][j].v[2].p);

      glNormal3f(verts[i+1][j].v[0].n, verts[i+1][j].v[1].n, verts[i+1][j].v[2].n);
      glTexCoord2f(1.0f, 0.0f);
      glTexCoord2f((i+1)*texScale, (j)*texScale);
      glVertex3f(verts[i+1][j].v[0].p, verts[i+1][j].v[1].p, verts[i+1][j].v[2].p);

      glNormal3f(verts[i][j+1].v[0].n, verts[i][j+1].v[1].n, verts[i][j+1].v[2].n);
      glTexCoord2f(0.0f, 1.0f);
      glTexCoord2f((i)*texScale, (j+1)*texScale);
      glVertex3f(verts[i][j+1].v[0].p, verts[i][j+1].v[1].p, verts[i][j+1].v[2].p);


      //tri 2

      glNormal3f(verts[i+1][j].v[0].n, verts[i+1][j].v[1].n, verts[i+1][j].v[2].n);
      glTexCoord2f(1.0f, 0.0f);
      glTexCoord2f((i+1)*texScale, (j)*texScale);
      glVertex3f(verts[i+1][j].v[0].p, verts[i+1][j].v[1].p, verts[i+1][j].v[2].p);

      glNormal3f(verts[i][j+1].v[0].n, verts[i][j+1].v[1].n, verts[i][j+1].v[2].n);
      glTexCoord2f(0.0f, 1.0f);
      glTexCoord2f((i)*texScale, (j+1)*texScale);
      glVertex3f(verts[i][j+1].v[0].p, verts[i][j+1].v[1].p, verts[i][j+1].v[2].p);

      glNormal3f(verts[i+1][j+1].v[0].n, verts[i+1][j+1].v[1].n, verts[i+1][j+1].v[2].n);
      glTexCoord2f(1.0f, 1.0f);
      glTexCoord2f((i+1)*texScale, (j+1)*texScale);
      glVertex3f(verts[i+1][j+1].v[0].p, verts[i+1][j+1].v[1].p, verts[i+1][j+1].v[2].p);

    }
  }
  glEnd();
  glEndList();

	cout << "Finished Creating Terrain Tile" << endl;
}

void Terrain::renderTerrain(){
 //glDisable(GL_COLOR_MATERIAL);
 glEnable(GL_TEXTURE_2D);
 glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
 glShadeModel(GL_SMOOTH);
 // glShadeModel(GL_FLAT);
 glCallList(m_displayListPoly);
 glDisable(GL_TEXTURE_2D);
 glEnable(GL_COLOR_MATERIAL);
}

/*Texturing and modeling: aprocedural approach"
* by D.S.Ebert, F K. Musgrave, D. Peachey, K. Perlin, S. Worley*/
float Terrain::noise3(vec3 point){
  int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
  float rx0, rx1, ry0, ry1, rz0, rz1, *q, sx, sy, sz, a, b, c, d, t, u, v;
  register int i, j;

  if(start) {
    start = 0;
    noiseInit();
  }

  noiseSetup(point.x, bx0, bx1, rx0, rx1);
  noiseSetup(point.y, by0, by1, ry0, ry1);
  noiseSetup(point.z, bz0, bz1, rz0, rz1);

  i = p[bx0];
  j = p[bx1];

  b00 = p[i + by0];
  b10 = p[j + by0];
  b01 = p[i + by1];
  b11 = p[j + by1];

#define at(rx, ry, rz) (rx * q[0] + ry * q[1] + rz * q[2])
#define s_curve(t) (t * t * (3. -2. * t))
#define lerp(t, a, b) (a + t * (b - a))

  sx = s_curve(rx0);
  sy = s_curve(ry0);
  sz = s_curve(rz0);

  q = g[b00 + bz0]; u = at(rx0, ry0, rz0);
  q = g[b10 + bz0]; v = at(rx1, ry1, rz1);
  a = lerp(sx, u, v);

  q = g[b01 + bz0]; u = at(rx0, ry0, rz0);
  q = g[b11 + bz0]; v = at(rx1, ry1, rz1);
  b = lerp(sx, u, v);

  c = lerp(sy, a, b); /*interpolate in y at low x*/

  q = g[b00 + bz1]; u = at(rx0, ry0, rz0);
  q = g[b10 + bz1]; v = at(rx1, ry1, rz1);
  a = lerp(sx, u, v);

  q = g[b01 + bz1]; u = at(rx0, ry0, rz0);
  q = g[b11 + bz1]; v = at(rx1, ry1, rz1);
  b = lerp(sx, u, v);

  d = lerp(sy, a, b); /*interpolate in y at high x*/

  return 1.5 * lerp(sz, d, c); /*interpolate in z*/
}

/* Ridged multifractal terrain model.
 *
 * Some good parameter values to start with:
 *
 *  H:      1.0
 *  offset: 1.0
 *  gain:   2.0
 *
 *  H: the fractal dimension of the roughest areas; lower = more rough
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
  //  static int first = true;
  //  static float *exponent_array;

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

 void Terrain::noiseInit(){
  long random();
  int i, j, k;
  float /*v[3],*/ s;
  vec3 v;

  /*Create an array of random gradient vectors uniformly on the unit sphere*/

  srandom(seed/*time(NULL)*/);
  for(i = 0; i < B ; i++){
    do{
      /*choose uniformly in a cube*/
      /*
      for(j = 0; j < 3; j++){
        v[j] = (float)((random() % (B + B)) - B) / B;
      }
      */
      float v0 = (float)((random() % (B + B)) - B) / B;
      float v1 = (float)((random() % (B + B)) - B) / B;
      float v2 = (float)((random() % (B + B)) - B) / B;

      v = vec3(v0, v1, v2);

      s = dot(v,v);
    } while(s > 1.0);  /*if not in sphere try again*/
    s = sqrt(s);
    for(j = 0; j < 3; j++){ /*else normalize*/
      g[i][j] = v[j] / s;
    }
  }

  /*Create a psudorandom permutation of [1..B]*/
  for(i = 0; i < sizeof(p)/sizeof(*p); i++){
    p[i] = i;
  }
  for(i = B; i > 0; i -= 2){
    k = p[i];
    p[i] = p[j = random() % B];
    p[k] = k;
  }

  /*extend g and p arrays to allow for faster indexing*/
  for(i = 0; i < B + 2; i++){
    p[B + i] = p[i];
    for(j = 0; j < 3; j++){
      g[B + i][j] = g[i][j];
    }
  }
}
