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


