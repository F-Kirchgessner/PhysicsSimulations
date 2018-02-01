#define IDX(x, y, z, w) (x + w * (y + w * z))

#pragma once
#include "Sphere.h"

#include <vector>
#include <map>

class SphereSystem
{
public:
	SphereSystem();
	~SphereSystem();

	void addSphere(float radius, float mass, Vec3 position, Vec3 color);
	void updateStep(float elapsedTime, float damping, int accelerator);
	void checkBox();
	void naiveCollision();
	void uniformGridCollision();
	void checkCells(Sphere **cell1, Sphere **cell2);
	void resolveCollision(Sphere &a, Sphere &b);
	void checkForCollision(Sphere &a, Sphere &b);

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::vector<Sphere> spheres;
	float boxSize;

	bool acc = false;

	std::map<int, std::vector<std::reference_wrapper<Sphere>>> grid_map;

	static const int numCells = 5;
	static const int maxSpheres = 10;

};

