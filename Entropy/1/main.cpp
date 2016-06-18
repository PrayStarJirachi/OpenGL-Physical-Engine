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
	for(int i = 0; i < 25; i++){
		atom[n].p = sjtu::PGLPoint(rand() % 250 + 25, rand() % 650 + 25);
		atom[n].v = sjtu::PGLPoint(1.0 * (rand() % 30  + 1)/100 * rr(), 1.0 * (rand() % 30 + 1)/100 * rr());
		atom[n].r = RADIUS;
		atom[n].m = 1.0;
		n++;
	}
	const int tmp = 35;
	for(int i = 0; i < tmp; i++)
		for(int j  = 0; j < tmp; j++)
		{
			double x = 300.0 + 700.0 / (double)tmp * (double)i;
			double y = 700.0 / (double)tmp * (double)j;
			if((sqrr(x - 650.0) + sqrr(y - 350.0)) < sqrr(300.0)){
				atom[n].p = sjtu::PGLPoint(x, y);
				atom[n].v = sjtu::PGLPoint(0, 0);	
				atom[n].r = RADIUS;
				atom[n].m = 1.0;
				n++;
			}
		}
	printf("n = %d\n",n);
	return n;
}

int main(int argc, char **argv) {
	printf("%s\n", title);
	//printf("Press ENTER to run the program.");
	//getchar();
	sjtu::run(argc, argv, title, sjtu::WHITE, init);
	return 0;
}
