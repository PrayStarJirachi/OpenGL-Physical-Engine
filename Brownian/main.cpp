#include "PGLCore.hpp"
#include <bits/stdc++.h>

const char *title = "OpenGL Physical Engine V1.2";
const int RADIUS = 5;

int init(sjtu::PGLAtom *atom) {
	/*atom[0].p = sjtu::PGLPoint(100, 100);
	atom[0].v = sjtu::PGLPoint(0, 0);
	atom[0].r = RADIUS;
	atom[0].m = 1.0;
	
	atom[1].p = sjtu::PGLPoint(500, 110);
	atom[1].v = sjtu::PGLPoint(-0.005, 0);
	atom[1].r = RADIUS;
	atom[1].m = 1.0;
	
	atom[2].p = sjtu::PGLPoint(100, 400);
	atom[2].v = sjtu::PGLPoint(0.001, 0.001) * 2;
	atom[2].r = RADIUS;
	atom[2].m = 1.0;
	
	atom[3].p = sjtu::PGLPoint(50, 400);
	atom[3].v = sjtu::PGLPoint(0.002, 0.0001) * 2;
	atom[3].r = RADIUS;
	atom[3].m = 1.0;
	
	atom[4].p = sjtu::PGLPoint(30, 400);
	atom[4].v = sjtu::PGLPoint(0.002, 0.001) * 5;
	atom[4].r = RADIUS;
	atom[4].m = 1.0;*/
	double rate = 70;
	int n = 1001;
	atom[0].p = sjtu::PGLPoint(rand() % (1000 - RADIUS) + RADIUS, rand() % (700 - RADIUS) + RADIUS);
	atom[0].v = sjtu::PGLPoint(0, 0);
	atom[0].r = RADIUS * 5;
	atom[0].m = 100.0;
	for (int i = 1; i < n; i++) {
		atom[i].p = sjtu::PGLPoint(rand() % (1000 - RADIUS) + RADIUS, rand() % (700 - RADIUS) + RADIUS);
		atom[i].v = sjtu::PGLPoint((rand() % 10 - 5) / rate, (rand() % 10 - 5) / rate);
		atom[i].r = RADIUS;
		atom[i].m = 1.0;
	}
	
	return n;
}

int main(int argc, char **argv) {
	printf("%s\n", title);
	//printf("Press ENTER to run the program.");
	//getchar();
	sjtu::run(argc, argv, title, sjtu::WHITE, init);
	return 0;
}
