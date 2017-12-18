#include "Sphere.h"

Sphere::Sphere(float radius=0.05, float mass=1, Vec3 position=Vec3(0,0,0)) : r(radius), m(mass), pos(position)
{
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

void Sphere::addGravity()
{
}

void Sphere::addDamping()
{
}

void Sphere::applyForce(Vec3& force)
{
	f += force;
	addDamping();
	addGravity();
}