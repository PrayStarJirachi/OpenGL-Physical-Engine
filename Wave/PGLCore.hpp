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
static int windowWidth = 1400;
static int windowHeight = 800;
static bool isRunning = false;
static double PI = acos(-1.0);
static const double G = 10.0;
static const double timeStep = 0.01;
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
		for (int i = 1; i < nAtom; i++) {
			PGLVector a;
			for (int j = 0; j < nAtom; j++) {
				if (i == j) continue;
				PGLVector r = atom[j].p - atom[i].p;
				a = a + G * r / r.length() * atom[j].m / r.sqrLength();
			}
			tmp[i] = atom[i];
			tmp[i].v = tmp[i].v + timeStep * a;
			tmp[i].p = tmp[i].p + timeStep * tmp[i].v;
			//printf("%.10f\n", a.length());
		}
		for (int i = 0; i < nAtom; i++) atom[i] = tmp[i];
		
		if (track.empty() || (atom[2].p - *(--track.end())).length() > 1e-3) {
			track.push_back(atom[2].p);
		}
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
	for (int i = 0; i < nAtom; i++) {
		//PGLColor now = getColor(atom[i].v.length() / maxSpeed);
		
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(atom[i].c.value[0], atom[i].c.value[1], atom[i].c.value[2]);
		glVertex2f(atom[i].p.x, atom[i].p.y);
		for (int angle = 0; angle <= 360; angle++) {
			PGLPoint dir = PGLPoint(cos(angle / 180.0 * PI), sin(angle / 180.0 * PI));
			PGLPoint pos = atom[i].p + atom[i].r * dir;
			glVertex2f(pos.x, pos.y);
		}
		glEnd();
	}
	if (!track.empty()) {
		glBegin(GL_LINES);
		for (int i = 1; i < (int)track.size(); i++) {
			glColor3f(0.0, 0.0, 0.0);
			glVertex2f(track[i - 1].x, track[i - 1].y);
			glVertex2f(track[i].x, track[i].y);
		}
		glEnd();
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(track[0].x, track[0].y);
		glVertex2f(track[(int)track.size() - 1].x, track[(int)track.size() - 1].y);
		glEnd();
	}
	
#ifndef DEBUG
	glutSwapBuffers();
#endif
}

void run(int argc, char **argv, const char *title, const PGLColor &background, int (*__initAtom)(PGLAtom *atom))
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
	
	nAtom = __initAtom(atom);
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
