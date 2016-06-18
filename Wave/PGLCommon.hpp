#ifndef SJTU_PGLCOMMON_HPP
#define SJTU_PGLCOMMON_HPP

#include <cmath>
#include <algorithm>

namespace sjtu{

/*****************************************************************/
struct PGLPoint{
	double x, y;
	
	PGLPoint() : x(0), y(0) {}
	PGLPoint(const double &x, const double &y) : x(x), y(y) {}
	
	double length()const;
	double sqrLength()const;
	
};

typedef PGLPoint PGLVector;

double PGLPoint::length()const {
	return sqrt(x * x + y * y);
}
double PGLPoint::sqrLength()const {
	return x * x + y * y;
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
double dot(const PGLPoint &a, const PGLPoint &b) {
	return a.x * b.x + a.y * b.y;
}
double det(const PGLPoint &a, const PGLPoint &b) {
	return a.x * b.y - b.x * a.y;
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
	{0.0, 0.0, 1.0},
	{0.5, 0.5, 0.5},
	{1.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
};
struct PGLColor{
	double value[3];
	
	PGLColor(const Color &rhs = BLACK);
	
};

PGLColor::PGLColor(const Color &rhs) {
	value[0] = CMAP[rhs][0];
	value[1] = CMAP[rhs][1];
	value[2] = CMAP[rhs][2];
}

PGLColor getColor(const double &rate) {
	PGLColor ret;
	ret.value[0] = std::min(1.0, rate);
	ret.value[1] = std::max(0.0, 0.3 - rate);
	ret.value[2] = std::max(0.0, 0.8 - rate * rate);
	return ret;
}
/*****************************************************************/

}
#endif
