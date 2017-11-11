#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator() {
	m_iTestCase = 0;
}


const char * MassSpringSystemSimulator::getTestCasesStr() {
	return "Demo_1,Demo_2,Demo_3,Demo_4,Demo_5";
}


void MassSpringSystemSimulator::initTestScene() {
	addMassPoint(Vec3(0.25f, 0.0f, 0), Vec3(-1.0, 0.0f, 0), false);
	addMassPoint(Vec3(0.0, 0.5f, 0), Vec3(1.0, 0.0f, 0), true);
	addSpring(0, 1, 0.25f);
}


void MassSpringSystemSimulator::reset() {
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;

	m_fMass = 1;
	m_fStiffness = 1;
	m_fDamping = 1;
	m_iIntegrator = 0;

	m_masspointList = {};
	m_springList = {};

	initTestScene();
}


void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC) {

	TwType TW_TYPE_INTEGRATORTYPE = TwDefineEnumFromString("IntegrationType", "Euler,Leapfrog,Midpoint");
	
	this->DUC = DUC;
	switch (m_iTestCase)
	{
	case 0: TwAddVarRW(DUC->g_pTweakBar, "IntegrationType", TW_TYPE_INTEGRATORTYPE, &m_iIntegrator, ""); break;
	default:break;
	}
}


void MassSpringSystemSimulator::notifyCaseChanged(int testCase) {
	m_iTestCase = testCase;
	switch (m_iTestCase)
	{
	case 0:
		//ToDo: Clear point and spring list and add them again.
		cout << "Mass System!\n";
		break;
	default:
		cout << "Empty Test!\n";
		break;
	}
}


void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed) {
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
		// find a proper scale!
		float inputScale = 0.001f;
		inputWorld = inputWorld * inputScale;
	}
	else {
	}
}


void MassSpringSystemSimulator::simulateTimestep(float timeStep) {
	// update current setup for each frame


	integrate(timeStep);
}


void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext) {
	switch (m_iTestCase)
	{
	case 0: 
		// Draw mass points
		for (auto& masspoint : m_masspointList) {
			DUC->drawSphere(masspoint.position, Vec3(MASS_POINT_SIZE, MASS_POINT_SIZE, MASS_POINT_SIZE));
		}

		// Draw springs
		DUC->beginLine();
		for (auto& spring : m_springList) {
			DUC->drawLine(spring.mass_point1->position, Vec3(0, 1, 0), spring.mass_point2->position, Vec3(0, 1, 0));
		}
		DUC->endLine();
		break;
	}
}


void MassSpringSystemSimulator::onClick(int x, int y) {
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}


void MassSpringSystemSimulator::onMouse(int x, int y) {
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}


void MassSpringSystemSimulator::setMass(float mass) {
	m_fMass = mass;
}


void MassSpringSystemSimulator::setStiffness(float stiffness) {
	m_fStiffness = stiffness;
}


void MassSpringSystemSimulator::setDampingFactor(float damping) {
	m_fDamping = damping;
}


int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 velocity, bool isFixed) {
	m_masspointList.push_back(Masspoint(position, velocity, isFixed, Vec3(0, 0, 0), m_fMass, m_fDamping));
	return m_masspointList.size() - 1;
}


void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength) {
	Spring s(m_masspointList.at(masspoint1), m_masspointList.at(masspoint2), m_fStiffness, initialLength);
	m_springList.push_back(s);
	int a = 0;
}


int MassSpringSystemSimulator::getNumberOfMassPoints() {
	return m_masspointList.size();
}


int MassSpringSystemSimulator::getNumberOfSprings() {
	return m_springList.size();
}


Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index) {
	return m_masspointList.at(index).getPosition();
}


Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index) {
	return m_masspointList.at(index).getVelocity();
}


void MassSpringSystemSimulator::applyExternalForce(Vec3 force) {
}

void MassSpringSystemSimulator::integrate(float elapsedTime) {
	
		switch (m_iIntegrator) {
			//euler
		case 0:
			for (auto& masspoint : m_masspointList) {
				masspoint.clearForce();
				masspoint.addGravity();
			}

			for (auto& spring : m_springList) {
				spring.computeElasticForces();
				spring.addToEndPoints();
			}

			for (auto &massspoint : m_masspointList) {
				massspoint.integrateVelocityEuler();
				massspoint.integratePositionsEuler(elapsedTime);
			}
			break;
			//leapfrog
		case 1:
			//massspoint.integrateVelocityMidpoint();
			//massspoint.integratePositionsMidpoint(elapsedTime); break;
			//midpoint
		case 2:
			//massspoint.integrateVelocityLeapfrog();
			//massspoint.integratePositionsLeapfrog(elapsedTime); break;
		}
}