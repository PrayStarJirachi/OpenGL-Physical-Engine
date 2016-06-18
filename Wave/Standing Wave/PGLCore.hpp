#ifndef SJTU_PGLCORE_HPP
#define SJTU_PGLCORE_HPP

#include "PGLCommon.hpp"
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <vector>

namespace sjtu{

static const int MAXN = 5000;
static int windowWidth = 500;
static int windowHeight = 200;
static bool isRunning = false;
static double PI = acos(-1.0);
static const double G = 10.0;
static const double k = 500.0;
static const double timeStep = 0.03;
std::vector<PGLPoint> track;

struct PGLAtom{
	PGLPoint p;
	PGLVector v;
	PGLColor c;
	double r, m;
	
	PGLAtom() {}
	PGLAtom(const PGLPoint &a, const PGLVector &v, const double &r, const double &m) : p(a), v(v), r(r), m(m) {}
	
}atom[MAXN];

static int nAtom = 0, size_of_heap = 0;
static long initTime = 0;

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

void reDisplay(void) {
	if (isRunning) {
		static PGLAtom tmp[MAXN];
		static int flag = 0;
		if (true) {
			PGLVector a;
			tmp[0] = atom[0];
			a = -(tmp[0].p.y - 100) * 5.8 * PGLPoint(0, 1) / atom[0].m;
			tmp[0].v = tmp[0].v + timeStep * a;
			tmp[0].p = tmp[0].p + timeStep * tmp[0].v;
			//printf("Acc[0] = (%.2f, %.2f)\n", a.x, a.y);
		} else {
			tmp[0] = atom[0];
			tmp[0].p.y = 100;
		}
		for (int i = 1; i + 1 < nAtom; i++) {
			PGLVector a;
			if (i - 1 >= 0) {
				double r = atom[i - 1].p.y - atom[i].p.y;
				a = a + (k * r) / atom[i].m * PGLVector(0, 1);
			}
			if (i + 1 < nAtom) {
				double r = atom[i + 1].p.y - atom[i].p.y;
				a = a + (k * r) / atom[i].m * PGLVector(0, 1);
			}
			tmp[i] = atom[i];
			tmp[i].v = tmp[i].v + timeStep * a;
			tmp[i].p = tmp[i].p + timeStep * tmp[i].v;
			//printf("%.10f\n", a.length());
		}
		for (int i = 0; i + 1 < nAtom; i++) atom[i] = tmp[i];
		
		glutPostRedisplay();
	}
}

void display(void) {
#ifndef DEBUG
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
	double maxSpeed = 0;
	for (int i = 0; i < nAtom; i++) {
		maxSpeed = std::max(maxSpeed, atom[i].v.length());
	}
	
	glBegin(GL_LINE_STRIP);
	glColor3f(atom[0].c.value[0], atom[0].c.value[1], atom[0].c.value[2]);
	glVertex2f(atom[0].p.x, atom[0].p.y);
	for (int i = 1; i < nAtom; i++) {
		//PGLColor now = getColor(atom[i].v.length() / maxSpeed);
		
		/*glBegin(GL_TRIANGLE_FAN);
		glColor3f(atom[i].c.value[0], atom[i].c.value[1], atom[i].c.value[2]);
		glVertex2f(atom[i].p.x, atom[i].p.y);
		for (int angle = 0; angle <= 360; angle++) {
			PGLPoint dir = PGLPoint(cos(angle / 180.0 * PI), sin(angle / 180.0 * PI));
			PGLPoint pos = atom[i].p + atom[i].r * dir;
			glVertex2f(pos.x, pos.y);
		}
		glEnd();*/
		glVertex2f(atom[i].p.x, atom[i].p.y);
	}
	glEnd();
	
#ifndef DEBUG
	glutSwapBuffers();
#endif
}

void run(int argc, char **argv, const char *title, const PGLColor &background, int n)
{
	isRunning = true;
	track.clear();
#ifndef DEBUG
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (windowWidth, windowHeight); 
	glutInitWindowPosition (0, 0);
	glutCreateWindow(title);
	
	glClearColor(background.value[0], background.value[1], background.value[2], 0.0);
	gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
#endif
	
	nAtom = n;
	for (int i = 0, w = 50; i < n; i++, w += 2) {
		atom[i].p = PGLPoint(w, 100);
		atom[i].v = PGLPoint(0, 0);
		atom[i].c = BLACK;
		atom[i].r = 2;
		atom[i].m = 0.5;
	}
	atom[0].v = PGLPoint(0, 60);
	
	initTime = clock();

#ifndef DEBUG
	glutDisplayFunc(display);
	glutReshapeFunc(reShape);
	glutIdleFunc(reDisplay);
	
	glutMainLoop();
#endif
}

};

#endif
