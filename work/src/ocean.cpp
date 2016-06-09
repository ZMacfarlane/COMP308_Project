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
#include "ocean.hpp"
#include "imageLoader.hpp"
#include "opengl.hpp"

using namespace std;
using namespace cgra;



Ocean::Ocean(float h){
  createDisplayListTile();
}

Ocean::~Ocean() {}

void Ocean::createDisplayListTile() {

	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);

	// Create a new list
	cout << "Creating Ocean Tile" << endl;
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);

  const int x = tileSize;
  const int y = tileSize;
  float height;

  triangl verts[x][y];

  for(int i = 0; i < x; i++){
    for(int j = 0; j < y; j++){

      height = 1;
      height = seaLevel;

      triangl tri;
      tri.v[0].p = float(i);
      tri.v[1].p = height;
      tri.v[2].p = float(j);

      tri.v[0].n = 0;
      tri.v[1].n = 0;
      tri.v[2].n = 0;

      verts[i][j] = tri;

    }
  }
  vec3 u;
  vec3 v;
  vec3 normal;
/*
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
*/
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
      glNormal3f(0, 1, 0);
      // glNormal3f(verts[i][j].v[0].n, verts[i][j].v[1].n, verts[i][j].v[2].n);
      glTexCoord2f(i*texScale, j*texScale);
      glVertex3f(verts[i][j].v[0].p, verts[i][j].v[1].p, verts[i][j].v[2].p);

      // glNormal3f(verts[i+1][j].v[0].n, verts[i+1][j].v[1].n, verts[i+1][j].v[2].n);
      // glTexCoord2f(1.0f, 0.0f);
      glTexCoord2f((i+1)*texScale, (j)*texScale);
      glVertex3f(verts[i+1][j].v[0].p, verts[i+1][j].v[1].p, verts[i+1][j].v[2].p);

      // glNormal3f(verts[i][j+1].v[0].n, verts[i][j+1].v[1].n, verts[i][j+1].v[2].n);
      // glTexCoord2f(0.0f, 1.0f);
      glTexCoord2f((i)*texScale, (j+1)*texScale);
      glVertex3f(verts[i][j+1].v[0].p, verts[i][j+1].v[1].p, verts[i][j+1].v[2].p);


      //tri 2

      // glNormal3f(verts[i+1][j].v[0].n, verts[i+1][j].v[1].n, verts[i+1][j].v[2].n);
      // glTexCoord2f(1.0f, 0.0f);
      glTexCoord2f((i+1)*texScale, (j)*texScale);
      glVertex3f(verts[i+1][j].v[0].p, verts[i+1][j].v[1].p, verts[i+1][j].v[2].p);

      // glNormal3f(verts[i][j+1].v[0].n, verts[i][j+1].v[1].n, verts[i][j+1].v[2].n);
      // glTexCoord2f(0.0f, 1.0f);
      glTexCoord2f((i)*texScale, (j+1)*texScale);
      glVertex3f(verts[i][j+1].v[0].p, verts[i][j+1].v[1].p, verts[i][j+1].v[2].p);

      // glNormal3f(verts[i+1][j+1].v[0].n, verts[i+1][j+1].v[1].n, verts[i+1][j+1].v[2].n);
      // glTexCoord2f(1.0f, 1.0f);
      glTexCoord2f((i+1)*texScale, (j+1)*texScale);
      glVertex3f(verts[i+1][j+1].v[0].p, verts[i+1][j+1].v[1].p, verts[i+1][j+1].v[2].p);

    }
  }
  glEnd();
  glEndList();

	cout << "Finished Creating Ocean Tile" << endl;
}

void Ocean::renderOcean(){
 //glDisable(GL_COLOR_MATERIAL);
 glEnable(GL_TEXTURE_2D);
 glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
 glShadeModel(GL_SMOOTH);
 // glShadeModel(GL_FLAT);
 glCallList(m_displayListPoly);
 glDisable(GL_TEXTURE_2D);
 glEnable(GL_COLOR_MATERIAL);
}
