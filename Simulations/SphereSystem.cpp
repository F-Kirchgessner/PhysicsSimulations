#include "SphereSystem.h"



SphereSystem::SphereSystem()
{
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
			float distance;
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
}


