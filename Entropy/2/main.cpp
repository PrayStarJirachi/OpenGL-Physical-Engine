#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "PGLCore.hpp"
#include <bits/stdc++.h>

const char *title = "OpenGL Physical Engine V1.2";
const int RADIUS = 5;
double sqrr(double x){
	return x * x;
}
int rr(){
	if(rand() % 2 == 0) return 1;
	return -1;
}
int init(sjtu::PGLAtom *atom) {
	double rate = 100;
	srand(time(NULL));
	int n = 0;
	for(int i = 20; i < 580; i +=24)
	for(int j = 20; j < 580; j +=24){
		atom[n].p = sjtu::PGLPoint(i, j);
		atom[n].v = sjtu::PGLPoint(0, 0);	
		atom[n].r = RADIUS;
		atom[n].m = 1.0;
		if(rand() % 625 < 20){
			atom[n].v = sjtu::PGLPoint(1.0 * (rand() % 22  + 1)/100 * rr(), 1.0 * (rand() % 22 + 1)/100 * rr());
		}
		n++;
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
