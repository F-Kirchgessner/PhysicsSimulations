#include "RigidbodySystem.h"



RigidbodySystem::RigidbodySystem()
{

}

RigidbodySystem::RigidbodySystem(Vec3 size, Vec3 position, int mass) : size(size), position(position), orientation(orientation)
{
	// 45 deg. 
	// need to be removed
	// ------------------------
	orientation = sqrt(2) / 2;
	// ------------------------
	rotMat.initRotationZ(orientation);
	transMat.initTranslation(position.x, position.y, position.z);
	scaleMat.initScaling(size.x, size.y, size.z);
}

RigidbodySystem::~RigidbodySystem()
{
}
