#ifndef SPHSYSTEMSIMULATOR_h
#define SPHSYSTEMSIMULATOR_h
#include "Simulator.h"
#include "SphereSystem.h"

#include <random>

#define NAIVEACC 0
#define GRIDACC 1

class SphereSystemSimulator:public Simulator{
public:
	// Construtors
	SphereSystemSimulator();
	~SphereSystemSimulator();
	// Functions
	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void initTestScene();
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void notifyCaseChanged(int testCase);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void onClick(int x, int y);
	void onMouse(int x, int y);
	
protected:
	// Attributes
	Vec3 externalForce;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
	float m_fMass;
	float m_fRadius;
	float m_fForceScaling;
	float m_fDamping;
	float m_fInputScale;
	int   m_iNumSpheres;

	float m_fOldRadius;
	float m_fOldMass;
	
	int   m_iKernel; // index of the m_Kernels[5], more detials in SphereSystemSimulator.cpp
	static std::function<float(float)> m_Kernels[5];
	
	int   m_iAccelerator; // switch between NAIVEACC and GRIDACC, (optionally, KDTREEACC, 2)
	
	std::random_device rd;  //Will be used to obtain a seed for the random number engine

	SphereSystem * m_pSphereSystem; // add your own sphere system member!
	// for Demo 3 only:
	// you will need multiple SphereSystem objects to do comparisons in Demo 3
	// m_iAccelerator should be ignored.
	SphereSystem * m_pSphereSystemGrid;

};

#endif