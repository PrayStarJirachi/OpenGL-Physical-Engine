#ifndef SJTU_PGLCOMMON_HPP
#define SJTU_PGLCOMMON_HPP

#include <cmath>

namespace sjtu{

/*****************************************************************/
struct PGLPoint{
	double x, y;
	
	PGLPoint() : x(0), y(0) {}
	PGLPoint(const double &x, const double &y) : x(x), y(y) {}
	
	double length()const;
	
};

typedef PGLPoint PGLVector;

double PGLPoint::length()const {
	return sqrt(x * x + y * y);
}

PGLPoint operator +(const PGLPoint &a, const PGLPoint &b) {
	return PGLPoint(a.x + b.x, a.y + b.y);
}
PGLPoint operator -(const PGLPoint &a, const PGLPoint &b) {
	return PGLPoint(a.x - b.x, a.y - b.y);
}
PGLPoint operator *(const PGLPoint &a, const double &t) {
	return PGLPoint(a.x * t, a.y * t);
}
PGLPoint operator *(const double &t, const PGLPoint &a) {
	return PGLPoint(a.x * t, a.y * t);
}
PGLPoint operator /(const PGLPoint &a, const double &t) {
	return PGLPoint(a.x / t, a.y / t);
}
PGLPoint operator /(const double &t, const PGLPoint &a) {
	return PGLPoint(a.x / t, a.y / t);
}
/*****************************************************************/

/*****************************************************************/
enum Color{
	BLACK, WHITE, RED, GREEN, BLUE, GRAY, PURPLE, YELLOW
};
const double CMAP[][3] = {
	{0.0, 0.0, 0.0},
	{1.0, 1.0, 1.0},
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0},
	{0.5, 0.5, 0.5},
	{0.5, 0.0, 0.5},
};
struct PGLColor{
	double value[3];
	
	PGLColor(const Color &rhs = BLACK);
	
};

PGLColor::PGLColor(const Color &rhs) {
	value[1] = CMAP[rhs][1];
	value[2] = CMAP[rhs][2];
	value[0] = CMAP[rhs][0];
}

PGLColor getColor(const double &rate) {
	if (rate < 0.2) return BLUE;
	else if (rate < 0.5) return GREEN;
	else if (rate < 0.7) return YELLOW;
	else return RED;
}
/*****************************************************************/

}
#endif
