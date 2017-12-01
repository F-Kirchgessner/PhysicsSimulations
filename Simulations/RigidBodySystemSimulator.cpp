#include "RigidBodySystemSimulator.h"

// Construtors
RigidBodySystemSimulator::RigidBodySystemSimulator() {
	m_iTestCase = 0;
}

// Functions
const char * RigidBodySystemSimulator::getTestCasesStr() {
	return "Simple One Step Test, Simple Single Body Simulation, Two-Rigid-Body Collision Scene, Complex Simulation";
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass * DUC) {
	this->DUC = DUC;
}
void RigidBodySystemSimulator::initTestScene()
{
	switch (m_iTestCase)
	{
	case 0:
		addRigidBody(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.25f, 0.25f, 0.25f), 2.0f);
		applyForceOnBody(getNumberOfRigidBodies() - 1, Vec3(0.25f, 0.20f,0), Vec3(100, 50, 50));
		break;
	}
}

void RigidBodySystemSimulator::reset() {
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;

	m_rigidbodysystems.clear();

	initTestScene();

}
void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext) {
	DUC->setUpLighting(Vec3(0, 0, 0), 0.4*Vec3(1, 1, 1), 2.0, Vec3(0.5, 0.5, 0.5));
	// Draw mass points
	for (auto& rigidbodySystem : m_rigidbodysystems) {
		rigidbodySystem.Obj2WorldMatrix = rigidbodySystem.scaleMat * rigidbodySystem.rotMat * rigidbodySystem.transMat;
		DUC->drawRigidBody(rigidbodySystem.Obj2WorldMatrix);
	}

}
void RigidBodySystemSimulator::notifyCaseChanged(int testCase) {

}
void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed) {

}
void RigidBodySystemSimulator::simulateTimestep(float timeStep) {

	// update current setup for each frame
	switch (m_iTestCase)
	{
	case 0: 
		for (auto& rigidbodySystem : m_rigidbodysystems) {
			rigidbodySystem.updateStep(timeStep);
		}
		break;
	}

}
void RigidBodySystemSimulator::onClick(int x, int y) {

}
void RigidBodySystemSimulator::onMouse(int x, int y) {

}

// ExtraFunctions
int RigidBodySystemSimulator::getNumberOfRigidBodies() {
	return m_rigidbodysystems.size();
}
Vec3 RigidBodySystemSimulator::getPositionOfRigidBody(int i) {
	return m_rigidbodysystems.at(i).m_position;
}
Vec3 RigidBodySystemSimulator::getLinearVelocityOfRigidBody(int i) {
	return m_rigidbodysystems.at(i).velocity;
}
Vec3 RigidBodySystemSimulator::getAngularVelocityOfRigidBody(int i) {
	return m_rigidbodysystems.at(i).angluarvelocity;
}
void RigidBodySystemSimulator::applyForceOnBody(int i, Vec3 loc, Vec3 force) {
	m_rigidbodysystems.at(i).applyForce(loc, force);
}
void RigidBodySystemSimulator::addRigidBody(Vec3 position, Vec3 size, float mass) {
	RigidbodySystem rig(size,position,mass);
	//create; copy; delete; because of inner function, maybe emplace_back?
	m_rigidbodysystems.push_back(rig);
}
void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation) {
	// ToDo use Quaterion for orientation;
	
}
void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity) {
	m_rigidbodysystems.at(i).velocity = velocity;
}