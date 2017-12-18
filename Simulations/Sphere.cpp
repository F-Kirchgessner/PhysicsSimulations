#include "Sphere.h"

Sphere::Sphere(float radius=0.05, float mass=1, Vec3 position=Vec3(0,0,0)) : r(radius), m(mass), pos(position)
{
	f = (0, 0, 0);
	v = (0, 0, 0);
}

Sphere::~Sphere()
{
}

void Sphere::clearForce()
{
	f.x = 0;
	f.y = 0;
	f.z = 0;
}

void Sphere::addGravity(float gForce)
{
	f.y -= m * gForce;
}

void Sphere::addDamping(float gamma)
{
	f.x -= gamma * v.x;
	f.y -= gamma * v.y;
	f.z -= gamma * v.z;
}

void Sphere::addPenaltyForce(Vec3& force)
{
	f += force;
}
