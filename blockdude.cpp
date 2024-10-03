/*
	Blockdude
	Assignment #2 in CSCI1097 -- OpenGL
	Based on the popular game for the TI-83
	
	please read "readme.txt" in this directory
	for information about this program and other random stuff
	
	Thanks for your interest in this program :)
*/


#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
/* #include <string.h>*/
/* #include <math.h>*/
#include <cstdio>
#include <cassert>
#include <cstring>
#include "Level.h"
#include "modes.h"


#define WINDOW_HEIGHT	500
#define WINDOW_WIDTH	700

//level object
Level theLevel(1);
int gMode = TITLE_SCREEN_MODE;
int gLevel = 1;
int gSelected = NONE_SELECTED;
int gAnim = 0;
int gcount = 0;
//screen coords

int h_min = 0;
int h_max = 600;
int v_min = 0;
int v_max = 350;

//world coords (the blockdude game on the ti-83 was 12x7, that's why i chose
// these world coords
double world_x_min = 0.0;
double world_y_min = 0.0;
double world_x_max = 12.0;
double world_y_max = 7.0;


//scale factors & functions
double x_scale, y_scale, x_translate, y_translate;
void calculateScaleFactors();
void WorldToView(double x, double y, int *h, int *v);
void ViewToWorld(int h, int v, double *x, double *y);

void getDocumentPath(char *arg, char *ret);

void getDocumentPath(char *arg, char *ret) {
	int len = strlen(arg);
	int i = len, j;
	while (i >= 0) {
		if(arg[i] == '/')
			break;
		else
			i--;
	}
	for (j = 0; j <= i; j++) {
		ret[j] = arg[j];
	}
	ret[j+1] = '\0';
}
	


void init(void);

void display(void);
void myMouse(int, int, int, int);
void myReshape(int, int);
void myMouseMove(int, int);
void myKeyboard(unsigned char, int, int);
void myTimer(int);
void myMenu(int);

int main(int argc, char *argv[]) {
	char base[512];
	getDocumentPath(argv[0], base);
	printf("base: %s\n", base);
	calculateScaleFactors();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(h_max, v_max);
	glutInitWindowPosition(100, 40);
	glutCreateWindow("Blockdude");
	init();
	
	glutDisplayFunc(display);
	glutMouseFunc(myMouse);
	glutReshapeFunc(myReshape);
	glutPassiveMotionFunc(myMouseMove); //this is function that gets called
	//when you don't have your mouse clicked but it is just moving around
	// so like on a web page when you put your mouse over a link and it changes
	// color
	glutKeyboardFunc(myKeyboard);
	
	glutCreateMenu(myMenu);
	
	//should probably add a Quit Game to this menu...
	glutAddMenuEntry("New Game", NEW_GAME);
	glutAddMenuEntry("Reset Level", RESET_LEVEL);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//glutTimerFunc(100, myTimer, 1);
	glutTimerFunc(50, myTimer, 2);
	//glEnable(GL_COLOR_LOGIC_OP);
	glutMainLoop();
	
	return 0;
}

void init(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world_x_min, world_x_max, world_y_min, world_y_max);
}

void display(void) { 
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);
	/*glBegin(GL_QUADS);
		glVertex2f(2.0, 2.0);
		glVertex2f(1.0, 2.0);
		glVertex2f(1.0, 1.0);
		glVertex2f(2.0, 1.0);
	glEnd();*/
	if (gMode == TITLE_SCREEN_MODE && gAnim == 0)
		gAnim = theLevel.animIntro(gcount);
	if (gMode == TITLE_SCREEN_MODE && gAnim == 1)
		theLevel.drawTitleScreen(gSelected);
	else if (gMode == LEVEL_MODE)
		theLevel.draw();

	glutSwapBuffers();
	//glFlush();
}

void myMouse(int button, int state, int x, int y) {
	//if x < location of sprite, MOVE_LEFT
	//if x > location of sprite, MOVE_RIGHT
	//if x near location of sprite & y > sprite MOVE_UP
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if(gSelected == NEW_SELECTED) {
			gMode = LEVEL_MODE;
			glutPostRedisplay();
			glutTimerFunc(100, myTimer, 1);
		} else if(gSelected == QUIT_SELECTED)
			exit(EXIT_SUCCESS);
	}
}

void myMouseMove(int x, int y) {
	//have the blocks light up or something? idk lol
	double xdouble, ydouble;
	y = v_max - y;
	ViewToWorld(x, y, &xdouble, &ydouble);
	if (gMode == TITLE_SCREEN_MODE) {
	if (xdouble > 4.2 && xdouble < 7.8) {
		if (ydouble > 5.2 && ydouble < 5.8) {
			gSelected = NEW_SELECTED;
		} else if (ydouble > 3.2 && ydouble < 3.8) {
			gSelected = PASS_SELECTED;
		} else if (ydouble > 1.2 && ydouble < 1.8) {
			gSelected = QUIT_SELECTED;
		} else {
			gSelected = NONE_SELECTED;
		}
	}
	}
	//printf("(%f, %f)\n", xdouble, ydouble);
	glutPostRedisplay();
	
}

void myKeyboard(unsigned char key, int x, int y) {
	if (key == 'n') {
		//new game
		//gMode = TITLE_SCREEN_MODE;
		//gLevel = 1;
	} else if (key == 'r') {
		//reset level
	} else if (key == 'w') {
		theLevel.move(MOVE_UP);
	} else if (key == 'a') {
		theLevel.move(MOVE_LEFT);
	} else if (key == 'd') {
		theLevel.move(MOVE_RIGHT);
	} else if (key == 's') {
		theLevel.pickup();
	}
	glutPostRedisplay();
}

void myTimer(int v) {
	// have the moveable blocks light up every so many seconds
	if (v == 1) {
		static float offset = .125;
		if (theLevel.block_color[1] > 1.0f)
			offset = -.125;
		else if (theLevel.block_color[1] < 0.0f)
			offset = .125;
		theLevel.block_color[0] = theLevel.block_color[1] += offset;

		glutPostRedisplay();
		glutTimerFunc(100, myTimer, 1);
	} else if (v == 2) {
		gcount++;
		glutTimerFunc(10, myTimer, 2);
		//glutPostRedisplay();
	}
}

void myMenu(int id) { 
	// attach menu with new game and reset
	if (id == NEW_GAME) {
		theLevel.newLevel(1);
	} else if (id == RESET_LEVEL) {
		theLevel.resetLevel();
	}
	
	glutPostRedisplay();
}

void myReshape(int h, int v) {
	h_max = h;
	v_max = v;
	glViewport(0,0, h, v);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world_x_min, world_x_max, world_y_min, world_y_max);
	calculateScaleFactors();
}

void calculateScaleFactors() {
	x_scale = (h_max - h_min)/(world_x_max - world_x_min);
	y_scale = (v_max - v_min)/(world_y_max - world_y_min);
	
	x_translate = h_max - world_x_max * x_scale;
	y_translate = v_max - world_y_max * y_scale;
}

void WorldToView(double x, double y, int *h, int *v) {
	*h =  (int) (x * x_scale +  x_translate);
	*v =  (int) (y * y_scale +  y_translate);
}

void ViewToWorld(int h, int v, double *x, double *y) { 
	*x =  (h  - x_translate)/x_scale;
	*y =  (v  - y_translate)/y_scale;
} 
