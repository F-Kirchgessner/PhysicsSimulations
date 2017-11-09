#pragma once
#include "Simulator.h"

class Masspoint
{
public:
	Masspoint();
	Masspoint(Vec3 position, Vec3 velocity, Vec3 force, float mass, float damping);
	~Masspoint();

public:
	Vec3 position;
	Vec3 velocity;
	Vec3 force;
	float mass;
	float damping;

	void setPosition(Vec3 position);
	void setVelocity(Vec3 velocity);
	void setForce(Vec3 force);
	void setMass(float mass);
	void setDamping(float damping);

	Vec3 getPosition();
	Vec3 getVelocity();
	Vec3 getForce();
	float getMass();
	float getDamping();


};

