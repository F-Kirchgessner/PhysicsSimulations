#include "Masspoint.h"

Masspoint::Masspoint()
{

}

Masspoint::Masspoint(Vec3 position, Vec3 velocity, bool isFixed, Vec3 force, float mass, float damping) : position(position), velocity(velocity), isFixed(isFixed), force(force), mass(mass), damping(damping)
{

}


Masspoint::~Masspoint()
{
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