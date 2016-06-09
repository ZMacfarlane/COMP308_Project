//---------------------------------------------------------------------------
//
// Copyright (c) 2016 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

#include "cgra_math.hpp"
#include "simple_image.hpp"
#include "simple_shader.hpp"
#include "opengl.hpp"
#include "terrain.hpp"
#include "ocean.hpp"

using namespace std;
using namespace cgra;

// Window
//
GLFWwindow* g_window;
const int maxTiles = 1000;
const int g_tileSize = 256;
const int initNumTiles = 3;
int numTiles = 3;
Terrain* tTiles[maxTiles][maxTiles];
Ocean* oTiles[maxTiles][maxTiles];

Terrain *g_terrain= nullptr;
Terrain *g_terrain2= nullptr;
Terrain *g_terrain3= nullptr;
Terrain *g_terrain4= nullptr;

Ocean *g_ocean= nullptr;
Ocean *g_ocean2= nullptr;
Ocean *g_ocean3= nullptr;
Ocean *g_ocean4= nullptr;

// Projection values
//
float g_fovy = 20.0;
float g_znear = 0.1;
float g_zfar = 10000.0;


// Mouse controlled Camera values
//
bool g_leftMouseDown = false;
vec2 g_mousePosition = vec2(30, 15);
float g_pitch = 30.0;
float g_yaw = 15;
float g_zoom = 1.0;

//Key controlled camera values
float g_fVal = 512.0f;
float g_rVal = -128.0f;
float g_uVal = -256.0f;

float g_panStep = 5.0f;
float g_rotStep = 10.0f;


float g_fTrans = 0.0f;
float g_rTrans = 0.0f;
float g_upPan = 0.0f;
float g_rightPan = 0.0f;

bool g_forward = false;
bool g_right = false;
bool g_back = false;
bool g_left = false;

bool g_up = false;
bool g_down = false;

bool g_panUp = false;
bool g_panDown = false;
bool g_panRight = false;
bool g_panLeft = false;

bool g_drawWater = false;

// Values and fields to showcase the use of shaders
// Remove when modifying main.cpp for Assignment 3
//
bool g_useShader = false;
GLuint g_texture = 0;
GLuint g_waterTexture = 0;
GLuint numTextures = 0;
GLuint g_shader = 0;
void genTile();


// Mouse Button callback
// Called for mouse movement event on since the last glfwPollEvents
//
void cursorPosCallback(GLFWwindow* win, double xpos, double ypos) {
	// cout << "Mouse Movement Callback :: xpos=" << xpos << "ypos=" << ypos << endl;
	if (g_leftMouseDown) {
		g_yaw -= g_mousePosition.x - xpos;
		g_pitch -= g_mousePosition.y - ypos;
	}
	g_mousePosition = vec2(xpos, ypos);
}


// Mouse Button callback
// Called for mouse button event on since the last glfwPollEvents
//
void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
	// cout << "Mouse Button Callback :: button=" << button << "action=" << action << "mods=" << mods << endl;
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		g_leftMouseDown = (action == GLFW_PRESS);
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if (g_useShader) {
			g_useShader = false;
			cout << "Using the default OpenGL pipeline" << endl;
		}
		else {
			g_useShader = true;
			cout << "Using a shader" << endl;
		}
	}
}


// Scroll callback
// Called for scroll event on since the last glfwPollEvents
//
void scrollCallback(GLFWwindow *win, double xoffset, double yoffset) {
	// cout << "Scroll Callback :: xoffset=" << xoffset << "yoffset=" << yoffset << endl;
	g_zoom -= yoffset * g_zoom * 0.2;
}


// Keyboard callback
// Called for every key event on since the last glfwPollEvents
//
void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) {
	// cout << "Key Callback :: key=" << key << "scancode=" << scancode
	// 	<< "action=" << action << "mods=" << mods << endl;
	// g_right += g_right;
	// g_forward += g_forward;
	if(key == GLFW_KEY_W && action == GLFW_PRESS){
		g_up = true;
	}
	if(key == GLFW_KEY_W && action == GLFW_RELEASE){
		g_up = false;
	}
	if(key == GLFW_KEY_D && action == GLFW_PRESS){
		g_right = true;
	}
	if(key == GLFW_KEY_D && action == GLFW_RELEASE){
		g_right = false;
	}
	if(key == GLFW_KEY_S && action == GLFW_PRESS){
		g_down = true;
	}
	if(key == GLFW_KEY_S && action == GLFW_RELEASE){
		g_down = false;
	}
	if(key == GLFW_KEY_A && action == GLFW_PRESS){
		g_left = true;
	}
	if(key == GLFW_KEY_A && action == GLFW_RELEASE){
		g_left = false;
	}

	if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS){
		g_forward = true;
	}
	if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE){
		g_forward = false;
	}
	if(key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS){
		g_back = true;
	}
	if(key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE){
		g_back = false;
	}




	if(key == GLFW_KEY_UP && action == GLFW_PRESS){
		g_panUp = true;
	}
	if(key == GLFW_KEY_UP && action == GLFW_RELEASE){
		g_panUp = false;
	}
	if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		g_panDown = true;
	}
	if(key == GLFW_KEY_DOWN && action == GLFW_RELEASE){
		g_panDown = false;
	}
	if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
		g_panRight = true;
	}
	if(key == GLFW_KEY_RIGHT && action == GLFW_RELEASE){
		g_panRight = false;
	}
	if(key == GLFW_KEY_LEFT && action == GLFW_PRESS){
		g_panLeft = true;
	}
	if(key == GLFW_KEY_LEFT && action == GLFW_RELEASE){
		g_panLeft = false;
	}


	if(key == GLFW_KEY_F && action == GLFW_PRESS){
		genTile();
	}
	if(key == GLFW_KEY_G && action == GLFW_PRESS){
		if(g_drawWater)
			g_drawWater = false;
		else
			g_drawWater = true;
	}
}


// Character callback
// Called for every character input event on since the last glfwPollEvents
//
void charCallback(GLFWwindow *win, unsigned int c) {
	// cout << "Char Callback :: c=" << char(c) << endl;
	// Not needed for this assignment, but useful to have later on
}


float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void genTile(){
	int done = 0;
	int stepped = 0;
	for(int i = 0; i != numTiles+1; i++){
		for(int j = 0; j != numTiles+1; j++){
			if(i == numTiles || j == numTiles){
				if(!tTiles[i][j] && !done && !stepped){
					tTiles[i][j] = new Terrain(RandomFloat(0.8, 2.5), rand() % 1000, RandomFloat(30, 60));
					oTiles[i][j] = new Ocean(1.0f);
					stepped = 1;
					if(i == numTiles && j == numTiles)
						done = 1;
				}

				// tTiles[i][j] = new Terrain(RandomFloat(0.8, 2.5), rand() % 1000);
			}

		}
	}

	if(done)
		numTiles += 1;
}


void initTerrain() {
	/*
	g_terrain = new Terrain(1.0, rand() % 1000);
	g_terrain2 = new Terrain(2.0, rand() % 1000);
	g_terrain3 = new Terrain(1.2, rand() % 1000);
	g_terrain4 = new Terrain(1.8, rand() % 1000);

	g_ocean = new Ocean(1.0f);
	g_ocean2 = new Ocean(1.0f);
	g_ocean3 = new Ocean(1.0f);
	g_ocean4 = new Ocean(1.0f);
	*/
	for(int i = 0; i < initNumTiles; i++){
		for(int j = 0; j < initNumTiles; j++){
			tTiles[i][j] = new Terrain(RandomFloat(0.8, 2.5), rand() % 1000, RandomFloat(30, 60));
			oTiles[i][j] = new Ocean(1.0f);
		}
	}
}

// Sets up where and what the light is
// Called once on start up
//
void initLight() {
	float direction[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	float diffintensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);


	glEnable(GL_LIGHT0);
}


// An example of how to load a texure from a hardcoded location
//
void initTexture() {
	Image tex1("./work/res/textures/grass.jpg");

	glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	glGenTextures(1, &g_texture); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, g_texture); // Bind it as a 2D texture


	// Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finnaly, actually fill the data into our texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex1.w, tex1.h, tex1.glFormat(), GL_UNSIGNED_BYTE, tex1.dataPointer());

	Image tex2("./work/res/textures/water.jpg");

	glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	glGenTextures(1, &g_waterTexture); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, g_waterTexture); // Bind it as a 2D texture


	// Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finnaly, actually fill the data into our texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex2.w, tex2.h, tex2.glFormat(), GL_UNSIGNED_BYTE, tex2.dataPointer());

}


// An example of how to load a shader from a hardcoded location
//
void initShader() {
	// To create a shader program we use a helper function
	// We pass it an array of the types of shaders we want to compile
	// and the corrosponding locations for the files of each stage
	g_shader = makeShaderProgramFromFile({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }, { "./work/res/shaders/shaderDemo.vert", "./work/res/shaders/shaderDemo.frag" });
}




// Sets up where the camera is in the scene
//
void setupCamera(int width, int height) {
	// Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_fovy, width / float(height), g_znear, g_zfar);

	// Set up the view part of the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	if(g_forward){
		g_fVal += g_panStep;
	}
	if(g_back){
		g_fVal -= g_panStep;
	}
	if(g_right){
		g_rVal += g_rotStep;
	}
	if(g_left){
		g_rVal -= g_rotStep;
	}
	if(g_up){
		g_uVal += g_rotStep;
	}
	if(g_down){
		g_uVal -= g_rotStep;
	}
	if(g_panUp){
		g_upPan += g_panStep;
	}
	if(g_panDown){
		g_upPan -= g_panStep;
	}
	if(g_panRight){
		g_rightPan += g_panStep;
	}
	if(g_panLeft){
		g_rightPan -= g_panStep;
	}


	// glTranslatef(0, 0, -50 * g_zoom);
	// glTranslatef( g_right, g_forward, -50 * g_zoom);
	/*
	gluLookAt(	g_rightPan, g_upPan, g_fVal,// position of camera
			// g_rVal + g_rightPan,  g_fVal + g_upPan, g_uVal,// position to look at
			  g_rightPan /*- g_rightPan,  g_upPan - 0.5*g_upPan, g_uVal - g_uVal,// position to look at

			0.0, 1.0, 0.0);// up relative to camera
			*/
	glTranslatef(g_rightPan, g_upPan , -50 * g_zoom);
	glRotatef(g_pitch, 1, 0, 0);
	glRotatef(g_yaw, 0, 1, 0);


}


// Draw function
//
void render(int width, int height) {
    // Make the viewport the whole screen
    glViewport(0, 0, width, height);

	// Grey/Blueish background
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Enable flags for normal rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	setupCamera(width, height);

	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, g_texture);

	//Render Terrain
	// int y;
	// int x;
	for(int i = 0; i < numTiles+1; i++){
		for(int j = 0; j < numTiles+1; j++){
			/*
			x = i;
			y = j;
			if(x % 2 > 0)
				x = -i;
			if(y % 2 > 0)
				y = -j;
				*/
			if(tTiles[i][j] ){
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, g_texture);
				glTranslatef((i * g_tileSize) -1, 0, (j * g_tileSize) -1);
				tTiles[i][j]->renderTerrain();
				glActiveTexture(GL_TEXTURE0);

				if(oTiles[i][j] && g_drawWater){
					glBindTexture(GL_TEXTURE_2D, g_waterTexture);
					oTiles[i][j]->renderOcean();

				}
				glTranslatef(-((i * g_tileSize) -1), 0, -((j * g_tileSize) -1));
			}
		}
	}
	/*
	g_terrain->renderTerrain();

	glTranslatef(-255, 0, 0);
	g_terrain2->renderTerrain();
	glTranslatef(255, 0, 0);

	glTranslatef(-255, 0, -255);
	g_terrain3->renderTerrain();
	glTranslatef(255, 0, 255);

	glTranslatef(0, 0, -255);
	g_terrain4->renderTerrain();
	glTranslatef(0, 0, 255);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_waterTexture);

	g_ocean->renderOcean();

	glTranslatef(-255, 0, 0);
	g_ocean2->renderOcean();
	glTranslatef(255, 0, 0);

	glTranslatef(-255, 0, -255);
	g_ocean3->renderOcean();
	glTranslatef(255, 0, 255);

	glTranslatef(0, 0, -255);
	g_ocean4->renderOcean();
	glTranslatef(0, 0, 255);
	*/

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);

}


// Forward decleration for cleanliness (Ignore)
void APIENTRY debugCallbackARB(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, GLvoid*);


//Main program
//
int main(int argc, char **argv) {


	// Initialize the GLFW library
	if (!glfwInit()) {
		cerr << "Error: Could not initialize GLFW" << endl;
		abort(); // Unrecoverable error
	}

	// Get the version for GLFW for later
	int glfwMajor, glfwMinor, glfwRevision;
	glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);

	// Create a windowed mode window and its OpenGL context
	// g_window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
	g_window = glfwCreateWindow(1366, 768, "Hello World", nullptr, nullptr);
	if (!g_window) {
		cerr << "Error: Could not create GLFW window" << endl;
		abort(); // Unrecoverable error
	}

	// Make the g_window's context is current.
	// If we have multiple windows we will need to switch contexts
	glfwMakeContextCurrent(g_window);



	// Initialize GLEW
	// must be done after making a GL context current (glfwMakeContextCurrent in this case)
	glewExperimental = GL_TRUE; // required for full GLEW functionality for OpenGL 3.0+
	GLenum err = glewInit();
	if (GLEW_OK != err) { // Problem: glewInit failed, something is seriously wrong.
		cerr << "Error: " << glewGetErrorString(err) << endl;
		abort(); // Unrecoverable error
	}



	// Print out our OpenGL verisions
	cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	cout << "Using GLFW " << glfwMajor << "." << glfwMinor << "." << glfwRevision << endl;



	// Attach input callbacks to g_window
	glfwSetCursorPosCallback(g_window, cursorPosCallback);
	glfwSetMouseButtonCallback(g_window, mouseButtonCallback);
	glfwSetScrollCallback(g_window, scrollCallback);
	glfwSetKeyCallback(g_window, keyCallback);
	glfwSetCharCallback(g_window, charCallback);



	// Enable GL_ARB_debug_output if available. Not nessesary, just helpful
	if (glfwExtensionSupported("GL_ARB_debug_output")) {
		// This allows the error location to be determined from a stacktrace
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		// Set the up callback
		glDebugMessageCallbackARB(debugCallbackARB, nullptr);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
		cout << "GL_ARB_debug_output callback installed" << endl;
	}
	else {
		cout << "GL_ARB_debug_output not available. No worries." << endl;
	}

	initTerrain();
	initLight();
	initTexture();
	initShader();



	// Loop until the user closes the window
	while (!glfwWindowShouldClose(g_window)) {

		// Make sure we draw to the WHOLE window
		int width, height;
		glfwGetFramebufferSize(g_window, &width, &height);
		//TODO
		//
		// updateLights();

		// Main Render
		render(width, height);

		// Swap front and back buffers
		glfwSwapBuffers(g_window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
}






//-------------------------------------------------------------
// Fancy debug stuff
//-------------------------------------------------------------

// function to translate source to string
string getStringForSource(GLenum source) {

	switch (source) {
	case GL_DEBUG_SOURCE_API:
		return("API");
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return("Window System");
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return("Shader Compiler");
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return("Third Party");
	case GL_DEBUG_SOURCE_APPLICATION:
		return("Application");
	case GL_DEBUG_SOURCE_OTHER:
		return("Other");
	default:
		return("n/a");
	}
}

// function to translate severity to string
string getStringForSeverity(GLenum severity) {

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		return("HIGH!");
	case GL_DEBUG_SEVERITY_MEDIUM:
		return("Medium");
	case GL_DEBUG_SEVERITY_LOW:
		return("Low");
	default:
		return("n/a");
	}
}

// function to translate type to string
string getStringForType(GLenum type) {
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		return("Error");
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return("Deprecated Behaviour");
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return("Undefined Behaviour");
	case GL_DEBUG_TYPE_PORTABILITY:
		return("Portability Issue");
	case GL_DEBUG_TYPE_PERFORMANCE:
		return("Performance Issue");
	case GL_DEBUG_TYPE_OTHER:
		return("Other");
	default:
		return("n/a");
	}
}

// actually define the function
void APIENTRY debugCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, GLvoid*) {
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) return;

	cerr << endl; // extra space

	cerr << "Type: " <<
		getStringForType(type) << "; Source: " <<
		getStringForSource(source) << "; ID: " << id << "; Severity: " <<
		getStringForSeverity(severity) << endl;

	cerr << message << endl;

	if (type == GL_DEBUG_TYPE_ERROR_ARB) throw runtime_error("");
}
