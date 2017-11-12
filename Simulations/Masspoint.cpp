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
	force.y -= mass * GRAVITY_ACCEL;
}

void Masspoint::integratePositionsLeapfrog(float elapsedTime) {

}

void Masspoint::integratePositionsMidpoint(float elapsedTime) {

}

void Masspoint::integratePositionsEuler(float elapsedTime) {
	if (!isFixed) {
		position.x += velocity.x * elapsedTime;
		position.y += velocity.y * elapsedTime;
		position.z += velocity.z * elapsedTime;
	}
}

void Masspoint::integrateVelocityLeapfrog() {

}

void Masspoint::integrateVelocityMidpoint() {

}

void Masspoint::integrateVelocityEuler() {
	velocity.x += (force.x - damping * velocity.x) / mass;
	velocity.y += (force.y - damping * velocity.y) / mass;
	velocity.z += (force.z - damping * velocity.z) / mass;

	if (velocity.x * velocity.x < VELOCITY_MIN_SQ)
		velocity.x = 0;
	if (velocity.y * velocity.y < VELOCITY_MIN_SQ)
		velocity.y = 0;
	if (velocity.z * velocity.z < VELOCITY_MIN_SQ)
		velocity.z = 0;

	//std::cout << velocity.x << " | " << velocity.y << " | " << velocity.z << endl;
}

void Masspoint::applyForce(Vec3 force) {
	this->force += force;
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