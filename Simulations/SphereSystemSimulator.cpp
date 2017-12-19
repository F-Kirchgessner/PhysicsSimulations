#include "SphereSystemSimulator.h"

std::function<float(float)> SphereSystemSimulator::m_Kernels[5] = {
	[](float x) {return 1.0f; },              // Constant, m_iKernel = 0
	[](float x) {return 1.0f - x; },          // Linear, m_iKernel = 1, as given in the exercise Sheet, x = d/2r
	[](float x) {return (1.0f - x)*(1.0f - x); }, // Quadratic, m_iKernel = 2
	[](float x) {return 1.0f / (x)-1.0f; },     // Weak Electric Charge, m_iKernel = 3
	[](float x) {return 1.0f / (x*x) - 1.0f; },   // Electric Charge, m_iKernel = 4
};

// SphereSystemSimulator member functions

SphereSystemSimulator::SphereSystemSimulator() {
	m_iNumSpheres = 2;
	m_fRadius = m_fOldRadius = 0.08f;
	m_fMass = m_fOldMass = 0.80f;
	m_fInputScale = 0.01f;
	m_fDamping = 0.70f;
}

SphereSystemSimulator::~SphereSystemSimulator()
{
	delete m_pSphereSystem;
}

const char * SphereSystemSimulator::getTestCasesStr() {
	return "Naive,Uniform Grid,Both";
}

void SphereSystemSimulator::initUI(DrawingUtilitiesClass * DUC) {
	this->DUC = DUC;
	TwAddVarRW(DUC->g_pTweakBar, "NumberOfSpheres", TW_TYPE_INT32, &m_iNumSpheres, "step=1 min=0");
	TwAddVarRW(DUC->g_pTweakBar, "Radius", TW_TYPE_FLOAT, &m_fRadius, "step=0.01 min=0.01");
	TwAddVarRW(DUC->g_pTweakBar, "Mass", TW_TYPE_FLOAT, &m_fMass, "step=0.1 min=0.1");
	TwAddVarRW(DUC->g_pTweakBar, "Damping", TW_TYPE_FLOAT, &m_fDamping, "step=0.01 min=0.00");
	TwAddVarRW(DUC->g_pTweakBar, "InputScale", TW_TYPE_FLOAT, &m_fInputScale, "step=0.001 min=0");
}

void SphereSystemSimulator::initTestScene()
{
	switch (m_iTestCase)
	{
	case 0:
		m_iAccelerator = NAIVEACC;
		m_pSphereSystem = new SphereSystem();
		break;
	case 1:
		m_iAccelerator = GRIDACC;
		m_pSphereSystem = new SphereSystem();
		break;
	case 2:
		m_pSphereSystem = new SphereSystem();
		m_pSphereSystemGrid = new SphereSystem();
		break;
	}
}

void SphereSystemSimulator::reset() {
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;

	initTestScene();
}

void SphereSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext) {
	//check for changes
	if (m_iNumSpheres != m_pSphereSystem->spheres.size())
	{
		if (m_iNumSpheres > m_pSphereSystem->spheres.size())
		{
			std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
			std::uniform_real_distribution<float> randPos(-0.5f + m_fRadius, 0.5f - m_fRadius);

			float randX = randPos(gen);
			float randY = randPos(gen);
			float randZ = randPos(gen);

			if (m_iTestCase == 2) {
				m_pSphereSystem->addSphere(m_fRadius, m_fMass, Vec3(randX, randY, randZ), Vec3(1, 0, 0));
				m_pSphereSystemGrid->addSphere(m_fRadius, m_fMass, Vec3(randX, randY, randZ), Vec3(0, 1, 0));
			}
			else {
				std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
				std::uniform_real_distribution<> dis(0.0, 1.0);
				m_pSphereSystem->addSphere(m_fRadius, m_fMass, Vec3(randX, randY, randZ), Vec3(dis(gen), dis(gen), dis(gen)));
			}
		}
		else
		{
			int delN = m_pSphereSystem->spheres.size()-m_iNumSpheres;
			//not sure
			for (int i = 0; i < delN; i++)
				m_pSphereSystem->spheres.pop_back();
		}
	}

	if (m_fRadius != m_fOldRadius)
	{
		for (auto& sphere : m_pSphereSystem->spheres) {
			sphere.r = m_fRadius;
			m_fOldRadius = m_fRadius;
		}
	}

	if (m_fMass != m_fOldMass)
	{
		for (auto& sphere : m_pSphereSystem->spheres) {
			sphere.m = m_fMass;
			m_fOldMass = m_fMass;
		}

	}

	// Draw spheres
	for (auto& sphere : m_pSphereSystem->spheres) {
		DUC->setUpLighting(Vec3(sphere.red, sphere.green, sphere.blue), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(sphere.red, sphere.green, sphere.blue));
		DUC->drawSphere(sphere.pos, Vec3(sphere.r, sphere.r, sphere.r));
	}
	if (m_iTestCase == 2) {
		for (auto& sphere : m_pSphereSystemGrid->spheres) {
			DUC->setUpLighting(Vec3(sphere.red, sphere.green, sphere.blue), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(sphere.red, sphere.green, sphere.blue));
			DUC->drawSphere(sphere.pos, Vec3(sphere.r, sphere.r, sphere.r));
		}
	}
}

void SphereSystemSimulator::notifyCaseChanged(int testCase) {
	m_iTestCase = testCase;
	switch (m_iTestCase)
	{
	default:
		reset();
		break;
	}
}

void SphereSystemSimulator::externalForcesCalculations(float timeElapsed) {
	// Apply the mouse deltas to g_vfMovableObjectPos (move along cameras view plane)
	Point2D mouseDiff;
	mouseDiff.x = m_trackmouse.x - m_oldtrackmouse.x;
	mouseDiff.y = m_trackmouse.y - m_oldtrackmouse.y;
	if (mouseDiff.x != 0 || mouseDiff.y != 0)
	{
		Mat4 worldViewInv = Mat4(DUC->g_camera.GetWorldMatrix() * DUC->g_camera.GetViewMatrix());
		worldViewInv = worldViewInv.inverse();
		Vec3 inputView = Vec3((float)mouseDiff.x, (float)-mouseDiff.y, 0);
		Vec3 inputWorld = worldViewInv.transformVectorNormal(inputView);
		inputWorld = inputWorld * m_fInputScale;

		// Apply to mass points
		for (auto& sphere : m_pSphereSystem->spheres) {
			sphere.addPenaltyForce(inputWorld);
		}
		if (m_iTestCase == 2) {
			for (auto& sphere : m_pSphereSystemGrid->spheres) {
				sphere.addPenaltyForce(inputWorld);
			}
		}
	}
}

void SphereSystemSimulator::simulateTimestep(float timeStep) {
	switch (m_iTestCase)
	{
	case 2:
		m_pSphereSystem->updateStep(timeStep, m_fDamping, 0);
		m_pSphereSystemGrid->updateStep(timeStep, m_fDamping, 1);
		break;
	default:
		m_pSphereSystem->updateStep(timeStep, m_fDamping, m_iAccelerator);
		break;
	}
}

void SphereSystemSimulator::onClick(int x, int y) {
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void SphereSystemSimulator::onMouse(int x, int y) {
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}
