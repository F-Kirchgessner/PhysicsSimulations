#pragma once
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

	void computeElasticForces();
	void addToEndPoints();

	void setPoint1(int point);
	void setPoint2(int point);
	void setStiffness(float stiff);
	void setInitalLength(float initlength);

	int getPoint1();
	int getPoint2();
	float getStiffness();
	float getInitialLength();

};

