// Revised 2019 with a bit better variable names.

// This worked 2015: 
// Linux: gcc lab0.c ../common/*.c ../common/Linux/MicroGlut.c -lGL -o lab0 -I../common -I../common/Linux
// Mac: gcc lab0.c ../common/*.c ../common/Mac/MicroGlut.m -o lab0 -framework OpenGL -framework Cocoa -I../common/Mac -I../common


// gcc lab1-1.c ../common/*.c -framework OpenGL -framework Cocoa -o lab1-1 -I../common
// Mac: gcc lab1-1.c ../common/*.c  -o lab0 -framework OpenGL -framework Cocoa -I../common/Mac -I../common

//gcc lab1-1.c ../common/*.c -lGL -o lab1-1 -I../common
//gcc lab1-1.c ../common/*.c -o lab1-1 -framework OpenGL -framework Cocoa -I../common

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	//uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

//constants
const int initWidth=512, initHeight=512;

float time = 0.0;
// Model-to-world matrix
// Modify this matrix.
// See below for how it is applied to your model.
mat4 modelToWorld = {{ 1.0, 0.0, 0.0, 0.0,
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 1.0}};
// World-to-view matrix. Usually set by lookAt() or similar.
mat4 worldToView;
// Projection matrix, set by a call to perspective().
mat4 projectionMatrix;

// Globals
// * Model(s)
Model *bunny;
Model *cube;
// * Reference(s) to shader program(s)
GLuint program;
// * Texture(s)
GLuint texture;



void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits"); // This is merely a vague indication of where something might be wrong

	projectionMatrix = perspective(90, 1.0, 0.1, 500);
	worldToView = lookAt(0,0,1.5, 
							0,0,0, 
							0,1,0);

	// Load and compile shader
	program = loadShaders("lab0.vert", "lab0.frag");
	printError("init shader");

	// Upload geometry to the GPU:
	bunny = LoadModelPlus("objects/stanford-bunny.obj");
	cube = LoadModelPlus("objects/cubeplus.obj");
	printError("load models");

	// Load textures
	LoadTGATextureSimple("textures/maskros512.tga",&texture);
	printError("load textures");

}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//activate the program, and set its variables
	glUseProgram(program);
	//glUniform1f(glGetUniformLocation(program, "t"), t);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	mat4 modelToWorldToView = Mult(worldToView, modelToWorld); // Combine to one matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorldToView"), 1, GL_TRUE, modelToWorldToView.m);

	//Texture
	glUniform1i(glGetUniformLocation(program,"exampletexture"),0);//the last argument has to be the same as the texture-unit that is to be used
	glActiveTexture(GL_TEXTURE0);//which texture-unit is active
	glBindTexture(GL_TEXTURE_2D, texture);//load the texture to active texture-unit

	//Add time variable
	glUniform1f(glGetUniformLocation(program, "time"), time);
	time = time + 0.1;

	//draw the model
	DrawModel(bunny, program, "in_Position", "in_Normal", "in_TexCoord");
	DrawModel(cube, program, "in_Position", "in_Normal", "in_TexCoord");
	
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Lab 0 - OpenGL 3.2+ Introduction");
	glutDisplayFunc(display); 
	glutRepeatingTimer(20);
	init ();
	glutMainLoop();
	exit(0);



}

