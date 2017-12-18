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
				float distance = FLT_MAX;
				float dx = sphere.pos.x - sphere2.pos.x;
				float dy = sphere.pos.y - sphere2.pos.y;
				float dz = sphere.pos.z - sphere2.pos.z;
				distance = sqrt(dx*dx + dy*dy + dz*dz);
				if (distance <= (sphere.r + sphere2.r)) {
					printf("collision detected");
					resolveCollision(sphere, sphere2);
				}
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
	Sphere *spheres[numCells * numCells * numCells][10];
	float cellSize = boxSize / numCells;

	for (int x = 0; x < numCells; x++) {
		for (int y = 0; y < numCells; y++) {
			for (int z = 0; z < numCells; z++) {
				
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
