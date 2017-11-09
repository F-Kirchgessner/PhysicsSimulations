#include "Masspoint.h"

Masspoint::Masspoint() {
}

Masspoint::Masspoint(Vec3 position, Vec3 velocity, bool isFixed, Vec3 force, float mass, float damping) : position(position), velocity(velocity), isFixed(isFixed), force(force), mass(mass), damping(damping) {
}


Masspoint::~Masspoint() {
}

void Masspoint::clearForce() {
	force.x = 0;
	force.y = 0;
	force.z = 0;
}

void Masspoint::addGravity() {
	force.y = mass * GRAVITY_ACCEL;
}

void Masspoint::integratePositionsLeapfrog(float elapsedTime) {

}

void Masspoint::integratePositionsMidpoint(float elapsedTime) {

}

void Masspoint::integratePositionsEuler(float elapsedTime) {
	position.x += velocity.x * elapsedTime;
	position.y += velocity.y * elapsedTime;
	position.z += velocity.z * elapsedTime;
}

void Masspoint::integrateVelocityLeapfrog() {

}

void Masspoint::integrateVelocityMidpoint() {

}

void Masspoint::integrateVelocityEuler() {
	velocity.x += force.x / mass;
	velocity.y += force.y / mass;
	velocity.z += force.z / mass;
}

void Masspoint::setPosition(Vec3 position) {
	position = position;
}
void Masspoint::setVelocity(Vec3 velocity) {
	velocity = velocity;
}
void Masspoint::setForce(Vec3 force) {
	force = force;
}
void Masspoint::setIsFixed(bool isFixed) {
	isFixed = isFixed;
}
void Masspoint::setMass(float mass) {
	mass = mass;
}
void Masspoint::setDamping(float damping) {
	damping = damping;
}

Vec3 Masspoint::getPosition() {
	return position;
}
Vec3 Masspoint::getVelocity() {
	return velocity;
}
Vec3 Masspoint::getForce() {
	return force;
}
bool  Masspoint::getIsFixed() {
	return isFixed;
}
float Masspoint::getMass() {
	return mass;
}
float Masspoint::getDamping() {
	return damping;
}