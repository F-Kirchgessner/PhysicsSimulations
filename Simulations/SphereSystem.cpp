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
			if (&sphere == &sphere2) {
				//sphere = sphere2
			}
			else {
				checkForCollision(sphere, sphere2);
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
	
	Sphere *grid[numCells * numCells * numCells][maxSpheres];
	std::vector<int> validCells;
	float cellSize = boxSize / numCells;

	for (auto& sphere : spheres) {
		int x = int((sphere.pos.x + (boxSize / 2)) / numCells);
		int y = int((sphere.pos.y + (boxSize / 2)) / numCells);
		int z = int((sphere.pos.z + (boxSize / 2)) / numCells);
		int gridPos = x + y * numCells + z * numCells * numCells;
		int i = 0;
		for (; i < maxSpheres && grid[gridPos][i] == NULL; i++) {}

		grid[gridPos][i] = &sphere;
		validCells.push_back(gridPos);
	}

	for (int cell : validCells) {
		checkCell(grid, cell);
	}
}


void SphereSystem::checkCell(Sphere *grid[][10], int cell) {
	for (int i = 0; i < maxSpheres && grid[cell][i] != NULL; i++) {
		for (int j = i + 1; j < maxSpheres && grid[cell][j] != NULL; i++) {

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
	Vec3 n1 = a.v * (a.m - b.m) + 2 * b.m*b.v;
	float z1 = a.m + b.m;
	a.v = n1 / z1;
	Vec3 n2 = b.v * (b.m - a.m) + 2 * a.m*a.v;
	float z2 = b.m + a.m;
	b.v = n2 / z2;

	float lam = 0.3;
	Vec3 penForce = lam*(1 - (b.pos.value - a.pos.value) / (2 * a.r));
	a.addPenaltyForce(-penForce);
	b.addPenaltyForce(-penForce);
}

void SphereSystem::checkForCollision(Sphere &a, Sphere &b) {
	float distance = FLT_MAX;
	float dx = a.pos.x - b.pos.x;
	float dy = a.pos.y - b.pos.y;
	float dz = a.pos.z - b.pos.z;
	distance = dx*dx + dy * dy + dz * dz;
	if (distance <= (a.r + b.r) * (a.r + b.r)) {
		printf("collision detected");
		resolveCollision(a, b);
	}
}