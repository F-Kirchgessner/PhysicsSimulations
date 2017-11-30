#pragma once
#include "Simulator.h"
#include "util\matrixbase.h"

#include <DirectXMath.h>

class RigidbodySystem
{
public:
	RigidbodySystem();
	RigidbodySystem(Vec3 size, Vec3 position, int mass);
	~RigidbodySystem();

	Vec3 position;
	// Translation Matrix
	Mat4 transMat;

	Vec3 size;
	//Scale Matrix
	Mat4 scaleMat;

	// For test purpose only! One axis, hence orientation is a scalar i.e. radian [0...2pi]! 
	// ToDo use quaterions!
	float orientation;
	//RotationMatrix;
	Mat4 rotMat;
	
	Mat4 Obj2WorldMatrix;


	float mass;
	float interiatensor;
	Vec3 velocity;
	Vec3 force;
	float anglevelocity;
	float torque;

};

