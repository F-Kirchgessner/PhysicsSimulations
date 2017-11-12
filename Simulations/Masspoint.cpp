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

void Masspoint::integratePositionsEuler(float elapsedTime) {
	if (!isFixed) {
		position.x += velocity.x * elapsedTime;
		position.y += velocity.y * elapsedTime;
		position.z += velocity.z * elapsedTime;

		if (position.y < GROUND_HEIGHT) {
			position.y = GROUND_HEIGHT;
			velocity.y = -velocity.y * GROUND_DAMPING;
		}

		if (position.x < -BOX_SIZE) {
			position.x = -BOX_SIZE;
			velocity.x = -velocity.x * GROUND_DAMPING;
		}
		else if(position.x > BOX_SIZE) {
			position.x = BOX_SIZE;
			velocity.x = -velocity.x * GROUND_DAMPING;
		}

		if (position.z < -BOX_SIZE) {
			position.z = -BOX_SIZE;
			velocity.z = -velocity.z * GROUND_DAMPING;
		}
		else if (position.z > BOX_SIZE) {
			position.z = BOX_SIZE;
			velocity.z = -velocity.z * GROUND_DAMPING;
		}
	}
}

void Masspoint::integrateVelocityLeapfrog(float elapsedTime) {

}

void Masspoint::integrateMidpoint(float elapsedTime) {
	if (!isFixed) {

		position.x += (elapsedTime) * velocity.x;
		position.y += (elapsedTime) * velocity.y;
		position.z += (elapsedTime) * velocity.z;

		if (position.y < GROUND_HEIGHT) {
			position.y = GROUND_HEIGHT;
			velocity.y = -velocity.y * GROUND_DAMPING;
		}

		if (position.x < -BOX_SIZE) {
			position.x = -BOX_SIZE;
			velocity.x = -velocity.x * GROUND_DAMPING;
		}
		else if (position.x > BOX_SIZE) {
			position.x = BOX_SIZE;
			velocity.x = -velocity.x * GROUND_DAMPING;
		}

		if (position.z < -BOX_SIZE) {
			position.z = -BOX_SIZE;
			velocity.z = -velocity.z * GROUND_DAMPING;
		}
		else if (position.z > BOX_SIZE) {
			position.z = BOX_SIZE;
			velocity.z = -velocity.z * GROUND_DAMPING;
		}
	}

	velocity.x += (elapsedTime)*(force.x - damping * velocity.x ) / mass;
	velocity.y += (elapsedTime)*(force.y - damping * velocity.y ) / mass;
	velocity.z += (elapsedTime)*(force.z - damping * velocity.z ) / mass;
}

void Masspoint::integrateVelocityEuler(float elapsedTime) {
	velocity.x += (force.x - damping * velocity.x * elapsedTime) / mass;
	velocity.y += (force.y - damping * velocity.y * elapsedTime) / mass;
	velocity.z += (force.z - damping * velocity.z * elapsedTime) / mass;

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