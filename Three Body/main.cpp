#include "PGLCore.hpp"
#include <bits/stdc++.h>

const char *title = "OpenGL Physical Engine V1.2";
const int RADIUS = 5;

int init(sjtu::PGLAtom *atom) {
	int n = 3;
	atom[0].p = sjtu::PGLPoint(200, 300);
	atom[0].v = sjtu::PGLPoint(0, 22);
	atom[0].r = 10;
	atom[0].m = 100000;
	atom[0].c = sjtu::RED;
	
	
	atom[1].p = sjtu::PGLPoint(300, 300);
	atom[1].v = sjtu::PGLPoint(0, -22);
	atom[1].r = 10;
	atom[1].m = 100000;
	atom[1].c = sjtu::BLUE;
	
	atom[2].p = sjtu::PGLPoint(350, 20.01);
	atom[2].v = sjtu::PGLPoint(0.001, 0);
	atom[2].r = 2;
	atom[2].m = 0.001;
	atom[2].c = sjtu::BLACK;
	return 3;
}

int main(int argc, char **argv) {
	printf("%s\n", title);
	//printf("Press ENTER to run the program.");
	//getchar();
	sjtu::run(argc, argv, title, sjtu::WHITE, init);
	return 0;
}
