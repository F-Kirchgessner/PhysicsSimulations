#include "RigidBodySystemSimulator.h"

// Construtors
RigidBodySystemSimulator::RigidBodySystemSimulator() {

}

// Functions
const char * RigidBodySystemSimulator::getTestCasesStr() {
	return "Simple One Step Test, Simple Single Body Simulation, Two-Rigid-Body Collision Scene, Complex Simulation";
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass * DUC) {
	//test
}
void RigidBodySystemSimulator::reset() {

}
void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext) {

}
void RigidBodySystemSimulator::notifyCaseChanged(int testCase) {

}
void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed) {

}
void RigidBodySystemSimulator::simulateTimestep(float timeStep) {

}
void RigidBodySystemSimulator::onClick(int x, int y) {

}
void RigidBodySystemSimulator::onMouse(int x, int y) {

}

// ExtraFunctions
int RigidBodySystemSimulator::getNumberOfRigidBodies() {
	return 42;
}
Vec3 RigidBodySystemSimulator::getPositionOfRigidBody(int i) {
	Vec3 res;
	return res;
}
Vec3 RigidBodySystemSimulator::getLinearVelocityOfRigidBody(int i) {
	Vec3 res;
	return res;
}
Vec3 RigidBodySystemSimulator::getAngularVelocityOfRigidBody(int i) {
	Vec3 res;
	return res;
}
void RigidBodySystemSimulator::applyForceOnBody(int i, Vec3 loc, Vec3 force) {

}
void RigidBodySystemSimulator::addRigidBody(Vec3 position, Vec3 size, int mass) {

}
void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation) {

}
void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity) {

}