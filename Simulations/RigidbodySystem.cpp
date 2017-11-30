#include "RigidbodySystem.h"



RigidbodySystem::RigidbodySystem()
{
	rotMat.initRotationZ(orientation);
	transMat.initTranslation(position.x, position.y, position.z);
	scaleMat.initScaling(shape.size.x, shape.size.y, shape.size.z);
}


RigidbodySystem::~RigidbodySystem()
{
}
