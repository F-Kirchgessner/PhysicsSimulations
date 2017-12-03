#include "RigidBodySystemSimulator.h"


// Construtors
RigidBodySystemSimulator::RigidBodySystemSimulator() {
	m_iTestCase = 1;

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
		applyForceOnBody(getNumberOfRigidBodies() - 1, Vec3(0, 0, 0.125f), Vec3(10, 0, 0));
		break;
	case 1:
		addRigidBody(Vec3(-0.6f, 0.0f, 0.0f), Vec3(0.25f, 0.25f, 0.25f), 2.0f);
		addRigidBody(Vec3(0.3f, 0.0f, 0.0f), Vec3(0.25f, 0.25f, 0.25f), 2.0f);
		addRigidBody(Vec3(-0.3f, 0.0f, 0.0f), Vec3(0.25f, 0.25f, 0.25f), 2.0f);
		applyForceOnBody(getNumberOfRigidBodies() - 1, Vec3(-0.25f, 0.0f, 0), Vec3(10, 0, 0));
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

	// Draw mass points
	for (auto& rigidbodySystem : m_rigidbodysystems) {
		DUC->setUpLighting(Vec3(rigidbodySystem.red, rigidbodySystem.green, rigidbodySystem.blue), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(rigidbodySystem.red, rigidbodySystem.green, rigidbodySystem.blue));
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
	case 1:
		checkForCollisions();
		for (auto& rigidbodySystem : m_rigidbodysystems) {
			rigidbodySystem.updateStep(timeStep=0.01);
		}
		break;
	}
}

void RigidBodySystemSimulator::checkForCollisions() {
	for (int a = 0; a < m_rigidbodysystems.size(); a++) {
		for (int b = a+1; b < m_rigidbodysystems.size(); b++) {

				RigidbodySystem &bodyA = m_rigidbodysystems[a];
				RigidbodySystem &bodyB = m_rigidbodysystems[b];
				Mat4 worldA = bodyA.scaleMat * bodyA.rotMat * bodyA.transMat;
				Mat4 worldB = bodyB.scaleMat * bodyB.rotMat * bodyB.transMat;
				CollisionInfo simpletest = checkCollisionSAT(worldA, worldB);
				if (simpletest.isValid) {
					std::printf("collision detected at normal: %f, %f, %f\n", simpletest.normalWorld.x, simpletest.normalWorld.y, simpletest.normalWorld.z);
					std::printf("collision point : %f, %f, %f\n", (simpletest.collisionPointWorld).x, (simpletest.collisionPointWorld).y, (simpletest.collisionPointWorld).z);
					collisionDetected(bodyA, bodyB, simpletest.collisionPointWorld, simpletest.normalWorld);

				}
			

		}
	}
}

void RigidBodySystemSimulator::collisionDetected(RigidbodySystem &bodyA, RigidbodySystem &bodyB, Vec3 collisionPointWorld, Vec3 normalWorld) {
	Mat4 worldInvA = (bodyA.scaleMat * bodyA.rotMat * bodyA.transMat).inverse();
	Mat4 worldInvB = (bodyA.scaleMat * bodyB.rotMat * bodyB.transMat).inverse();
	Vec3 collisionPointA = worldInvA.transformVector(collisionPointWorld);
	Vec3 collisionPointB = worldInvB.transformVector(collisionPointWorld);
	//------------------------------------------------------------------------------------------------
	// NUMERATOR
	Vec3 velA = bodyA.velocity + cross(bodyA.angluarvelocity, collisionPointA);
	Vec3 velB = bodyB.velocity + cross(bodyB.angluarvelocity, collisionPointB);

	Vec3 vrel = velA - velB;
	float c = 1.0;
	float numerator = -(1 + c)*dot(vrel, normalWorld);
	//-----------------------------------------------------------------------------------------------

	float massInvA = 1.0 / bodyA.mass;
	float massInvB = 1.0 / bodyB.mass;
	Mat4 interiaTensorInvA = bodyA.interiatensor.inverse();
	Mat4 interiaTensorInvB = bodyB.interiatensor.inverse();

	//DENOMINATOR
	Vec3 xaCrossN = GamePhysics::cross(collisionPointA, normalWorld);
	Vec3 iaInverseTimesXaCrossN = interiaTensorInvA.transformVector(xaCrossN);
	Vec3 leftPlus = GamePhysics::cross(iaInverseTimesXaCrossN, collisionPointA);

	Vec3 xbCrossN = GamePhysics::cross(collisionPointB, normalWorld);
	Vec3 ibInverseTimesXbCrossN = interiaTensorInvB.transformVector(xbCrossN);
	Vec3 rightPlus = GamePhysics::cross(ibInverseTimesXbCrossN, collisionPointB);

	float denominator = massInvA+massInvB+ GamePhysics::dot((leftPlus + rightPlus),normalWorld);

	// Update velocity and angular momentum
	Vec3 result = (numerator / denominator) * normalWorld;
	bodyA.velocity += result * massInvA;
	bodyB.velocity -= result * massInvB;
	bodyA.angularMomentum += GamePhysics::cross(collisionPointA, result);
	bodyB.angularMomentum -= GamePhysics::cross(collisionPointB, result);

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
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(0.0, 1.0);
	rig.red = dis(gen);
	rig.green = dis(gen);
	rig.blue = dis(gen);
	//create; copy; delete; because of inner function, maybe emplace_back?
	m_rigidbodysystems.push_back(rig);
}
void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation) {
	// ToDo use Quaterion for orientation;
	
}
void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity) {
	m_rigidbodysystems.at(i).velocity = velocity;
}