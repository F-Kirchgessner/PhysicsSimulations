#include "SphereSystem.h"



SphereSystem::SphereSystem()
{
	boxSize = 0.5f * 2;
}


SphereSystem::~SphereSystem()
{
}

void SphereSystem::addSphere(float radius, float mass, Vec3 position)
{
	Sphere s(radius,mass,position);
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(0.0, 1.0);
	s.red = dis(gen);
	s.green = dis(gen);
	s.blue = dis(gen);
	spheres.push_back(s);

}

void SphereSystem::updateStep(float elapsedTime, float damping)
{
	float h = elapsedTime;

	//add Gravity
	//add damping force
	for (auto& sphere : spheres) {
		sphere.addGravity(0.10f);
		sphere.addDamping(damping);
	}

	//check for collision and add penalty-force
	for (auto& sphere : spheres) {
		for (auto& sphere2 : spheres) {
			float distance = FLT_MAX;
			float dx = sphere.pos.x - sphere2.pos.x;
			float dy = sphere.pos.y - sphere2.pos.y;
			float dz = sphere.pos.z - sphere2.pos.z;
			if (dx == 0 && dy == 0 && dz == 0) {
				//sphere = sphere2
			}
			else {
				distance = sqrt(dx*dx + dy*dy + dz*dz);
			}
			if (distance <= (sphere.r + sphere2.r)) {
				printf("collision detected");
				resolveCollision(sphere, sphere2);
			}
		}
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


void SphereSystem::uniformGridCollision() {
	Sphere *grid[numCells * numCells * numCells][10];
	std::vector<int> validCells;
	float cellSize = boxSize / numCells;

	for (auto& sphere : spheres) {
		int x = int((sphere.pos.x + (boxSize / 2)) / numCells);
		int y = int((sphere.pos.y + (boxSize / 2)) / numCells);
		int z = int((sphere.pos.z + (boxSize / 2)) / numCells);
		int gridPos = x + y * numCells + z * numCells * numCells;
		int i = 0;
		for (; grid[gridPos][i] == NULL && i < 10; i++) {}

		grid[gridPos][i] = &sphere;
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
			sphere.pos.y = -0.5 + radius;
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
		if (sphere.pos.z + radius > boxSizeHalf) {
			sphere.pos.z = boxSizeHalf - radius;
			sphere.v.z = -sphere.v.z;
		}
	}
}

void SphereSystem::resolveCollision(Sphere &a, Sphere &b) {
	a.v = (a.v * (a.m - b.m) + 2 * b.m*b.v) / (a.m+b.m);
	b.v = (b.v * (b.m - a.m) + 2 * a.m*a.v) / (b.m + a.m);

	float lam = 1;
	Vec3 penForce = lam*(1 - (a.pos.value - b.pos.value) / (2 * a.r));
	a.addPenaltyForce(penForce);
	b.addPenaltyForce(penForce);
}
