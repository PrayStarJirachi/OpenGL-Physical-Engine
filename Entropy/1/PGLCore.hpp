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
#include <iostream>

namespace sjtu{

static const int MAXN = 5000;
static const double eps = 1e-10;
static int windowWidth = 1000;
static int windowHeight = 700;
static bool isRunning = false;
static double PI = acos(-1.0);

struct PGLAtom{
	PGLPoint p;
	PGLVector v;
	double r, m;
	
	PGLAtom() {}
	PGLAtom(const PGLPoint &a, const PGLVector &v, const double &r, const double &m) : p(a), v(v), r(r), m(m) {}
	
}atom[MAXN], show[MAXN];

struct PGLCollision{
	double time;
	int first, second;
	
	PGLCollision(){}
	PGLCollision(double time, int first, int second):time(time), first(first), second(second){}
	
	bool operator <(const PGLCollision &rhs)const {
		return time > rhs.time;
	}
	bool operator >(const PGLCollision &rhs)const {
		return time < rhs.time;
	}
}nextCollision;

static int nAtom = 0, size_of_heap = 0;
static long initTime = 0;
static int heap_id[MAXN][MAXN];
PGLCollision heap[MAXN * MAXN];

/****************************************************************************/
inline double sqr(double x) {return x * x;}
double ball_ball(double x1, double y1, double vx1, double vy1, double r1,
			     double x2, double y2, double vx2, double vy2, double r2)
{
	double px = x2 - x1, py = y2 - y1;
	double vx = vx2 - vx1, vy = vy2 - vy1;
	double r = r1 + r2;
	if (sqr(px) + sqr(py) < sqr(r)) return -1;
	//assert(sqr(px) + sqr(py) >= sqr(r));
	double A = sqr(vx) + sqr(vy);
	if (fabs(A) < eps) return -1.;
	double B = 2. * (px * vx + py * vy);
	double C = sqr(px) + sqr(py) - sqr(r);
	double del = sqr(B) - 4. * A * C;
	if (del < 0) return -1.;
	double ans = (-B - sqrt(del))/ (2. * A);
	if (ans < 0) return -1.;
	return ans;
}
double ball_xwall(double x1, double y1, double vx1, double vy1, double r1, double wx)
{
	//assert(fabs(x1 - wx) >= r1);
	if (fabs(x1 - wx) < r1) return -1;
	if (fabs(vx1) < eps) return -1.;
	double t = (wx - x1) / vx1 - r1 / fabs(vx1);
	if (t < 0) return -1.;
	return t;
}
double ball_ywall(double x1, double y1, double vx1, double vy1, double r1, double wy)
{
	//assert(fabs(y1 - wy) >= r1);
	if (fabs(y1 - wy) < r1) return -1;
	if (fabs(vy1) < eps) return -1.;
	double t = (wy - y1) / vy1 - r1 / fabs(vy1);
	if (t < 0) return -1.;
	return t;
}
double obj_obj(int i, int j)
{
	if (j < nAtom)
	{
		return ball_ball(atom[i].p.x, atom[i].p.y, atom[i].v.x, atom[i].v.y, atom[i].r,
						 atom[j].p.x, atom[j].p.y, atom[j].v.x, atom[j].v.y, atom[j].r);
	}
	else if (j == nAtom)
	{
		return ball_ywall(atom[i].p.x, atom[i].p.y, atom[i].v.x, atom[i].v.y, atom[i].r, windowHeight);
	}
	else if (j == nAtom + 1)
	{
		return ball_ywall(atom[i].p.x, atom[i].p.y, atom[i].v.x, atom[i].v.y, atom[i].r, 0);
	}
	else if (j == nAtom + 2)
	{
		return ball_xwall(atom[i].p.x, atom[i].p.y, atom[i].v.x, atom[i].v.y, atom[i].r, 0);
	}
	else
	{
		return ball_xwall(atom[i].p.x, atom[i].p.y, atom[i].v.x, atom[i].v.y, atom[i].r, windowWidth);
	}
}
/****************************************************************************/

/****************************************************************************/
void maintain_down(int x){
	while((x << 1)<=size_of_heap){
		int y = (x << 1);
		if(y != size_of_heap && heap[y + 1] > heap[y]) ++y;
		if(heap[x] > heap[y]) break;
		std::swap(heap[x], heap[y]);
		std::swap(x, y);
		heap_id[heap[x].first][heap[x].second] = x;
		heap_id[heap[y].first][heap[y].second] = y;
	}
}

void maintain_up(int x){
	while (x != 1){
		int y = x >> 1;
		if(heap[x] < heap[y]) break;
		std::swap(heap[x], heap[y]);
		std::swap(x, y);
		heap_id[heap[x].first][heap[x].second] = x;
		heap_id[heap[y].first][heap[y].second] = y;
	}
}

void build_heap(){
	memset(heap_id, 0, sizeof(heap_id));
	size_of_heap = 0;
	for(int i = 0; i < nAtom; i++)
		for(int j = i + 1; j <= nAtom + 3; j++){
			double collision_time = obj_obj(i, j);
			//printf("Find Col(%d, %d, %.2f)\n", i, j, collision_time);
			if(collision_time < 0) continue;
			heap_id[i][j] = ++size_of_heap;
			heap[size_of_heap] = PGLCollision(collision_time, i, j);
		}
	for(int i = size_of_heap; i; i--) maintain_down(i);
	printf("%d\n", size_of_heap);
}

void real_delete(int x,int y){
	int tmp = heap_id[x][y];
	heap_id[x][y] = 0;
	if(tmp == size_of_heap){
		size_of_heap--;
		return;
	}
	heap[tmp] = heap[size_of_heap --];
	heap_id[heap[tmp].first][heap[tmp].second] = tmp;
	maintain_up(tmp);
	maintain_down(tmp);
}

void delete_from_heap(int x){
	for(int i = 0; i < x; i++)
		if(heap_id[i][x]) real_delete(i,x);
	for(int i = x + 1; i <= nAtom + 3; i++)
		if(heap_id[x][i]) real_delete(x,i);
}

void add_in_heap(int x, double now_time){
	for(int i = 0; i < x; i++){
		double collision_time = obj_obj(i, x);
		if(collision_time > now_time){
			heap_id[i][x] = ++size_of_heap;
			heap[size_of_heap] = PGLCollision(collision_time, i, x);
			maintain_up(size_of_heap);
		}
	}
	for(int i = x + 1; i <= nAtom + 3; i++){
		double collision_time = obj_obj(x, i);
		if(collision_time > now_time){
			heap_id[x][i] = ++size_of_heap;
			heap[size_of_heap] = PGLCollision(collision_time, x, i);
			maintain_up(size_of_heap);
		}
	}
}

void updateNextCollision(){
	assert(size_of_heap);
	nextCollision = heap[1];
}

void updateAtom() {
	assert(size_of_heap);
	PGLCollision collision = heap[1];
	int x = collision.first;
	int y = collision.second;
	delete_from_heap(x);
	delete_from_heap(y);
	
	PGLAtom a = PGLAtom(atom[x].p + collision.time * atom[x].v, atom[x].v, atom[x].r, atom[x].m);
	atom[x].p = atom[x].p + collision.time * atom[x].v;
	if (y == nAtom || y == nAtom + 1) {
		atom[x].v.y = -atom[x].v.y;
	} else if (y == nAtom + 2 || y == nAtom + 3) {
		atom[x].v.x = -atom[x].v.x;
	} else {
		PGLAtom b = PGLAtom(atom[y].p + collision.time * atom[y].v, atom[y].v, atom[y].r, atom[y].m);
		
		//assert(fabs((a.p - b.p).length() - 2 * a.r) <= 1e-5);
		double energy = 0;
		energy += a.v.length() * a.v.length();
		energy += b.v.length() * b.v.length();
		//printf("Before Ene = %.10f\n", energy);
		atom[y].p = atom[y].p + collision.time * atom[y].v;
		
		PGLVector r = b.p - a.p;
		double l = r.length();
		l *= l;
		PGLVector pVa = dot(r, a.v) / l * r;
		PGLVector pVb = dot(r, b.v) / l * r;
		PGLVector hVa = a.v - pVa;
		PGLVector hVb = b.v - pVb;
		PGLVector tVa = ((a.m - b.m) * pVa + 2.0 * b.m * pVb) / (a.m + b.m);
		PGLVector tVb = ((b.m - a.m) * pVb + 2.0 * a.m * pVa) / (a.m + b.m);
		atom[x].v = tVa + hVa;
		atom[y].v = tVb + hVb;
		
		energy = 0;
		energy += atom[x].v.length() * atom[x].v.length();
		energy += atom[y].v.length() * atom[y].v.length();
		//printf("After Ene = %.10f\n", energy);
		
		atom[y].p = atom[y].p - collision.time * atom[y].v;
		
	}
	atom[x].p = atom[x].p - collision.time * atom[x].v;
	
	//printf("HeapSize = %d\n", size_of_heap);
	add_in_heap(x, collision.time);
	add_in_heap(y, collision.time);
	//printf("HeapSize = %d\n", size_of_heap);
}
/****************************************************************************/

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
int collision_cnt = 0;
int array[100];
bool flag = 1;
int TTT = 2000;
void reDisplay(void) {
	if (isRunning) {
		long time = clock() - initTime;
		while (time > nextCollision.time) {
			if(nextCollision.second >= nAtom) array[time / TTT]++;
			collision_cnt++;
			updateAtom();
			updateNextCollision();
		}
		for (int i = 0; i < nAtom; i++) {
			show[i] = atom[i];
			show[i].p = show[i].p + time * atom[i].v;
		}
		glutPostRedisplay();
		if (flag && time > 20000) {
			flag = 0;
			std::cout<<"OK"<<std::endl;
			freopen("data.out","w",stdout);
			for(int i = 0; i < 20000 / TTT; i++){
				std::cout<<i<<' '<<array[i]<<std::endl;
			}
		}
		/*double energy = 0;
		for (int i = 0 ; i < nAtom; i++) {
			energy += show[i].v.length() * show[i].v.length();
		}
		printf("%.10f\n", energy);*/
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
		PGLColor now = getColor(atom[i].v.length() / maxSpeed);
		
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(now.value[0], now.value[1], now.value[2]);
		glVertex2f(show[i].p.x, show[i].p.y);
		for (int angle = 0; angle <= 360; angle++) {
			PGLPoint dir = PGLPoint(cos(angle / 180.0 * PI), sin(angle / 180.0 * PI));
			PGLPoint pos = show[i].p + show[i].r * dir;
			glVertex2f(pos.x, pos.y);
		}
		glEnd();
	}
	
#ifndef DEBUG
	glutSwapBuffers();
#endif
}

void run(int argc, char **argv, const char *title, const PGLColor &background, int (*__initAtom)(PGLAtom *atom))
{
	isRunning = true;
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
	build_heap();
	updateNextCollision();

#ifndef DEBUG
	glutDisplayFunc(display);
	glutReshapeFunc(reShape);
	glutIdleFunc(reDisplay);
	
	glutMainLoop();
#endif
}

};

#endif
