#include "Spring.h"

Spring::Spring(){
}

Spring::Spring(Masspoint& point1, Masspoint& point2, float stiffness, float initialLength){
	Vec3 force = Vec3(0, 0, 0);
	point1 = point1;
	point2 = point2;
	stiffness = stiffness;
	initialLength = initialLength;
}


Spring::~Spring(){
}


float Spring::calcDirectedForce(float currentLength, float pos1, float pos2) {
	return -stiffness * (currentLength - initialLength) * ((pos1 - pos2) / currentLength);
}


void Spring::computeElasticForces() {
	float currentLength = sqrt(point1->position.squaredDistanceTo(point2->position));

	force.x = calcDirectedForce(currentLength, point1->position.x, point2->position.x);
	force.y = calcDirectedForce(currentLength, point1->position.y, point2->position.y);
	force.z = calcDirectedForce(currentLength, point1->position.z, point2->position.z);
}


void Spring::addToEndPoints() {
	point1->force = point1->force + this->force;
	point2->force = point2->force + this->force;
}


void Spring::setPoint1(Masspoint *point) {
	point1 = point;
}
void Spring::setPoint2(Masspoint *point) {
	point2 = point;
}
void Spring::setStiffness(float stiff) {
	stiffness = stiff;
}
void Spring::setInitalLength(float initlength) {
	initialLength = initlength;
}

Masspoint *Spring::getPoint1() {
	return point1;
}

Masspoint *Spring::getPoint2() {
	return point2;
}

float Spring::getStiffness() {
	return stiffness;
}

float Spring::getInitialLength() {
	return initialLength;
}