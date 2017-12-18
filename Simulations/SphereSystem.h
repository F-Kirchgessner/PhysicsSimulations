#pragma once
#include "Sphere.h"

class SphereSystem
{
public:
	SphereSystem();
	~SphereSystem();

	void addSphere(float radius, float mass, Vec3 position);
	void updateStep(float elapsedTime, float damping);
	void checkBox();
	void uniformGridCollision();
	void checkCell(Sphere *grid[][10], int cell);
	void resolveCollision(Sphere &a, Sphere &b);

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::vector<Sphere> spheres;
	float boxSize;

	static const int numCells = 5;
	static const int maxSpheres = 10;
};

