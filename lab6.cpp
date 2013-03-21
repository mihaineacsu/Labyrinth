#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <vector>

#include "glut.h"
#include "ground.h"
#include "camera.h"
#include "MazeGen.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 800
#define MAZE_LINES 40
#define MAZE_COLS 40
#define WALL_SIZE 60
#define WALL_MARGIN 0.001
#define MOUSE_SPEED 0.001
#define MOVE_SPEED 2
#define RADIUS 23 //3rd Person distance from target

enum Direction{
	forward,
	backward,
	left,
	right,
	none
};

enum{
	MOUSE_LEFT_BUTTON = 1,
	MOUSE_RIGHT_BUTTON = 2,
};

GameMode crntMode = first_person;
Direction toMove = none;

int **mazeMap;
Maze *maze;

int newWidth, newHeight;
float radiusNew = RADIUS;

//cam
Camera camera;

//cuburi
std::vector<cubeCoords> cube;

void getCubeCoords(){
	for (int i = 0; i < MAZE_LINES; ++i)
		for (int j = 0; j < MAZE_COLS; ++j)
			if (mazeMap[i][j] != ROAD)
				cube.push_back(cubeCoords(i * (WALL_SIZE + WALL_MARGIN), 
													j * (WALL_SIZE + WALL_MARGIN),
													WALL_SIZE));
}

//functie afisare
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LIGHTING);

	//set proiectie #1
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,newWidth,newHeight);
	gluPerspective(45,(float)newWidth/(float)newHeight,0.2,500);
	//setup view
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	camera.render(crntMode, radiusNew);

	//rest scena
	GLfloat white[] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat cyan[] = {0.f, .8f, .8f, 1.f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	GLfloat shininess[] = {50};
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	for (int i = 0; i < MAZE_LINES; ++i)
		for (int j = 0; j < MAZE_COLS; ++j){
			switch(mazeMap[i][j]){
				case ROAD:

					break;
				
				case WALL:
				case MARGIN_WALL:
					glPushMatrix();
					
					glColor3f(1, 0, 1);
					glTranslatef(0, WALL_SIZE / 2, 0);
					glTranslatef(i * (WALL_SIZE + WALL_MARGIN), 
						0, 
						j * (WALL_SIZE + WALL_MARGIN));

					glScalef(WALL_SIZE, 60, WALL_SIZE);
					glutSolidCube(1);

					glPopMatrix();
					break;
			}
		}
	glPushMatrix();
	glTranslatef(maze->finishX * (WALL_SIZE + WALL_MARGIN), 20, maze->finishY * (WALL_SIZE + WALL_MARGIN));
	gluSphere(gluNewQuadric(), 10, 50,50);
	glPopMatrix();

	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, (float)newWidth / 4 , newHeight / 4);


	glClear(GL_DEPTH_BUFFER_BIT);


	//set proiectie #2
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(20, 20, newWidth / 4, newHeight / 4);
	gluPerspective(45,(float)newWidth/(float)newHeight,0.2,500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	camera.render(mini_map, radiusNew);
	
	glColor3f(0.5,0.5,0.5);

	//rest scena
	for (int i = 0; i < MAZE_LINES; ++i)
		for (int j = 0; j < MAZE_COLS; ++j){
			switch(mazeMap[i][j]){
				case ROAD:

					break;
				
				case WALL:
				case MARGIN_WALL:
					glPushMatrix();
					
					glColor3f(1, 0, 0);
					glTranslatef(0, -WALL_SIZE / 2, 0);
					glTranslatef(i * (WALL_SIZE + WALL_MARGIN), 
						0, 
						j * (WALL_SIZE + WALL_MARGIN));
					glutSolidCube(WALL_SIZE);

					glPopMatrix();
					break;
			}
		}

	//swap buffers
	glDisable(GL_SCISSOR_TEST);
	glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height){
	newWidth = width;
	newHeight = height;
}


void idle();

void keyboardRelease(unsigned char ch, int x, int y){
	toMove = none;
}

void keyboard(unsigned char ch, int x, int y){
	switch(ch){
		case 27:	//esc
			exit(0);
			break;

		case 'w':
			toMove = forward;
			break;

		case 's':
			toMove = backward;
			break;

		case 'a':
			toMove = left;
			break;

		case 'd':
			toMove = right;
			break;

		case '1':
			crntMode = first_person;
			break;

		case '2':
			crntMode = third_person;
			break;

		case '3':
			crntMode = map;
			break;

		case '+':
			radiusNew++;

			if (radiusNew > 50)
				radiusNew = 50;

			break;

		case '-':
			radiusNew--;

			if (radiusNew < 4)
				radiusNew = 4;

			break;
		
		default:
			break;
	}

}

//idle
void idle(){
	switch(toMove){
		case none:
			break;

		case forward:
			camera.translateForward(1, MOVE_SPEED);

			break;

		case backward:
			camera.translateForward(-1, MOVE_SPEED);

			break;

		case left:
			camera.translateRight(-1, MOVE_SPEED);

			break;

		case right:
			camera.translateRight(1, MOVE_SPEED);

			break;
	}
	
	glutPostRedisplay();
}

void onMouseMove(int x, int y){
	switch (crntMode){
		case map:
		case third_person:
			
		case first_person:
			camera.onMouseMove(x, y, MOUSE_SPEED, crntMode);

			break;
	}

	glutPostRedisplay();
}

void onMouseClick(int button, int state, int x, int y){
    onMouseMove(x, y);
}

int main(int argc, char *argv[]){
	Maze m = Maze(MAZE_LINES, MAZE_COLS);
	maze = &m;
	mazeMap = m.getMap();
	getCubeCoords();
	
	//init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	//init window
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition(200,400);
	glutCreateWindow("Tema 3");
	
	glutSetCursor(GLUT_CURSOR_NONE); 

	//callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardRelease);
	glutPassiveMotionFunc(onMouseMove);
	glutMouseFunc(onMouseClick);
	glutIdleFunc(idle);

	//z test on
	glEnable(GL_DEPTH_TEST);

	//set background
	glClearColor(0.2,0.2,0.2,1.0);

	//init camera
	cubeCoords endCoords = cubeCoords(maze->finishX * (WALL_SIZE + WALL_MARGIN),  maze->finishY * (WALL_SIZE + WALL_MARGIN), WALL_SIZE);
	camera.init(Vector3D(m.startX * (WALL_SIZE + WALL_MARGIN), 0, m.startY * (WALL_SIZE + WALL_MARGIN)), 
		cube, &endCoords);

	//loop
	glutMainLoop();

	return 0;
}