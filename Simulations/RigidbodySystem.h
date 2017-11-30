#pragma once
#include "Simulator.h"
#include "util\matrixbase.h"

#include <DirectXMath.h>

typedef struct {
	//should be a Vec3;
	Vec3 size;
	//should be int
	float mass;
	float interiatensor;
} Boxshape;

class RigidbodySystem
{
public:
	RigidbodySystem();
	~RigidbodySystem();

	Vec3 position;
	// Translation Matrix
	Mat4 transMat;

	Vec3 velocity;
	Vec3 force;
	// For test purpose only! One axis, hence orientation is a scalar i.e. radian [0...2pi]! 
	// ToDo use quaterions!
	float orientation;
	//RotationMatrix;
	Mat4 rotMat;

	float anglevelocity;
	float torque;
	Boxshape shape;

	//Scale Matrix
	Mat4 scaleMat;
};

