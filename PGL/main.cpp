#include "PGLCore.hpp"
#include <bits/stdc++.h>

const char *title = "OpenGL Physical Engine V1.0";

int init(sjtu::PGLAtom *atom) {
	atom[0].p = sjtu::PGLPoint(300, 300);
	atom[0].v = sjtu::PGLPoint(1, 2);
	atom[0].r = 5;
	atom[1].p = sjtu::PGLPoint(600, 600);
	atom[1].v = sjtu::PGLPoint(5, 6);
	atom[1].r = 5;
	return 2;
}

void nextCol(sjtu::PGLCollision &next) {
	next.time = 0;
	next.first = 0;
	next.second = 0;
}

int main(int argc, char **argv) {
	printf("%s\n", title);
	printf("Press ENTER to run the program.");
	getchar();
	sjtu::run(argc, argv, title, sjtu::BLACK, init, nextCol);
	return 0;
}
