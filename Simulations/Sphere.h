#pragma once
#include "Simulator.h"

class Sphere
{
public:
	Sphere(float radius, float mass, Vec3 position);
	~Sphere();
	//radius
	float r;
	//mass
	float m;
	//position: center World Coord.
	Vec3 pos;

	float red;
	float green;
	float blue;

	void clearForce();
	void addGravity();
	void addDamping();
	void applyForce(Vec3& force);

private:
	//force
	Vec3 f;
	//velocity
	Vec3 v;

};

