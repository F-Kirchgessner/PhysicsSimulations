#pragma once
#include "Sphere.h"

class SphereSystem
{
public:
	SphereSystem();
	~SphereSystem();

	void addSphere(float radius, float mass, Vec3 position);
	void updateStep(float elapsedTime);

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::vector<Sphere> spheres;

	bool init = true;
};

