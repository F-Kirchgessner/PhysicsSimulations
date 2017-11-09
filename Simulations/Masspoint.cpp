#include "Masspoint.h"

Masspoint::Masspoint()
{

}

Masspoint::Masspoint(Vec3 position, Vec3 velocity, Vec3 force, float mass, float damping) : position(position), velocity(velocity), force(force), mass(mass), damping(damping)
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
float Masspoint::getMass() {
	return mass;
}
float Masspoint::getDamping() {
	return damping;
}