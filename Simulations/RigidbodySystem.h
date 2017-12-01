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

	// x_cm
	Vec3 m_position;
	// Translation Matrix
	Mat4 transMat;

	Vec3 size;
	//Scale Matrix
	Mat4 scaleMat;

	// For test purpose only! One axis, hence orientation is a scalar i.e. radian [0...2pi]! 
	// ToDo use quaterions!
	// r
	float orientation;
	//RotationMatrix;
	Mat4 rotMat;
	
	Mat4 Obj2WorldMatrix;

	//M
	int mass;
	//i
	float interiatensor;
	//v_cm
	Vec3 velocity;
	//F
	Vec3 force;
	//w
	float angluarvelocity;
	//q
	Vec3 torque;

	void calculateInteriaTensor() {
	// i = sum(x_n*<x_n,x_n>)
	};
	// add Force and Torque, External Forces in the Simulation Algo.
	void applyForce(Vec3& loc, Vec3& f);
	void updateStep(float elapsedTime);
	void clearForce();
};

