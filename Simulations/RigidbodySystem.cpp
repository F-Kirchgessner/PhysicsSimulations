#include "RigidbodySystem.h"



RigidbodySystem::RigidbodySystem()
{

}

RigidbodySystem::RigidbodySystem(Vec3 size, Vec3 position, int mass) : size(size), m_position(position), mass(mass)
{
	// 45 deg. 
	// need to be removed
	// ------------------------
	orientation = sqrt(2) / 2;
	// ------------------------
	rotMat.initRotationZ(orientation);
	transMat.initTranslation(position.x, position.y, position.z);
	scaleMat.initScaling(size.x, size.y, size.z);
	angluarvelocity = 0;
	calculateInteriaTensor();
}

RigidbodySystem::~RigidbodySystem()
{
}

void RigidbodySystem::applyForce(Vec3& loc, Vec3& f)
{
	//for 3D vector
	// F += f;
	// xi = loc - center
	// torque += xi x force
	
	force += f;
	//loc is probably in world space as m_position
	// armvector = x - loc
	Vec3 armVector = m_position - loc;
	//Watch out should be += not =
	torque += GamePhysics::cross(armVector,f);

}

void RigidbodySystem::updateStep(float elapsedTime)
{
	float h = elapsedTime;

}
