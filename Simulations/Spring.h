#pragma once
#include "Simulator.h"

class Spring
{
public:
	Spring();
	Spring(int point1, int point2, float stiffness, float initialLength);
	~Spring();

public:
	int point1;
	int point2;
	float stiffness;
	float initialLength;
	float currentLength;
	Vec3 force;

	float calcDirectedForce(float currentLength, float pos1, float pos2);
	void computeElasticForces(Vec3 point1Pos, Vec3 point2Pos);
	void addToEndPoints(Vec3 *points, Vec3 force);

	void setPoint1(int point);
	void setPoint2(int point);
	void setStiffness(float stiff);
	void setInitalLength(float initlength);

	int getPoint1();
	int getPoint2();
	float getStiffness();
	float getInitialLength();

};

