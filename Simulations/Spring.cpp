#include "Spring.h"

Spring::Spring(){

}

Spring::Spring(int point1, int point2, float stiffness, float initialLength) : point1(point1), point2(point2), stiffness(stiffness), initialLength(initialLength){
	Vec3 force = Vec3(0, 0, 0);
}


Spring::~Spring(){
}


float Spring::calcDirectedForce(float currentLength, float pos1, float pos2) {
	return -stiffness * (currentLength - initialLength) * ((pos1 - pos2) / currentLength);
}


void Spring::computeElasticForces(Vec3 point1Pos, Vec3 point2Pos) {
	float currentLength = sqrt(point1Pos.squaredDistanceTo(point2Pos));

	force.x = calcDirectedForce(currentLength, point1Pos.x, point2Pos.x);
	force.y = calcDirectedForce(currentLength, point1Pos.y, point2Pos.y);
	force.z = calcDirectedForce(currentLength, point1Pos.z, point2Pos.z);
}


void Spring::addToEndPoints() {
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