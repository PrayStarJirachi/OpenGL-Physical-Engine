#ifndef SJTU_PGLCORE_HPP
#define SJTU_PGLCORE_HPP

#include "PGLCommon.hpp"
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace sjtu{

static const int MAXN = 5000;
static int windowWidth = 1000;
static int windowHeight = 700;
static bool isRunning = false;
static double PI = acos(-1.0);

struct PGLAtom{
	PGLPoint p;
	PGLVector v;
	double r;
	
}atom[MAXN];

static int nAtom = 0;
static long initTime = 0;

struct PGLCollision{
	long time;
	int first, second;
	
	bool operator <(const PGLCollision &rhs)const {
		return time > rhs.time;
	}
}nextCollision;

void (*updateNextCollision)(PGLCollision &nextCollision);

void reShape(GLsizei Width, GLsizei Height) {
	glViewport(0,0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	windowWidth = Width;
	windowHeight = Height;
}

void updateAtom() {
	if (!isRunning) return;
	/*long colTime = nextCollision.time;
	PGLAtom a = atom[nextCollision.first];*/
}

void reDisplay(void) {
	if (isRunning) {
		long time = clock() - initTime;
		while (time > nextCollision.time) {
			updateAtom();
			updateNextCollision(nextCollision);
		}
		for (int i = 0; i < nAtom; i++) {
			atom[i].p = atom[i].p + time * atom[i].v;
		}
		glutPostRedisplay();
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	double maxSpeed = 0;
	for (int i = 0; i < nAtom; i++) {
		maxSpeed = std::max(maxSpeed, atom[i].v.length());
	}
	for (int i = 0; i < nAtom; i++) {
		PGLColor now = getColor(atom[i].v.length() / maxSpeed);
		
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(now.value[0], now.value[1], now.value[2]);
		glVertex2f(atom[i].p.x, atom[i].p.y);
		for (int angle = 0; angle <= 360; angle = angle + 1) {
			PGLPoint dir = PGLPoint(cos(angle / 180.0 * PI), sin(angle / 180.0 * PI));
			PGLPoint pos = atom[i].p + atom[i].r * dir;
			glVertex2f(pos.x, pos.y);
		}
		glEnd();
	}
	glutSwapBuffers();
}

void run(int argc, char **argv, const char *title, const PGLColor &background, int (*__initAtom)(PGLAtom *atom),
         void (*__updateNextCollision)(PGLCollision &nextCollision))
{
	isRunning = true;
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (windowWidth, windowHeight); 
	glutInitWindowPosition (0, 0);
	glutCreateWindow (title);
	
	glClearColor(background.value[0], background.value[1], background.value[2], 0.0);
	gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
	
	nAtom = __initAtom(atom);
	updateNextCollision = __updateNextCollision;
	initTime = clock();
	
	glutDisplayFunc(display);
	glutReshapeFunc(reShape);
	glutIdleFunc(reDisplay);
	
	glutMainLoop();
}

};

#endif
