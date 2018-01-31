#include "SphereSystem.h"

#include <set>
#include <tuple>
#include <utility>

SphereSystem::SphereSystem()
{
	boxSize = 0.5f * 2;
}


SphereSystem::~SphereSystem()
{
}

void SphereSystem::addSphere(float radius, float mass, Vec3 position, Vec3 color)
{
	Sphere s(radius, mass, position);
	s.red = color.x;
	s.green = color.y;
	s.blue = color.z;
	spheres.push_back(s);
}

void SphereSystem::updateStep(float elapsedTime, float damping, int accelerator)
{
	if (spheres.size() == 0)
		return;

	float h = elapsedTime;

	//add Gravity
	//add damping force
	for (auto& sphere : spheres) {
		sphere.addGravity(0.50f);
		sphere.addDamping(damping);
	}

	//check for collision and add penalty-force
	if (accelerator == 0) {
		acc = 0;
		naiveCollision();
	}
	else {
		acc = 1;
		uniformGridCollision();
	}

	//LeapFrog
	for (auto& sphere : spheres) {
		if (sphere.init) {
			sphere.v.x += (sphere.f.x / sphere.m) * h / 2.0f;
			sphere.v.y += (sphere.f.y / sphere.m) * h / 2.0f;
			sphere.v.z += (sphere.f.z / sphere.m) * h / 2.0f;
			
		}
		sphere.init = false;
	}
	
	for (auto& sphere : spheres) {
		sphere.v.x += (sphere.f.x / sphere.m) * h;
		sphere.v.y += (sphere.f.y / sphere.m) * h;
		sphere.v.z += (sphere.f.z / sphere.m) * h;
		
		sphere.pos.x += sphere.v.x * h;
		sphere.pos.y += sphere.v.y * h;
		sphere.pos.z += sphere.v.z * h;
	}

	for (auto& sphere : spheres) {
		sphere.clearForce();
	}

	checkBox();
}


void SphereSystem::naiveCollision() {
	for (int i = 0; i < spheres.size(); i++) {
		for (int j = i + 1; j < spheres.size(); j++) {
			checkForCollision(spheres[i], spheres[j]);
		}
	}
}


void SphereSystem::uniformGridCollision() {
	Sphere *grid[numCells * numCells * numCells][maxSpheres] = { { NULL } };
	std::set<std::tuple<int, int, int>> occCells;
	
	float cellSize = boxSize / numCells;

	for (auto& sphere : spheres) {

		int x = int((sphere.pos.x + boxSize / 2.0f) * (numCells ) );
		int y = int((sphere.pos.y + boxSize / 2.0f) * (numCells ) );
		int z = int((sphere.pos.z + boxSize / 2.0f) * (numCells ) );
		int gridPos = x + numCells * (y + z * numCells);
		
		int i = 0;
		for (; grid[gridPos] != NULL && i < maxSpheres - 1 && grid[gridPos][i] != NULL; i++) {}
		grid[gridPos][i] = &sphere;

		occCells.insert(std::tuple<int, int, int>(x,y,z));	
	}

	for(auto & occCell : occCells){

		for (int i = -1; i <= 0; i++)
		{
			for (int j = -1; j <= 0; j++)
			{
				for (int k = -1; k <=0;  k++)
				{
					if (!((std::get<1>(occCell) + j) <= -1 || (std::get<1>(occCell) + j) >= numCells || (std::get<0>(occCell) + i) <= -1 || (std::get<0>(occCell) + i) >= numCells || (std::get<2>(occCell) + k) <= -1 || (std::get<2>(occCell) + k) >=numCells))
						checkCells(grid[IDX(std::get<0>(occCell), std::get<1>(occCell), std::get<2>(occCell), numCells)], grid[IDX(std::get<0>(occCell) + i, std::get<1>(occCell) + j, std::get<2>(occCell) + k, numCells)]);
					
				}
			}
		}
	
		/*
		int j = -1;

		for (int i = -1; i <= 1; i++)
		{
			for (int k = -1; k <= 1; k++)
			{
				if (!((std::get<1>(occCell) + j) <= -1 || (std::get<1>(occCell) + j) >= numCells || (std::get<0>(occCell) + i) <= -1 || (std::get<0>(occCell) + i) >= numCells || (std::get<2>(occCell) + k) <= -1 || (std::get<2>(occCell) + k) >= numCells))
					checkCells(grid[IDX(std::get<0>(occCell), std::get<1>(occCell), std::get<2>(occCell), numCells)], grid[IDX(std::get<0>(occCell) + i, std::get<1>(occCell) + j, std::get<2>(occCell) + k, numCells)]);

			}
		}

		j = 0; int k = -1;

		for (int i = -1; i <= 1; i++)
		{
			if (!((std::get<1>(occCell) + j) <= -1 || (std::get<1>(occCell) + j) >= numCells || (std::get<0>(occCell) + i) <= -1 || (std::get<0>(occCell) + i) >= numCells || (std::get<2>(occCell) + k) <= -1 || (std::get<2>(occCell) + k) >= numCells))
				checkCells(grid[IDX(std::get<0>(occCell), std::get<1>(occCell), std::get<2>(occCell), numCells)], grid[IDX(std::get<0>(occCell) + i, std::get<1>(occCell) + j, std::get<2>(occCell) + k, numCells)]);
		}

		int i = -1; j = 0; k = 0;

		if (!((std::get<1>(occCell) + j) <= -1 || (std::get<1>(occCell) + j) >= numCells || (std::get<0>(occCell) + i) <= -1 || (std::get<0>(occCell) + i) >= numCells || (std::get<2>(occCell) + k) <= -1 || (std::get<2>(occCell) + k) >= numCells))
			checkCells(grid[IDX(std::get<0>(occCell), std::get<1>(occCell), std::get<2>(occCell), numCells)], grid[IDX(std::get<0>(occCell) + i, std::get<1>(occCell) + j, std::get<2>(occCell) + k, numCells)]);

		i = 0; j = 0; k = 0;
		checkCells(grid[IDX(std::get<0>(occCell), std::get<1>(occCell), std::get<2>(occCell), numCells)], grid[IDX(std::get<0>(occCell) + i, std::get<1>(occCell) + j, std::get<2>(occCell) + k, numCells)]);
		*/
	}

}

void SphereSystem::checkCells(Sphere **cell1, Sphere **cell2) {
	if (*cell1 == NULL || *cell2 == NULL)
		return;
	if (*cell1 == *cell2)
	{
		for (int i = 0; i < maxSpheres && cell1[i] != NULL; i++) {
			for (int j = i + 1; j < maxSpheres && cell2[j] != NULL; j++) {
				checkForCollision(*cell1[i], *cell2[j]);
			}
		}
	}
	else {

		for (int i = 0; i < maxSpheres && cell1[i] != NULL; i++) {
			for (int j = 0; j < maxSpheres && cell2[j] != NULL; j++) {
				checkForCollision(*cell1[i], *cell2[j]);
			}
		}
	}
}

void SphereSystem::checkBox() {
	float boxSizeHalf = boxSize / 2;
	for (auto& sphere : spheres) {
		float radius = sphere.r;

		if (sphere.pos.x - radius < -boxSizeHalf) {
			sphere.pos.x = -boxSizeHalf + radius;
			sphere.v.x = -sphere.v.x;
		}
		if (sphere.pos.y - radius < -0.5) {
			sphere.pos.y = -boxSizeHalf + radius;
			sphere.v.y = -sphere.v.y;
		}
		if (sphere.pos.z - radius < -boxSizeHalf) {
			sphere.pos.z = -boxSizeHalf + radius;
			sphere.v.z = -sphere.v.z;
		}

		if (sphere.pos.x + radius > boxSizeHalf) {
			sphere.pos.x = boxSizeHalf - radius;
			sphere.v.x = -sphere.v.x;
		}
		if (sphere.pos.y + radius > boxSizeHalf) {
			sphere.pos.y = boxSizeHalf - radius;
			sphere.v.y = -sphere.v.y;
		}
		if (sphere.pos.z + radius > boxSizeHalf) {
			sphere.pos.z = boxSizeHalf - radius;
			sphere.v.z = -sphere.v.z;
		}
	}
}

void SphereSystem::resolveCollision(Sphere &a, Sphere &b) {
	float lam = 10;
	Vec3 penForce;
	penForce.x = lam*(1 - (b.pos.x - a.pos.x) / (2 * a.r));
	penForce.y = lam*(1 - (b.pos.y - a.pos.y) / (2 * a.r));
	penForce.z = lam*(1 - (b.pos.z - a.pos.z) / (2 * a.r));
	//std::cout << penForce.x << ' ' << penForce.y << ' ' << penForce.z << '\n';

	a.addPenaltyForce(penForce);
	b.addPenaltyForce(-penForce);
}

void SphereSystem::checkForCollision(Sphere& a, Sphere& b) {
	float distance = FLT_MAX;
	float dx = a.pos.x - b.pos.x;
	float dy = a.pos.y - b.pos.y;
	float dz = a.pos.z - b.pos.z;
	distance = dx*dx + dy * dy + dz * dz;

	//std::cout << a.pos.z << " " << b.pos.z << std::endl;

	if (distance <= 4*a.r*a.r){
		resolveCollision(a, b);
	}
}