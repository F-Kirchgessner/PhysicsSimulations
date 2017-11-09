#include "Spring.h"

Spring::Spring()
{

}

Spring::Spring(int point1, int point2, float stiffness, float initialLength) : point1(point1), point2(point2), stiffness(stiffness), initialLength(initialLength)
{

}


Spring::~Spring()
{
}


void Spring::computeElasticForces() {
}


void Spring::addToEndPoints(Vec3 *points, Vec3 force) {
	for (int i = 0; i < sizeof(points); i++) {
		points[i].x = points[i].x + force.x;
		points[i].y = points[i].y + force.y;
		points[i].z = points[i].z + force.z;
	}
}


void Spring::setPoint1(int point) {
	point1 = point;
}
void Spring::setPoint2(int point) {
	point2 = point;
}
void Spring::setStiffness(float stiff) {
	stiffness = stiff;
}
void Spring::setInitalLength(float initlength) {
	initialLength = initlength;
}

int Spring::getPoint1() {
	return point1;
}

int Spring::getPoint2() {
	return point2;
}

float Spring::getStiffness() {
	return stiffness;
}

float Spring::getInitialLength() {
	return initialLength;
}