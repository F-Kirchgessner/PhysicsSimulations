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

void SphereSystem::updateStep(float elapsedTime, float damping, int accelerator)
{
	if (spheres.size() == 0)
		return;

	float h = elapsedTime;

	//add Gravity
	//add damping force
	for (auto& sphere : spheres) {
		sphere.addGravity(0.10f);
		sphere.addDamping(damping);
	}

	//check for collision and add penalty-force
	if (accelerator == 0) {
		naiveCollision();
	}
	else {
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
}


void SphereSystem::uniformGridCollision() {
	
	Sphere *grid[numCells * numCells * numCells][maxSpheres] = {{ NULL }};
	float cellSize = boxSize / numCells;

	for (auto& sphere : spheres) {
		int x = int((sphere.pos.x + (boxSize / 2)) / (boxSize / numCells));
		int y = int((sphere.pos.y + (boxSize / 2)) / (boxSize / numCells));
		int z = int((sphere.pos.z + (boxSize / 2)) / (boxSize / numCells));
		int gridPos = x + y * numCells + z * numCells * numCells;
		int i = 0;
		for (; grid[gridPos] != NULL && i < maxSpheres - 1 && grid[gridPos][i] != NULL; i++) {}

		grid[gridPos][i] = &sphere;
	}

	for (int x = 0; x < numCells; x++) {
		for (int y = 0; y < numCells; y++) {
			for (int z = 0; z < numCells; z++) {
				int cell = x + y * numCells + z * numCells * numCells;
				checkCells(grid[cell], grid[cell]);
				if (y < numCells - 1) checkCells(grid[cell], grid[cell + numCells]);
				if (x < numCells - 1) checkCells(grid[cell], grid[cell + 1]);
				if (x < numCells - 1 && y < numCells - 1) checkCells(grid[cell], grid[cell + 1 + numCells]);
				if (z < numCells - 1) checkCells(grid[cell], grid[cell + numCells * numCells]);
				if (x < numCells - 1 && z < numCells - 1) checkCells(grid[cell], grid[cell + 1 + numCells * numCells]);
				if (y < numCells - 1 && z < numCells - 1) checkCells(grid[cell], grid[cell + numCells + numCells * numCells]);
				if (x < numCells - 1 && y < numCells - 1 && z < numCells - 1) checkCells(grid[cell], grid[cell + 1 + numCells + numCells * numCells]);
			}
		}
	}
}


void SphereSystem::checkCells(Sphere **cell1, Sphere **cell2) {
	if (cell1 == NULL || cell2 == NULL)
		return;
	for (int i = 0; i < maxSpheres && cell1[i] != NULL; i++) {
		for (int j = i + 1; j < maxSpheres && cell2[j] != NULL; j++) {
			checkForCollision(*cell1[i], *cell2[j]);
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