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
	void addGravity(float gForce);
	void addDamping(float gamma);
	void addPenaltyForce(Vec3& force);

	//force
	Vec3 f;
	//velocity
	Vec3 v;

};

