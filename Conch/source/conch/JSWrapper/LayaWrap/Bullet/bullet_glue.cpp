
#include "bullet_glue.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletDynamics/ConstraintSolver/btConeTwistConstraint.h"
#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"
#include "BulletDynamics/Vehicle/btVehicleRaycaster.h"
#include "LinearMath/btDefaultMotionState.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletDynamics/ConstraintSolver/btContactSolverInfo.h"
#include "LinearMath/btQuaternion.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/BroadphaseCollision/btDispatcher.h"
#include "BulletCollision/CollisionDispatch/btCollisionConfiguration.h"
#include "BulletDynamics/Dynamics/btActionInterface.h"
#include "BulletCollision/CollisionShapes/btConeShape.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "BulletCollision/NarrowPhaseCollision/btManifoldPoint.h"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletDynamics/ConstraintSolver/btFixedConstraint.h"
#include "BulletCollision/CollisionShapes/btStridingMeshInterface.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h"
#include "BulletCollision/NarrowPhaseCollision/btPersistentManifold.h"
#include "LinearMath/btMotionState.h"
#include "BulletDynamics/Vehicle/btWheelInfo.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletDynamics/ConstraintSolver/btConstraintSolver.h"
#include "LinearMath/btTransform.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "LinearMath/btMatrix3x3.h"
#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "BulletDynamics/ConstraintSolver/btSliderConstraint.h"
#include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "LinearMath/btVector3.h"
#include "BulletCollision/CollisionShapes/btConvexShape.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletSoftBody/btSoftBody.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btDefaultSoftBodySolver.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
namespace laya {
ADDJSCLSINFO(JSLayaMotionStateHandler, JSObjNode);

	//TODO
	class ConcreteContactResultCallback : public btCollisionWorld::ContactResultCallback {
	public:
  		virtual	btScalar addSingleResult(btManifoldPoint& arg0, const btCollisionObjectWrapper* arg1, int arg2, int arg3, const btCollisionObjectWrapper* arg4, int arg5, int arg6) {
			return 0;
    	}
  		void __destroy__() {
    	}
	};
// btCollisionShape

void btCollisionShape_setLocalScaling_1(btCollisionShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btCollisionShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btCollisionShape_getLocalScaling_0(btCollisionShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btCollisionShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btCollisionShape_calculateLocalInertia_2(btCollisionShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btCollisionShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btCollisionShape_setMargin_1(btCollisionShapeAddr self, double arg0) {
	reinterpret_cast<btCollisionShape*>(self)->setMargin(arg0);
}

double btCollisionShape_getMargin_0(btCollisionShapeAddr self) {
	double ret = reinterpret_cast<btCollisionShape*>(self)->getMargin();
	return ret;
}

void btCollisionShape___destroy___0(btCollisionShapeAddr self) {
	delete reinterpret_cast<btCollisionShape*>(self);
}

// btCollisionObject

btCollisionObjectAddr btCollisionObject_btCollisionObject_0() {
	btCollisionObject* newObj = new btCollisionObject();
	return reinterpret_cast<btCollisionObjectAddr>(newObj);
}

void btCollisionObject_setAnisotropicFriction_2(btCollisionObjectAddr self, btVector3Addr arg0, long_t arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btCollisionObject*>(self)->setAnisotropicFriction(*pArg0, arg1);
}

btCollisionShapeAddr btCollisionObject_getCollisionShape_0(btCollisionObjectAddr self) {
	btCollisionShape* ret = reinterpret_cast<btCollisionObject*>(self)->getCollisionShape();
	return reinterpret_cast<btCollisionShapeAddr>(ret);
}

void btCollisionObject_setContactProcessingThreshold_1(btCollisionObjectAddr self, double arg0) {
	reinterpret_cast<btCollisionObject*>(self)->setContactProcessingThreshold(arg0);
}

void btCollisionObject_setActivationState_1(btCollisionObjectAddr self, long_t arg0) {
	reinterpret_cast<btCollisionObject*>(self)->setActivationState(arg0);
}

void btCollisionObject_forceActivationState_1(btCollisionObjectAddr self, long_t arg0) {
	reinterpret_cast<btCollisionObject*>(self)->forceActivationState(arg0);
}

void btCollisionObject_activate_0(btCollisionObjectAddr self) {
	reinterpret_cast<btCollisionObject*>(self)->activate();
}

void btCollisionObject_activate_1(btCollisionObjectAddr self, bool arg0) {
	reinterpret_cast<btCollisionObject*>(self)->activate(arg0);
}

bool btCollisionObject_isActive_0(btCollisionObjectAddr self) {
	bool ret = reinterpret_cast<btCollisionObject*>(self)->isActive();
	return ret;
}

bool btCollisionObject_isKinematicObject_0(btCollisionObjectAddr self) {
	bool ret = reinterpret_cast<btCollisionObject*>(self)->isKinematicObject();
	return ret;
}

bool btCollisionObject_isStaticObject_0(btCollisionObjectAddr self) {
	bool ret = reinterpret_cast<btCollisionObject*>(self)->isStaticObject();
	return ret;
}

bool btCollisionObject_isStaticOrKinematicObject_0(btCollisionObjectAddr self) {
	bool ret = reinterpret_cast<btCollisionObject*>(self)->isStaticOrKinematicObject();
	return ret;
}

double btCollisionObject_getRestitution_0(btCollisionObjectAddr self) {
	double ret = reinterpret_cast<btCollisionObject*>(self)->getRestitution();
	return ret;
}

void btCollisionObject_setRestitution_1(btCollisionObjectAddr self, double arg0) {
	reinterpret_cast<btCollisionObject*>(self)->setRestitution(arg0);
}

double btCollisionObject_getFriction_0(btCollisionObjectAddr self) {
	double ret = reinterpret_cast<btCollisionObject*>(self)->getFriction();
	return ret;
}

void btCollisionObject_setFriction_1(btCollisionObjectAddr self, double arg0) {
	reinterpret_cast<btCollisionObject*>(self)->setFriction(arg0);
}

double btCollisionObject_getRollingFriction_0(btCollisionObjectAddr self) {
	double ret = reinterpret_cast<btCollisionObject*>(self)->getRollingFriction();
	return ret;
}

void btCollisionObject_setRollingFriction_1(btCollisionObjectAddr self, double arg0) {
	reinterpret_cast<btCollisionObject*>(self)->setRollingFriction(arg0);
}

long_t btCollisionObject_getCollisionFlags_0(btCollisionObjectAddr self) {
	long_t ret = reinterpret_cast<btCollisionObject*>(self)->getCollisionFlags();
	return ret;
}

void btCollisionObject_setCollisionFlags_1(btCollisionObjectAddr self, long_t arg0) {
	reinterpret_cast<btCollisionObject*>(self)->setCollisionFlags(arg0);
}

btTransformAddr btCollisionObject_getWorldTransform_0(btCollisionObjectAddr self) {
	btTransform& ret = reinterpret_cast<btCollisionObject*>(self)->getWorldTransform();
	return reinterpret_cast<btTransformAddr>(&ret);
}

void btCollisionObject_setWorldTransform_1(btCollisionObjectAddr self, btTransformAddr arg0) {
	const btTransform* pArg0 = reinterpret_cast<const btTransform*>(arg0);

	reinterpret_cast<btCollisionObject*>(self)->setWorldTransform(*pArg0);
}

void btCollisionObject_setCollisionShape_1(btCollisionObjectAddr self, btCollisionShapeAddr arg0) {
	btCollisionShape* pArg0 = reinterpret_cast<btCollisionShape*>(arg0);

	reinterpret_cast<btCollisionObject*>(self)->setCollisionShape(pArg0);
}

double btCollisionObject_getCcdMotionThreshold_0(btCollisionObjectAddr self) {
	double ret = reinterpret_cast<btCollisionObject*>(self)->getCcdMotionThreshold();
	return ret;
}

void btCollisionObject_setCcdMotionThreshold_1(btCollisionObjectAddr self, double arg0) {
	reinterpret_cast<btCollisionObject*>(self)->setCcdMotionThreshold(arg0);
}

double btCollisionObject_getCcdSweptSphereRadius_0(btCollisionObjectAddr self) {
	double ret = reinterpret_cast<btCollisionObject*>(self)->getCcdSweptSphereRadius();
	return ret;
}

void btCollisionObject_setCcdSweptSphereRadius_1(btCollisionObjectAddr self, double arg0) {
	reinterpret_cast<btCollisionObject*>(self)->setCcdSweptSphereRadius(arg0);
}

long_t btCollisionObject_getUserIndex_0(btCollisionObjectAddr self) {
	long_t ret = reinterpret_cast<btCollisionObject*>(self)->getUserIndex();
	return ret;
}

void btCollisionObject_setUserIndex_1(btCollisionObjectAddr self, long_t arg0) {
	reinterpret_cast<btCollisionObject*>(self)->setUserIndex(arg0);
}

VoidPtrAddr btCollisionObject_getUserPointer_0(btCollisionObjectAddr self) {
	void* ret = reinterpret_cast<btCollisionObject*>(self)->getUserPointer();
	return reinterpret_cast<VoidPtrAddr>(ret);
}

void btCollisionObject_setUserPointer_1(btCollisionObjectAddr self, VoidPtrAddr arg0) {
	void* pArg0 = reinterpret_cast<void*>(arg0);

	reinterpret_cast<btCollisionObject*>(self)->setUserPointer(pArg0);
}

btVector3Addr btCollisionObject_getInterpolationAngularVelocity_0(btCollisionObjectAddr self) {
	const btVector3& ret = reinterpret_cast<btCollisionObject*>(self)->getInterpolationAngularVelocity();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btCollisionObject_setInterpolationAngularVelocity_1(btCollisionObjectAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btCollisionObject*>(self)->setInterpolationAngularVelocity(*pArg0);
}

btVector3Addr btCollisionObject_getInterpolationLinearVelocity_0(btCollisionObjectAddr self) {
	const btVector3& ret = reinterpret_cast<btCollisionObject*>(self)->getInterpolationLinearVelocity();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btCollisionObject_setInterpolationLinearVelocity_1(btCollisionObjectAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btCollisionObject*>(self)->setInterpolationLinearVelocity(*pArg0);
}

btBroadphaseProxyAddr btCollisionObject_getBroadphaseHandle_0(btCollisionObjectAddr self) {
	btBroadphaseProxy* ret = reinterpret_cast<btCollisionObject*>(self)->getBroadphaseHandle();
	return reinterpret_cast<btBroadphaseProxyAddr>(ret);
}

long_t btCollisionObject_getActivationState_0(btCollisionObjectAddr self) {
	long_t ret = reinterpret_cast<btCollisionObject*>(self)->getActivationState();
	return ret;
}

void btCollisionObject___destroy___0(btCollisionObjectAddr self) {
	delete reinterpret_cast<btCollisionObject*>(self);
}

// btTypedConstraint

void btTypedConstraint_enableFeedback_1(btTypedConstraintAddr self, bool arg0) {
	reinterpret_cast<btTypedConstraint*>(self)->enableFeedback(arg0);
}

double btTypedConstraint_getBreakingImpulseThreshold_0(btTypedConstraintAddr self) {
	double ret = reinterpret_cast<btTypedConstraint*>(self)->getBreakingImpulseThreshold();
	return ret;
}

void btTypedConstraint_setBreakingImpulseThreshold_1(btTypedConstraintAddr self, double arg0) {
	reinterpret_cast<btTypedConstraint*>(self)->setBreakingImpulseThreshold(arg0);
}

double btTypedConstraint_getParam_2(btTypedConstraintAddr self, long_t arg0, long_t arg1) {
	double ret = reinterpret_cast<btTypedConstraint*>(self)->getParam(arg0, arg1);
	return ret;
}

void btTypedConstraint_setParam_3(btTypedConstraintAddr self, long_t arg0, double arg1, long_t arg2) {
	reinterpret_cast<btTypedConstraint*>(self)->setParam(arg0, arg1, arg2);
}

void btTypedConstraint___destroy___0(btTypedConstraintAddr self) {
	delete reinterpret_cast<btTypedConstraint*>(self);
}

// btCollisionWorld

btDispatcherAddr btCollisionWorld_getDispatcher_0(btCollisionWorldAddr self) {
	btDispatcher* ret = reinterpret_cast<btCollisionWorld*>(self)->getDispatcher();
	return reinterpret_cast<btDispatcherAddr>(ret);
}

void btCollisionWorld_rayTest_3(btCollisionWorldAddr self, btVector3Addr arg0, btVector3Addr arg1, RayResultCallbackAddr arg2) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);
btCollisionWorld::RayResultCallback* pArg2 = reinterpret_cast<btCollisionWorld::RayResultCallback*>(arg2);

	reinterpret_cast<btCollisionWorld*>(self)->rayTest(*pArg0, *pArg1, *pArg2);
}

btOverlappingPairCacheAddr btCollisionWorld_getPairCache_0(btCollisionWorldAddr self) {
	btOverlappingPairCache* ret = reinterpret_cast<btCollisionWorld*>(self)->getPairCache();
	return reinterpret_cast<btOverlappingPairCacheAddr>(ret);
}

btDispatcherInfoAddr btCollisionWorld_getDispatchInfo_0(btCollisionWorldAddr self) {
	btDispatcherInfo& ret = reinterpret_cast<btCollisionWorld*>(self)->getDispatchInfo();
	return reinterpret_cast<btDispatcherInfoAddr>(&ret);
}

void btCollisionWorld_addCollisionObject_1(btCollisionWorldAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btCollisionWorld*>(self)->addCollisionObject(pArg0);
}

void btCollisionWorld_addCollisionObject_2(btCollisionWorldAddr self, btCollisionObjectAddr arg0, long_t arg1) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btCollisionWorld*>(self)->addCollisionObject(pArg0, arg1);
}

void btCollisionWorld_addCollisionObject_3(btCollisionWorldAddr self, btCollisionObjectAddr arg0, long_t arg1, long_t arg2) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btCollisionWorld*>(self)->addCollisionObject(pArg0, arg1, arg2);
}

void btCollisionWorld_removeCollisionObject_1(btCollisionWorldAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btCollisionWorld*>(self)->removeCollisionObject(pArg0);
}

btBroadphaseInterfaceAddr btCollisionWorld_getBroadphase_0(btCollisionWorldAddr self) {
	const btBroadphaseInterface* ret = reinterpret_cast<btCollisionWorld*>(self)->getBroadphase();
	return reinterpret_cast<btBroadphaseInterfaceAddr>(ret);
}

void btCollisionWorld_convexSweepTest_5(btCollisionWorldAddr self, btConvexShapeAddr arg0, btTransformAddr arg1, btTransformAddr arg2, ConvexResultCallbackAddr arg3, double arg4) {
	const btConvexShape* pArg0 = reinterpret_cast<const btConvexShape*>(arg0);
const btTransform* pArg1 = reinterpret_cast<const btTransform*>(arg1);
const btTransform* pArg2 = reinterpret_cast<const btTransform*>(arg2);
btCollisionWorld::ConvexResultCallback* pArg3 = reinterpret_cast<btCollisionWorld::ConvexResultCallback*>(arg3);

	reinterpret_cast<btCollisionWorld*>(self)->convexSweepTest(pArg0, *pArg1, *pArg2, *pArg3, arg4);
}

void btCollisionWorld_contactPairTest_3(btCollisionWorldAddr self, btCollisionObjectAddr arg0, btCollisionObjectAddr arg1, ContactResultCallbackAddr arg2) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);
btCollisionObject* pArg1 = reinterpret_cast<btCollisionObject*>(arg1);
btCollisionWorld::ContactResultCallback* pArg2 = reinterpret_cast<btCollisionWorld::ContactResultCallback*>(arg2);

	reinterpret_cast<btCollisionWorld*>(self)->contactPairTest(pArg0, pArg1, *pArg2);
}

void btCollisionWorld_contactTest_2(btCollisionWorldAddr self, btCollisionObjectAddr arg0, ContactResultCallbackAddr arg1) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);
btCollisionWorld::ContactResultCallback* pArg1 = reinterpret_cast<btCollisionWorld::ContactResultCallback*>(arg1);

	reinterpret_cast<btCollisionWorld*>(self)->contactTest(pArg0, *pArg1);
}

void btCollisionWorld_updateSingleAabb_1(btCollisionWorldAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btCollisionWorld*>(self)->updateSingleAabb(pArg0);
}

void btCollisionWorld___destroy___0(btCollisionWorldAddr self) {
	delete reinterpret_cast<btCollisionWorld*>(self);
}

// btConcaveShape

void btConcaveShape_setLocalScaling_1(btConcaveShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btConcaveShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btConcaveShape_getLocalScaling_0(btConcaveShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btConcaveShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btConcaveShape_calculateLocalInertia_2(btConcaveShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btConcaveShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btConcaveShape___destroy___0(btConcaveShapeAddr self) {
	delete reinterpret_cast<btConcaveShape*>(self);
}

// btCapsuleShape

btCapsuleShapeAddr btCapsuleShape_btCapsuleShape_2(double arg0, double arg1) {
	btCapsuleShape* newObj = new btCapsuleShape(arg0, arg1);
	return reinterpret_cast<btCapsuleShapeAddr>(newObj);
}

void btCapsuleShape_setMargin_1(btCapsuleShapeAddr self, double arg0) {
	reinterpret_cast<btCapsuleShape*>(self)->setMargin(arg0);
}

double btCapsuleShape_getMargin_0(btCapsuleShapeAddr self) {
	double ret = reinterpret_cast<btCapsuleShape*>(self)->getMargin();
	return ret;
}

long_t btCapsuleShape_getUpAxis_0(btCapsuleShapeAddr self) {
	long_t ret = reinterpret_cast<btCapsuleShape*>(self)->getUpAxis();
	return ret;
}

double btCapsuleShape_getRadius_0(btCapsuleShapeAddr self) {
	double ret = reinterpret_cast<btCapsuleShape*>(self)->getRadius();
	return ret;
}

double btCapsuleShape_getHalfHeight_0(btCapsuleShapeAddr self) {
	double ret = reinterpret_cast<btCapsuleShape*>(self)->getHalfHeight();
	return ret;
}

void btCapsuleShape_setLocalScaling_1(btCapsuleShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btCapsuleShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btCapsuleShape_getLocalScaling_0(btCapsuleShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btCapsuleShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btCapsuleShape_calculateLocalInertia_2(btCapsuleShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btCapsuleShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btCapsuleShape___destroy___0(btCapsuleShapeAddr self) {
	delete reinterpret_cast<btCapsuleShape*>(self);
}

// btGImpactShapeInterface

void btGImpactShapeInterface_updateBound_0(btGImpactShapeInterfaceAddr self) {
	reinterpret_cast<btGImpactShapeInterface*>(self)->updateBound();
}

void btGImpactShapeInterface_setLocalScaling_1(btGImpactShapeInterfaceAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGImpactShapeInterface*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btGImpactShapeInterface_getLocalScaling_0(btGImpactShapeInterfaceAddr self) {
	const btVector3& ret = reinterpret_cast<btGImpactShapeInterface*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btGImpactShapeInterface_calculateLocalInertia_2(btGImpactShapeInterfaceAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btGImpactShapeInterface*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btGImpactShapeInterface___destroy___0(btGImpactShapeInterfaceAddr self) {
	delete reinterpret_cast<btGImpactShapeInterface*>(self);
}

// btDynamicsWorld

void btDynamicsWorld_addAction_1(btDynamicsWorldAddr self, btActionInterfaceAddr arg0) {
	btActionInterface* pArg0 = reinterpret_cast<btActionInterface*>(arg0);

	reinterpret_cast<btDynamicsWorld*>(self)->addAction(pArg0);
}

void btDynamicsWorld_removeAction_1(btDynamicsWorldAddr self, btActionInterfaceAddr arg0) {
	btActionInterface* pArg0 = reinterpret_cast<btActionInterface*>(arg0);

	reinterpret_cast<btDynamicsWorld*>(self)->removeAction(pArg0);
}

btContactSolverInfoAddr btDynamicsWorld_getSolverInfo_0(btDynamicsWorldAddr self) {
	btContactSolverInfo& ret = reinterpret_cast<btDynamicsWorld*>(self)->getSolverInfo();
	return reinterpret_cast<btContactSolverInfoAddr>(&ret);
}

btDispatcherAddr btDynamicsWorld_getDispatcher_0(btDynamicsWorldAddr self) {
	btDispatcher* ret = reinterpret_cast<btDynamicsWorld*>(self)->getDispatcher();
	return reinterpret_cast<btDispatcherAddr>(ret);
}

void btDynamicsWorld_rayTest_3(btDynamicsWorldAddr self, btVector3Addr arg0, btVector3Addr arg1, RayResultCallbackAddr arg2) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);
btCollisionWorld::RayResultCallback* pArg2 = reinterpret_cast<btCollisionWorld::RayResultCallback*>(arg2);

	reinterpret_cast<btDynamicsWorld*>(self)->rayTest(*pArg0, *pArg1, *pArg2);
}

btOverlappingPairCacheAddr btDynamicsWorld_getPairCache_0(btDynamicsWorldAddr self) {
	btOverlappingPairCache* ret = reinterpret_cast<btDynamicsWorld*>(self)->getPairCache();
	return reinterpret_cast<btOverlappingPairCacheAddr>(ret);
}

btDispatcherInfoAddr btDynamicsWorld_getDispatchInfo_0(btDynamicsWorldAddr self) {
	btDispatcherInfo& ret = reinterpret_cast<btDynamicsWorld*>(self)->getDispatchInfo();
	return reinterpret_cast<btDispatcherInfoAddr>(&ret);
}

void btDynamicsWorld_addCollisionObject_1(btDynamicsWorldAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btDynamicsWorld*>(self)->addCollisionObject(pArg0);
}

void btDynamicsWorld_addCollisionObject_2(btDynamicsWorldAddr self, btCollisionObjectAddr arg0, long_t arg1) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btDynamicsWorld*>(self)->addCollisionObject(pArg0, arg1);
}

void btDynamicsWorld_addCollisionObject_3(btDynamicsWorldAddr self, btCollisionObjectAddr arg0, long_t arg1, long_t arg2) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btDynamicsWorld*>(self)->addCollisionObject(pArg0, arg1, arg2);
}

void btDynamicsWorld_removeCollisionObject_1(btDynamicsWorldAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btDynamicsWorld*>(self)->removeCollisionObject(pArg0);
}

btBroadphaseInterfaceAddr btDynamicsWorld_getBroadphase_0(btDynamicsWorldAddr self) {
	const btBroadphaseInterface* ret = reinterpret_cast<btDynamicsWorld*>(self)->getBroadphase();
	return reinterpret_cast<btBroadphaseInterfaceAddr>(ret);
}

void btDynamicsWorld_convexSweepTest_5(btDynamicsWorldAddr self, btConvexShapeAddr arg0, btTransformAddr arg1, btTransformAddr arg2, ConvexResultCallbackAddr arg3, double arg4) {
	const btConvexShape* pArg0 = reinterpret_cast<const btConvexShape*>(arg0);
const btTransform* pArg1 = reinterpret_cast<const btTransform*>(arg1);
const btTransform* pArg2 = reinterpret_cast<const btTransform*>(arg2);
btCollisionWorld::ConvexResultCallback* pArg3 = reinterpret_cast<btCollisionWorld::ConvexResultCallback*>(arg3);

	reinterpret_cast<btDynamicsWorld*>(self)->convexSweepTest(pArg0, *pArg1, *pArg2, *pArg3, arg4);
}

void btDynamicsWorld_contactPairTest_3(btDynamicsWorldAddr self, btCollisionObjectAddr arg0, btCollisionObjectAddr arg1, ContactResultCallbackAddr arg2) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);
btCollisionObject* pArg1 = reinterpret_cast<btCollisionObject*>(arg1);
btCollisionWorld::ContactResultCallback* pArg2 = reinterpret_cast<btCollisionWorld::ContactResultCallback*>(arg2);

	reinterpret_cast<btDynamicsWorld*>(self)->contactPairTest(pArg0, pArg1, *pArg2);
}

void btDynamicsWorld_contactTest_2(btDynamicsWorldAddr self, btCollisionObjectAddr arg0, ContactResultCallbackAddr arg1) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);
btCollisionWorld::ContactResultCallback* pArg1 = reinterpret_cast<btCollisionWorld::ContactResultCallback*>(arg1);

	reinterpret_cast<btDynamicsWorld*>(self)->contactTest(pArg0, *pArg1);
}

void btDynamicsWorld_updateSingleAabb_1(btDynamicsWorldAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btDynamicsWorld*>(self)->updateSingleAabb(pArg0);
}

void btDynamicsWorld___destroy___0(btDynamicsWorldAddr self) {
	delete reinterpret_cast<btDynamicsWorld*>(self);
}

// btTriangleMeshShape

void btTriangleMeshShape_setLocalScaling_1(btTriangleMeshShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btTriangleMeshShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btTriangleMeshShape_getLocalScaling_0(btTriangleMeshShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btTriangleMeshShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btTriangleMeshShape_calculateLocalInertia_2(btTriangleMeshShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btTriangleMeshShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btTriangleMeshShape___destroy___0(btTriangleMeshShapeAddr self) {
	delete reinterpret_cast<btTriangleMeshShape*>(self);
}

// btGhostObject

btGhostObjectAddr btGhostObject_btGhostObject_0() {
	btGhostObject* newObj = new btGhostObject();
	return reinterpret_cast<btGhostObjectAddr>(newObj);
}

long_t btGhostObject_getNumOverlappingObjects_0(btGhostObjectAddr self) {
	long_t ret = reinterpret_cast<btGhostObject*>(self)->getNumOverlappingObjects();
	return ret;
}

btCollisionObjectAddr btGhostObject_getOverlappingObject_1(btGhostObjectAddr self, long_t arg0) {
	btCollisionObject* ret = reinterpret_cast<btGhostObject*>(self)->getOverlappingObject(arg0);
	return reinterpret_cast<btCollisionObjectAddr>(ret);
}

void btGhostObject_setAnisotropicFriction_2(btGhostObjectAddr self, btVector3Addr arg0, long_t arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGhostObject*>(self)->setAnisotropicFriction(*pArg0, arg1);
}

btCollisionShapeAddr btGhostObject_getCollisionShape_0(btGhostObjectAddr self) {
	btCollisionShape* ret = reinterpret_cast<btGhostObject*>(self)->getCollisionShape();
	return reinterpret_cast<btCollisionShapeAddr>(ret);
}

void btGhostObject_setContactProcessingThreshold_1(btGhostObjectAddr self, double arg0) {
	reinterpret_cast<btGhostObject*>(self)->setContactProcessingThreshold(arg0);
}

void btGhostObject_setActivationState_1(btGhostObjectAddr self, long_t arg0) {
	reinterpret_cast<btGhostObject*>(self)->setActivationState(arg0);
}

void btGhostObject_forceActivationState_1(btGhostObjectAddr self, long_t arg0) {
	reinterpret_cast<btGhostObject*>(self)->forceActivationState(arg0);
}

void btGhostObject_activate_0(btGhostObjectAddr self) {
	reinterpret_cast<btGhostObject*>(self)->activate();
}

void btGhostObject_activate_1(btGhostObjectAddr self, bool arg0) {
	reinterpret_cast<btGhostObject*>(self)->activate(arg0);
}

bool btGhostObject_isActive_0(btGhostObjectAddr self) {
	bool ret = reinterpret_cast<btGhostObject*>(self)->isActive();
	return ret;
}

bool btGhostObject_isKinematicObject_0(btGhostObjectAddr self) {
	bool ret = reinterpret_cast<btGhostObject*>(self)->isKinematicObject();
	return ret;
}

bool btGhostObject_isStaticObject_0(btGhostObjectAddr self) {
	bool ret = reinterpret_cast<btGhostObject*>(self)->isStaticObject();
	return ret;
}

bool btGhostObject_isStaticOrKinematicObject_0(btGhostObjectAddr self) {
	bool ret = reinterpret_cast<btGhostObject*>(self)->isStaticOrKinematicObject();
	return ret;
}

double btGhostObject_getRestitution_0(btGhostObjectAddr self) {
	double ret = reinterpret_cast<btGhostObject*>(self)->getRestitution();
	return ret;
}

void btGhostObject_setRestitution_1(btGhostObjectAddr self, double arg0) {
	reinterpret_cast<btGhostObject*>(self)->setRestitution(arg0);
}

double btGhostObject_getFriction_0(btGhostObjectAddr self) {
	double ret = reinterpret_cast<btGhostObject*>(self)->getFriction();
	return ret;
}

void btGhostObject_setFriction_1(btGhostObjectAddr self, double arg0) {
	reinterpret_cast<btGhostObject*>(self)->setFriction(arg0);
}

double btGhostObject_getRollingFriction_0(btGhostObjectAddr self) {
	double ret = reinterpret_cast<btGhostObject*>(self)->getRollingFriction();
	return ret;
}

void btGhostObject_setRollingFriction_1(btGhostObjectAddr self, double arg0) {
	reinterpret_cast<btGhostObject*>(self)->setRollingFriction(arg0);
}

long_t btGhostObject_getCollisionFlags_0(btGhostObjectAddr self) {
	long_t ret = reinterpret_cast<btGhostObject*>(self)->getCollisionFlags();
	return ret;
}

void btGhostObject_setCollisionFlags_1(btGhostObjectAddr self, long_t arg0) {
	reinterpret_cast<btGhostObject*>(self)->setCollisionFlags(arg0);
}

btTransformAddr btGhostObject_getWorldTransform_0(btGhostObjectAddr self) {
	btTransform& ret = reinterpret_cast<btGhostObject*>(self)->getWorldTransform();
	return reinterpret_cast<btTransformAddr>(&ret);
}

void btGhostObject_setWorldTransform_1(btGhostObjectAddr self, btTransformAddr arg0) {
	const btTransform* pArg0 = reinterpret_cast<const btTransform*>(arg0);

	reinterpret_cast<btGhostObject*>(self)->setWorldTransform(*pArg0);
}

void btGhostObject_setCollisionShape_1(btGhostObjectAddr self, btCollisionShapeAddr arg0) {
	btCollisionShape* pArg0 = reinterpret_cast<btCollisionShape*>(arg0);

	reinterpret_cast<btGhostObject*>(self)->setCollisionShape(pArg0);
}

double btGhostObject_getCcdMotionThreshold_0(btGhostObjectAddr self) {
	double ret = reinterpret_cast<btGhostObject*>(self)->getCcdMotionThreshold();
	return ret;
}

void btGhostObject_setCcdMotionThreshold_1(btGhostObjectAddr self, double arg0) {
	reinterpret_cast<btGhostObject*>(self)->setCcdMotionThreshold(arg0);
}

double btGhostObject_getCcdSweptSphereRadius_0(btGhostObjectAddr self) {
	double ret = reinterpret_cast<btGhostObject*>(self)->getCcdSweptSphereRadius();
	return ret;
}

void btGhostObject_setCcdSweptSphereRadius_1(btGhostObjectAddr self, double arg0) {
	reinterpret_cast<btGhostObject*>(self)->setCcdSweptSphereRadius(arg0);
}

long_t btGhostObject_getUserIndex_0(btGhostObjectAddr self) {
	long_t ret = reinterpret_cast<btGhostObject*>(self)->getUserIndex();
	return ret;
}

void btGhostObject_setUserIndex_1(btGhostObjectAddr self, long_t arg0) {
	reinterpret_cast<btGhostObject*>(self)->setUserIndex(arg0);
}

VoidPtrAddr btGhostObject_getUserPointer_0(btGhostObjectAddr self) {
	void* ret = reinterpret_cast<btGhostObject*>(self)->getUserPointer();
	return reinterpret_cast<VoidPtrAddr>(ret);
}

void btGhostObject_setUserPointer_1(btGhostObjectAddr self, VoidPtrAddr arg0) {
	void* pArg0 = reinterpret_cast<void*>(arg0);

	reinterpret_cast<btGhostObject*>(self)->setUserPointer(pArg0);
}

btVector3Addr btGhostObject_getInterpolationAngularVelocity_0(btGhostObjectAddr self) {
	const btVector3& ret = reinterpret_cast<btGhostObject*>(self)->getInterpolationAngularVelocity();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btGhostObject_setInterpolationAngularVelocity_1(btGhostObjectAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGhostObject*>(self)->setInterpolationAngularVelocity(*pArg0);
}

btVector3Addr btGhostObject_getInterpolationLinearVelocity_0(btGhostObjectAddr self) {
	const btVector3& ret = reinterpret_cast<btGhostObject*>(self)->getInterpolationLinearVelocity();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btGhostObject_setInterpolationLinearVelocity_1(btGhostObjectAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGhostObject*>(self)->setInterpolationLinearVelocity(*pArg0);
}

btBroadphaseProxyAddr btGhostObject_getBroadphaseHandle_0(btGhostObjectAddr self) {
	btBroadphaseProxy* ret = reinterpret_cast<btGhostObject*>(self)->getBroadphaseHandle();
	return reinterpret_cast<btBroadphaseProxyAddr>(ret);
}

long_t btGhostObject_getActivationState_0(btGhostObjectAddr self) {
	long_t ret = reinterpret_cast<btGhostObject*>(self)->getActivationState();
	return ret;
}

void btGhostObject___destroy___0(btGhostObjectAddr self) {
	delete reinterpret_cast<btGhostObject*>(self);
}

// btConeShape

btConeShapeAddr btConeShape_btConeShape_2(double arg0, double arg1) {
	btConeShape* newObj = new btConeShape(arg0, arg1);
	return reinterpret_cast<btConeShapeAddr>(newObj);
}

void btConeShape_setLocalScaling_1(btConeShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btConeShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btConeShape_getLocalScaling_0(btConeShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btConeShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btConeShape_calculateLocalInertia_2(btConeShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btConeShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btConeShape___destroy___0(btConeShapeAddr self) {
	delete reinterpret_cast<btConeShape*>(self);
}

// btActionInterface

void btActionInterface_updateAction_2(btActionInterfaceAddr self, btCollisionWorldAddr arg0, double arg1) {
	btCollisionWorld* pArg0 = reinterpret_cast<btCollisionWorld*>(arg0);

	reinterpret_cast<btActionInterface*>(self)->updateAction(pArg0, arg1);
}

void btActionInterface___destroy___0(btActionInterfaceAddr self) {
	delete reinterpret_cast<btActionInterface*>(self);
}

// btVector3

btVector3Addr btVector3_btVector3_0() {
	btVector3* newObj = new btVector3();
	return reinterpret_cast<btVector3Addr>(newObj);
}

btVector3Addr btVector3_btVector3_3(double arg0, double arg1, double arg2) {
	btVector3* newObj = new btVector3(arg0, arg1, arg2);
	return reinterpret_cast<btVector3Addr>(newObj);
}

double btVector3_length_0(btVector3Addr self) {
	double ret = reinterpret_cast<btVector3*>(self)->length();
	return ret;
}

double btVector3_x_0(btVector3Addr self) {
	double ret = reinterpret_cast<btVector3*>(self)->x();
	return ret;
}

double btVector3_y_0(btVector3Addr self) {
	double ret = reinterpret_cast<btVector3*>(self)->y();
	return ret;
}

double btVector3_z_0(btVector3Addr self) {
	double ret = reinterpret_cast<btVector3*>(self)->z();
	return ret;
}

void btVector3_setX_1(btVector3Addr self, double arg0) {
	reinterpret_cast<btVector3*>(self)->setX(arg0);
}

void btVector3_setY_1(btVector3Addr self, double arg0) {
	reinterpret_cast<btVector3*>(self)->setY(arg0);
}

void btVector3_setZ_1(btVector3Addr self, double arg0) {
	reinterpret_cast<btVector3*>(self)->setZ(arg0);
}

void btVector3_setValue_3(btVector3Addr self, double arg0, double arg1, double arg2) {
	reinterpret_cast<btVector3*>(self)->setValue(arg0, arg1, arg2);
}

void btVector3_normalize_0(btVector3Addr self) {
	reinterpret_cast<btVector3*>(self)->normalize();
}

btVector3Addr btVector3_rotate_2(btVector3Addr self, btVector3Addr arg0, double arg1) {
	static btVector3 temp;
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	temp = reinterpret_cast<btVector3*>(self)->rotate(*pArg0, arg1);
	return reinterpret_cast<btVector3Addr>(&temp);
}

double btVector3_dot_1(btVector3Addr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	double ret = reinterpret_cast<btVector3*>(self)->dot(*pArg0);
	return ret;
}

btVector3Addr btVector3_op_mul_1(btVector3Addr self, double arg0) {
	btVector3* pSelf = reinterpret_cast<btVector3*>(self);
	*pSelf *= arg0;
	return reinterpret_cast<btVector3Addr>(pSelf);
}

btVector3Addr btVector3_op_add_1(btVector3Addr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);
btVector3* pSelf = reinterpret_cast<btVector3*>(self);
	*pSelf += *pArg0;
	return reinterpret_cast<btVector3Addr>(pSelf);
}

btVector3Addr btVector3_op_sub_1(btVector3Addr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);
btVector3* pSelf = reinterpret_cast<btVector3*>(self);
	*pSelf -= *pArg0;
	return reinterpret_cast<btVector3Addr>(pSelf);
}

void btVector3___destroy___0(btVector3Addr self) {
	delete reinterpret_cast<btVector3*>(self);
}

// btVehicleRaycaster

void btVehicleRaycaster_castRay_3(btVehicleRaycasterAddr self, btVector3Addr arg0, btVector3Addr arg1, btVehicleRaycasterResultAddr arg2) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);
btDefaultVehicleRaycaster::btVehicleRaycasterResult* pArg2 = reinterpret_cast<btDefaultVehicleRaycaster::btVehicleRaycasterResult*>(arg2);

	reinterpret_cast<btVehicleRaycaster*>(self)->castRay(*pArg0, *pArg1, *pArg2);
}

void btVehicleRaycaster___destroy___0(btVehicleRaycasterAddr self) {
	delete reinterpret_cast<btVehicleRaycaster*>(self);
}

// btQuadWord

double btQuadWord_x_0(btQuadWordAddr self) {
	double ret = reinterpret_cast<btQuadWord*>(self)->x();
	return ret;
}

double btQuadWord_y_0(btQuadWordAddr self) {
	double ret = reinterpret_cast<btQuadWord*>(self)->y();
	return ret;
}

double btQuadWord_z_0(btQuadWordAddr self) {
	double ret = reinterpret_cast<btQuadWord*>(self)->z();
	return ret;
}

double btQuadWord_w_0(btQuadWordAddr self) {
	double ret = reinterpret_cast<btQuadWord*>(self)->w();
	return ret;
}

void btQuadWord_setX_1(btQuadWordAddr self, double arg0) {
	reinterpret_cast<btQuadWord*>(self)->setX(arg0);
}

void btQuadWord_setY_1(btQuadWordAddr self, double arg0) {
	reinterpret_cast<btQuadWord*>(self)->setY(arg0);
}

void btQuadWord_setZ_1(btQuadWordAddr self, double arg0) {
	reinterpret_cast<btQuadWord*>(self)->setZ(arg0);
}

void btQuadWord_setW_1(btQuadWordAddr self, double arg0) {
	reinterpret_cast<btQuadWord*>(self)->setW(arg0);
}

void btQuadWord___destroy___0(btQuadWordAddr self) {
	delete reinterpret_cast<btQuadWord*>(self);
}

// btCylinderShape

btCylinderShapeAddr btCylinderShape_btCylinderShape_1(btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	btCylinderShape* newObj = new btCylinderShape(*pArg0);
	return reinterpret_cast<btCylinderShapeAddr>(newObj);
}

void btCylinderShape_setMargin_1(btCylinderShapeAddr self, double arg0) {
	reinterpret_cast<btCylinderShape*>(self)->setMargin(arg0);
}

double btCylinderShape_getMargin_0(btCylinderShapeAddr self) {
	double ret = reinterpret_cast<btCylinderShape*>(self)->getMargin();
	return ret;
}

void btCylinderShape_setLocalScaling_1(btCylinderShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btCylinderShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btCylinderShape_getLocalScaling_0(btCylinderShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btCylinderShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btCylinderShape_calculateLocalInertia_2(btCylinderShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btCylinderShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btCylinderShape___destroy___0(btCylinderShapeAddr self) {
	delete reinterpret_cast<btCylinderShape*>(self);
}

// btConvexShape

void btConvexShape_setLocalScaling_1(btConvexShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btConvexShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btConvexShape_getLocalScaling_0(btConvexShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btConvexShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btConvexShape_calculateLocalInertia_2(btConvexShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btConvexShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btConvexShape_setMargin_1(btConvexShapeAddr self, double arg0) {
	reinterpret_cast<btConvexShape*>(self)->setMargin(arg0);
}

double btConvexShape_getMargin_0(btConvexShapeAddr self) {
	double ret = reinterpret_cast<btConvexShape*>(self)->getMargin();
	return ret;
}

void btConvexShape___destroy___0(btConvexShapeAddr self) {
	delete reinterpret_cast<btConvexShape*>(self);
}

// btDispatcher

long_t btDispatcher_getNumManifolds_0(btDispatcherAddr self) {
	long_t ret = reinterpret_cast<btDispatcher*>(self)->getNumManifolds();
	return ret;
}

btPersistentManifoldAddr btDispatcher_getManifoldByIndexInternal_1(btDispatcherAddr self, long_t arg0) {
	btPersistentManifold* ret = reinterpret_cast<btDispatcher*>(self)->getManifoldByIndexInternal(arg0);
	return reinterpret_cast<btPersistentManifoldAddr>(ret);
}

void btDispatcher___destroy___0(btDispatcherAddr self) {
	delete reinterpret_cast<btDispatcher*>(self);
}

// btGeneric6DofConstraint

btGeneric6DofConstraintAddr btGeneric6DofConstraint_btGeneric6DofConstraint_3(btRigidBodyAddr arg0, btTransformAddr arg1, bool arg2) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btTransform* pArg1 = reinterpret_cast<btTransform*>(arg1);

	btGeneric6DofConstraint* newObj = new btGeneric6DofConstraint(*pArg0, *pArg1, arg2);
	return reinterpret_cast<btGeneric6DofConstraintAddr>(newObj);
}

btGeneric6DofConstraintAddr btGeneric6DofConstraint_btGeneric6DofConstraint_5(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3, bool arg4) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btRigidBody* pArg1 = reinterpret_cast<btRigidBody*>(arg1);
btTransform* pArg2 = reinterpret_cast<btTransform*>(arg2);
btTransform* pArg3 = reinterpret_cast<btTransform*>(arg3);

	btGeneric6DofConstraint* newObj = new btGeneric6DofConstraint(*pArg0, *pArg1, *pArg2, *pArg3, arg4);
	return reinterpret_cast<btGeneric6DofConstraintAddr>(newObj);
}

void btGeneric6DofConstraint_setLinearLowerLimit_1(btGeneric6DofConstraintAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGeneric6DofConstraint*>(self)->setLinearLowerLimit(*pArg0);
}

void btGeneric6DofConstraint_setLinearUpperLimit_1(btGeneric6DofConstraintAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGeneric6DofConstraint*>(self)->setLinearUpperLimit(*pArg0);
}

void btGeneric6DofConstraint_setAngularLowerLimit_1(btGeneric6DofConstraintAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGeneric6DofConstraint*>(self)->setAngularLowerLimit(*pArg0);
}

void btGeneric6DofConstraint_setAngularUpperLimit_1(btGeneric6DofConstraintAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGeneric6DofConstraint*>(self)->setAngularUpperLimit(*pArg0);
}

btTransformAddr btGeneric6DofConstraint_getFrameOffsetA_0(btGeneric6DofConstraintAddr self) {
	const btTransform& ret = reinterpret_cast<btGeneric6DofConstraint*>(self)->getFrameOffsetA();
	return reinterpret_cast<btTransformAddr>(&ret);
}

void btGeneric6DofConstraint_enableFeedback_1(btGeneric6DofConstraintAddr self, bool arg0) {
	reinterpret_cast<btGeneric6DofConstraint*>(self)->enableFeedback(arg0);
}

double btGeneric6DofConstraint_getBreakingImpulseThreshold_0(btGeneric6DofConstraintAddr self) {
	double ret = reinterpret_cast<btGeneric6DofConstraint*>(self)->getBreakingImpulseThreshold();
	return ret;
}

void btGeneric6DofConstraint_setBreakingImpulseThreshold_1(btGeneric6DofConstraintAddr self, double arg0) {
	reinterpret_cast<btGeneric6DofConstraint*>(self)->setBreakingImpulseThreshold(arg0);
}

double btGeneric6DofConstraint_getParam_2(btGeneric6DofConstraintAddr self, long_t arg0, long_t arg1) {
	double ret = reinterpret_cast<btGeneric6DofConstraint*>(self)->getParam(arg0, arg1);
	return ret;
}

void btGeneric6DofConstraint_setParam_3(btGeneric6DofConstraintAddr self, long_t arg0, double arg1, long_t arg2) {
	reinterpret_cast<btGeneric6DofConstraint*>(self)->setParam(arg0, arg1, arg2);
}

void btGeneric6DofConstraint___destroy___0(btGeneric6DofConstraintAddr self) {
	delete reinterpret_cast<btGeneric6DofConstraint*>(self);
}

// btStridingMeshInterface

void btStridingMeshInterface___destroy___0(btStridingMeshInterfaceAddr self) {
	delete reinterpret_cast<btStridingMeshInterface*>(self);
}

// btMotionState

void btMotionState_getWorldTransform_1(btMotionStateAddr self, btTransformAddr arg0) {
	btTransform* pArg0 = reinterpret_cast<btTransform*>(arg0);

	reinterpret_cast<btMotionState*>(self)->getWorldTransform(*pArg0);
}

void btMotionState_setWorldTransform_1(btMotionStateAddr self, btTransformAddr arg0) {
	btTransform* pArg0 = reinterpret_cast<btTransform*>(arg0);

	reinterpret_cast<btMotionState*>(self)->setWorldTransform(*pArg0);
}

void btMotionState___destroy___0(btMotionStateAddr self) {
	delete reinterpret_cast<btMotionState*>(self);
}

// ConvexResultCallback

bool ConvexResultCallback_hasHit_0(ConvexResultCallbackAddr self) {
	bool ret = reinterpret_cast<btCollisionWorld::ConvexResultCallback*>(self)->hasHit();
	return ret;
}

long_t ConvexResultCallback_get_m_collisionFilterGroup_0(ConvexResultCallbackAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::ConvexResultCallback*>(self)->m_collisionFilterGroup;
	return ret;
}

void ConvexResultCallback_set_m_collisionFilterGroup_1(ConvexResultCallbackAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::ConvexResultCallback*>(self)->m_collisionFilterGroup = arg0;
}

long_t ConvexResultCallback_get_m_collisionFilterMask_0(ConvexResultCallbackAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::ConvexResultCallback*>(self)->m_collisionFilterMask;
	return ret;
}

void ConvexResultCallback_set_m_collisionFilterMask_1(ConvexResultCallbackAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::ConvexResultCallback*>(self)->m_collisionFilterMask = arg0;
}

double ConvexResultCallback_get_m_closestHitFraction_0(ConvexResultCallbackAddr self) {
	double ret = reinterpret_cast<btCollisionWorld::ConvexResultCallback*>(self)->m_closestHitFraction;
	return ret;
}

void ConvexResultCallback_set_m_closestHitFraction_1(ConvexResultCallbackAddr self, double arg0) {
	reinterpret_cast<btCollisionWorld::ConvexResultCallback*>(self)->m_closestHitFraction = arg0;
}

void ConvexResultCallback___destroy___0(ConvexResultCallbackAddr self) {
	delete reinterpret_cast<btCollisionWorld::ConvexResultCallback*>(self);
}

// ContactResultCallback

double ContactResultCallback_addSingleResult_7(ContactResultCallbackAddr self, btManifoldPointAddr arg0, btCollisionObjectWrapperAddr arg1, long_t arg2, long_t arg3, btCollisionObjectWrapperAddr arg4, long_t arg5, long_t arg6) {
	btManifoldPoint* pArg0 = reinterpret_cast<btManifoldPoint*>(arg0);
const btCollisionObjectWrapper* pArg1 = reinterpret_cast<const btCollisionObjectWrapper*>(arg1);
const btCollisionObjectWrapper* pArg4 = reinterpret_cast<const btCollisionObjectWrapper*>(arg4);

	double ret = reinterpret_cast<btCollisionWorld::ContactResultCallback*>(self)->addSingleResult(*pArg0, pArg1, arg2, arg3, pArg4, arg5, arg6);
	return ret;
}

void ContactResultCallback___destroy___0(ContactResultCallbackAddr self) {
	delete reinterpret_cast<btCollisionWorld::ContactResultCallback*>(self);
}

// RayResultCallback

bool RayResultCallback_hasHit_0(RayResultCallbackAddr self) {
	bool ret = reinterpret_cast<btCollisionWorld::RayResultCallback*>(self)->hasHit();
	return ret;
}

long_t RayResultCallback_get_m_collisionFilterGroup_0(RayResultCallbackAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::RayResultCallback*>(self)->m_collisionFilterGroup;
	return ret;
}

void RayResultCallback_set_m_collisionFilterGroup_1(RayResultCallbackAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::RayResultCallback*>(self)->m_collisionFilterGroup = arg0;
}

long_t RayResultCallback_get_m_collisionFilterMask_0(RayResultCallbackAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::RayResultCallback*>(self)->m_collisionFilterMask;
	return ret;
}

void RayResultCallback_set_m_collisionFilterMask_1(RayResultCallbackAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::RayResultCallback*>(self)->m_collisionFilterMask = arg0;
}

double RayResultCallback_get_m_closestHitFraction_0(RayResultCallbackAddr self) {
	double ret = reinterpret_cast<btCollisionWorld::RayResultCallback*>(self)->m_closestHitFraction;
	return ret;
}

void RayResultCallback_set_m_closestHitFraction_1(RayResultCallbackAddr self, double arg0) {
	reinterpret_cast<btCollisionWorld::RayResultCallback*>(self)->m_closestHitFraction = arg0;
}

btCollisionObjectAddr RayResultCallback_get_m_collisionObject_0(RayResultCallbackAddr self) {
	const btCollisionObject* ret = reinterpret_cast<btCollisionWorld::RayResultCallback*>(self)->m_collisionObject;
	return reinterpret_cast<btCollisionObjectAddr>(ret);
}

void RayResultCallback_set_m_collisionObject_1(RayResultCallbackAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btCollisionWorld::RayResultCallback*>(self)->m_collisionObject = pArg0;
}

void RayResultCallback___destroy___0(RayResultCallbackAddr self) {
	delete reinterpret_cast<btCollisionWorld::RayResultCallback*>(self);
}

// btMatrix3x3

void btMatrix3x3_setEulerZYX_3(btMatrix3x3Addr self, double arg0, double arg1, double arg2) {
	reinterpret_cast<btMatrix3x3*>(self)->setEulerZYX(arg0, arg1, arg2);
}

void btMatrix3x3_getRotation_1(btMatrix3x3Addr self, btQuaternionAddr arg0) {
	btQuaternion* pArg0 = reinterpret_cast<btQuaternion*>(arg0);

	reinterpret_cast<btMatrix3x3*>(self)->getRotation(*pArg0);
}

btVector3Addr btMatrix3x3_getRow_1(btMatrix3x3Addr self, long_t arg0) {
	static btVector3 temp;
	temp = reinterpret_cast<btMatrix3x3*>(self)->getRow(arg0);
	return reinterpret_cast<btVector3Addr>(&temp);
}

void btMatrix3x3___destroy___0(btMatrix3x3Addr self) {
	delete reinterpret_cast<btMatrix3x3*>(self);
}

// btDispatcherInfo

double btDispatcherInfo_get_m_timeStep_0(btDispatcherInfoAddr self) {
	double ret = reinterpret_cast<btDispatcherInfo*>(self)->m_timeStep;
	return ret;
}

void btDispatcherInfo_set_m_timeStep_1(btDispatcherInfoAddr self, double arg0) {
	reinterpret_cast<btDispatcherInfo*>(self)->m_timeStep = arg0;
}

long_t btDispatcherInfo_get_m_stepCount_0(btDispatcherInfoAddr self) {
	long_t ret = reinterpret_cast<btDispatcherInfo*>(self)->m_stepCount;
	return ret;
}

void btDispatcherInfo_set_m_stepCount_1(btDispatcherInfoAddr self, long_t arg0) {
	reinterpret_cast<btDispatcherInfo*>(self)->m_stepCount = arg0;
}

long_t btDispatcherInfo_get_m_dispatchFunc_0(btDispatcherInfoAddr self) {
	long_t ret = reinterpret_cast<btDispatcherInfo*>(self)->m_dispatchFunc;
	return ret;
}

void btDispatcherInfo_set_m_dispatchFunc_1(btDispatcherInfoAddr self, long_t arg0) {
	reinterpret_cast<btDispatcherInfo*>(self)->m_dispatchFunc = arg0;
}

double btDispatcherInfo_get_m_timeOfImpact_0(btDispatcherInfoAddr self) {
	double ret = reinterpret_cast<btDispatcherInfo*>(self)->m_timeOfImpact;
	return ret;
}

void btDispatcherInfo_set_m_timeOfImpact_1(btDispatcherInfoAddr self, double arg0) {
	reinterpret_cast<btDispatcherInfo*>(self)->m_timeOfImpact = arg0;
}

bool btDispatcherInfo_get_m_useContinuous_0(btDispatcherInfoAddr self) {
	bool ret = reinterpret_cast<btDispatcherInfo*>(self)->m_useContinuous;
	return ret;
}

void btDispatcherInfo_set_m_useContinuous_1(btDispatcherInfoAddr self, bool arg0) {
	reinterpret_cast<btDispatcherInfo*>(self)->m_useContinuous = arg0;
}

bool btDispatcherInfo_get_m_enableSatConvex_0(btDispatcherInfoAddr self) {
	bool ret = reinterpret_cast<btDispatcherInfo*>(self)->m_enableSatConvex;
	return ret;
}

void btDispatcherInfo_set_m_enableSatConvex_1(btDispatcherInfoAddr self, bool arg0) {
	reinterpret_cast<btDispatcherInfo*>(self)->m_enableSatConvex = arg0;
}

bool btDispatcherInfo_get_m_enableSPU_0(btDispatcherInfoAddr self) {
	bool ret = reinterpret_cast<btDispatcherInfo*>(self)->m_enableSPU;
	return ret;
}

void btDispatcherInfo_set_m_enableSPU_1(btDispatcherInfoAddr self, bool arg0) {
	reinterpret_cast<btDispatcherInfo*>(self)->m_enableSPU = arg0;
}

bool btDispatcherInfo_get_m_useEpa_0(btDispatcherInfoAddr self) {
	bool ret = reinterpret_cast<btDispatcherInfo*>(self)->m_useEpa;
	return ret;
}

void btDispatcherInfo_set_m_useEpa_1(btDispatcherInfoAddr self, bool arg0) {
	reinterpret_cast<btDispatcherInfo*>(self)->m_useEpa = arg0;
}

double btDispatcherInfo_get_m_allowedCcdPenetration_0(btDispatcherInfoAddr self) {
	double ret = reinterpret_cast<btDispatcherInfo*>(self)->m_allowedCcdPenetration;
	return ret;
}

void btDispatcherInfo_set_m_allowedCcdPenetration_1(btDispatcherInfoAddr self, double arg0) {
	reinterpret_cast<btDispatcherInfo*>(self)->m_allowedCcdPenetration = arg0;
}

bool btDispatcherInfo_get_m_useConvexConservativeDistanceUtil_0(btDispatcherInfoAddr self) {
	bool ret = reinterpret_cast<btDispatcherInfo*>(self)->m_useConvexConservativeDistanceUtil;
	return ret;
}

void btDispatcherInfo_set_m_useConvexConservativeDistanceUtil_1(btDispatcherInfoAddr self, bool arg0) {
	reinterpret_cast<btDispatcherInfo*>(self)->m_useConvexConservativeDistanceUtil = arg0;
}

double btDispatcherInfo_get_m_convexConservativeDistanceThreshold_0(btDispatcherInfoAddr self) {
	double ret = reinterpret_cast<btDispatcherInfo*>(self)->m_convexConservativeDistanceThreshold;
	return ret;
}

void btDispatcherInfo_set_m_convexConservativeDistanceThreshold_1(btDispatcherInfoAddr self, double arg0) {
	reinterpret_cast<btDispatcherInfo*>(self)->m_convexConservativeDistanceThreshold = arg0;
}

void btDispatcherInfo___destroy___0(btDispatcherInfoAddr self) {
	delete reinterpret_cast<btDispatcherInfo*>(self);
}

// LayaMotionState

LayaMotionStateAddr LayaMotionState_LayaMotionState_0() {
	LayaMotionState0* newObj = new LayaMotionState0();
	return reinterpret_cast<LayaMotionStateAddr>(newObj);
}

void LayaMotionState_getWorldTransform_1(LayaMotionStateAddr self, btTransformAddr arg0) {
	btTransform* pArg0 = reinterpret_cast<btTransform*>(arg0);

	reinterpret_cast<LayaMotionState0*>(self)->getWorldTransform(*pArg0);
}

void LayaMotionState_setWorldTransform_1(LayaMotionStateAddr self, btTransformAddr arg0) {
	const btTransform* pArg0 = reinterpret_cast<const btTransform*>(arg0);

	reinterpret_cast<LayaMotionState0*>(self)->setWorldTransform(*pArg0);
}

void LayaMotionState_setNativeGetWorldTransform_1(LayaMotionStateAddr self, JSValueAsParam arg0) {
	reinterpret_cast<LayaMotionState0*>(self)->setNativeGetWorldTransform(arg0);
}

void LayaMotionState_setNativeSetWorldTransform_1(LayaMotionStateAddr self, JSValueAsParam arg0) {
	reinterpret_cast<LayaMotionState0*>(self)->setNativeSetWorldTransform(arg0);
}

void LayaMotionState_setRigidbody_1(LayaMotionStateAddr self, JsValue arg0) {
	reinterpret_cast<LayaMotionState0*>(self)->setRigidbody(arg0);
}

void LayaMotionState___destroy___0(LayaMotionStateAddr self) {
	delete reinterpret_cast<LayaMotionState0*>(self);
}

// btWheelInfoConstructionInfo

btVector3Addr btWheelInfoConstructionInfo_get_m_chassisConnectionCS_0(btWheelInfoConstructionInfoAddr self) {
	btVector3* ret = &reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_chassisConnectionCS;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btWheelInfoConstructionInfo_set_m_chassisConnectionCS_1(btWheelInfoConstructionInfoAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_chassisConnectionCS = *pArg0;
}

btVector3Addr btWheelInfoConstructionInfo_get_m_wheelDirectionCS_0(btWheelInfoConstructionInfoAddr self) {
	btVector3* ret = &reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_wheelDirectionCS;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btWheelInfoConstructionInfo_set_m_wheelDirectionCS_1(btWheelInfoConstructionInfoAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_wheelDirectionCS = *pArg0;
}

btVector3Addr btWheelInfoConstructionInfo_get_m_wheelAxleCS_0(btWheelInfoConstructionInfoAddr self) {
	btVector3* ret = &reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_wheelAxleCS;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btWheelInfoConstructionInfo_set_m_wheelAxleCS_1(btWheelInfoConstructionInfoAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_wheelAxleCS = *pArg0;
}

double btWheelInfoConstructionInfo_get_m_suspensionRestLength_0(btWheelInfoConstructionInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_suspensionRestLength;
	return ret;
}

void btWheelInfoConstructionInfo_set_m_suspensionRestLength_1(btWheelInfoConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_suspensionRestLength = arg0;
}

double btWheelInfoConstructionInfo_get_m_maxSuspensionTravelCm_0(btWheelInfoConstructionInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_maxSuspensionTravelCm;
	return ret;
}

void btWheelInfoConstructionInfo_set_m_maxSuspensionTravelCm_1(btWheelInfoConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_maxSuspensionTravelCm = arg0;
}

double btWheelInfoConstructionInfo_get_m_wheelRadius_0(btWheelInfoConstructionInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_wheelRadius;
	return ret;
}

void btWheelInfoConstructionInfo_set_m_wheelRadius_1(btWheelInfoConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_wheelRadius = arg0;
}

double btWheelInfoConstructionInfo_get_m_suspensionStiffness_0(btWheelInfoConstructionInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_suspensionStiffness;
	return ret;
}

void btWheelInfoConstructionInfo_set_m_suspensionStiffness_1(btWheelInfoConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_suspensionStiffness = arg0;
}

double btWheelInfoConstructionInfo_get_m_wheelsDampingCompression_0(btWheelInfoConstructionInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_wheelsDampingCompression;
	return ret;
}

void btWheelInfoConstructionInfo_set_m_wheelsDampingCompression_1(btWheelInfoConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_wheelsDampingCompression = arg0;
}

double btWheelInfoConstructionInfo_get_m_wheelsDampingRelaxation_0(btWheelInfoConstructionInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_wheelsDampingRelaxation;
	return ret;
}

void btWheelInfoConstructionInfo_set_m_wheelsDampingRelaxation_1(btWheelInfoConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_wheelsDampingRelaxation = arg0;
}

double btWheelInfoConstructionInfo_get_m_frictionSlip_0(btWheelInfoConstructionInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_frictionSlip;
	return ret;
}

void btWheelInfoConstructionInfo_set_m_frictionSlip_1(btWheelInfoConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_frictionSlip = arg0;
}

double btWheelInfoConstructionInfo_get_m_maxSuspensionForce_0(btWheelInfoConstructionInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_maxSuspensionForce;
	return ret;
}

void btWheelInfoConstructionInfo_set_m_maxSuspensionForce_1(btWheelInfoConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_maxSuspensionForce = arg0;
}

bool btWheelInfoConstructionInfo_get_m_bIsFrontWheel_0(btWheelInfoConstructionInfoAddr self) {
	bool ret = reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_bIsFrontWheel;
	return ret;
}

void btWheelInfoConstructionInfo_set_m_bIsFrontWheel_1(btWheelInfoConstructionInfoAddr self, bool arg0) {
	reinterpret_cast<btWheelInfoConstructionInfo*>(self)->m_bIsFrontWheel = arg0;
}

void btWheelInfoConstructionInfo___destroy___0(btWheelInfoConstructionInfoAddr self) {
	delete reinterpret_cast<btWheelInfoConstructionInfo*>(self);
}

// btConvexTriangleMeshShape

btConvexTriangleMeshShapeAddr btConvexTriangleMeshShape_btConvexTriangleMeshShape_1(btStridingMeshInterfaceAddr arg0) {
	btStridingMeshInterface* pArg0 = reinterpret_cast<btStridingMeshInterface*>(arg0);

	btConvexTriangleMeshShape* newObj = new btConvexTriangleMeshShape(pArg0);
	return reinterpret_cast<btConvexTriangleMeshShapeAddr>(newObj);
}

btConvexTriangleMeshShapeAddr btConvexTriangleMeshShape_btConvexTriangleMeshShape_2(btStridingMeshInterfaceAddr arg0, bool arg1) {
	btStridingMeshInterface* pArg0 = reinterpret_cast<btStridingMeshInterface*>(arg0);

	btConvexTriangleMeshShape* newObj = new btConvexTriangleMeshShape(pArg0, arg1);
	return reinterpret_cast<btConvexTriangleMeshShapeAddr>(newObj);
}

void btConvexTriangleMeshShape_setLocalScaling_1(btConvexTriangleMeshShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btConvexTriangleMeshShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btConvexTriangleMeshShape_getLocalScaling_0(btConvexTriangleMeshShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btConvexTriangleMeshShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btConvexTriangleMeshShape_calculateLocalInertia_2(btConvexTriangleMeshShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btConvexTriangleMeshShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btConvexTriangleMeshShape_setMargin_1(btConvexTriangleMeshShapeAddr self, double arg0) {
	reinterpret_cast<btConvexTriangleMeshShape*>(self)->setMargin(arg0);
}

double btConvexTriangleMeshShape_getMargin_0(btConvexTriangleMeshShapeAddr self) {
	double ret = reinterpret_cast<btConvexTriangleMeshShape*>(self)->getMargin();
	return ret;
}

void btConvexTriangleMeshShape___destroy___0(btConvexTriangleMeshShapeAddr self) {
	delete reinterpret_cast<btConvexTriangleMeshShape*>(self);
}

// btBroadphaseInterface

void btBroadphaseInterface___destroy___0(btBroadphaseInterfaceAddr self) {
	delete reinterpret_cast<btBroadphaseInterface*>(self);
}

// btDefaultCollisionConfiguration

btDefaultCollisionConfigurationAddr btDefaultCollisionConfiguration_btDefaultCollisionConfiguration_0() {
	btDefaultCollisionConfiguration* newObj = new btDefaultCollisionConfiguration();
	return reinterpret_cast<btDefaultCollisionConfigurationAddr>(newObj);
}

btDefaultCollisionConfigurationAddr btDefaultCollisionConfiguration_btDefaultCollisionConfiguration_1(btDefaultCollisionConstructionInfoAddr arg0) {
	btDefaultCollisionConstructionInfo* pArg0 = reinterpret_cast<btDefaultCollisionConstructionInfo*>(arg0);

	btDefaultCollisionConfiguration* newObj = new btDefaultCollisionConfiguration(*pArg0);
	return reinterpret_cast<btDefaultCollisionConfigurationAddr>(newObj);
}

void btDefaultCollisionConfiguration___destroy___0(btDefaultCollisionConfigurationAddr self) {
	delete reinterpret_cast<btDefaultCollisionConfiguration*>(self);
}

// btRigidBodyConstructionInfo

btRigidBodyConstructionInfoAddr btRigidBodyConstructionInfo_btRigidBodyConstructionInfo_3(double arg0, btMotionStateAddr arg1, btCollisionShapeAddr arg2) {
	btMotionState* pArg1 = reinterpret_cast<btMotionState*>(arg1);
btCollisionShape* pArg2 = reinterpret_cast<btCollisionShape*>(arg2);

	btRigidBody::btRigidBodyConstructionInfo* newObj = new btRigidBody::btRigidBodyConstructionInfo(arg0, pArg1, pArg2);
	return reinterpret_cast<btRigidBodyConstructionInfoAddr>(newObj);
}

btRigidBodyConstructionInfoAddr btRigidBodyConstructionInfo_btRigidBodyConstructionInfo_4(double arg0, btMotionStateAddr arg1, btCollisionShapeAddr arg2, btVector3Addr arg3) {
	btMotionState* pArg1 = reinterpret_cast<btMotionState*>(arg1);
btCollisionShape* pArg2 = reinterpret_cast<btCollisionShape*>(arg2);
btVector3* pArg3 = reinterpret_cast<btVector3*>(arg3);

	btRigidBody::btRigidBodyConstructionInfo* newObj = new btRigidBody::btRigidBodyConstructionInfo(arg0, pArg1, pArg2, *pArg3);
	return reinterpret_cast<btRigidBodyConstructionInfoAddr>(newObj);
}

double btRigidBodyConstructionInfo_get_m_linearDamping_0(btRigidBodyConstructionInfoAddr self) {
	double ret = reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_linearDamping;
	return ret;
}

void btRigidBodyConstructionInfo_set_m_linearDamping_1(btRigidBodyConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_linearDamping = arg0;
}

double btRigidBodyConstructionInfo_get_m_angularDamping_0(btRigidBodyConstructionInfoAddr self) {
	double ret = reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_angularDamping;
	return ret;
}

void btRigidBodyConstructionInfo_set_m_angularDamping_1(btRigidBodyConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_angularDamping = arg0;
}

double btRigidBodyConstructionInfo_get_m_friction_0(btRigidBodyConstructionInfoAddr self) {
	double ret = reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_friction;
	return ret;
}

void btRigidBodyConstructionInfo_set_m_friction_1(btRigidBodyConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_friction = arg0;
}

double btRigidBodyConstructionInfo_get_m_rollingFriction_0(btRigidBodyConstructionInfoAddr self) {
	double ret = reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_rollingFriction;
	return ret;
}

void btRigidBodyConstructionInfo_set_m_rollingFriction_1(btRigidBodyConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_rollingFriction = arg0;
}

double btRigidBodyConstructionInfo_get_m_restitution_0(btRigidBodyConstructionInfoAddr self) {
	double ret = reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_restitution;
	return ret;
}

void btRigidBodyConstructionInfo_set_m_restitution_1(btRigidBodyConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_restitution = arg0;
}

double btRigidBodyConstructionInfo_get_m_linearSleepingThreshold_0(btRigidBodyConstructionInfoAddr self) {
	double ret = reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_linearSleepingThreshold;
	return ret;
}

void btRigidBodyConstructionInfo_set_m_linearSleepingThreshold_1(btRigidBodyConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_linearSleepingThreshold = arg0;
}

double btRigidBodyConstructionInfo_get_m_angularSleepingThreshold_0(btRigidBodyConstructionInfoAddr self) {
	double ret = reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_angularSleepingThreshold;
	return ret;
}

void btRigidBodyConstructionInfo_set_m_angularSleepingThreshold_1(btRigidBodyConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_angularSleepingThreshold = arg0;
}

bool btRigidBodyConstructionInfo_get_m_additionalDamping_0(btRigidBodyConstructionInfoAddr self) {
	bool ret = reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_additionalDamping;
	return ret;
}

void btRigidBodyConstructionInfo_set_m_additionalDamping_1(btRigidBodyConstructionInfoAddr self, bool arg0) {
	reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_additionalDamping = arg0;
}

double btRigidBodyConstructionInfo_get_m_additionalDampingFactor_0(btRigidBodyConstructionInfoAddr self) {
	double ret = reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_additionalDampingFactor;
	return ret;
}

void btRigidBodyConstructionInfo_set_m_additionalDampingFactor_1(btRigidBodyConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_additionalDampingFactor = arg0;
}

double btRigidBodyConstructionInfo_get_m_additionalLinearDampingThresholdSqr_0(btRigidBodyConstructionInfoAddr self) {
	double ret = reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_additionalLinearDampingThresholdSqr;
	return ret;
}

void btRigidBodyConstructionInfo_set_m_additionalLinearDampingThresholdSqr_1(btRigidBodyConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_additionalLinearDampingThresholdSqr = arg0;
}

double btRigidBodyConstructionInfo_get_m_additionalAngularDampingThresholdSqr_0(btRigidBodyConstructionInfoAddr self) {
	double ret = reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_additionalAngularDampingThresholdSqr;
	return ret;
}

void btRigidBodyConstructionInfo_set_m_additionalAngularDampingThresholdSqr_1(btRigidBodyConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_additionalAngularDampingThresholdSqr = arg0;
}

double btRigidBodyConstructionInfo_get_m_additionalAngularDampingFactor_0(btRigidBodyConstructionInfoAddr self) {
	double ret = reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_additionalAngularDampingFactor;
	return ret;
}

void btRigidBodyConstructionInfo_set_m_additionalAngularDampingFactor_1(btRigidBodyConstructionInfoAddr self, double arg0) {
	reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self)->m_additionalAngularDampingFactor = arg0;
}

void btRigidBodyConstructionInfo___destroy___0(btRigidBodyConstructionInfoAddr self) {
	delete reinterpret_cast<btRigidBody::btRigidBodyConstructionInfo*>(self);
}

// btCollisionConfiguration

void btCollisionConfiguration___destroy___0(btCollisionConfigurationAddr self) {
	delete reinterpret_cast<btCollisionConfiguration*>(self);
}

// btPersistentManifold

btPersistentManifoldAddr btPersistentManifold_btPersistentManifold_0() {
	btPersistentManifold* newObj = new btPersistentManifold();
	return reinterpret_cast<btPersistentManifoldAddr>(newObj);
}

btCollisionObjectAddr btPersistentManifold_getBody0_0(btPersistentManifoldAddr self) {
	const btCollisionObject* ret = reinterpret_cast<btPersistentManifold*>(self)->getBody0();
	return reinterpret_cast<btCollisionObjectAddr>(ret);
}

btCollisionObjectAddr btPersistentManifold_getBody1_0(btPersistentManifoldAddr self) {
	const btCollisionObject* ret = reinterpret_cast<btPersistentManifold*>(self)->getBody1();
	return reinterpret_cast<btCollisionObjectAddr>(ret);
}

long_t btPersistentManifold_getNumContacts_0(btPersistentManifoldAddr self) {
	long_t ret = reinterpret_cast<btPersistentManifold*>(self)->getNumContacts();
	return ret;
}

btManifoldPointAddr btPersistentManifold_getContactPoint_1(btPersistentManifoldAddr self, long_t arg0) {
	btManifoldPoint& ret = reinterpret_cast<btPersistentManifold*>(self)->getContactPoint(arg0);
	return reinterpret_cast<btManifoldPointAddr>(&ret);
}

void btPersistentManifold___destroy___0(btPersistentManifoldAddr self) {
	delete reinterpret_cast<btPersistentManifold*>(self);
}

// btCompoundShape

btCompoundShapeAddr btCompoundShape_btCompoundShape_0() {
	btCompoundShape* newObj = new btCompoundShape();
	return reinterpret_cast<btCompoundShapeAddr>(newObj);
}

btCompoundShapeAddr btCompoundShape_btCompoundShape_1(bool arg0) {
	btCompoundShape* newObj = new btCompoundShape(arg0);
	return reinterpret_cast<btCompoundShapeAddr>(newObj);
}

void btCompoundShape_addChildShape_2(btCompoundShapeAddr self, btTransformAddr arg0, btCollisionShapeAddr arg1) {
	const btTransform* pArg0 = reinterpret_cast<const btTransform*>(arg0);
btCollisionShape* pArg1 = reinterpret_cast<btCollisionShape*>(arg1);

	reinterpret_cast<btCompoundShape*>(self)->addChildShape(*pArg0, pArg1);
}

void btCompoundShape_removeChildShapeByIndex_1(btCompoundShapeAddr self, long_t arg0) {
	reinterpret_cast<btCompoundShape*>(self)->removeChildShapeByIndex(arg0);
}

long_t btCompoundShape_getNumChildShapes_0(btCompoundShapeAddr self) {
	long_t ret = reinterpret_cast<btCompoundShape*>(self)->getNumChildShapes();
	return ret;
}

btCollisionShapeAddr btCompoundShape_getChildShape_1(btCompoundShapeAddr self, long_t arg0) {
	btCollisionShape* ret = reinterpret_cast<btCompoundShape*>(self)->getChildShape(arg0);
	return reinterpret_cast<btCollisionShapeAddr>(ret);
}

void btCompoundShape_setMargin_1(btCompoundShapeAddr self, double arg0) {
	reinterpret_cast<btCompoundShape*>(self)->setMargin(arg0);
}

double btCompoundShape_getMargin_0(btCompoundShapeAddr self) {
	double ret = reinterpret_cast<btCompoundShape*>(self)->getMargin();
	return ret;
}

void btCompoundShape_updateChildTransform_2(btCompoundShapeAddr self, long_t arg0, btTransformAddr arg1) {
	const btTransform* pArg1 = reinterpret_cast<const btTransform*>(arg1);

	reinterpret_cast<btCompoundShape*>(self)->updateChildTransform(arg0, *pArg1);
}

void btCompoundShape_updateChildTransform_3(btCompoundShapeAddr self, long_t arg0, btTransformAddr arg1, bool arg2) {
	const btTransform* pArg1 = reinterpret_cast<const btTransform*>(arg1);

	reinterpret_cast<btCompoundShape*>(self)->updateChildTransform(arg0, *pArg1, arg2);
}

void btCompoundShape_setLocalScaling_1(btCompoundShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btCompoundShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btCompoundShape_getLocalScaling_0(btCompoundShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btCompoundShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btCompoundShape_calculateLocalInertia_2(btCompoundShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btCompoundShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btCompoundShape___destroy___0(btCompoundShapeAddr self) {
	delete reinterpret_cast<btCompoundShape*>(self);
}

// ClosestConvexResultCallback

ClosestConvexResultCallbackAddr ClosestConvexResultCallback_ClosestConvexResultCallback_2(btVector3Addr arg0, btVector3Addr arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);

	btCollisionWorld::ClosestConvexResultCallback* newObj = new btCollisionWorld::ClosestConvexResultCallback(*pArg0, *pArg1);
	return reinterpret_cast<ClosestConvexResultCallbackAddr>(newObj);
}

bool ClosestConvexResultCallback_hasHit_0(ClosestConvexResultCallbackAddr self) {
	bool ret = reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->hasHit();
	return ret;
}

btVector3Addr ClosestConvexResultCallback_get_m_convexFromWorld_0(ClosestConvexResultCallbackAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_convexFromWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void ClosestConvexResultCallback_set_m_convexFromWorld_1(ClosestConvexResultCallbackAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_convexFromWorld = *pArg0;
}

btVector3Addr ClosestConvexResultCallback_get_m_convexToWorld_0(ClosestConvexResultCallbackAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_convexToWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void ClosestConvexResultCallback_set_m_convexToWorld_1(ClosestConvexResultCallbackAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_convexToWorld = *pArg0;
}

btVector3Addr ClosestConvexResultCallback_get_m_hitNormalWorld_0(ClosestConvexResultCallbackAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_hitNormalWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void ClosestConvexResultCallback_set_m_hitNormalWorld_1(ClosestConvexResultCallbackAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_hitNormalWorld = *pArg0;
}

btVector3Addr ClosestConvexResultCallback_get_m_hitPointWorld_0(ClosestConvexResultCallbackAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_hitPointWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void ClosestConvexResultCallback_set_m_hitPointWorld_1(ClosestConvexResultCallbackAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_hitPointWorld = *pArg0;
}

btCollisionObjectAddr ClosestConvexResultCallback_get_m_hitCollisionObject_0(ClosestConvexResultCallbackAddr self) {
	const btCollisionObject* ret = reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_hitCollisionObject;
	return reinterpret_cast<btCollisionObjectAddr>(ret);
}

void ClosestConvexResultCallback_set_m_hitCollisionObject_1(ClosestConvexResultCallbackAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_hitCollisionObject = pArg0;
}

long_t ClosestConvexResultCallback_get_m_collisionFilterGroup_0(ClosestConvexResultCallbackAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_collisionFilterGroup;
	return ret;
}

void ClosestConvexResultCallback_set_m_collisionFilterGroup_1(ClosestConvexResultCallbackAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_collisionFilterGroup = arg0;
}

long_t ClosestConvexResultCallback_get_m_collisionFilterMask_0(ClosestConvexResultCallbackAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_collisionFilterMask;
	return ret;
}

void ClosestConvexResultCallback_set_m_collisionFilterMask_1(ClosestConvexResultCallbackAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_collisionFilterMask = arg0;
}

double ClosestConvexResultCallback_get_m_closestHitFraction_0(ClosestConvexResultCallbackAddr self) {
	double ret = reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_closestHitFraction;
	return ret;
}

void ClosestConvexResultCallback_set_m_closestHitFraction_1(ClosestConvexResultCallbackAddr self, double arg0) {
	reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self)->m_closestHitFraction = arg0;
}

void ClosestConvexResultCallback___destroy___0(ClosestConvexResultCallbackAddr self) {
	delete reinterpret_cast<btCollisionWorld::ClosestConvexResultCallback*>(self);
}

// AllHitsRayResultCallback

AllHitsRayResultCallbackAddr AllHitsRayResultCallback_AllHitsRayResultCallback_2(btVector3Addr arg0, btVector3Addr arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);

	btCollisionWorld::AllHitsRayResultCallback* newObj = new btCollisionWorld::AllHitsRayResultCallback(*pArg0, *pArg1);
	return reinterpret_cast<AllHitsRayResultCallbackAddr>(newObj);
}

bool AllHitsRayResultCallback_hasHit_0(AllHitsRayResultCallbackAddr self) {
	bool ret = reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->hasHit();
	return ret;
}

tBtCollisionObjectArrayAddr AllHitsRayResultCallback_get_m_collisionObjects_0(AllHitsRayResultCallbackAddr self) {
	btCollisionWorld::tBtCollisionObjectArray* ret = &reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_collisionObjects;
	return reinterpret_cast<tBtCollisionObjectArrayAddr>(ret);
}

void AllHitsRayResultCallback_set_m_collisionObjects_1(AllHitsRayResultCallbackAddr self, tBtCollisionObjectArrayAddr arg0) {
	btCollisionWorld::tBtCollisionObjectArray* pArg0 = reinterpret_cast<btCollisionWorld::tBtCollisionObjectArray*>(arg0);

	reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_collisionObjects = *pArg0;
}

btVector3Addr AllHitsRayResultCallback_get_m_rayFromWorld_0(AllHitsRayResultCallbackAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_rayFromWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void AllHitsRayResultCallback_set_m_rayFromWorld_1(AllHitsRayResultCallbackAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_rayFromWorld = *pArg0;
}

btVector3Addr AllHitsRayResultCallback_get_m_rayToWorld_0(AllHitsRayResultCallbackAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_rayToWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void AllHitsRayResultCallback_set_m_rayToWorld_1(AllHitsRayResultCallbackAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_rayToWorld = *pArg0;
}

tVector3ArrayAddr AllHitsRayResultCallback_get_m_hitNormalWorld_0(AllHitsRayResultCallbackAddr self) {
	btCollisionWorld::tVector3Array* ret = &reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_hitNormalWorld;
	return reinterpret_cast<tVector3ArrayAddr>(ret);
}

void AllHitsRayResultCallback_set_m_hitNormalWorld_1(AllHitsRayResultCallbackAddr self, tVector3ArrayAddr arg0) {
	btCollisionWorld::tVector3Array* pArg0 = reinterpret_cast<btCollisionWorld::tVector3Array*>(arg0);

	reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_hitNormalWorld = *pArg0;
}

tVector3ArrayAddr AllHitsRayResultCallback_get_m_hitPointWorld_0(AllHitsRayResultCallbackAddr self) {
	btCollisionWorld::tVector3Array* ret = &reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_hitPointWorld;
	return reinterpret_cast<tVector3ArrayAddr>(ret);
}

void AllHitsRayResultCallback_set_m_hitPointWorld_1(AllHitsRayResultCallbackAddr self, tVector3ArrayAddr arg0) {
	btCollisionWorld::tVector3Array* pArg0 = reinterpret_cast<btCollisionWorld::tVector3Array*>(arg0);

	reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_hitPointWorld = *pArg0;
}

tScalarArrayAddr AllHitsRayResultCallback_get_m_hitFractions_0(AllHitsRayResultCallbackAddr self) {
	btCollisionWorld::tScalarArray* ret = &reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_hitFractions;
	return reinterpret_cast<tScalarArrayAddr>(ret);
}

void AllHitsRayResultCallback_set_m_hitFractions_1(AllHitsRayResultCallbackAddr self, tScalarArrayAddr arg0) {
	btCollisionWorld::tScalarArray* pArg0 = reinterpret_cast<btCollisionWorld::tScalarArray*>(arg0);

	reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_hitFractions = *pArg0;
}

long_t AllHitsRayResultCallback_get_m_collisionFilterGroup_0(AllHitsRayResultCallbackAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_collisionFilterGroup;
	return ret;
}

void AllHitsRayResultCallback_set_m_collisionFilterGroup_1(AllHitsRayResultCallbackAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_collisionFilterGroup = arg0;
}

long_t AllHitsRayResultCallback_get_m_collisionFilterMask_0(AllHitsRayResultCallbackAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_collisionFilterMask;
	return ret;
}

void AllHitsRayResultCallback_set_m_collisionFilterMask_1(AllHitsRayResultCallbackAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_collisionFilterMask = arg0;
}

double AllHitsRayResultCallback_get_m_closestHitFraction_0(AllHitsRayResultCallbackAddr self) {
	double ret = reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_closestHitFraction;
	return ret;
}

void AllHitsRayResultCallback_set_m_closestHitFraction_1(AllHitsRayResultCallbackAddr self, double arg0) {
	reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_closestHitFraction = arg0;
}

btCollisionObjectAddr AllHitsRayResultCallback_get_m_collisionObject_0(AllHitsRayResultCallbackAddr self) {
	const btCollisionObject* ret = reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_collisionObject;
	return reinterpret_cast<btCollisionObjectAddr>(ret);
}

void AllHitsRayResultCallback_set_m_collisionObject_1(AllHitsRayResultCallbackAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self)->m_collisionObject = pArg0;
}

void AllHitsRayResultCallback___destroy___0(AllHitsRayResultCallbackAddr self) {
	delete reinterpret_cast<btCollisionWorld::AllHitsRayResultCallback*>(self);
}

// btDefaultVehicleRaycaster

btDefaultVehicleRaycasterAddr btDefaultVehicleRaycaster_btDefaultVehicleRaycaster_1(btDynamicsWorldAddr arg0) {
	btDynamicsWorld* pArg0 = reinterpret_cast<btDynamicsWorld*>(arg0);

	btDefaultVehicleRaycaster* newObj = new btDefaultVehicleRaycaster(pArg0);
	return reinterpret_cast<btDefaultVehicleRaycasterAddr>(newObj);
}

void btDefaultVehicleRaycaster_castRay_3(btDefaultVehicleRaycasterAddr self, btVector3Addr arg0, btVector3Addr arg1, btVehicleRaycasterResultAddr arg2) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);
btDefaultVehicleRaycaster::btVehicleRaycasterResult* pArg2 = reinterpret_cast<btDefaultVehicleRaycaster::btVehicleRaycasterResult*>(arg2);

	reinterpret_cast<btDefaultVehicleRaycaster*>(self)->castRay(*pArg0, *pArg1, *pArg2);
}

void btDefaultVehicleRaycaster___destroy___0(btDefaultVehicleRaycasterAddr self) {
	delete reinterpret_cast<btDefaultVehicleRaycaster*>(self);
}

// btConstraintSetting

btConstraintSettingAddr btConstraintSetting_btConstraintSetting_0() {
	btConstraintSetting* newObj = new btConstraintSetting();
	return reinterpret_cast<btConstraintSettingAddr>(newObj);
}

double btConstraintSetting_get_m_tau_0(btConstraintSettingAddr self) {
	double ret = reinterpret_cast<btConstraintSetting*>(self)->m_tau;
	return ret;
}

void btConstraintSetting_set_m_tau_1(btConstraintSettingAddr self, double arg0) {
	reinterpret_cast<btConstraintSetting*>(self)->m_tau = arg0;
}

double btConstraintSetting_get_m_damping_0(btConstraintSettingAddr self) {
	double ret = reinterpret_cast<btConstraintSetting*>(self)->m_damping;
	return ret;
}

void btConstraintSetting_set_m_damping_1(btConstraintSettingAddr self, double arg0) {
	reinterpret_cast<btConstraintSetting*>(self)->m_damping = arg0;
}

double btConstraintSetting_get_m_impulseClamp_0(btConstraintSettingAddr self) {
	double ret = reinterpret_cast<btConstraintSetting*>(self)->m_impulseClamp;
	return ret;
}

void btConstraintSetting_set_m_impulseClamp_1(btConstraintSettingAddr self, double arg0) {
	reinterpret_cast<btConstraintSetting*>(self)->m_impulseClamp = arg0;
}

void btConstraintSetting___destroy___0(btConstraintSettingAddr self) {
	delete reinterpret_cast<btConstraintSetting*>(self);
}

// LocalShapeInfo

long_t LocalShapeInfo_get_m_shapePart_0(LocalShapeInfoAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::LocalShapeInfo*>(self)->m_shapePart;
	return ret;
}

void LocalShapeInfo_set_m_shapePart_1(LocalShapeInfoAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::LocalShapeInfo*>(self)->m_shapePart = arg0;
}

long_t LocalShapeInfo_get_m_triangleIndex_0(LocalShapeInfoAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::LocalShapeInfo*>(self)->m_triangleIndex;
	return ret;
}

void LocalShapeInfo_set_m_triangleIndex_1(LocalShapeInfoAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::LocalShapeInfo*>(self)->m_triangleIndex = arg0;
}

void LocalShapeInfo___destroy___0(LocalShapeInfoAddr self) {
	delete reinterpret_cast<btCollisionWorld::LocalShapeInfo*>(self);
}

// btRigidBody

btRigidBodyAddr btRigidBody_btRigidBody_1(btRigidBodyConstructionInfoAddr arg0) {
	const btRigidBody::btRigidBodyConstructionInfo* pArg0 = reinterpret_cast<const btRigidBody::btRigidBodyConstructionInfo*>(arg0);

	btRigidBody* newObj = new btRigidBody(*pArg0);
	return reinterpret_cast<btRigidBodyAddr>(newObj);
}

btTransformAddr btRigidBody_getCenterOfMassTransform_0(btRigidBodyAddr self) {
	const btTransform& ret = reinterpret_cast<btRigidBody*>(self)->getCenterOfMassTransform();
	return reinterpret_cast<btTransformAddr>(&ret);
}

void btRigidBody_setCenterOfMassTransform_1(btRigidBodyAddr self, btTransformAddr arg0) {
	const btTransform* pArg0 = reinterpret_cast<const btTransform*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setCenterOfMassTransform(*pArg0);
}

void btRigidBody_setSleepingThresholds_2(btRigidBodyAddr self, double arg0, double arg1) {
	reinterpret_cast<btRigidBody*>(self)->setSleepingThresholds(arg0, arg1);
}

double btRigidBody_getLinearSleepingThreshold_0(btRigidBodyAddr self) {
	double ret = reinterpret_cast<btRigidBody*>(self)->getLinearSleepingThreshold();
	return ret;
}

double btRigidBody_getAngularSleepingThreshold_0(btRigidBodyAddr self) {
	double ret = reinterpret_cast<btRigidBody*>(self)->getAngularSleepingThreshold();
	return ret;
}

void btRigidBody_setDamping_2(btRigidBodyAddr self, double arg0, double arg1) {
	reinterpret_cast<btRigidBody*>(self)->setDamping(arg0, arg1);
}

void btRigidBody_setMassProps_2(btRigidBodyAddr self, double arg0, btVector3Addr arg1) {
	const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);

	reinterpret_cast<btRigidBody*>(self)->setMassProps(arg0, *pArg1);
}

void btRigidBody_setLinearFactor_1(btRigidBodyAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setLinearFactor(*pArg0);
}

void btRigidBody_applyTorque_1(btRigidBodyAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->applyTorque(*pArg0);
}

void btRigidBody_applyForce_2(btRigidBodyAddr self, btVector3Addr arg0, btVector3Addr arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);

	reinterpret_cast<btRigidBody*>(self)->applyForce(*pArg0, *pArg1);
}

void btRigidBody_applyCentralForce_1(btRigidBodyAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->applyCentralForce(*pArg0);
}

void btRigidBody_applyTorqueImpulse_1(btRigidBodyAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->applyTorqueImpulse(*pArg0);
}

void btRigidBody_applyImpulse_2(btRigidBodyAddr self, btVector3Addr arg0, btVector3Addr arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);

	reinterpret_cast<btRigidBody*>(self)->applyImpulse(*pArg0, *pArg1);
}

void btRigidBody_applyCentralImpulse_1(btRigidBodyAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->applyCentralImpulse(*pArg0);
}

void btRigidBody_updateInertiaTensor_0(btRigidBodyAddr self) {
	reinterpret_cast<btRigidBody*>(self)->updateInertiaTensor();
}

btVector3Addr btRigidBody_getLinearVelocity_0(btRigidBodyAddr self) {
	const btVector3& ret = reinterpret_cast<btRigidBody*>(self)->getLinearVelocity();
	return reinterpret_cast<btVector3Addr>(&ret);
}

btVector3Addr btRigidBody_getAngularVelocity_0(btRigidBodyAddr self) {
	const btVector3& ret = reinterpret_cast<btRigidBody*>(self)->getAngularVelocity();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btRigidBody_setLinearVelocity_1(btRigidBodyAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setLinearVelocity(*pArg0);
}

void btRigidBody_setAngularVelocity_1(btRigidBodyAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setAngularVelocity(*pArg0);
}

btMotionStateAddr btRigidBody_getMotionState_0(btRigidBodyAddr self) {
	btMotionState* ret = reinterpret_cast<btRigidBody*>(self)->getMotionState();
	return reinterpret_cast<btMotionStateAddr>(ret);
}

void btRigidBody_setMotionState_1(btRigidBodyAddr self, btMotionStateAddr arg0) {
	btMotionState* pArg0 = reinterpret_cast<btMotionState*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setMotionState(pArg0);
}

void btRigidBody_setAngularFactor_1(btRigidBodyAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setAngularFactor(*pArg0);
}

btRigidBodyAddr btRigidBody_upcast_1(btRigidBodyAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	btRigidBody* ret = reinterpret_cast<btRigidBody*>(self)->upcast(pArg0);
	return reinterpret_cast<btRigidBodyAddr>(ret);
}

void btRigidBody_getAabb_2(btRigidBodyAddr self, btVector3Addr arg0, btVector3Addr arg1) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);
btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btRigidBody*>(self)->getAabb(*pArg0, *pArg1);
}

void btRigidBody_applyGravity_0(btRigidBodyAddr self) {
	reinterpret_cast<btRigidBody*>(self)->applyGravity();
}

btVector3Addr btRigidBody_getGravity_0(btRigidBodyAddr self) {
	const btVector3& ret = reinterpret_cast<btRigidBody*>(self)->getGravity();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btRigidBody_setGravity_1(btRigidBodyAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setGravity(*pArg0);
}

btVector3Addr btRigidBody_getTotalForce_0(btRigidBodyAddr self) {
	const btVector3& ret = reinterpret_cast<btRigidBody*>(self)->getTotalForce();
	return reinterpret_cast<btVector3Addr>(&ret);
}

btVector3Addr btRigidBody_getTotalTorque_0(btRigidBodyAddr self) {
	const btVector3& ret = reinterpret_cast<btRigidBody*>(self)->getTotalTorque();
	return reinterpret_cast<btVector3Addr>(&ret);
}

long_t btRigidBody_getFlags_0(btRigidBodyAddr self) {
	long_t ret = reinterpret_cast<btRigidBody*>(self)->getFlags();
	return ret;
}

void btRigidBody_setFlags_1(btRigidBodyAddr self, long_t arg0) {
	reinterpret_cast<btRigidBody*>(self)->setFlags(arg0);
}

void btRigidBody_clearForces_0(btRigidBodyAddr self) {
	reinterpret_cast<btRigidBody*>(self)->clearForces();
}

void btRigidBody_setAnisotropicFriction_2(btRigidBodyAddr self, btVector3Addr arg0, long_t arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setAnisotropicFriction(*pArg0, arg1);
}

btCollisionShapeAddr btRigidBody_getCollisionShape_0(btRigidBodyAddr self) {
	btCollisionShape* ret = reinterpret_cast<btRigidBody*>(self)->getCollisionShape();
	return reinterpret_cast<btCollisionShapeAddr>(ret);
}

void btRigidBody_setContactProcessingThreshold_1(btRigidBodyAddr self, double arg0) {
	reinterpret_cast<btRigidBody*>(self)->setContactProcessingThreshold(arg0);
}

void btRigidBody_setActivationState_1(btRigidBodyAddr self, long_t arg0) {
	reinterpret_cast<btRigidBody*>(self)->setActivationState(arg0);
}

void btRigidBody_forceActivationState_1(btRigidBodyAddr self, long_t arg0) {
	reinterpret_cast<btRigidBody*>(self)->forceActivationState(arg0);
}

void btRigidBody_activate_0(btRigidBodyAddr self) {
	reinterpret_cast<btRigidBody*>(self)->activate();
}

void btRigidBody_activate_1(btRigidBodyAddr self, bool arg0) {
	reinterpret_cast<btRigidBody*>(self)->activate(arg0);
}

bool btRigidBody_isActive_0(btRigidBodyAddr self) {
	bool ret = reinterpret_cast<btRigidBody*>(self)->isActive();
	return ret;
}

bool btRigidBody_isKinematicObject_0(btRigidBodyAddr self) {
	bool ret = reinterpret_cast<btRigidBody*>(self)->isKinematicObject();
	return ret;
}

bool btRigidBody_isStaticObject_0(btRigidBodyAddr self) {
	bool ret = reinterpret_cast<btRigidBody*>(self)->isStaticObject();
	return ret;
}

bool btRigidBody_isStaticOrKinematicObject_0(btRigidBodyAddr self) {
	bool ret = reinterpret_cast<btRigidBody*>(self)->isStaticOrKinematicObject();
	return ret;
}

double btRigidBody_getRestitution_0(btRigidBodyAddr self) {
	double ret = reinterpret_cast<btRigidBody*>(self)->getRestitution();
	return ret;
}

void btRigidBody_setRestitution_1(btRigidBodyAddr self, double arg0) {
	reinterpret_cast<btRigidBody*>(self)->setRestitution(arg0);
}

double btRigidBody_getFriction_0(btRigidBodyAddr self) {
	double ret = reinterpret_cast<btRigidBody*>(self)->getFriction();
	return ret;
}

void btRigidBody_setFriction_1(btRigidBodyAddr self, double arg0) {
	reinterpret_cast<btRigidBody*>(self)->setFriction(arg0);
}

double btRigidBody_getRollingFriction_0(btRigidBodyAddr self) {
	double ret = reinterpret_cast<btRigidBody*>(self)->getRollingFriction();
	return ret;
}

void btRigidBody_setRollingFriction_1(btRigidBodyAddr self, double arg0) {
	reinterpret_cast<btRigidBody*>(self)->setRollingFriction(arg0);
}

long_t btRigidBody_getCollisionFlags_0(btRigidBodyAddr self) {
	long_t ret = reinterpret_cast<btRigidBody*>(self)->getCollisionFlags();
	return ret;
}

void btRigidBody_setCollisionFlags_1(btRigidBodyAddr self, long_t arg0) {
	reinterpret_cast<btRigidBody*>(self)->setCollisionFlags(arg0);
}

btTransformAddr btRigidBody_getWorldTransform_0(btRigidBodyAddr self) {
	btTransform& ret = reinterpret_cast<btRigidBody*>(self)->getWorldTransform();
	return reinterpret_cast<btTransformAddr>(&ret);
}

void btRigidBody_setWorldTransform_1(btRigidBodyAddr self, btTransformAddr arg0) {
	const btTransform* pArg0 = reinterpret_cast<const btTransform*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setWorldTransform(*pArg0);
}

void btRigidBody_setCollisionShape_1(btRigidBodyAddr self, btCollisionShapeAddr arg0) {
	btCollisionShape* pArg0 = reinterpret_cast<btCollisionShape*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setCollisionShape(pArg0);
}

double btRigidBody_getCcdMotionThreshold_0(btRigidBodyAddr self) {
	double ret = reinterpret_cast<btRigidBody*>(self)->getCcdMotionThreshold();
	return ret;
}

void btRigidBody_setCcdMotionThreshold_1(btRigidBodyAddr self, double arg0) {
	reinterpret_cast<btRigidBody*>(self)->setCcdMotionThreshold(arg0);
}

double btRigidBody_getCcdSweptSphereRadius_0(btRigidBodyAddr self) {
	double ret = reinterpret_cast<btRigidBody*>(self)->getCcdSweptSphereRadius();
	return ret;
}

void btRigidBody_setCcdSweptSphereRadius_1(btRigidBodyAddr self, double arg0) {
	reinterpret_cast<btRigidBody*>(self)->setCcdSweptSphereRadius(arg0);
}

long_t btRigidBody_getUserIndex_0(btRigidBodyAddr self) {
	long_t ret = reinterpret_cast<btRigidBody*>(self)->getUserIndex();
	return ret;
}

void btRigidBody_setUserIndex_1(btRigidBodyAddr self, long_t arg0) {
	reinterpret_cast<btRigidBody*>(self)->setUserIndex(arg0);
}

VoidPtrAddr btRigidBody_getUserPointer_0(btRigidBodyAddr self) {
	void* ret = reinterpret_cast<btRigidBody*>(self)->getUserPointer();
	return reinterpret_cast<VoidPtrAddr>(ret);
}

void btRigidBody_setUserPointer_1(btRigidBodyAddr self, VoidPtrAddr arg0) {
	void* pArg0 = reinterpret_cast<void*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setUserPointer(pArg0);
}

btVector3Addr btRigidBody_getInterpolationAngularVelocity_0(btRigidBodyAddr self) {
	const btVector3& ret = reinterpret_cast<btRigidBody*>(self)->getInterpolationAngularVelocity();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btRigidBody_setInterpolationAngularVelocity_1(btRigidBodyAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setInterpolationAngularVelocity(*pArg0);
}

btVector3Addr btRigidBody_getInterpolationLinearVelocity_0(btRigidBodyAddr self) {
	const btVector3& ret = reinterpret_cast<btRigidBody*>(self)->getInterpolationLinearVelocity();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btRigidBody_setInterpolationLinearVelocity_1(btRigidBodyAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btRigidBody*>(self)->setInterpolationLinearVelocity(*pArg0);
}

btBroadphaseProxyAddr btRigidBody_getBroadphaseHandle_0(btRigidBodyAddr self) {
	btBroadphaseProxy* ret = reinterpret_cast<btRigidBody*>(self)->getBroadphaseHandle();
	return reinterpret_cast<btBroadphaseProxyAddr>(ret);
}

long_t btRigidBody_getActivationState_0(btRigidBodyAddr self) {
	long_t ret = reinterpret_cast<btRigidBody*>(self)->getActivationState();
	return ret;
}

void btRigidBody___destroy___0(btRigidBodyAddr self) {
	delete reinterpret_cast<btRigidBody*>(self);
}

// btDbvtBroadphase

btDbvtBroadphaseAddr btDbvtBroadphase_btDbvtBroadphase_0() {
	btDbvtBroadphase* newObj = new btDbvtBroadphase();
	return reinterpret_cast<btDbvtBroadphaseAddr>(newObj);
}

btOverlappingPairCacheAddr btDbvtBroadphase_getOverlappingPairCache_0(btDbvtBroadphaseAddr self) {
	btOverlappingPairCache* ret = reinterpret_cast<btDbvtBroadphase*>(self)->getOverlappingPairCache();
	return reinterpret_cast<btOverlappingPairCacheAddr>(ret);
}

void btDbvtBroadphase___destroy___0(btDbvtBroadphaseAddr self) {
	delete reinterpret_cast<btDbvtBroadphase*>(self);
}

// btCollisionDispatcher

btCollisionDispatcherAddr btCollisionDispatcher_btCollisionDispatcher_1(btDefaultCollisionConfigurationAddr arg0) {
	btDefaultCollisionConfiguration* pArg0 = reinterpret_cast<btDefaultCollisionConfiguration*>(arg0);

	btCollisionDispatcher* newObj = new btCollisionDispatcher(pArg0);
	return reinterpret_cast<btCollisionDispatcherAddr>(newObj);
}

long_t btCollisionDispatcher_getNumManifolds_0(btCollisionDispatcherAddr self) {
	long_t ret = reinterpret_cast<btCollisionDispatcher*>(self)->getNumManifolds();
	return ret;
}

btPersistentManifoldAddr btCollisionDispatcher_getManifoldByIndexInternal_1(btCollisionDispatcherAddr self, long_t arg0) {
	btPersistentManifold* ret = reinterpret_cast<btCollisionDispatcher*>(self)->getManifoldByIndexInternal(arg0);
	return reinterpret_cast<btPersistentManifoldAddr>(ret);
}

void btCollisionDispatcher___destroy___0(btCollisionDispatcherAddr self) {
	delete reinterpret_cast<btCollisionDispatcher*>(self);
}

// btAxisSweep3

btAxisSweep3Addr btAxisSweep3_btAxisSweep3_2(btVector3Addr arg0, btVector3Addr arg1) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);
btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	btAxisSweep3* newObj = new btAxisSweep3(*pArg0, *pArg1);
	return reinterpret_cast<btAxisSweep3Addr>(newObj);
}

btAxisSweep3Addr btAxisSweep3_btAxisSweep3_3(btVector3Addr arg0, btVector3Addr arg1, long_t arg2) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);
btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	btAxisSweep3* newObj = new btAxisSweep3(*pArg0, *pArg1, arg2);
	return reinterpret_cast<btAxisSweep3Addr>(newObj);
}

btAxisSweep3Addr btAxisSweep3_btAxisSweep3_4(btVector3Addr arg0, btVector3Addr arg1, long_t arg2, btOverlappingPairCacheAddr arg3) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);
btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);
btOverlappingPairCache* pArg3 = reinterpret_cast<btOverlappingPairCache*>(arg3);

	btAxisSweep3* newObj = new btAxisSweep3(*pArg0, *pArg1, arg2, pArg3);
	return reinterpret_cast<btAxisSweep3Addr>(newObj);
}

btAxisSweep3Addr btAxisSweep3_btAxisSweep3_5(btVector3Addr arg0, btVector3Addr arg1, long_t arg2, btOverlappingPairCacheAddr arg3, bool arg4) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);
btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);
btOverlappingPairCache* pArg3 = reinterpret_cast<btOverlappingPairCache*>(arg3);

	btAxisSweep3* newObj = new btAxisSweep3(*pArg0, *pArg1, arg2, pArg3, arg4);
	return reinterpret_cast<btAxisSweep3Addr>(newObj);
}

void btAxisSweep3___destroy___0(btAxisSweep3Addr self) {
	delete reinterpret_cast<btAxisSweep3*>(self);
}

// VoidPtr

void VoidPtr___destroy___0(VoidPtrAddr self) {
	delete reinterpret_cast<void**>(self);
}

// btConeTwistConstraint

btConeTwistConstraintAddr btConeTwistConstraint_btConeTwistConstraint_2(btRigidBodyAddr arg0, btTransformAddr arg1) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btTransform* pArg1 = reinterpret_cast<btTransform*>(arg1);

	btConeTwistConstraint* newObj = new btConeTwistConstraint(*pArg0, *pArg1);
	return reinterpret_cast<btConeTwistConstraintAddr>(newObj);
}

btConeTwistConstraintAddr btConeTwistConstraint_btConeTwistConstraint_4(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btRigidBody* pArg1 = reinterpret_cast<btRigidBody*>(arg1);
btTransform* pArg2 = reinterpret_cast<btTransform*>(arg2);
btTransform* pArg3 = reinterpret_cast<btTransform*>(arg3);

	btConeTwistConstraint* newObj = new btConeTwistConstraint(*pArg0, *pArg1, *pArg2, *pArg3);
	return reinterpret_cast<btConeTwistConstraintAddr>(newObj);
}

void btConeTwistConstraint_setLimit_2(btConeTwistConstraintAddr self, long_t arg0, double arg1) {
	reinterpret_cast<btConeTwistConstraint*>(self)->setLimit(arg0, arg1);
}

void btConeTwistConstraint_setAngularOnly_1(btConeTwistConstraintAddr self, bool arg0) {
	reinterpret_cast<btConeTwistConstraint*>(self)->setAngularOnly(arg0);
}

void btConeTwistConstraint_setDamping_1(btConeTwistConstraintAddr self, double arg0) {
	reinterpret_cast<btConeTwistConstraint*>(self)->setDamping(arg0);
}

void btConeTwistConstraint_enableMotor_1(btConeTwistConstraintAddr self, bool arg0) {
	reinterpret_cast<btConeTwistConstraint*>(self)->enableMotor(arg0);
}

void btConeTwistConstraint_setMaxMotorImpulse_1(btConeTwistConstraintAddr self, double arg0) {
	reinterpret_cast<btConeTwistConstraint*>(self)->setMaxMotorImpulse(arg0);
}

void btConeTwistConstraint_setMaxMotorImpulseNormalized_1(btConeTwistConstraintAddr self, double arg0) {
	reinterpret_cast<btConeTwistConstraint*>(self)->setMaxMotorImpulseNormalized(arg0);
}

void btConeTwistConstraint_setMotorTarget_1(btConeTwistConstraintAddr self, btQuaternionAddr arg0) {
	const btQuaternion* pArg0 = reinterpret_cast<const btQuaternion*>(arg0);

	reinterpret_cast<btConeTwistConstraint*>(self)->setMotorTarget(*pArg0);
}

void btConeTwistConstraint_setMotorTargetInConstraintSpace_1(btConeTwistConstraintAddr self, btQuaternionAddr arg0) {
	const btQuaternion* pArg0 = reinterpret_cast<const btQuaternion*>(arg0);

	reinterpret_cast<btConeTwistConstraint*>(self)->setMotorTargetInConstraintSpace(*pArg0);
}

void btConeTwistConstraint_enableFeedback_1(btConeTwistConstraintAddr self, bool arg0) {
	reinterpret_cast<btConeTwistConstraint*>(self)->enableFeedback(arg0);
}

double btConeTwistConstraint_getBreakingImpulseThreshold_0(btConeTwistConstraintAddr self) {
	double ret = reinterpret_cast<btConeTwistConstraint*>(self)->getBreakingImpulseThreshold();
	return ret;
}

void btConeTwistConstraint_setBreakingImpulseThreshold_1(btConeTwistConstraintAddr self, double arg0) {
	reinterpret_cast<btConeTwistConstraint*>(self)->setBreakingImpulseThreshold(arg0);
}

double btConeTwistConstraint_getParam_2(btConeTwistConstraintAddr self, long_t arg0, long_t arg1) {
	double ret = reinterpret_cast<btConeTwistConstraint*>(self)->getParam(arg0, arg1);
	return ret;
}

void btConeTwistConstraint_setParam_3(btConeTwistConstraintAddr self, long_t arg0, double arg1, long_t arg2) {
	reinterpret_cast<btConeTwistConstraint*>(self)->setParam(arg0, arg1, arg2);
}

void btConeTwistConstraint___destroy___0(btConeTwistConstraintAddr self) {
	delete reinterpret_cast<btConeTwistConstraint*>(self);
}

// btHingeConstraint

btHingeConstraintAddr btHingeConstraint_btHingeConstraint_2(btRigidBodyAddr arg0, btTransformAddr arg1) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btTransform* pArg1 = reinterpret_cast<btTransform*>(arg1);

	btHingeConstraint* newObj = new btHingeConstraint(*pArg0, *pArg1);
	return reinterpret_cast<btHingeConstraintAddr>(newObj);
}

btHingeConstraintAddr btHingeConstraint_btHingeConstraint_3(btRigidBodyAddr arg0, btTransformAddr arg1, bool arg2) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btTransform* pArg1 = reinterpret_cast<btTransform*>(arg1);

	btHingeConstraint* newObj = new btHingeConstraint(*pArg0, *pArg1, arg2);
	return reinterpret_cast<btHingeConstraintAddr>(newObj);
}

btHingeConstraintAddr btHingeConstraint_btHingeConstraint_4(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btRigidBody* pArg1 = reinterpret_cast<btRigidBody*>(arg1);
btTransform* pArg2 = reinterpret_cast<btTransform*>(arg2);
btTransform* pArg3 = reinterpret_cast<btTransform*>(arg3);

	btHingeConstraint* newObj = new btHingeConstraint(*pArg0, *pArg1, *pArg2, *pArg3);
	return reinterpret_cast<btHingeConstraintAddr>(newObj);
}

btHingeConstraintAddr btHingeConstraint_btHingeConstraint_5(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3, bool arg4) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btRigidBody* pArg1 = reinterpret_cast<btRigidBody*>(arg1);
btTransform* pArg2 = reinterpret_cast<btTransform*>(arg2);
btTransform* pArg3 = reinterpret_cast<btTransform*>(arg3);

	btHingeConstraint* newObj = new btHingeConstraint(*pArg0, *pArg1, *pArg2, *pArg3, arg4);
	return reinterpret_cast<btHingeConstraintAddr>(newObj);
}

btHingeConstraintAddr btHingeConstraint_btHingeConstraint_6(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btVector3Addr arg2, btVector3Addr arg3, btVector3Addr arg4, btVector3Addr arg5) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btRigidBody* pArg1 = reinterpret_cast<btRigidBody*>(arg1);
btVector3* pArg2 = reinterpret_cast<btVector3*>(arg2);
btVector3* pArg3 = reinterpret_cast<btVector3*>(arg3);
btVector3* pArg4 = reinterpret_cast<btVector3*>(arg4);
btVector3* pArg5 = reinterpret_cast<btVector3*>(arg5);

	btHingeConstraint* newObj = new btHingeConstraint(*pArg0, *pArg1, *pArg2, *pArg3, *pArg4, *pArg5);
	return reinterpret_cast<btHingeConstraintAddr>(newObj);
}

btHingeConstraintAddr btHingeConstraint_btHingeConstraint_7(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btVector3Addr arg2, btVector3Addr arg3, btVector3Addr arg4, btVector3Addr arg5, bool arg6) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btRigidBody* pArg1 = reinterpret_cast<btRigidBody*>(arg1);
btVector3* pArg2 = reinterpret_cast<btVector3*>(arg2);
btVector3* pArg3 = reinterpret_cast<btVector3*>(arg3);
btVector3* pArg4 = reinterpret_cast<btVector3*>(arg4);
btVector3* pArg5 = reinterpret_cast<btVector3*>(arg5);

	btHingeConstraint* newObj = new btHingeConstraint(*pArg0, *pArg1, *pArg2, *pArg3, *pArg4, *pArg5, arg6);
	return reinterpret_cast<btHingeConstraintAddr>(newObj);
}

void btHingeConstraint_setLimit_4(btHingeConstraintAddr self, double arg0, double arg1, double arg2, double arg3) {
	reinterpret_cast<btHingeConstraint*>(self)->setLimit(arg0, arg1, arg2, arg3);
}

void btHingeConstraint_setLimit_5(btHingeConstraintAddr self, double arg0, double arg1, double arg2, double arg3, double arg4) {
	reinterpret_cast<btHingeConstraint*>(self)->setLimit(arg0, arg1, arg2, arg3, arg4);
}

void btHingeConstraint_enableAngularMotor_3(btHingeConstraintAddr self, bool arg0, double arg1, double arg2) {
	reinterpret_cast<btHingeConstraint*>(self)->enableAngularMotor(arg0, arg1, arg2);
}

void btHingeConstraint_setAngularOnly_1(btHingeConstraintAddr self, bool arg0) {
	reinterpret_cast<btHingeConstraint*>(self)->setAngularOnly(arg0);
}

void btHingeConstraint_enableMotor_1(btHingeConstraintAddr self, bool arg0) {
	reinterpret_cast<btHingeConstraint*>(self)->enableMotor(arg0);
}

void btHingeConstraint_setMaxMotorImpulse_1(btHingeConstraintAddr self, double arg0) {
	reinterpret_cast<btHingeConstraint*>(self)->setMaxMotorImpulse(arg0);
}

void btHingeConstraint_setMotorTarget_2(btHingeConstraintAddr self, double arg0, double arg1) {
	reinterpret_cast<btHingeConstraint*>(self)->setMotorTarget(arg0, arg1);
}

void btHingeConstraint_enableFeedback_1(btHingeConstraintAddr self, bool arg0) {
	reinterpret_cast<btHingeConstraint*>(self)->enableFeedback(arg0);
}

double btHingeConstraint_getBreakingImpulseThreshold_0(btHingeConstraintAddr self) {
	double ret = reinterpret_cast<btHingeConstraint*>(self)->getBreakingImpulseThreshold();
	return ret;
}

void btHingeConstraint_setBreakingImpulseThreshold_1(btHingeConstraintAddr self, double arg0) {
	reinterpret_cast<btHingeConstraint*>(self)->setBreakingImpulseThreshold(arg0);
}

double btHingeConstraint_getParam_2(btHingeConstraintAddr self, long_t arg0, long_t arg1) {
	double ret = reinterpret_cast<btHingeConstraint*>(self)->getParam(arg0, arg1);
	return ret;
}

void btHingeConstraint_setParam_3(btHingeConstraintAddr self, long_t arg0, double arg1, long_t arg2) {
	reinterpret_cast<btHingeConstraint*>(self)->setParam(arg0, arg1, arg2);
}

void btHingeConstraint___destroy___0(btHingeConstraintAddr self) {
	delete reinterpret_cast<btHingeConstraint*>(self);
}

// JSValueAsParam

// btConeShapeZ

btConeShapeZAddr btConeShapeZ_btConeShapeZ_2(double arg0, double arg1) {
	btConeShapeZ* newObj = new btConeShapeZ(arg0, arg1);
	return reinterpret_cast<btConeShapeZAddr>(newObj);
}

void btConeShapeZ_setLocalScaling_1(btConeShapeZAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btConeShapeZ*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btConeShapeZ_getLocalScaling_0(btConeShapeZAddr self) {
	const btVector3& ret = reinterpret_cast<btConeShapeZ*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btConeShapeZ_calculateLocalInertia_2(btConeShapeZAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btConeShapeZ*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btConeShapeZ___destroy___0(btConeShapeZAddr self) {
	delete reinterpret_cast<btConeShapeZ*>(self);
}

// btConeShapeX

btConeShapeXAddr btConeShapeX_btConeShapeX_2(double arg0, double arg1) {
	btConeShapeX* newObj = new btConeShapeX(arg0, arg1);
	return reinterpret_cast<btConeShapeXAddr>(newObj);
}

void btConeShapeX_setLocalScaling_1(btConeShapeXAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btConeShapeX*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btConeShapeX_getLocalScaling_0(btConeShapeXAddr self) {
	const btVector3& ret = reinterpret_cast<btConeShapeX*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btConeShapeX_calculateLocalInertia_2(btConeShapeXAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btConeShapeX*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btConeShapeX___destroy___0(btConeShapeXAddr self) {
	delete reinterpret_cast<btConeShapeX*>(self);
}

// btTriangleMesh

btTriangleMeshAddr btTriangleMesh_btTriangleMesh_0() {
	btTriangleMesh* newObj = new btTriangleMesh();
	return reinterpret_cast<btTriangleMeshAddr>(newObj);
}

btTriangleMeshAddr btTriangleMesh_btTriangleMesh_1(bool arg0) {
	btTriangleMesh* newObj = new btTriangleMesh(arg0);
	return reinterpret_cast<btTriangleMeshAddr>(newObj);
}

btTriangleMeshAddr btTriangleMesh_btTriangleMesh_2(bool arg0, bool arg1) {
	btTriangleMesh* newObj = new btTriangleMesh(arg0, arg1);
	return reinterpret_cast<btTriangleMeshAddr>(newObj);
}

void btTriangleMesh_addTriangle_3(btTriangleMeshAddr self, btVector3Addr arg0, btVector3Addr arg1, btVector3Addr arg2) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);
const btVector3* pArg2 = reinterpret_cast<const btVector3*>(arg2);

	reinterpret_cast<btTriangleMesh*>(self)->addTriangle(*pArg0, *pArg1, *pArg2);
}

void btTriangleMesh_addTriangle_4(btTriangleMeshAddr self, btVector3Addr arg0, btVector3Addr arg1, btVector3Addr arg2, bool arg3) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);
const btVector3* pArg2 = reinterpret_cast<const btVector3*>(arg2);

	reinterpret_cast<btTriangleMesh*>(self)->addTriangle(*pArg0, *pArg1, *pArg2, arg3);
}

void btTriangleMesh___destroy___0(btTriangleMeshAddr self) {
	delete reinterpret_cast<btTriangleMesh*>(self);
}

// btConvexHullShape

btConvexHullShapeAddr btConvexHullShape_btConvexHullShape_0() {
	btConvexHullShape* newObj = new btConvexHullShape();
	return reinterpret_cast<btConvexHullShapeAddr>(newObj);
}

void btConvexHullShape_addPoint_1(btConvexHullShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btConvexHullShape*>(self)->addPoint(*pArg0);
}

void btConvexHullShape_addPoint_2(btConvexHullShapeAddr self, btVector3Addr arg0, bool arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btConvexHullShape*>(self)->addPoint(*pArg0, arg1);
}

void btConvexHullShape_setMargin_1(btConvexHullShapeAddr self, double arg0) {
	reinterpret_cast<btConvexHullShape*>(self)->setMargin(arg0);
}

double btConvexHullShape_getMargin_0(btConvexHullShapeAddr self) {
	double ret = reinterpret_cast<btConvexHullShape*>(self)->getMargin();
	return ret;
}

void btConvexHullShape_setLocalScaling_1(btConvexHullShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btConvexHullShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btConvexHullShape_getLocalScaling_0(btConvexHullShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btConvexHullShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btConvexHullShape_calculateLocalInertia_2(btConvexHullShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btConvexHullShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btConvexHullShape___destroy___0(btConvexHullShapeAddr self) {
	delete reinterpret_cast<btConvexHullShape*>(self);
}

// btVehicleTuning

btVehicleTuningAddr btVehicleTuning_btVehicleTuning_0() {
	btRaycastVehicle::btVehicleTuning* newObj = new btRaycastVehicle::btVehicleTuning();
	return reinterpret_cast<btVehicleTuningAddr>(newObj);
}

double btVehicleTuning_get_m_suspensionStiffness_0(btVehicleTuningAddr self) {
	double ret = reinterpret_cast<btRaycastVehicle::btVehicleTuning*>(self)->m_suspensionStiffness;
	return ret;
}

void btVehicleTuning_set_m_suspensionStiffness_1(btVehicleTuningAddr self, double arg0) {
	reinterpret_cast<btRaycastVehicle::btVehicleTuning*>(self)->m_suspensionStiffness = arg0;
}

double btVehicleTuning_get_m_suspensionCompression_0(btVehicleTuningAddr self) {
	double ret = reinterpret_cast<btRaycastVehicle::btVehicleTuning*>(self)->m_suspensionCompression;
	return ret;
}

void btVehicleTuning_set_m_suspensionCompression_1(btVehicleTuningAddr self, double arg0) {
	reinterpret_cast<btRaycastVehicle::btVehicleTuning*>(self)->m_suspensionCompression = arg0;
}

double btVehicleTuning_get_m_suspensionDamping_0(btVehicleTuningAddr self) {
	double ret = reinterpret_cast<btRaycastVehicle::btVehicleTuning*>(self)->m_suspensionDamping;
	return ret;
}

void btVehicleTuning_set_m_suspensionDamping_1(btVehicleTuningAddr self, double arg0) {
	reinterpret_cast<btRaycastVehicle::btVehicleTuning*>(self)->m_suspensionDamping = arg0;
}

double btVehicleTuning_get_m_maxSuspensionTravelCm_0(btVehicleTuningAddr self) {
	double ret = reinterpret_cast<btRaycastVehicle::btVehicleTuning*>(self)->m_maxSuspensionTravelCm;
	return ret;
}

void btVehicleTuning_set_m_maxSuspensionTravelCm_1(btVehicleTuningAddr self, double arg0) {
	reinterpret_cast<btRaycastVehicle::btVehicleTuning*>(self)->m_maxSuspensionTravelCm = arg0;
}

double btVehicleTuning_get_m_frictionSlip_0(btVehicleTuningAddr self) {
	double ret = reinterpret_cast<btRaycastVehicle::btVehicleTuning*>(self)->m_frictionSlip;
	return ret;
}

void btVehicleTuning_set_m_frictionSlip_1(btVehicleTuningAddr self, double arg0) {
	reinterpret_cast<btRaycastVehicle::btVehicleTuning*>(self)->m_frictionSlip = arg0;
}

double btVehicleTuning_get_m_maxSuspensionForce_0(btVehicleTuningAddr self) {
	double ret = reinterpret_cast<btRaycastVehicle::btVehicleTuning*>(self)->m_maxSuspensionForce;
	return ret;
}

void btVehicleTuning_set_m_maxSuspensionForce_1(btVehicleTuningAddr self, double arg0) {
	reinterpret_cast<btRaycastVehicle::btVehicleTuning*>(self)->m_maxSuspensionForce = arg0;
}

// btCollisionObjectWrapper

// btGImpactMeshShape

btGImpactMeshShapeAddr btGImpactMeshShape_btGImpactMeshShape_1(btStridingMeshInterfaceAddr arg0) {
	btStridingMeshInterface* pArg0 = reinterpret_cast<btStridingMeshInterface*>(arg0);

	btGImpactMeshShape* newObj = new btGImpactMeshShape(pArg0);
	return reinterpret_cast<btGImpactMeshShapeAddr>(newObj);
}

void btGImpactMeshShape_setLocalScaling_1(btGImpactMeshShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGImpactMeshShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btGImpactMeshShape_getLocalScaling_0(btGImpactMeshShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btGImpactMeshShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btGImpactMeshShape_calculateLocalInertia_2(btGImpactMeshShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btGImpactMeshShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btGImpactMeshShape_updateBound_0(btGImpactMeshShapeAddr self) {
	reinterpret_cast<btGImpactMeshShape*>(self)->updateBound();
}

void btGImpactMeshShape___destroy___0(btGImpactMeshShapeAddr self) {
	delete reinterpret_cast<btGImpactMeshShape*>(self);
}

// btDefaultMotionState

btDefaultMotionStateAddr btDefaultMotionState_btDefaultMotionState_0() {
	btDefaultMotionState* newObj = new btDefaultMotionState();
	return reinterpret_cast<btDefaultMotionStateAddr>(newObj);
}

btDefaultMotionStateAddr btDefaultMotionState_btDefaultMotionState_1(btTransformAddr arg0) {
	btTransform* pArg0 = reinterpret_cast<btTransform*>(arg0);

	btDefaultMotionState* newObj = new btDefaultMotionState(*pArg0);
	return reinterpret_cast<btDefaultMotionStateAddr>(newObj);
}

btDefaultMotionStateAddr btDefaultMotionState_btDefaultMotionState_2(btTransformAddr arg0, btTransformAddr arg1) {
	btTransform* pArg0 = reinterpret_cast<btTransform*>(arg0);
btTransform* pArg1 = reinterpret_cast<btTransform*>(arg1);

	btDefaultMotionState* newObj = new btDefaultMotionState(*pArg0, *pArg1);
	return reinterpret_cast<btDefaultMotionStateAddr>(newObj);
}

void btDefaultMotionState_getWorldTransform_1(btDefaultMotionStateAddr self, btTransformAddr arg0) {
	btTransform* pArg0 = reinterpret_cast<btTransform*>(arg0);

	reinterpret_cast<btDefaultMotionState*>(self)->getWorldTransform(*pArg0);
}

void btDefaultMotionState_setWorldTransform_1(btDefaultMotionStateAddr self, btTransformAddr arg0) {
	btTransform* pArg0 = reinterpret_cast<btTransform*>(arg0);

	reinterpret_cast<btDefaultMotionState*>(self)->setWorldTransform(*pArg0);
}

btTransformAddr btDefaultMotionState_get_m_graphicsWorldTrans_0(btDefaultMotionStateAddr self) {
	btTransform* ret = &reinterpret_cast<btDefaultMotionState*>(self)->m_graphicsWorldTrans;
	return reinterpret_cast<btTransformAddr>(ret);
}

void btDefaultMotionState_set_m_graphicsWorldTrans_1(btDefaultMotionStateAddr self, btTransformAddr arg0) {
	btTransform* pArg0 = reinterpret_cast<btTransform*>(arg0);

	reinterpret_cast<btDefaultMotionState*>(self)->m_graphicsWorldTrans = *pArg0;
}

void btDefaultMotionState___destroy___0(btDefaultMotionStateAddr self) {
	delete reinterpret_cast<btDefaultMotionState*>(self);
}

// btWheelInfo

btWheelInfoAddr btWheelInfo_btWheelInfo_1(btWheelInfoConstructionInfoAddr arg0) {
	btWheelInfoConstructionInfo* pArg0 = reinterpret_cast<btWheelInfoConstructionInfo*>(arg0);

	btWheelInfo* newObj = new btWheelInfo(*pArg0);
	return reinterpret_cast<btWheelInfoAddr>(newObj);
}

double btWheelInfo_getSuspensionRestLength_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->getSuspensionRestLength();
	return ret;
}

void btWheelInfo_updateWheel_2(btWheelInfoAddr self, btRigidBodyAddr arg0, RaycastInfoAddr arg1) {
	const btRigidBody* pArg0 = reinterpret_cast<const btRigidBody*>(arg0);
btWheelInfo::RaycastInfo* pArg1 = reinterpret_cast<btWheelInfo::RaycastInfo*>(arg1);

	reinterpret_cast<btWheelInfo*>(self)->updateWheel(*pArg0, *pArg1);
}

double btWheelInfo_get_m_suspensionStiffness_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_suspensionStiffness;
	return ret;
}

void btWheelInfo_set_m_suspensionStiffness_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_suspensionStiffness = arg0;
}

double btWheelInfo_get_m_frictionSlip_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_frictionSlip;
	return ret;
}

void btWheelInfo_set_m_frictionSlip_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_frictionSlip = arg0;
}

double btWheelInfo_get_m_engineForce_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_engineForce;
	return ret;
}

void btWheelInfo_set_m_engineForce_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_engineForce = arg0;
}

double btWheelInfo_get_m_rollInfluence_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_rollInfluence;
	return ret;
}

void btWheelInfo_set_m_rollInfluence_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_rollInfluence = arg0;
}

double btWheelInfo_get_m_suspensionRestLength1_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_suspensionRestLength1;
	return ret;
}

void btWheelInfo_set_m_suspensionRestLength1_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_suspensionRestLength1 = arg0;
}

double btWheelInfo_get_m_wheelsRadius_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_wheelsRadius;
	return ret;
}

void btWheelInfo_set_m_wheelsRadius_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_wheelsRadius = arg0;
}

double btWheelInfo_get_m_wheelsDampingCompression_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_wheelsDampingCompression;
	return ret;
}

void btWheelInfo_set_m_wheelsDampingCompression_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_wheelsDampingCompression = arg0;
}

double btWheelInfo_get_m_wheelsDampingRelaxation_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_wheelsDampingRelaxation;
	return ret;
}

void btWheelInfo_set_m_wheelsDampingRelaxation_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_wheelsDampingRelaxation = arg0;
}

double btWheelInfo_get_m_steering_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_steering;
	return ret;
}

void btWheelInfo_set_m_steering_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_steering = arg0;
}

double btWheelInfo_get_m_maxSuspensionForce_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_maxSuspensionForce;
	return ret;
}

void btWheelInfo_set_m_maxSuspensionForce_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_maxSuspensionForce = arg0;
}

double btWheelInfo_get_m_maxSuspensionTravelCm_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_maxSuspensionTravelCm;
	return ret;
}

void btWheelInfo_set_m_maxSuspensionTravelCm_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_maxSuspensionTravelCm = arg0;
}

double btWheelInfo_get_m_wheelsSuspensionForce_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_wheelsSuspensionForce;
	return ret;
}

void btWheelInfo_set_m_wheelsSuspensionForce_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_wheelsSuspensionForce = arg0;
}

bool btWheelInfo_get_m_bIsFrontWheel_0(btWheelInfoAddr self) {
	bool ret = reinterpret_cast<btWheelInfo*>(self)->m_bIsFrontWheel;
	return ret;
}

void btWheelInfo_set_m_bIsFrontWheel_1(btWheelInfoAddr self, bool arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_bIsFrontWheel = arg0;
}

RaycastInfoAddr btWheelInfo_get_m_raycastInfo_0(btWheelInfoAddr self) {
	btWheelInfo::RaycastInfo* ret = &reinterpret_cast<btWheelInfo*>(self)->m_raycastInfo;
	return reinterpret_cast<RaycastInfoAddr>(ret);
}

void btWheelInfo_set_m_raycastInfo_1(btWheelInfoAddr self, RaycastInfoAddr arg0) {
	btWheelInfo::RaycastInfo* pArg0 = reinterpret_cast<btWheelInfo::RaycastInfo*>(arg0);

	reinterpret_cast<btWheelInfo*>(self)->m_raycastInfo = *pArg0;
}

btVector3Addr btWheelInfo_get_m_chassisConnectionPointCS_0(btWheelInfoAddr self) {
	btVector3* ret = &reinterpret_cast<btWheelInfo*>(self)->m_chassisConnectionPointCS;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btWheelInfo_set_m_chassisConnectionPointCS_1(btWheelInfoAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btWheelInfo*>(self)->m_chassisConnectionPointCS = *pArg0;
}

btTransformAddr btWheelInfo_get_m_worldTransform_0(btWheelInfoAddr self) {
	btTransform* ret = &reinterpret_cast<btWheelInfo*>(self)->m_worldTransform;
	return reinterpret_cast<btTransformAddr>(ret);
}

void btWheelInfo_set_m_worldTransform_1(btWheelInfoAddr self, btTransformAddr arg0) {
	btTransform* pArg0 = reinterpret_cast<btTransform*>(arg0);

	reinterpret_cast<btWheelInfo*>(self)->m_worldTransform = *pArg0;
}

btVector3Addr btWheelInfo_get_m_wheelDirectionCS_0(btWheelInfoAddr self) {
	btVector3* ret = &reinterpret_cast<btWheelInfo*>(self)->m_wheelDirectionCS;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btWheelInfo_set_m_wheelDirectionCS_1(btWheelInfoAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btWheelInfo*>(self)->m_wheelDirectionCS = *pArg0;
}

btVector3Addr btWheelInfo_get_m_wheelAxleCS_0(btWheelInfoAddr self) {
	btVector3* ret = &reinterpret_cast<btWheelInfo*>(self)->m_wheelAxleCS;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btWheelInfo_set_m_wheelAxleCS_1(btWheelInfoAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btWheelInfo*>(self)->m_wheelAxleCS = *pArg0;
}

double btWheelInfo_get_m_rotation_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_rotation;
	return ret;
}

void btWheelInfo_set_m_rotation_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_rotation = arg0;
}

double btWheelInfo_get_m_deltaRotation_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_deltaRotation;
	return ret;
}

void btWheelInfo_set_m_deltaRotation_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_deltaRotation = arg0;
}

double btWheelInfo_get_m_brake_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_brake;
	return ret;
}

void btWheelInfo_set_m_brake_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_brake = arg0;
}

double btWheelInfo_get_m_clippedInvContactDotSuspension_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_clippedInvContactDotSuspension;
	return ret;
}

void btWheelInfo_set_m_clippedInvContactDotSuspension_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_clippedInvContactDotSuspension = arg0;
}

double btWheelInfo_get_m_suspensionRelativeVelocity_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_suspensionRelativeVelocity;
	return ret;
}

void btWheelInfo_set_m_suspensionRelativeVelocity_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_suspensionRelativeVelocity = arg0;
}

double btWheelInfo_get_m_skidInfo_0(btWheelInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo*>(self)->m_skidInfo;
	return ret;
}

void btWheelInfo_set_m_skidInfo_1(btWheelInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo*>(self)->m_skidInfo = arg0;
}

void btWheelInfo___destroy___0(btWheelInfoAddr self) {
	delete reinterpret_cast<btWheelInfo*>(self);
}

// btVector4

btVector4Addr btVector4_btVector4_0() {
	btVector4* newObj = new btVector4();
	return reinterpret_cast<btVector4Addr>(newObj);
}

btVector4Addr btVector4_btVector4_4(double arg0, double arg1, double arg2, double arg3) {
	btVector4* newObj = new btVector4(arg0, arg1, arg2, arg3);
	return reinterpret_cast<btVector4Addr>(newObj);
}

double btVector4_w_0(btVector4Addr self) {
	double ret = reinterpret_cast<btVector4*>(self)->w();
	return ret;
}

void btVector4_setValue_4(btVector4Addr self, double arg0, double arg1, double arg2, double arg3) {
	reinterpret_cast<btVector4*>(self)->setValue(arg0, arg1, arg2, arg3);
}

double btVector4_length_0(btVector4Addr self) {
	double ret = reinterpret_cast<btVector4*>(self)->length();
	return ret;
}

double btVector4_x_0(btVector4Addr self) {
	double ret = reinterpret_cast<btVector4*>(self)->x();
	return ret;
}

double btVector4_y_0(btVector4Addr self) {
	double ret = reinterpret_cast<btVector4*>(self)->y();
	return ret;
}

double btVector4_z_0(btVector4Addr self) {
	double ret = reinterpret_cast<btVector4*>(self)->z();
	return ret;
}

void btVector4_setX_1(btVector4Addr self, double arg0) {
	reinterpret_cast<btVector4*>(self)->setX(arg0);
}

void btVector4_setY_1(btVector4Addr self, double arg0) {
	reinterpret_cast<btVector4*>(self)->setY(arg0);
}

void btVector4_setZ_1(btVector4Addr self, double arg0) {
	reinterpret_cast<btVector4*>(self)->setZ(arg0);
}

void btVector4_normalize_0(btVector4Addr self) {
	reinterpret_cast<btVector4*>(self)->normalize();
}

btVector3Addr btVector4_rotate_2(btVector4Addr self, btVector3Addr arg0, double arg1) {
	static btVector3 temp;
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	temp = reinterpret_cast<btVector4*>(self)->rotate(*pArg0, arg1);
	return reinterpret_cast<btVector3Addr>(&temp);
}

double btVector4_dot_1(btVector4Addr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	double ret = reinterpret_cast<btVector4*>(self)->dot(*pArg0);
	return ret;
}

btVector3Addr btVector4_op_mul_1(btVector4Addr self, double arg0) {
	btVector4* pSelf = reinterpret_cast<btVector4*>(self);
	*pSelf *= arg0;
	return reinterpret_cast<btVector4Addr>(pSelf);
}

btVector3Addr btVector4_op_add_1(btVector4Addr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);
btVector4* pSelf = reinterpret_cast<btVector4*>(self);
	*pSelf += *pArg0;
	return reinterpret_cast<btVector4Addr>(pSelf);
}

btVector3Addr btVector4_op_sub_1(btVector4Addr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);
btVector4* pSelf = reinterpret_cast<btVector4*>(self);
	*pSelf -= *pArg0;
	return reinterpret_cast<btVector4Addr>(pSelf);
}

void btVector4___destroy___0(btVector4Addr self) {
	delete reinterpret_cast<btVector4*>(self);
}

// JsValue

// btDefaultCollisionConstructionInfo

btDefaultCollisionConstructionInfoAddr btDefaultCollisionConstructionInfo_btDefaultCollisionConstructionInfo_0() {
	btDefaultCollisionConstructionInfo* newObj = new btDefaultCollisionConstructionInfo();
	return reinterpret_cast<btDefaultCollisionConstructionInfoAddr>(newObj);
}

void btDefaultCollisionConstructionInfo___destroy___0(btDefaultCollisionConstructionInfoAddr self) {
	delete reinterpret_cast<btDefaultCollisionConstructionInfo*>(self);
}

// btVehicleRaycasterResult

btVector3Addr btVehicleRaycasterResult_get_m_hitPointInWorld_0(btVehicleRaycasterResultAddr self) {
	btVector3* ret = &reinterpret_cast<btDefaultVehicleRaycaster::btVehicleRaycasterResult*>(self)->m_hitPointInWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btVehicleRaycasterResult_set_m_hitPointInWorld_1(btVehicleRaycasterResultAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btDefaultVehicleRaycaster::btVehicleRaycasterResult*>(self)->m_hitPointInWorld = *pArg0;
}

btVector3Addr btVehicleRaycasterResult_get_m_hitNormalInWorld_0(btVehicleRaycasterResultAddr self) {
	btVector3* ret = &reinterpret_cast<btDefaultVehicleRaycaster::btVehicleRaycasterResult*>(self)->m_hitNormalInWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btVehicleRaycasterResult_set_m_hitNormalInWorld_1(btVehicleRaycasterResultAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btDefaultVehicleRaycaster::btVehicleRaycasterResult*>(self)->m_hitNormalInWorld = *pArg0;
}

double btVehicleRaycasterResult_get_m_distFraction_0(btVehicleRaycasterResultAddr self) {
	double ret = reinterpret_cast<btDefaultVehicleRaycaster::btVehicleRaycasterResult*>(self)->m_distFraction;
	return ret;
}

void btVehicleRaycasterResult_set_m_distFraction_1(btVehicleRaycasterResultAddr self, double arg0) {
	reinterpret_cast<btDefaultVehicleRaycaster::btVehicleRaycasterResult*>(self)->m_distFraction = arg0;
}

void btVehicleRaycasterResult___destroy___0(btVehicleRaycasterResultAddr self) {
	delete reinterpret_cast<btDefaultVehicleRaycaster::btVehicleRaycasterResult*>(self);
}

// btConstraintSolver

void btConstraintSolver___destroy___0(btConstraintSolverAddr self) {
	delete reinterpret_cast<btConstraintSolver*>(self);
}

// btRaycastVehicle

btRaycastVehicleAddr btRaycastVehicle_btRaycastVehicle_3(btVehicleTuningAddr arg0, btRigidBodyAddr arg1, btVehicleRaycasterAddr arg2) {
	const btRaycastVehicle::btVehicleTuning* pArg0 = reinterpret_cast<const btRaycastVehicle::btVehicleTuning*>(arg0);
btRigidBody* pArg1 = reinterpret_cast<btRigidBody*>(arg1);
btVehicleRaycaster* pArg2 = reinterpret_cast<btVehicleRaycaster*>(arg2);

	btRaycastVehicle* newObj = new btRaycastVehicle(*pArg0, pArg1, pArg2);
	return reinterpret_cast<btRaycastVehicleAddr>(newObj);
}

void btRaycastVehicle_applyEngineForce_2(btRaycastVehicleAddr self, double arg0, long_t arg1) {
	reinterpret_cast<btRaycastVehicle*>(self)->applyEngineForce(arg0, arg1);
}

void btRaycastVehicle_setSteeringValue_2(btRaycastVehicleAddr self, double arg0, long_t arg1) {
	reinterpret_cast<btRaycastVehicle*>(self)->setSteeringValue(arg0, arg1);
}

btTransformAddr btRaycastVehicle_getWheelTransformWS_1(btRaycastVehicleAddr self, long_t arg0) {
	const btTransform& ret = reinterpret_cast<btRaycastVehicle*>(self)->getWheelTransformWS(arg0);
	return reinterpret_cast<btTransformAddr>(&ret);
}

void btRaycastVehicle_updateWheelTransform_2(btRaycastVehicleAddr self, long_t arg0, bool arg1) {
	reinterpret_cast<btRaycastVehicle*>(self)->updateWheelTransform(arg0, arg1);
}

btWheelInfoAddr btRaycastVehicle_addWheel_7(btRaycastVehicleAddr self, btVector3Addr arg0, btVector3Addr arg1, btVector3Addr arg2, double arg3, double arg4, btVehicleTuningAddr arg5, bool arg6) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);
const btVector3* pArg2 = reinterpret_cast<const btVector3*>(arg2);
const btRaycastVehicle::btVehicleTuning* pArg5 = reinterpret_cast<const btRaycastVehicle::btVehicleTuning*>(arg5);

	btWheelInfo& ret = reinterpret_cast<btRaycastVehicle*>(self)->addWheel(*pArg0, *pArg1, *pArg2, arg3, arg4, *pArg5, arg6);
	return reinterpret_cast<btWheelInfoAddr>(&ret);
}

long_t btRaycastVehicle_getNumWheels_0(btRaycastVehicleAddr self) {
	long_t ret = reinterpret_cast<btRaycastVehicle*>(self)->getNumWheels();
	return ret;
}

btRigidBodyAddr btRaycastVehicle_getRigidBody_0(btRaycastVehicleAddr self) {
	btRigidBody* ret = reinterpret_cast<btRaycastVehicle*>(self)->getRigidBody();
	return reinterpret_cast<btRigidBodyAddr>(ret);
}

btWheelInfoAddr btRaycastVehicle_getWheelInfo_1(btRaycastVehicleAddr self, long_t arg0) {
	btWheelInfo& ret = reinterpret_cast<btRaycastVehicle*>(self)->getWheelInfo(arg0);
	return reinterpret_cast<btWheelInfoAddr>(&ret);
}

void btRaycastVehicle_setBrake_2(btRaycastVehicleAddr self, double arg0, long_t arg1) {
	reinterpret_cast<btRaycastVehicle*>(self)->setBrake(arg0, arg1);
}

void btRaycastVehicle_setCoordinateSystem_3(btRaycastVehicleAddr self, long_t arg0, long_t arg1, long_t arg2) {
	reinterpret_cast<btRaycastVehicle*>(self)->setCoordinateSystem(arg0, arg1, arg2);
}

double btRaycastVehicle_getCurrentSpeedKmHour_0(btRaycastVehicleAddr self) {
	double ret = reinterpret_cast<btRaycastVehicle*>(self)->getCurrentSpeedKmHour();
	return ret;
}

btTransformAddr btRaycastVehicle_getChassisWorldTransform_0(btRaycastVehicleAddr self) {
	const btTransform& ret = reinterpret_cast<btRaycastVehicle*>(self)->getChassisWorldTransform();
	return reinterpret_cast<btTransformAddr>(&ret);
}

double btRaycastVehicle_rayCast_1(btRaycastVehicleAddr self, btWheelInfoAddr arg0) {
	btWheelInfo* pArg0 = reinterpret_cast<btWheelInfo*>(arg0);

	double ret = reinterpret_cast<btRaycastVehicle*>(self)->rayCast(*pArg0);
	return ret;
}

void btRaycastVehicle_updateVehicle_1(btRaycastVehicleAddr self, double arg0) {
	reinterpret_cast<btRaycastVehicle*>(self)->updateVehicle(arg0);
}

void btRaycastVehicle_resetSuspension_0(btRaycastVehicleAddr self) {
	reinterpret_cast<btRaycastVehicle*>(self)->resetSuspension();
}

double btRaycastVehicle_getSteeringValue_1(btRaycastVehicleAddr self, long_t arg0) {
	double ret = reinterpret_cast<btRaycastVehicle*>(self)->getSteeringValue(arg0);
	return ret;
}

void btRaycastVehicle_updateWheelTransformsWS_1(btRaycastVehicleAddr self, btWheelInfoAddr arg0) {
	btWheelInfo* pArg0 = reinterpret_cast<btWheelInfo*>(arg0);

	reinterpret_cast<btRaycastVehicle*>(self)->updateWheelTransformsWS(*pArg0);
}

void btRaycastVehicle_updateWheelTransformsWS_2(btRaycastVehicleAddr self, btWheelInfoAddr arg0, bool arg1) {
	btWheelInfo* pArg0 = reinterpret_cast<btWheelInfo*>(arg0);

	reinterpret_cast<btRaycastVehicle*>(self)->updateWheelTransformsWS(*pArg0, arg1);
}

void btRaycastVehicle_setPitchControl_1(btRaycastVehicleAddr self, double arg0) {
	reinterpret_cast<btRaycastVehicle*>(self)->setPitchControl(arg0);
}

void btRaycastVehicle_updateSuspension_1(btRaycastVehicleAddr self, double arg0) {
	reinterpret_cast<btRaycastVehicle*>(self)->updateSuspension(arg0);
}

void btRaycastVehicle_updateFriction_1(btRaycastVehicleAddr self, double arg0) {
	reinterpret_cast<btRaycastVehicle*>(self)->updateFriction(arg0);
}

long_t btRaycastVehicle_getRightAxis_0(btRaycastVehicleAddr self) {
	long_t ret = reinterpret_cast<btRaycastVehicle*>(self)->getRightAxis();
	return ret;
}

long_t btRaycastVehicle_getUpAxis_0(btRaycastVehicleAddr self) {
	long_t ret = reinterpret_cast<btRaycastVehicle*>(self)->getUpAxis();
	return ret;
}

long_t btRaycastVehicle_getForwardAxis_0(btRaycastVehicleAddr self) {
	long_t ret = reinterpret_cast<btRaycastVehicle*>(self)->getForwardAxis();
	return ret;
}

btVector3Addr btRaycastVehicle_getForwardVector_0(btRaycastVehicleAddr self) {
	static btVector3 temp;
	temp = reinterpret_cast<btRaycastVehicle*>(self)->getForwardVector();
	return reinterpret_cast<btVector3Addr>(&temp);
}

long_t btRaycastVehicle_getUserConstraintType_0(btRaycastVehicleAddr self) {
	long_t ret = reinterpret_cast<btRaycastVehicle*>(self)->getUserConstraintType();
	return ret;
}

void btRaycastVehicle_setUserConstraintType_1(btRaycastVehicleAddr self, long_t arg0) {
	reinterpret_cast<btRaycastVehicle*>(self)->setUserConstraintType(arg0);
}

void btRaycastVehicle_setUserConstraintId_1(btRaycastVehicleAddr self, long_t arg0) {
	reinterpret_cast<btRaycastVehicle*>(self)->setUserConstraintId(arg0);
}

long_t btRaycastVehicle_getUserConstraintId_0(btRaycastVehicleAddr self) {
	long_t ret = reinterpret_cast<btRaycastVehicle*>(self)->getUserConstraintId();
	return ret;
}

void btRaycastVehicle_updateAction_2(btRaycastVehicleAddr self, btCollisionWorldAddr arg0, double arg1) {
	btCollisionWorld* pArg0 = reinterpret_cast<btCollisionWorld*>(arg0);

	reinterpret_cast<btRaycastVehicle*>(self)->updateAction(pArg0, arg1);
}

void btRaycastVehicle___destroy___0(btRaycastVehicleAddr self) {
	delete reinterpret_cast<btRaycastVehicle*>(self);
}

// tVector3Array

long_t tVector3Array_size_0(tVector3ArrayAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::tVector3Array*>(self)->size();
	return ret;
}

btVector3Addr tVector3Array_at_1(tVector3ArrayAddr self, long_t arg0) {
	static btVector3 temp;
	temp = reinterpret_cast<btCollisionWorld::tVector3Array*>(self)->at(arg0);
	return reinterpret_cast<btVector3Addr>(&temp);
}

void tVector3Array_clear_0(tVector3ArrayAddr self) {
	reinterpret_cast<btCollisionWorld::tVector3Array*>(self)->clear();
}

void tVector3Array_push_back_1(tVector3ArrayAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::tVector3Array*>(self)->push_back(*pArg0);
}

void tVector3Array_pop_back_0(tVector3ArrayAddr self) {
	reinterpret_cast<btCollisionWorld::tVector3Array*>(self)->pop_back();
}

void tVector3Array___destroy___0(tVector3ArrayAddr self) {
	delete reinterpret_cast<btCollisionWorld::tVector3Array*>(self);
}

// btCylinderShapeX

btCylinderShapeXAddr btCylinderShapeX_btCylinderShapeX_1(btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	btCylinderShapeX* newObj = new btCylinderShapeX(*pArg0);
	return reinterpret_cast<btCylinderShapeXAddr>(newObj);
}

void btCylinderShapeX_setMargin_1(btCylinderShapeXAddr self, double arg0) {
	reinterpret_cast<btCylinderShapeX*>(self)->setMargin(arg0);
}

double btCylinderShapeX_getMargin_0(btCylinderShapeXAddr self) {
	double ret = reinterpret_cast<btCylinderShapeX*>(self)->getMargin();
	return ret;
}

void btCylinderShapeX_setLocalScaling_1(btCylinderShapeXAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btCylinderShapeX*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btCylinderShapeX_getLocalScaling_0(btCylinderShapeXAddr self) {
	const btVector3& ret = reinterpret_cast<btCylinderShapeX*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btCylinderShapeX_calculateLocalInertia_2(btCylinderShapeXAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btCylinderShapeX*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btCylinderShapeX___destroy___0(btCylinderShapeXAddr self) {
	delete reinterpret_cast<btCylinderShapeX*>(self);
}

// btCylinderShapeZ

btCylinderShapeZAddr btCylinderShapeZ_btCylinderShapeZ_1(btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	btCylinderShapeZ* newObj = new btCylinderShapeZ(*pArg0);
	return reinterpret_cast<btCylinderShapeZAddr>(newObj);
}

void btCylinderShapeZ_setMargin_1(btCylinderShapeZAddr self, double arg0) {
	reinterpret_cast<btCylinderShapeZ*>(self)->setMargin(arg0);
}

double btCylinderShapeZ_getMargin_0(btCylinderShapeZAddr self) {
	double ret = reinterpret_cast<btCylinderShapeZ*>(self)->getMargin();
	return ret;
}

void btCylinderShapeZ_setLocalScaling_1(btCylinderShapeZAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btCylinderShapeZ*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btCylinderShapeZ_getLocalScaling_0(btCylinderShapeZAddr self) {
	const btVector3& ret = reinterpret_cast<btCylinderShapeZ*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btCylinderShapeZ_calculateLocalInertia_2(btCylinderShapeZAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btCylinderShapeZ*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btCylinderShapeZ___destroy___0(btCylinderShapeZAddr self) {
	delete reinterpret_cast<btCylinderShapeZ*>(self);
}

// btSequentialImpulseConstraintSolver

btSequentialImpulseConstraintSolverAddr btSequentialImpulseConstraintSolver_btSequentialImpulseConstraintSolver_0() {
	btSequentialImpulseConstraintSolver* newObj = new btSequentialImpulseConstraintSolver();
	return reinterpret_cast<btSequentialImpulseConstraintSolverAddr>(newObj);
}

void btSequentialImpulseConstraintSolver___destroy___0(btSequentialImpulseConstraintSolverAddr self) {
	delete reinterpret_cast<btSequentialImpulseConstraintSolver*>(self);
}

// RaycastInfo

btVector3Addr RaycastInfo_get_m_contactNormalWS_0(RaycastInfoAddr self) {
	btVector3* ret = &reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_contactNormalWS;
	return reinterpret_cast<btVector3Addr>(ret);
}

void RaycastInfo_set_m_contactNormalWS_1(RaycastInfoAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_contactNormalWS = *pArg0;
}

btVector3Addr RaycastInfo_get_m_contactPointWS_0(RaycastInfoAddr self) {
	btVector3* ret = &reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_contactPointWS;
	return reinterpret_cast<btVector3Addr>(ret);
}

void RaycastInfo_set_m_contactPointWS_1(RaycastInfoAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_contactPointWS = *pArg0;
}

double RaycastInfo_get_m_suspensionLength_0(RaycastInfoAddr self) {
	double ret = reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_suspensionLength;
	return ret;
}

void RaycastInfo_set_m_suspensionLength_1(RaycastInfoAddr self, double arg0) {
	reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_suspensionLength = arg0;
}

btVector3Addr RaycastInfo_get_m_hardPointWS_0(RaycastInfoAddr self) {
	btVector3* ret = &reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_hardPointWS;
	return reinterpret_cast<btVector3Addr>(ret);
}

void RaycastInfo_set_m_hardPointWS_1(RaycastInfoAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_hardPointWS = *pArg0;
}

btVector3Addr RaycastInfo_get_m_wheelDirectionWS_0(RaycastInfoAddr self) {
	btVector3* ret = &reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_wheelDirectionWS;
	return reinterpret_cast<btVector3Addr>(ret);
}

void RaycastInfo_set_m_wheelDirectionWS_1(RaycastInfoAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_wheelDirectionWS = *pArg0;
}

btVector3Addr RaycastInfo_get_m_wheelAxleWS_0(RaycastInfoAddr self) {
	btVector3* ret = &reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_wheelAxleWS;
	return reinterpret_cast<btVector3Addr>(ret);
}

void RaycastInfo_set_m_wheelAxleWS_1(RaycastInfoAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_wheelAxleWS = *pArg0;
}

bool RaycastInfo_get_m_isInContact_0(RaycastInfoAddr self) {
	bool ret = reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_isInContact;
	return ret;
}

void RaycastInfo_set_m_isInContact_1(RaycastInfoAddr self, bool arg0) {
	reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_isInContact = arg0;
}

VoidPtrAddr RaycastInfo_get_m_groundObject_0(RaycastInfoAddr self) {
	void* ret = reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_groundObject;
	return reinterpret_cast<VoidPtrAddr>(ret);
}

void RaycastInfo_set_m_groundObject_1(RaycastInfoAddr self, VoidPtrAddr arg0) {
	void* pArg0 = reinterpret_cast<void*>(arg0);

	reinterpret_cast<btWheelInfo::RaycastInfo*>(self)->m_groundObject = pArg0;
}

void RaycastInfo___destroy___0(RaycastInfoAddr self) {
	delete reinterpret_cast<btWheelInfo::RaycastInfo*>(self);
}

// btHeightfieldTerrainShape

btHeightfieldTerrainShapeAddr btHeightfieldTerrainShape_btHeightfieldTerrainShape_9(long_t arg0, long_t arg1, VoidPtrAddr arg2, double arg3, double arg4, double arg5, long_t arg6, long_t arg7, bool arg8) {
	void* pArg2 = reinterpret_cast<void*>(arg2);

	btHeightfieldTerrainShape* newObj = new btHeightfieldTerrainShape(arg0, arg1, pArg2, arg3, arg4, arg5, arg6, (PHY_ScalarType) arg7, arg8);
	return reinterpret_cast<btHeightfieldTerrainShapeAddr>(newObj);
}

void btHeightfieldTerrainShape_setMargin_1(btHeightfieldTerrainShapeAddr self, double arg0) {
	reinterpret_cast<btHeightfieldTerrainShape*>(self)->setMargin(arg0);
}

double btHeightfieldTerrainShape_getMargin_0(btHeightfieldTerrainShapeAddr self) {
	double ret = reinterpret_cast<btHeightfieldTerrainShape*>(self)->getMargin();
	return ret;
}

void btHeightfieldTerrainShape_setLocalScaling_1(btHeightfieldTerrainShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btHeightfieldTerrainShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btHeightfieldTerrainShape_getLocalScaling_0(btHeightfieldTerrainShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btHeightfieldTerrainShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btHeightfieldTerrainShape_calculateLocalInertia_2(btHeightfieldTerrainShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btHeightfieldTerrainShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btHeightfieldTerrainShape___destroy___0(btHeightfieldTerrainShapeAddr self) {
	delete reinterpret_cast<btHeightfieldTerrainShape*>(self);
}

// btDiscreteDynamicsWorld

btDiscreteDynamicsWorldAddr btDiscreteDynamicsWorld_btDiscreteDynamicsWorld_4(btDispatcherAddr arg0, btBroadphaseInterfaceAddr arg1, btConstraintSolverAddr arg2, btCollisionConfigurationAddr arg3) {
	btDispatcher* pArg0 = reinterpret_cast<btDispatcher*>(arg0);
btBroadphaseInterface* pArg1 = reinterpret_cast<btBroadphaseInterface*>(arg1);
btConstraintSolver* pArg2 = reinterpret_cast<btConstraintSolver*>(arg2);
btCollisionConfiguration* pArg3 = reinterpret_cast<btCollisionConfiguration*>(arg3);

	btDiscreteDynamicsWorld* newObj = new btDiscreteDynamicsWorld(pArg0, pArg1, pArg2, pArg3);
	return reinterpret_cast<btDiscreteDynamicsWorldAddr>(newObj);
}

void btDiscreteDynamicsWorld_setGravity_1(btDiscreteDynamicsWorldAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->setGravity(*pArg0);
}

btVector3Addr btDiscreteDynamicsWorld_getGravity_0(btDiscreteDynamicsWorldAddr self) {
	static btVector3 temp;
	temp = reinterpret_cast<btDiscreteDynamicsWorld*>(self)->getGravity();
	return reinterpret_cast<btVector3Addr>(&temp);
}

void btDiscreteDynamicsWorld_addRigidBody_1(btDiscreteDynamicsWorldAddr self, btRigidBodyAddr arg0) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->addRigidBody(pArg0);
}

void btDiscreteDynamicsWorld_addRigidBody_3(btDiscreteDynamicsWorldAddr self, btRigidBodyAddr arg0, long_t arg1, long_t arg2) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->addRigidBody(pArg0, arg1, arg2);
}

void btDiscreteDynamicsWorld_removeRigidBody_1(btDiscreteDynamicsWorldAddr self, btRigidBodyAddr arg0) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->removeRigidBody(pArg0);
}

void btDiscreteDynamicsWorld_addConstraint_1(btDiscreteDynamicsWorldAddr self, btTypedConstraintAddr arg0) {
	btTypedConstraint* pArg0 = reinterpret_cast<btTypedConstraint*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->addConstraint(pArg0);
}

void btDiscreteDynamicsWorld_addConstraint_2(btDiscreteDynamicsWorldAddr self, btTypedConstraintAddr arg0, bool arg1) {
	btTypedConstraint* pArg0 = reinterpret_cast<btTypedConstraint*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->addConstraint(pArg0, arg1);
}

void btDiscreteDynamicsWorld_removeConstraint_1(btDiscreteDynamicsWorldAddr self, btTypedConstraintAddr arg0) {
	btTypedConstraint* pArg0 = reinterpret_cast<btTypedConstraint*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->removeConstraint(pArg0);
}

long_t btDiscreteDynamicsWorld_stepSimulation_1(btDiscreteDynamicsWorldAddr self, double arg0) {
	long_t ret = reinterpret_cast<btDiscreteDynamicsWorld*>(self)->stepSimulation(arg0);
	return ret;
}

long_t btDiscreteDynamicsWorld_stepSimulation_2(btDiscreteDynamicsWorldAddr self, double arg0, long_t arg1) {
	long_t ret = reinterpret_cast<btDiscreteDynamicsWorld*>(self)->stepSimulation(arg0, arg1);
	return ret;
}

long_t btDiscreteDynamicsWorld_stepSimulation_3(btDiscreteDynamicsWorldAddr self, double arg0, long_t arg1, double arg2) {
	long_t ret = reinterpret_cast<btDiscreteDynamicsWorld*>(self)->stepSimulation(arg0, arg1, arg2);
	return ret;
}

void btDiscreteDynamicsWorld_clearForces_0(btDiscreteDynamicsWorldAddr self) {
	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->clearForces();
}

void btDiscreteDynamicsWorld_setApplySpeculativeContactRestitution_1(btDiscreteDynamicsWorldAddr self, bool arg0) {
	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->setApplySpeculativeContactRestitution(arg0);
}

bool btDiscreteDynamicsWorld_getApplySpeculativeContactRestitution_0(btDiscreteDynamicsWorldAddr self) {
	bool ret = reinterpret_cast<btDiscreteDynamicsWorld*>(self)->getApplySpeculativeContactRestitution();
	return ret;
}

btDispatcherAddr btDiscreteDynamicsWorld_getDispatcher_0(btDiscreteDynamicsWorldAddr self) {
	btDispatcher* ret = reinterpret_cast<btDiscreteDynamicsWorld*>(self)->getDispatcher();
	return reinterpret_cast<btDispatcherAddr>(ret);
}

void btDiscreteDynamicsWorld_rayTest_3(btDiscreteDynamicsWorldAddr self, btVector3Addr arg0, btVector3Addr arg1, RayResultCallbackAddr arg2) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);
btCollisionWorld::RayResultCallback* pArg2 = reinterpret_cast<btCollisionWorld::RayResultCallback*>(arg2);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->rayTest(*pArg0, *pArg1, *pArg2);
}

btOverlappingPairCacheAddr btDiscreteDynamicsWorld_getPairCache_0(btDiscreteDynamicsWorldAddr self) {
	btOverlappingPairCache* ret = reinterpret_cast<btDiscreteDynamicsWorld*>(self)->getPairCache();
	return reinterpret_cast<btOverlappingPairCacheAddr>(ret);
}

btDispatcherInfoAddr btDiscreteDynamicsWorld_getDispatchInfo_0(btDiscreteDynamicsWorldAddr self) {
	btDispatcherInfo& ret = reinterpret_cast<btDiscreteDynamicsWorld*>(self)->getDispatchInfo();
	return reinterpret_cast<btDispatcherInfoAddr>(&ret);
}

void btDiscreteDynamicsWorld_addCollisionObject_1(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->addCollisionObject(pArg0);
}

void btDiscreteDynamicsWorld_addCollisionObject_2(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0, long_t arg1) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->addCollisionObject(pArg0, arg1);
}

void btDiscreteDynamicsWorld_addCollisionObject_3(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0, long_t arg1, long_t arg2) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->addCollisionObject(pArg0, arg1, arg2);
}

void btDiscreteDynamicsWorld_removeCollisionObject_1(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->removeCollisionObject(pArg0);
}

btBroadphaseInterfaceAddr btDiscreteDynamicsWorld_getBroadphase_0(btDiscreteDynamicsWorldAddr self) {
	const btBroadphaseInterface* ret = reinterpret_cast<btDiscreteDynamicsWorld*>(self)->getBroadphase();
	return reinterpret_cast<btBroadphaseInterfaceAddr>(ret);
}

void btDiscreteDynamicsWorld_convexSweepTest_5(btDiscreteDynamicsWorldAddr self, btConvexShapeAddr arg0, btTransformAddr arg1, btTransformAddr arg2, ConvexResultCallbackAddr arg3, double arg4) {
	const btConvexShape* pArg0 = reinterpret_cast<const btConvexShape*>(arg0);
const btTransform* pArg1 = reinterpret_cast<const btTransform*>(arg1);
const btTransform* pArg2 = reinterpret_cast<const btTransform*>(arg2);
btCollisionWorld::ConvexResultCallback* pArg3 = reinterpret_cast<btCollisionWorld::ConvexResultCallback*>(arg3);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->convexSweepTest(pArg0, *pArg1, *pArg2, *pArg3, arg4);
}

void btDiscreteDynamicsWorld_contactPairTest_3(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0, btCollisionObjectAddr arg1, ContactResultCallbackAddr arg2) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);
btCollisionObject* pArg1 = reinterpret_cast<btCollisionObject*>(arg1);
btCollisionWorld::ContactResultCallback* pArg2 = reinterpret_cast<btCollisionWorld::ContactResultCallback*>(arg2);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->contactPairTest(pArg0, pArg1, *pArg2);
}

void btDiscreteDynamicsWorld_contactTest_2(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0, ContactResultCallbackAddr arg1) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);
btCollisionWorld::ContactResultCallback* pArg1 = reinterpret_cast<btCollisionWorld::ContactResultCallback*>(arg1);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->contactTest(pArg0, *pArg1);
}

void btDiscreteDynamicsWorld_updateSingleAabb_1(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->updateSingleAabb(pArg0);
}

void btDiscreteDynamicsWorld_addAction_1(btDiscreteDynamicsWorldAddr self, btActionInterfaceAddr arg0) {
	btActionInterface* pArg0 = reinterpret_cast<btActionInterface*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->addAction(pArg0);
}

void btDiscreteDynamicsWorld_removeAction_1(btDiscreteDynamicsWorldAddr self, btActionInterfaceAddr arg0) {
	btActionInterface* pArg0 = reinterpret_cast<btActionInterface*>(arg0);

	reinterpret_cast<btDiscreteDynamicsWorld*>(self)->removeAction(pArg0);
}

btContactSolverInfoAddr btDiscreteDynamicsWorld_getSolverInfo_0(btDiscreteDynamicsWorldAddr self) {
	btContactSolverInfo& ret = reinterpret_cast<btDiscreteDynamicsWorld*>(self)->getSolverInfo();
	return reinterpret_cast<btContactSolverInfoAddr>(&ret);
}

void btDiscreteDynamicsWorld___destroy___0(btDiscreteDynamicsWorldAddr self) {
	delete reinterpret_cast<btDiscreteDynamicsWorld*>(self);
}

// btGhostPairCallback

btGhostPairCallbackAddr btGhostPairCallback_btGhostPairCallback_0() {
	btGhostPairCallback* newObj = new btGhostPairCallback();
	return reinterpret_cast<btGhostPairCallbackAddr>(newObj);
}

void btGhostPairCallback___destroy___0(btGhostPairCallbackAddr self) {
	delete reinterpret_cast<btGhostPairCallback*>(self);
}

// btOverlappingPairCallback

void btOverlappingPairCallback___destroy___0(btOverlappingPairCallbackAddr self) {
	delete reinterpret_cast<btOverlappingPairCallback*>(self);
}

// btKinematicCharacterController

btKinematicCharacterControllerAddr btKinematicCharacterController_btKinematicCharacterController_4(btPairCachingGhostObjectAddr arg0, btConvexShapeAddr arg1, double arg2, btVector3Addr arg3) {
	btPairCachingGhostObject* pArg0 = reinterpret_cast<btPairCachingGhostObject*>(arg0);
btConvexShape* pArg1 = reinterpret_cast<btConvexShape*>(arg1);
const btVector3* pArg3 = reinterpret_cast<const btVector3*>(arg3);

	btKinematicCharacterController* newObj = new btKinematicCharacterController(pArg0, pArg1, arg2, *pArg3);
	return reinterpret_cast<btKinematicCharacterControllerAddr>(newObj);
}

void btKinematicCharacterController_setUp_1(btKinematicCharacterControllerAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btKinematicCharacterController*>(self)->setUp(*pArg0);
}

void btKinematicCharacterController_setWalkDirection_1(btKinematicCharacterControllerAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btKinematicCharacterController*>(self)->setWalkDirection(*pArg0);
}

void btKinematicCharacterController_setVelocityForTimeInterval_2(btKinematicCharacterControllerAddr self, btVector3Addr arg0, double arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btKinematicCharacterController*>(self)->setVelocityForTimeInterval(*pArg0, arg1);
}

void btKinematicCharacterController_warp_1(btKinematicCharacterControllerAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btKinematicCharacterController*>(self)->warp(*pArg0);
}

void btKinematicCharacterController_preStep_1(btKinematicCharacterControllerAddr self, btCollisionWorldAddr arg0) {
	btCollisionWorld* pArg0 = reinterpret_cast<btCollisionWorld*>(arg0);

	reinterpret_cast<btKinematicCharacterController*>(self)->preStep(pArg0);
}

void btKinematicCharacterController_playerStep_2(btKinematicCharacterControllerAddr self, btCollisionWorldAddr arg0, double arg1) {
	btCollisionWorld* pArg0 = reinterpret_cast<btCollisionWorld*>(arg0);

	reinterpret_cast<btKinematicCharacterController*>(self)->playerStep(pArg0, arg1);
}

void btKinematicCharacterController_setFallSpeed_1(btKinematicCharacterControllerAddr self, double arg0) {
	reinterpret_cast<btKinematicCharacterController*>(self)->setFallSpeed(arg0);
}

void btKinematicCharacterController_setJumpSpeed_1(btKinematicCharacterControllerAddr self, double arg0) {
	reinterpret_cast<btKinematicCharacterController*>(self)->setJumpSpeed(arg0);
}

void btKinematicCharacterController_setMaxJumpHeight_1(btKinematicCharacterControllerAddr self, double arg0) {
	reinterpret_cast<btKinematicCharacterController*>(self)->setMaxJumpHeight(arg0);
}

bool btKinematicCharacterController_canJump_0(btKinematicCharacterControllerAddr self) {
	bool ret = reinterpret_cast<btKinematicCharacterController*>(self)->canJump();
	return ret;
}

void btKinematicCharacterController_jump_1(btKinematicCharacterControllerAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btKinematicCharacterController*>(self)->jump(*pArg0);
}

void btKinematicCharacterController_setGravity_1(btKinematicCharacterControllerAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btKinematicCharacterController*>(self)->setGravity(*pArg0);
}

btVector3Addr btKinematicCharacterController_getGravity_0(btKinematicCharacterControllerAddr self) {
	static btVector3 temp;
	temp = reinterpret_cast<btKinematicCharacterController*>(self)->getGravity();
	return reinterpret_cast<btVector3Addr>(&temp);
}

void btKinematicCharacterController_setMaxSlope_1(btKinematicCharacterControllerAddr self, double arg0) {
	reinterpret_cast<btKinematicCharacterController*>(self)->setMaxSlope(arg0);
}

double btKinematicCharacterController_getMaxSlope_0(btKinematicCharacterControllerAddr self) {
	double ret = reinterpret_cast<btKinematicCharacterController*>(self)->getMaxSlope();
	return ret;
}

btPairCachingGhostObjectAddr btKinematicCharacterController_getGhostObject_0(btKinematicCharacterControllerAddr self) {
	btPairCachingGhostObject* ret = reinterpret_cast<btKinematicCharacterController*>(self)->getGhostObject();
	return reinterpret_cast<btPairCachingGhostObjectAddr>(ret);
}

void btKinematicCharacterController_setUseGhostSweepTest_1(btKinematicCharacterControllerAddr self, bool arg0) {
	reinterpret_cast<btKinematicCharacterController*>(self)->setUseGhostSweepTest(arg0);
}

bool btKinematicCharacterController_onGround_0(btKinematicCharacterControllerAddr self) {
	bool ret = reinterpret_cast<btKinematicCharacterController*>(self)->onGround();
	return ret;
}

void btKinematicCharacterController_setUpInterpolate_1(btKinematicCharacterControllerAddr self, bool arg0) {
	reinterpret_cast<btKinematicCharacterController*>(self)->setUpInterpolate(arg0);
}

void btKinematicCharacterController_updateAction_2(btKinematicCharacterControllerAddr self, btCollisionWorldAddr arg0, double arg1) {
	btCollisionWorld* pArg0 = reinterpret_cast<btCollisionWorld*>(arg0);

	reinterpret_cast<btKinematicCharacterController*>(self)->updateAction(pArg0, arg1);
}

void btKinematicCharacterController___destroy___0(btKinematicCharacterControllerAddr self) {
	delete reinterpret_cast<btKinematicCharacterController*>(self);
}

// btStaticPlaneShape

btStaticPlaneShapeAddr btStaticPlaneShape_btStaticPlaneShape_2(btVector3Addr arg0, double arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	btStaticPlaneShape* newObj = new btStaticPlaneShape(*pArg0, arg1);
	return reinterpret_cast<btStaticPlaneShapeAddr>(newObj);
}

void btStaticPlaneShape_setLocalScaling_1(btStaticPlaneShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btStaticPlaneShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btStaticPlaneShape_getLocalScaling_0(btStaticPlaneShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btStaticPlaneShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btStaticPlaneShape_calculateLocalInertia_2(btStaticPlaneShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btStaticPlaneShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btStaticPlaneShape___destroy___0(btStaticPlaneShapeAddr self) {
	delete reinterpret_cast<btStaticPlaneShape*>(self);
}

// btOverlappingPairCache

void btOverlappingPairCache_setInternalGhostPairCallback_1(btOverlappingPairCacheAddr self, btOverlappingPairCallbackAddr arg0) {
	btOverlappingPairCallback* pArg0 = reinterpret_cast<btOverlappingPairCallback*>(arg0);

	reinterpret_cast<btOverlappingPairCache*>(self)->setInternalGhostPairCallback(pArg0);
}

void btOverlappingPairCache___destroy___0(btOverlappingPairCacheAddr self) {
	delete reinterpret_cast<btOverlappingPairCache*>(self);
}

// tBtCollisionObjectArray

long_t tBtCollisionObjectArray_size_0(tBtCollisionObjectArrayAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::tBtCollisionObjectArray*>(self)->size();
	return ret;
}

btCollisionObjectAddr tBtCollisionObjectArray_at_1(tBtCollisionObjectArrayAddr self, long_t arg0) {
	const btCollisionObject* ret = reinterpret_cast<btCollisionWorld::tBtCollisionObjectArray*>(self)->at(arg0);
	return reinterpret_cast<btCollisionObjectAddr>(ret);
}

void tBtCollisionObjectArray_clear_0(tBtCollisionObjectArrayAddr self) {
	reinterpret_cast<btCollisionWorld::tBtCollisionObjectArray*>(self)->clear();
}

void tBtCollisionObjectArray_push_back_1(tBtCollisionObjectArrayAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btCollisionWorld::tBtCollisionObjectArray*>(self)->push_back(pArg0);
}

void tBtCollisionObjectArray_pop_back_0(tBtCollisionObjectArrayAddr self) {
	reinterpret_cast<btCollisionWorld::tBtCollisionObjectArray*>(self)->pop_back();
}

void tBtCollisionObjectArray___destroy___0(tBtCollisionObjectArrayAddr self) {
	delete reinterpret_cast<btCollisionWorld::tBtCollisionObjectArray*>(self);
}

// btFixedConstraint

btFixedConstraintAddr btFixedConstraint_btFixedConstraint_4(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btRigidBody* pArg1 = reinterpret_cast<btRigidBody*>(arg1);
const btTransform* pArg2 = reinterpret_cast<const btTransform*>(arg2);
const btTransform* pArg3 = reinterpret_cast<const btTransform*>(arg3);

	btFixedConstraint* newObj = new btFixedConstraint(*pArg0, *pArg1, *pArg2, *pArg3);
	return reinterpret_cast<btFixedConstraintAddr>(newObj);
}

void btFixedConstraint_enableFeedback_1(btFixedConstraintAddr self, bool arg0) {
	reinterpret_cast<btFixedConstraint*>(self)->enableFeedback(arg0);
}

double btFixedConstraint_getBreakingImpulseThreshold_0(btFixedConstraintAddr self) {
	double ret = reinterpret_cast<btFixedConstraint*>(self)->getBreakingImpulseThreshold();
	return ret;
}

void btFixedConstraint_setBreakingImpulseThreshold_1(btFixedConstraintAddr self, double arg0) {
	reinterpret_cast<btFixedConstraint*>(self)->setBreakingImpulseThreshold(arg0);
}

double btFixedConstraint_getParam_2(btFixedConstraintAddr self, long_t arg0, long_t arg1) {
	double ret = reinterpret_cast<btFixedConstraint*>(self)->getParam(arg0, arg1);
	return ret;
}

void btFixedConstraint_setParam_3(btFixedConstraintAddr self, long_t arg0, double arg1, long_t arg2) {
	reinterpret_cast<btFixedConstraint*>(self)->setParam(arg0, arg1, arg2);
}

void btFixedConstraint___destroy___0(btFixedConstraintAddr self) {
	delete reinterpret_cast<btFixedConstraint*>(self);
}

// btTransform

btTransformAddr btTransform_btTransform_0() {
	btTransform* newObj = new btTransform();
	return reinterpret_cast<btTransformAddr>(newObj);
}

btTransformAddr btTransform_btTransform_2(btQuaternionAddr arg0, btVector3Addr arg1) {
	btQuaternion* pArg0 = reinterpret_cast<btQuaternion*>(arg0);
btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	btTransform* newObj = new btTransform(*pArg0, *pArg1);
	return reinterpret_cast<btTransformAddr>(newObj);
}

void btTransform_setIdentity_0(btTransformAddr self) {
	reinterpret_cast<btTransform*>(self)->setIdentity();
}

void btTransform_setOrigin_1(btTransformAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btTransform*>(self)->setOrigin(*pArg0);
}

void btTransform_setRotation_1(btTransformAddr self, btQuaternionAddr arg0) {
	btQuaternion* pArg0 = reinterpret_cast<btQuaternion*>(arg0);

	reinterpret_cast<btTransform*>(self)->setRotation(*pArg0);
}

btVector3Addr btTransform_getOrigin_0(btTransformAddr self) {
	btVector3& ret = reinterpret_cast<btTransform*>(self)->getOrigin();
	return reinterpret_cast<btVector3Addr>(&ret);
}

btQuaternionAddr btTransform_getRotation_0(btTransformAddr self) {
	static btQuaternion temp;
	temp = reinterpret_cast<btTransform*>(self)->getRotation();
	return reinterpret_cast<btQuaternionAddr>(&temp);
}

btMatrix3x3Addr btTransform_getBasis_0(btTransformAddr self) {
	btMatrix3x3& ret = reinterpret_cast<btTransform*>(self)->getBasis();
	return reinterpret_cast<btMatrix3x3Addr>(&ret);
}

void btTransform_setFromOpenGLMatrix_1(btTransformAddr self, double* arg0) {
	reinterpret_cast<btTransform*>(self)->setFromOpenGLMatrix(reinterpret_cast<const btScalar*>(arg0));
}

void btTransform___destroy___0(btTransformAddr self) {
	delete reinterpret_cast<btTransform*>(self);
}

// ClosestRayResultCallback

ClosestRayResultCallbackAddr ClosestRayResultCallback_ClosestRayResultCallback_2(btVector3Addr arg0, btVector3Addr arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);

	btCollisionWorld::ClosestRayResultCallback* newObj = new btCollisionWorld::ClosestRayResultCallback(*pArg0, *pArg1);
	return reinterpret_cast<ClosestRayResultCallbackAddr>(newObj);
}

bool ClosestRayResultCallback_hasHit_0(ClosestRayResultCallbackAddr self) {
	bool ret = reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->hasHit();
	return ret;
}

btVector3Addr ClosestRayResultCallback_get_m_rayFromWorld_0(ClosestRayResultCallbackAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_rayFromWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void ClosestRayResultCallback_set_m_rayFromWorld_1(ClosestRayResultCallbackAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_rayFromWorld = *pArg0;
}

btVector3Addr ClosestRayResultCallback_get_m_rayToWorld_0(ClosestRayResultCallbackAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_rayToWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void ClosestRayResultCallback_set_m_rayToWorld_1(ClosestRayResultCallbackAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_rayToWorld = *pArg0;
}

btVector3Addr ClosestRayResultCallback_get_m_hitNormalWorld_0(ClosestRayResultCallbackAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_hitNormalWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void ClosestRayResultCallback_set_m_hitNormalWorld_1(ClosestRayResultCallbackAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_hitNormalWorld = *pArg0;
}

btVector3Addr ClosestRayResultCallback_get_m_hitPointWorld_0(ClosestRayResultCallbackAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_hitPointWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void ClosestRayResultCallback_set_m_hitPointWorld_1(ClosestRayResultCallbackAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_hitPointWorld = *pArg0;
}

long_t ClosestRayResultCallback_get_m_collisionFilterGroup_0(ClosestRayResultCallbackAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_collisionFilterGroup;
	return ret;
}

void ClosestRayResultCallback_set_m_collisionFilterGroup_1(ClosestRayResultCallbackAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_collisionFilterGroup = arg0;
}

long_t ClosestRayResultCallback_get_m_collisionFilterMask_0(ClosestRayResultCallbackAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_collisionFilterMask;
	return ret;
}

void ClosestRayResultCallback_set_m_collisionFilterMask_1(ClosestRayResultCallbackAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_collisionFilterMask = arg0;
}

double ClosestRayResultCallback_get_m_closestHitFraction_0(ClosestRayResultCallbackAddr self) {
	double ret = reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_closestHitFraction;
	return ret;
}

void ClosestRayResultCallback_set_m_closestHitFraction_1(ClosestRayResultCallbackAddr self, double arg0) {
	reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_closestHitFraction = arg0;
}

btCollisionObjectAddr ClosestRayResultCallback_get_m_collisionObject_0(ClosestRayResultCallbackAddr self) {
	const btCollisionObject* ret = reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_collisionObject;
	return reinterpret_cast<btCollisionObjectAddr>(ret);
}

void ClosestRayResultCallback_set_m_collisionObject_1(ClosestRayResultCallbackAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self)->m_collisionObject = pArg0;
}

void ClosestRayResultCallback___destroy___0(ClosestRayResultCallbackAddr self) {
	delete reinterpret_cast<btCollisionWorld::ClosestRayResultCallback*>(self);
}

// AllConvexResultCallback

AllConvexResultCallbackAddr AllConvexResultCallback_AllConvexResultCallback_2(btVector3Addr arg0, btVector3Addr arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);
const btVector3* pArg1 = reinterpret_cast<const btVector3*>(arg1);

	btCollisionWorld::AllConvexResultCallback* newObj = new btCollisionWorld::AllConvexResultCallback(*pArg0, *pArg1);
	return reinterpret_cast<AllConvexResultCallbackAddr>(newObj);
}

bool AllConvexResultCallback_hasHit_0(AllConvexResultCallbackAddr self) {
	bool ret = reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->hasHit();
	return ret;
}

btVector3Addr AllConvexResultCallback_get_m_convexFromWorld_0(AllConvexResultCallbackAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_convexFromWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void AllConvexResultCallback_set_m_convexFromWorld_1(AllConvexResultCallbackAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_convexFromWorld = *pArg0;
}

btVector3Addr AllConvexResultCallback_get_m_convexToWorld_0(AllConvexResultCallbackAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_convexToWorld;
	return reinterpret_cast<btVector3Addr>(ret);
}

void AllConvexResultCallback_set_m_convexToWorld_1(AllConvexResultCallbackAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_convexToWorld = *pArg0;
}

tVector3ArrayAddr AllConvexResultCallback_get_m_hitNormalWorld_0(AllConvexResultCallbackAddr self) {
	btCollisionWorld::tVector3Array* ret = &reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_hitNormalWorld;
	return reinterpret_cast<tVector3ArrayAddr>(ret);
}

void AllConvexResultCallback_set_m_hitNormalWorld_1(AllConvexResultCallbackAddr self, tVector3ArrayAddr arg0) {
	btCollisionWorld::tVector3Array* pArg0 = reinterpret_cast<btCollisionWorld::tVector3Array*>(arg0);

	reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_hitNormalWorld = *pArg0;
}

tVector3ArrayAddr AllConvexResultCallback_get_m_hitPointWorld_0(AllConvexResultCallbackAddr self) {
	btCollisionWorld::tVector3Array* ret = &reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_hitPointWorld;
	return reinterpret_cast<tVector3ArrayAddr>(ret);
}

void AllConvexResultCallback_set_m_hitPointWorld_1(AllConvexResultCallbackAddr self, tVector3ArrayAddr arg0) {
	btCollisionWorld::tVector3Array* pArg0 = reinterpret_cast<btCollisionWorld::tVector3Array*>(arg0);

	reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_hitPointWorld = *pArg0;
}

tScalarArrayAddr AllConvexResultCallback_get_m_hitFractions_0(AllConvexResultCallbackAddr self) {
	btCollisionWorld::tScalarArray* ret = &reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_hitFractions;
	return reinterpret_cast<tScalarArrayAddr>(ret);
}

void AllConvexResultCallback_set_m_hitFractions_1(AllConvexResultCallbackAddr self, tScalarArrayAddr arg0) {
	btCollisionWorld::tScalarArray* pArg0 = reinterpret_cast<btCollisionWorld::tScalarArray*>(arg0);

	reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_hitFractions = *pArg0;
}

tBtCollisionObjectArrayAddr AllConvexResultCallback_get_m_collisionObjects_0(AllConvexResultCallbackAddr self) {
	btCollisionWorld::tBtCollisionObjectArray* ret = &reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_collisionObjects;
	return reinterpret_cast<tBtCollisionObjectArrayAddr>(ret);
}

void AllConvexResultCallback_set_m_collisionObjects_1(AllConvexResultCallbackAddr self, tBtCollisionObjectArrayAddr arg0) {
	btCollisionWorld::tBtCollisionObjectArray* pArg0 = reinterpret_cast<btCollisionWorld::tBtCollisionObjectArray*>(arg0);

	reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_collisionObjects = *pArg0;
}

long_t AllConvexResultCallback_get_m_collisionFilterGroup_0(AllConvexResultCallbackAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_collisionFilterGroup;
	return ret;
}

void AllConvexResultCallback_set_m_collisionFilterGroup_1(AllConvexResultCallbackAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_collisionFilterGroup = arg0;
}

long_t AllConvexResultCallback_get_m_collisionFilterMask_0(AllConvexResultCallbackAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_collisionFilterMask;
	return ret;
}

void AllConvexResultCallback_set_m_collisionFilterMask_1(AllConvexResultCallbackAddr self, long_t arg0) {
	reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_collisionFilterMask = arg0;
}

double AllConvexResultCallback_get_m_closestHitFraction_0(AllConvexResultCallbackAddr self) {
	double ret = reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_closestHitFraction;
	return ret;
}

void AllConvexResultCallback_set_m_closestHitFraction_1(AllConvexResultCallbackAddr self, double arg0) {
	reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self)->m_closestHitFraction = arg0;
}

void AllConvexResultCallback___destroy___0(AllConvexResultCallbackAddr self) {
	delete reinterpret_cast<btCollisionWorld::AllConvexResultCallback*>(self);
}

// ConcreteContactResultCallback

ConcreteContactResultCallbackAddr ConcreteContactResultCallback_ConcreteContactResultCallback_0() {
	ConcreteContactResultCallback* newObj = new ConcreteContactResultCallback();
	return reinterpret_cast<ConcreteContactResultCallbackAddr>(newObj);
}

double ConcreteContactResultCallback_addSingleResult_7(ConcreteContactResultCallbackAddr self, btManifoldPointAddr arg0, btCollisionObjectWrapperAddr arg1, long_t arg2, long_t arg3, btCollisionObjectWrapperAddr arg4, long_t arg5, long_t arg6) {
	btManifoldPoint* pArg0 = reinterpret_cast<btManifoldPoint*>(arg0);
const btCollisionObjectWrapper* pArg1 = reinterpret_cast<const btCollisionObjectWrapper*>(arg1);
const btCollisionObjectWrapper* pArg4 = reinterpret_cast<const btCollisionObjectWrapper*>(arg4);

	double ret = reinterpret_cast<ConcreteContactResultCallback*>(self)->addSingleResult(*pArg0, pArg1, arg2, arg3, pArg4, arg5, arg6);
	return ret;
}

void ConcreteContactResultCallback___destroy___0(ConcreteContactResultCallbackAddr self) {
	delete reinterpret_cast<ConcreteContactResultCallback*>(self);
}

// btBvhTriangleMeshShape

btBvhTriangleMeshShapeAddr btBvhTriangleMeshShape_btBvhTriangleMeshShape_2(btStridingMeshInterfaceAddr arg0, bool arg1) {
	btStridingMeshInterface* pArg0 = reinterpret_cast<btStridingMeshInterface*>(arg0);

	btBvhTriangleMeshShape* newObj = new btBvhTriangleMeshShape(pArg0, arg1);
	return reinterpret_cast<btBvhTriangleMeshShapeAddr>(newObj);
}

btBvhTriangleMeshShapeAddr btBvhTriangleMeshShape_btBvhTriangleMeshShape_3(btStridingMeshInterfaceAddr arg0, bool arg1, bool arg2) {
	btStridingMeshInterface* pArg0 = reinterpret_cast<btStridingMeshInterface*>(arg0);

	btBvhTriangleMeshShape* newObj = new btBvhTriangleMeshShape(pArg0, arg1, arg2);
	return reinterpret_cast<btBvhTriangleMeshShapeAddr>(newObj);
}

void btBvhTriangleMeshShape_setLocalScaling_1(btBvhTriangleMeshShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btBvhTriangleMeshShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btBvhTriangleMeshShape_getLocalScaling_0(btBvhTriangleMeshShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btBvhTriangleMeshShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btBvhTriangleMeshShape_calculateLocalInertia_2(btBvhTriangleMeshShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btBvhTriangleMeshShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btBvhTriangleMeshShape___destroy___0(btBvhTriangleMeshShapeAddr self) {
	delete reinterpret_cast<btBvhTriangleMeshShape*>(self);
}

// btSliderConstraint

btSliderConstraintAddr btSliderConstraint_btSliderConstraint_3(btRigidBodyAddr arg0, btTransformAddr arg1, bool arg2) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
const btTransform* pArg1 = reinterpret_cast<const btTransform*>(arg1);

	btSliderConstraint* newObj = new btSliderConstraint(*pArg0, *pArg1, arg2);
	return reinterpret_cast<btSliderConstraintAddr>(newObj);
}

btSliderConstraintAddr btSliderConstraint_btSliderConstraint_5(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3, bool arg4) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btRigidBody* pArg1 = reinterpret_cast<btRigidBody*>(arg1);
const btTransform* pArg2 = reinterpret_cast<const btTransform*>(arg2);
const btTransform* pArg3 = reinterpret_cast<const btTransform*>(arg3);

	btSliderConstraint* newObj = new btSliderConstraint(*pArg0, *pArg1, *pArg2, *pArg3, arg4);
	return reinterpret_cast<btSliderConstraintAddr>(newObj);
}

void btSliderConstraint_setLowerLinLimit_1(btSliderConstraintAddr self, double arg0) {
	reinterpret_cast<btSliderConstraint*>(self)->setLowerLinLimit(arg0);
}

void btSliderConstraint_setUpperLinLimit_1(btSliderConstraintAddr self, double arg0) {
	reinterpret_cast<btSliderConstraint*>(self)->setUpperLinLimit(arg0);
}

void btSliderConstraint_setLowerAngLimit_1(btSliderConstraintAddr self, double arg0) {
	reinterpret_cast<btSliderConstraint*>(self)->setLowerAngLimit(arg0);
}

void btSliderConstraint_setUpperAngLimit_1(btSliderConstraintAddr self, double arg0) {
	reinterpret_cast<btSliderConstraint*>(self)->setUpperAngLimit(arg0);
}

void btSliderConstraint_enableFeedback_1(btSliderConstraintAddr self, bool arg0) {
	reinterpret_cast<btSliderConstraint*>(self)->enableFeedback(arg0);
}

double btSliderConstraint_getBreakingImpulseThreshold_0(btSliderConstraintAddr self) {
	double ret = reinterpret_cast<btSliderConstraint*>(self)->getBreakingImpulseThreshold();
	return ret;
}

void btSliderConstraint_setBreakingImpulseThreshold_1(btSliderConstraintAddr self, double arg0) {
	reinterpret_cast<btSliderConstraint*>(self)->setBreakingImpulseThreshold(arg0);
}

double btSliderConstraint_getParam_2(btSliderConstraintAddr self, long_t arg0, long_t arg1) {
	double ret = reinterpret_cast<btSliderConstraint*>(self)->getParam(arg0, arg1);
	return ret;
}

void btSliderConstraint_setParam_3(btSliderConstraintAddr self, long_t arg0, double arg1, long_t arg2) {
	reinterpret_cast<btSliderConstraint*>(self)->setParam(arg0, arg1, arg2);
}

void btSliderConstraint___destroy___0(btSliderConstraintAddr self) {
	delete reinterpret_cast<btSliderConstraint*>(self);
}

// btPairCachingGhostObject

btPairCachingGhostObjectAddr btPairCachingGhostObject_btPairCachingGhostObject_0() {
	btPairCachingGhostObject* newObj = new btPairCachingGhostObject();
	return reinterpret_cast<btPairCachingGhostObjectAddr>(newObj);
}

void btPairCachingGhostObject_setAnisotropicFriction_2(btPairCachingGhostObjectAddr self, btVector3Addr arg0, long_t arg1) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btPairCachingGhostObject*>(self)->setAnisotropicFriction(*pArg0, arg1);
}

btCollisionShapeAddr btPairCachingGhostObject_getCollisionShape_0(btPairCachingGhostObjectAddr self) {
	btCollisionShape* ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getCollisionShape();
	return reinterpret_cast<btCollisionShapeAddr>(ret);
}

void btPairCachingGhostObject_setContactProcessingThreshold_1(btPairCachingGhostObjectAddr self, double arg0) {
	reinterpret_cast<btPairCachingGhostObject*>(self)->setContactProcessingThreshold(arg0);
}

void btPairCachingGhostObject_setActivationState_1(btPairCachingGhostObjectAddr self, long_t arg0) {
	reinterpret_cast<btPairCachingGhostObject*>(self)->setActivationState(arg0);
}

void btPairCachingGhostObject_forceActivationState_1(btPairCachingGhostObjectAddr self, long_t arg0) {
	reinterpret_cast<btPairCachingGhostObject*>(self)->forceActivationState(arg0);
}

void btPairCachingGhostObject_activate_0(btPairCachingGhostObjectAddr self) {
	reinterpret_cast<btPairCachingGhostObject*>(self)->activate();
}

void btPairCachingGhostObject_activate_1(btPairCachingGhostObjectAddr self, bool arg0) {
	reinterpret_cast<btPairCachingGhostObject*>(self)->activate(arg0);
}

bool btPairCachingGhostObject_isActive_0(btPairCachingGhostObjectAddr self) {
	bool ret = reinterpret_cast<btPairCachingGhostObject*>(self)->isActive();
	return ret;
}

bool btPairCachingGhostObject_isKinematicObject_0(btPairCachingGhostObjectAddr self) {
	bool ret = reinterpret_cast<btPairCachingGhostObject*>(self)->isKinematicObject();
	return ret;
}

bool btPairCachingGhostObject_isStaticObject_0(btPairCachingGhostObjectAddr self) {
	bool ret = reinterpret_cast<btPairCachingGhostObject*>(self)->isStaticObject();
	return ret;
}

bool btPairCachingGhostObject_isStaticOrKinematicObject_0(btPairCachingGhostObjectAddr self) {
	bool ret = reinterpret_cast<btPairCachingGhostObject*>(self)->isStaticOrKinematicObject();
	return ret;
}

double btPairCachingGhostObject_getRestitution_0(btPairCachingGhostObjectAddr self) {
	double ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getRestitution();
	return ret;
}

void btPairCachingGhostObject_setRestitution_1(btPairCachingGhostObjectAddr self, double arg0) {
	reinterpret_cast<btPairCachingGhostObject*>(self)->setRestitution(arg0);
}

double btPairCachingGhostObject_getFriction_0(btPairCachingGhostObjectAddr self) {
	double ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getFriction();
	return ret;
}

void btPairCachingGhostObject_setFriction_1(btPairCachingGhostObjectAddr self, double arg0) {
	reinterpret_cast<btPairCachingGhostObject*>(self)->setFriction(arg0);
}

double btPairCachingGhostObject_getRollingFriction_0(btPairCachingGhostObjectAddr self) {
	double ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getRollingFriction();
	return ret;
}

void btPairCachingGhostObject_setRollingFriction_1(btPairCachingGhostObjectAddr self, double arg0) {
	reinterpret_cast<btPairCachingGhostObject*>(self)->setRollingFriction(arg0);
}

long_t btPairCachingGhostObject_getCollisionFlags_0(btPairCachingGhostObjectAddr self) {
	long_t ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getCollisionFlags();
	return ret;
}

void btPairCachingGhostObject_setCollisionFlags_1(btPairCachingGhostObjectAddr self, long_t arg0) {
	reinterpret_cast<btPairCachingGhostObject*>(self)->setCollisionFlags(arg0);
}

btTransformAddr btPairCachingGhostObject_getWorldTransform_0(btPairCachingGhostObjectAddr self) {
	btTransform& ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getWorldTransform();
	return reinterpret_cast<btTransformAddr>(&ret);
}

void btPairCachingGhostObject_setWorldTransform_1(btPairCachingGhostObjectAddr self, btTransformAddr arg0) {
	const btTransform* pArg0 = reinterpret_cast<const btTransform*>(arg0);

	reinterpret_cast<btPairCachingGhostObject*>(self)->setWorldTransform(*pArg0);
}

void btPairCachingGhostObject_setCollisionShape_1(btPairCachingGhostObjectAddr self, btCollisionShapeAddr arg0) {
	btCollisionShape* pArg0 = reinterpret_cast<btCollisionShape*>(arg0);

	reinterpret_cast<btPairCachingGhostObject*>(self)->setCollisionShape(pArg0);
}

double btPairCachingGhostObject_getCcdMotionThreshold_0(btPairCachingGhostObjectAddr self) {
	double ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getCcdMotionThreshold();
	return ret;
}

void btPairCachingGhostObject_setCcdMotionThreshold_1(btPairCachingGhostObjectAddr self, double arg0) {
	reinterpret_cast<btPairCachingGhostObject*>(self)->setCcdMotionThreshold(arg0);
}

double btPairCachingGhostObject_getCcdSweptSphereRadius_0(btPairCachingGhostObjectAddr self) {
	double ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getCcdSweptSphereRadius();
	return ret;
}

void btPairCachingGhostObject_setCcdSweptSphereRadius_1(btPairCachingGhostObjectAddr self, double arg0) {
	reinterpret_cast<btPairCachingGhostObject*>(self)->setCcdSweptSphereRadius(arg0);
}

long_t btPairCachingGhostObject_getUserIndex_0(btPairCachingGhostObjectAddr self) {
	long_t ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getUserIndex();
	return ret;
}

void btPairCachingGhostObject_setUserIndex_1(btPairCachingGhostObjectAddr self, long_t arg0) {
	reinterpret_cast<btPairCachingGhostObject*>(self)->setUserIndex(arg0);
}

VoidPtrAddr btPairCachingGhostObject_getUserPointer_0(btPairCachingGhostObjectAddr self) {
	void* ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getUserPointer();
	return reinterpret_cast<VoidPtrAddr>(ret);
}

void btPairCachingGhostObject_setUserPointer_1(btPairCachingGhostObjectAddr self, VoidPtrAddr arg0) {
	void* pArg0 = reinterpret_cast<void*>(arg0);

	reinterpret_cast<btPairCachingGhostObject*>(self)->setUserPointer(pArg0);
}

btVector3Addr btPairCachingGhostObject_getInterpolationAngularVelocity_0(btPairCachingGhostObjectAddr self) {
	const btVector3& ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getInterpolationAngularVelocity();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btPairCachingGhostObject_setInterpolationAngularVelocity_1(btPairCachingGhostObjectAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btPairCachingGhostObject*>(self)->setInterpolationAngularVelocity(*pArg0);
}

btVector3Addr btPairCachingGhostObject_getInterpolationLinearVelocity_0(btPairCachingGhostObjectAddr self) {
	const btVector3& ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getInterpolationLinearVelocity();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btPairCachingGhostObject_setInterpolationLinearVelocity_1(btPairCachingGhostObjectAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btPairCachingGhostObject*>(self)->setInterpolationLinearVelocity(*pArg0);
}

btBroadphaseProxyAddr btPairCachingGhostObject_getBroadphaseHandle_0(btPairCachingGhostObjectAddr self) {
	btBroadphaseProxy* ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getBroadphaseHandle();
	return reinterpret_cast<btBroadphaseProxyAddr>(ret);
}

long_t btPairCachingGhostObject_getActivationState_0(btPairCachingGhostObjectAddr self) {
	long_t ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getActivationState();
	return ret;
}

long_t btPairCachingGhostObject_getNumOverlappingObjects_0(btPairCachingGhostObjectAddr self) {
	long_t ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getNumOverlappingObjects();
	return ret;
}

btCollisionObjectAddr btPairCachingGhostObject_getOverlappingObject_1(btPairCachingGhostObjectAddr self, long_t arg0) {
	btCollisionObject* ret = reinterpret_cast<btPairCachingGhostObject*>(self)->getOverlappingObject(arg0);
	return reinterpret_cast<btCollisionObjectAddr>(ret);
}

void btPairCachingGhostObject___destroy___0(btPairCachingGhostObjectAddr self) {
	delete reinterpret_cast<btPairCachingGhostObject*>(self);
}

// btManifoldPoint

btVector3Addr btManifoldPoint_getPositionWorldOnA_0(btManifoldPointAddr self) {
	const btVector3& ret = reinterpret_cast<btManifoldPoint*>(self)->getPositionWorldOnA();
	return reinterpret_cast<btVector3Addr>(&ret);
}

btVector3Addr btManifoldPoint_getPositionWorldOnB_0(btManifoldPointAddr self) {
	const btVector3& ret = reinterpret_cast<btManifoldPoint*>(self)->getPositionWorldOnB();
	return reinterpret_cast<btVector3Addr>(&ret);
}

double btManifoldPoint_getAppliedImpulse_0(btManifoldPointAddr self) {
	double ret = reinterpret_cast<btManifoldPoint*>(self)->getAppliedImpulse();
	return ret;
}

double btManifoldPoint_getDistance_0(btManifoldPointAddr self) {
	double ret = reinterpret_cast<btManifoldPoint*>(self)->getDistance();
	return ret;
}

btVector3Addr btManifoldPoint_get_m_localPointA_0(btManifoldPointAddr self) {
	btVector3* ret = &reinterpret_cast<btManifoldPoint*>(self)->m_localPointA;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btManifoldPoint_set_m_localPointA_1(btManifoldPointAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btManifoldPoint*>(self)->m_localPointA = *pArg0;
}

btVector3Addr btManifoldPoint_get_m_localPointB_0(btManifoldPointAddr self) {
	btVector3* ret = &reinterpret_cast<btManifoldPoint*>(self)->m_localPointB;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btManifoldPoint_set_m_localPointB_1(btManifoldPointAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btManifoldPoint*>(self)->m_localPointB = *pArg0;
}

btVector3Addr btManifoldPoint_get_m_positionWorldOnB_0(btManifoldPointAddr self) {
	btVector3* ret = &reinterpret_cast<btManifoldPoint*>(self)->m_positionWorldOnB;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btManifoldPoint_set_m_positionWorldOnB_1(btManifoldPointAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btManifoldPoint*>(self)->m_positionWorldOnB = *pArg0;
}

btVector3Addr btManifoldPoint_get_m_positionWorldOnA_0(btManifoldPointAddr self) {
	btVector3* ret = &reinterpret_cast<btManifoldPoint*>(self)->m_positionWorldOnA;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btManifoldPoint_set_m_positionWorldOnA_1(btManifoldPointAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btManifoldPoint*>(self)->m_positionWorldOnA = *pArg0;
}

btVector3Addr btManifoldPoint_get_m_normalWorldOnB_0(btManifoldPointAddr self) {
	btVector3* ret = &reinterpret_cast<btManifoldPoint*>(self)->m_normalWorldOnB;
	return reinterpret_cast<btVector3Addr>(ret);
}

void btManifoldPoint_set_m_normalWorldOnB_1(btManifoldPointAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btManifoldPoint*>(self)->m_normalWorldOnB = *pArg0;
}

void btManifoldPoint___destroy___0(btManifoldPointAddr self) {
	delete reinterpret_cast<btManifoldPoint*>(self);
}

// btPoint2PointConstraint

btPoint2PointConstraintAddr btPoint2PointConstraint_btPoint2PointConstraint_2(btRigidBodyAddr arg0, btVector3Addr arg1) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	btPoint2PointConstraint* newObj = new btPoint2PointConstraint(*pArg0, *pArg1);
	return reinterpret_cast<btPoint2PointConstraintAddr>(newObj);
}

btPoint2PointConstraintAddr btPoint2PointConstraint_btPoint2PointConstraint_4(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btVector3Addr arg2, btVector3Addr arg3) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btRigidBody* pArg1 = reinterpret_cast<btRigidBody*>(arg1);
btVector3* pArg2 = reinterpret_cast<btVector3*>(arg2);
btVector3* pArg3 = reinterpret_cast<btVector3*>(arg3);

	btPoint2PointConstraint* newObj = new btPoint2PointConstraint(*pArg0, *pArg1, *pArg2, *pArg3);
	return reinterpret_cast<btPoint2PointConstraintAddr>(newObj);
}

void btPoint2PointConstraint_setPivotA_1(btPoint2PointConstraintAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btPoint2PointConstraint*>(self)->setPivotA(*pArg0);
}

void btPoint2PointConstraint_setPivotB_1(btPoint2PointConstraintAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btPoint2PointConstraint*>(self)->setPivotB(*pArg0);
}

btVector3Addr btPoint2PointConstraint_getPivotInA_0(btPoint2PointConstraintAddr self) {
	const btVector3& ret = reinterpret_cast<btPoint2PointConstraint*>(self)->getPivotInA();
	return reinterpret_cast<btVector3Addr>(&ret);
}

btVector3Addr btPoint2PointConstraint_getPivotInB_0(btPoint2PointConstraintAddr self) {
	const btVector3& ret = reinterpret_cast<btPoint2PointConstraint*>(self)->getPivotInB();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btPoint2PointConstraint_enableFeedback_1(btPoint2PointConstraintAddr self, bool arg0) {
	reinterpret_cast<btPoint2PointConstraint*>(self)->enableFeedback(arg0);
}

double btPoint2PointConstraint_getBreakingImpulseThreshold_0(btPoint2PointConstraintAddr self) {
	double ret = reinterpret_cast<btPoint2PointConstraint*>(self)->getBreakingImpulseThreshold();
	return ret;
}

void btPoint2PointConstraint_setBreakingImpulseThreshold_1(btPoint2PointConstraintAddr self, double arg0) {
	reinterpret_cast<btPoint2PointConstraint*>(self)->setBreakingImpulseThreshold(arg0);
}

double btPoint2PointConstraint_getParam_2(btPoint2PointConstraintAddr self, long_t arg0, long_t arg1) {
	double ret = reinterpret_cast<btPoint2PointConstraint*>(self)->getParam(arg0, arg1);
	return ret;
}

void btPoint2PointConstraint_setParam_3(btPoint2PointConstraintAddr self, long_t arg0, double arg1, long_t arg2) {
	reinterpret_cast<btPoint2PointConstraint*>(self)->setParam(arg0, arg1, arg2);
}

btConstraintSettingAddr btPoint2PointConstraint_get_m_setting_0(btPoint2PointConstraintAddr self) {
	btConstraintSetting* ret = &reinterpret_cast<btPoint2PointConstraint*>(self)->m_setting;
	return reinterpret_cast<btConstraintSettingAddr>(ret);
}

void btPoint2PointConstraint_set_m_setting_1(btPoint2PointConstraintAddr self, btConstraintSettingAddr arg0) {
	btConstraintSetting* pArg0 = reinterpret_cast<btConstraintSetting*>(arg0);

	reinterpret_cast<btPoint2PointConstraint*>(self)->m_setting = *pArg0;
}

void btPoint2PointConstraint___destroy___0(btPoint2PointConstraintAddr self) {
	delete reinterpret_cast<btPoint2PointConstraint*>(self);
}

// btBroadphaseProxy

long_t btBroadphaseProxy_get_m_collisionFilterGroup_0(btBroadphaseProxyAddr self) {
	long_t ret = reinterpret_cast<btBroadphaseProxy*>(self)->m_collisionFilterGroup;
	return ret;
}

void btBroadphaseProxy_set_m_collisionFilterGroup_1(btBroadphaseProxyAddr self, long_t arg0) {
	reinterpret_cast<btBroadphaseProxy*>(self)->m_collisionFilterGroup = arg0;
}

long_t btBroadphaseProxy_get_m_collisionFilterMask_0(btBroadphaseProxyAddr self) {
	long_t ret = reinterpret_cast<btBroadphaseProxy*>(self)->m_collisionFilterMask;
	return ret;
}

void btBroadphaseProxy_set_m_collisionFilterMask_1(btBroadphaseProxyAddr self, long_t arg0) {
	reinterpret_cast<btBroadphaseProxy*>(self)->m_collisionFilterMask = arg0;
}

void btBroadphaseProxy___destroy___0(btBroadphaseProxyAddr self) {
	delete reinterpret_cast<btBroadphaseProxy*>(self);
}

// btGeneric6DofSpringConstraint

btGeneric6DofSpringConstraintAddr btGeneric6DofSpringConstraint_btGeneric6DofSpringConstraint_3(btRigidBodyAddr arg0, btTransformAddr arg1, bool arg2) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btTransform* pArg1 = reinterpret_cast<btTransform*>(arg1);

	btGeneric6DofSpringConstraint* newObj = new btGeneric6DofSpringConstraint(*pArg0, *pArg1, arg2);
	return reinterpret_cast<btGeneric6DofSpringConstraintAddr>(newObj);
}

btGeneric6DofSpringConstraintAddr btGeneric6DofSpringConstraint_btGeneric6DofSpringConstraint_5(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3, bool arg4) {
	btRigidBody* pArg0 = reinterpret_cast<btRigidBody*>(arg0);
btRigidBody* pArg1 = reinterpret_cast<btRigidBody*>(arg1);
btTransform* pArg2 = reinterpret_cast<btTransform*>(arg2);
btTransform* pArg3 = reinterpret_cast<btTransform*>(arg3);

	btGeneric6DofSpringConstraint* newObj = new btGeneric6DofSpringConstraint(*pArg0, *pArg1, *pArg2, *pArg3, arg4);
	return reinterpret_cast<btGeneric6DofSpringConstraintAddr>(newObj);
}

void btGeneric6DofSpringConstraint_enableSpring_2(btGeneric6DofSpringConstraintAddr self, long_t arg0, bool arg1) {
	reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->enableSpring(arg0, arg1);
}

void btGeneric6DofSpringConstraint_setStiffness_2(btGeneric6DofSpringConstraintAddr self, long_t arg0, double arg1) {
	reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->setStiffness(arg0, arg1);
}

void btGeneric6DofSpringConstraint_setDamping_2(btGeneric6DofSpringConstraintAddr self, long_t arg0, double arg1) {
	reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->setDamping(arg0, arg1);
}

void btGeneric6DofSpringConstraint_setLinearLowerLimit_1(btGeneric6DofSpringConstraintAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->setLinearLowerLimit(*pArg0);
}

void btGeneric6DofSpringConstraint_setLinearUpperLimit_1(btGeneric6DofSpringConstraintAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->setLinearUpperLimit(*pArg0);
}

void btGeneric6DofSpringConstraint_setAngularLowerLimit_1(btGeneric6DofSpringConstraintAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->setAngularLowerLimit(*pArg0);
}

void btGeneric6DofSpringConstraint_setAngularUpperLimit_1(btGeneric6DofSpringConstraintAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->setAngularUpperLimit(*pArg0);
}

btTransformAddr btGeneric6DofSpringConstraint_getFrameOffsetA_0(btGeneric6DofSpringConstraintAddr self) {
	const btTransform& ret = reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->getFrameOffsetA();
	return reinterpret_cast<btTransformAddr>(&ret);
}

void btGeneric6DofSpringConstraint_enableFeedback_1(btGeneric6DofSpringConstraintAddr self, bool arg0) {
	reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->enableFeedback(arg0);
}

double btGeneric6DofSpringConstraint_getBreakingImpulseThreshold_0(btGeneric6DofSpringConstraintAddr self) {
	double ret = reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->getBreakingImpulseThreshold();
	return ret;
}

void btGeneric6DofSpringConstraint_setBreakingImpulseThreshold_1(btGeneric6DofSpringConstraintAddr self, double arg0) {
	reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->setBreakingImpulseThreshold(arg0);
}

double btGeneric6DofSpringConstraint_getParam_2(btGeneric6DofSpringConstraintAddr self, long_t arg0, long_t arg1) {
	double ret = reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->getParam(arg0, arg1);
	return ret;
}

void btGeneric6DofSpringConstraint_setParam_3(btGeneric6DofSpringConstraintAddr self, long_t arg0, double arg1, long_t arg2) {
	reinterpret_cast<btGeneric6DofSpringConstraint*>(self)->setParam(arg0, arg1, arg2);
}

void btGeneric6DofSpringConstraint___destroy___0(btGeneric6DofSpringConstraintAddr self) {
	delete reinterpret_cast<btGeneric6DofSpringConstraint*>(self);
}

// btBoxShape

btBoxShapeAddr btBoxShape_btBoxShape_1(btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	btBoxShape* newObj = new btBoxShape(*pArg0);
	return reinterpret_cast<btBoxShapeAddr>(newObj);
}

void btBoxShape_setMargin_1(btBoxShapeAddr self, double arg0) {
	reinterpret_cast<btBoxShape*>(self)->setMargin(arg0);
}

double btBoxShape_getMargin_0(btBoxShapeAddr self) {
	double ret = reinterpret_cast<btBoxShape*>(self)->getMargin();
	return ret;
}

void btBoxShape_setLocalScaling_1(btBoxShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btBoxShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btBoxShape_getLocalScaling_0(btBoxShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btBoxShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btBoxShape_calculateLocalInertia_2(btBoxShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btBoxShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btBoxShape___destroy___0(btBoxShapeAddr self) {
	delete reinterpret_cast<btBoxShape*>(self);
}

// btCapsuleShapeX

btCapsuleShapeXAddr btCapsuleShapeX_btCapsuleShapeX_2(double arg0, double arg1) {
	btCapsuleShapeX* newObj = new btCapsuleShapeX(arg0, arg1);
	return reinterpret_cast<btCapsuleShapeXAddr>(newObj);
}

void btCapsuleShapeX_setMargin_1(btCapsuleShapeXAddr self, double arg0) {
	reinterpret_cast<btCapsuleShapeX*>(self)->setMargin(arg0);
}

double btCapsuleShapeX_getMargin_0(btCapsuleShapeXAddr self) {
	double ret = reinterpret_cast<btCapsuleShapeX*>(self)->getMargin();
	return ret;
}

long_t btCapsuleShapeX_getUpAxis_0(btCapsuleShapeXAddr self) {
	long_t ret = reinterpret_cast<btCapsuleShapeX*>(self)->getUpAxis();
	return ret;
}

double btCapsuleShapeX_getRadius_0(btCapsuleShapeXAddr self) {
	double ret = reinterpret_cast<btCapsuleShapeX*>(self)->getRadius();
	return ret;
}

double btCapsuleShapeX_getHalfHeight_0(btCapsuleShapeXAddr self) {
	double ret = reinterpret_cast<btCapsuleShapeX*>(self)->getHalfHeight();
	return ret;
}

void btCapsuleShapeX_setLocalScaling_1(btCapsuleShapeXAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btCapsuleShapeX*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btCapsuleShapeX_getLocalScaling_0(btCapsuleShapeXAddr self) {
	const btVector3& ret = reinterpret_cast<btCapsuleShapeX*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btCapsuleShapeX_calculateLocalInertia_2(btCapsuleShapeXAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btCapsuleShapeX*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btCapsuleShapeX___destroy___0(btCapsuleShapeXAddr self) {
	delete reinterpret_cast<btCapsuleShapeX*>(self);
}

// btQuaternion

btQuaternionAddr btQuaternion_btQuaternion_4(double arg0, double arg1, double arg2, double arg3) {
	btQuaternion* newObj = new btQuaternion(arg0, arg1, arg2, arg3);
	return reinterpret_cast<btQuaternionAddr>(newObj);
}

void btQuaternion_setValue_4(btQuaternionAddr self, double arg0, double arg1, double arg2, double arg3) {
	reinterpret_cast<btQuaternion*>(self)->setValue(arg0, arg1, arg2, arg3);
}

void btQuaternion_setEulerZYX_3(btQuaternionAddr self, double arg0, double arg1, double arg2) {
	reinterpret_cast<btQuaternion*>(self)->setEulerZYX(arg0, arg1, arg2);
}

void btQuaternion_setRotation_2(btQuaternionAddr self, btVector3Addr arg0, double arg1) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btQuaternion*>(self)->setRotation(*pArg0, arg1);
}

void btQuaternion_normalize_0(btQuaternionAddr self) {
	reinterpret_cast<btQuaternion*>(self)->normalize();
}

double btQuaternion_length2_0(btQuaternionAddr self) {
	double ret = reinterpret_cast<btQuaternion*>(self)->length2();
	return ret;
}

double btQuaternion_length_0(btQuaternionAddr self) {
	double ret = reinterpret_cast<btQuaternion*>(self)->length();
	return ret;
}

double btQuaternion_dot_1(btQuaternionAddr self, btQuaternionAddr arg0) {
	btQuaternion* pArg0 = reinterpret_cast<btQuaternion*>(arg0);

	double ret = reinterpret_cast<btQuaternion*>(self)->dot(*pArg0);
	return ret;
}

btQuaternionAddr btQuaternion_normalized_0(btQuaternionAddr self) {
	static btQuaternion temp;
	temp = reinterpret_cast<btQuaternion*>(self)->normalized();
	return reinterpret_cast<btQuaternionAddr>(&temp);
}

btVector3Addr btQuaternion_getAxis_0(btQuaternionAddr self) {
	static btVector3 temp;
	temp = reinterpret_cast<btQuaternion*>(self)->getAxis();
	return reinterpret_cast<btVector3Addr>(&temp);
}

btQuaternionAddr btQuaternion_inverse_0(btQuaternionAddr self) {
	static btQuaternion temp;
	temp = reinterpret_cast<btQuaternion*>(self)->inverse();
	return reinterpret_cast<btQuaternionAddr>(&temp);
}

double btQuaternion_getAngle_0(btQuaternionAddr self) {
	double ret = reinterpret_cast<btQuaternion*>(self)->getAngle();
	return ret;
}

double btQuaternion_getAngleShortestPath_0(btQuaternionAddr self) {
	double ret = reinterpret_cast<btQuaternion*>(self)->getAngleShortestPath();
	return ret;
}

double btQuaternion_angle_1(btQuaternionAddr self, btQuaternionAddr arg0) {
	btQuaternion* pArg0 = reinterpret_cast<btQuaternion*>(arg0);

	double ret = reinterpret_cast<btQuaternion*>(self)->angle(*pArg0);
	return ret;
}

double btQuaternion_angleShortestPath_1(btQuaternionAddr self, btQuaternionAddr arg0) {
	btQuaternion* pArg0 = reinterpret_cast<btQuaternion*>(arg0);

	double ret = reinterpret_cast<btQuaternion*>(self)->angleShortestPath(*pArg0);
	return ret;
}

btQuaternionAddr btQuaternion_op_add_1(btQuaternionAddr self, btQuaternionAddr arg0) {
	btQuaternion* pArg0 = reinterpret_cast<btQuaternion*>(arg0);
btQuaternion* pSelf = reinterpret_cast<btQuaternion*>(self);
	*pSelf += *pArg0;
	return reinterpret_cast<btQuaternionAddr>(pSelf);
}

btQuaternionAddr btQuaternion_op_sub_1(btQuaternionAddr self, btQuaternionAddr arg0) {
	btQuaternion* pArg0 = reinterpret_cast<btQuaternion*>(arg0);
btQuaternion* pSelf = reinterpret_cast<btQuaternion*>(self);
	*pSelf -= *pArg0;
	return reinterpret_cast<btQuaternionAddr>(pSelf);
}

btQuaternionAddr btQuaternion_op_mul_1(btQuaternionAddr self, double arg0) {
	btQuaternion* pSelf = reinterpret_cast<btQuaternion*>(self);
	*pSelf *= arg0;
	return reinterpret_cast<btQuaternionAddr>(pSelf);
}

btQuaternionAddr btQuaternion_op_mulq_1(btQuaternionAddr self, btQuaternionAddr arg0) {
	btQuaternion* pArg0 = reinterpret_cast<btQuaternion*>(arg0);
btQuaternion* pSelf = reinterpret_cast<btQuaternion*>(self);
	*pSelf *= *pArg0;
	return reinterpret_cast<btQuaternionAddr>(pSelf);
}

btQuaternionAddr btQuaternion_op_div_1(btQuaternionAddr self, double arg0) {
	btQuaternion* pSelf = reinterpret_cast<btQuaternion*>(self);
	*pSelf /= arg0;
	return reinterpret_cast<btQuaternionAddr>(pSelf);
}

double btQuaternion_x_0(btQuaternionAddr self) {
	double ret = reinterpret_cast<btQuaternion*>(self)->x();
	return ret;
}

double btQuaternion_y_0(btQuaternionAddr self) {
	double ret = reinterpret_cast<btQuaternion*>(self)->y();
	return ret;
}

double btQuaternion_z_0(btQuaternionAddr self) {
	double ret = reinterpret_cast<btQuaternion*>(self)->z();
	return ret;
}

double btQuaternion_w_0(btQuaternionAddr self) {
	double ret = reinterpret_cast<btQuaternion*>(self)->w();
	return ret;
}

void btQuaternion_setX_1(btQuaternionAddr self, double arg0) {
	reinterpret_cast<btQuaternion*>(self)->setX(arg0);
}

void btQuaternion_setY_1(btQuaternionAddr self, double arg0) {
	reinterpret_cast<btQuaternion*>(self)->setY(arg0);
}

void btQuaternion_setZ_1(btQuaternionAddr self, double arg0) {
	reinterpret_cast<btQuaternion*>(self)->setZ(arg0);
}

void btQuaternion_setW_1(btQuaternionAddr self, double arg0) {
	reinterpret_cast<btQuaternion*>(self)->setW(arg0);
}

void btQuaternion___destroy___0(btQuaternionAddr self) {
	delete reinterpret_cast<btQuaternion*>(self);
}

// btCapsuleShapeZ

btCapsuleShapeZAddr btCapsuleShapeZ_btCapsuleShapeZ_2(double arg0, double arg1) {
	btCapsuleShapeZ* newObj = new btCapsuleShapeZ(arg0, arg1);
	return reinterpret_cast<btCapsuleShapeZAddr>(newObj);
}

void btCapsuleShapeZ_setMargin_1(btCapsuleShapeZAddr self, double arg0) {
	reinterpret_cast<btCapsuleShapeZ*>(self)->setMargin(arg0);
}

double btCapsuleShapeZ_getMargin_0(btCapsuleShapeZAddr self) {
	double ret = reinterpret_cast<btCapsuleShapeZ*>(self)->getMargin();
	return ret;
}

long_t btCapsuleShapeZ_getUpAxis_0(btCapsuleShapeZAddr self) {
	long_t ret = reinterpret_cast<btCapsuleShapeZ*>(self)->getUpAxis();
	return ret;
}

double btCapsuleShapeZ_getRadius_0(btCapsuleShapeZAddr self) {
	double ret = reinterpret_cast<btCapsuleShapeZ*>(self)->getRadius();
	return ret;
}

double btCapsuleShapeZ_getHalfHeight_0(btCapsuleShapeZAddr self) {
	double ret = reinterpret_cast<btCapsuleShapeZ*>(self)->getHalfHeight();
	return ret;
}

void btCapsuleShapeZ_setLocalScaling_1(btCapsuleShapeZAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btCapsuleShapeZ*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btCapsuleShapeZ_getLocalScaling_0(btCapsuleShapeZAddr self) {
	const btVector3& ret = reinterpret_cast<btCapsuleShapeZ*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btCapsuleShapeZ_calculateLocalInertia_2(btCapsuleShapeZAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btCapsuleShapeZ*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btCapsuleShapeZ___destroy___0(btCapsuleShapeZAddr self) {
	delete reinterpret_cast<btCapsuleShapeZ*>(self);
}

// btContactSolverInfo

long_t btContactSolverInfo_get_m_solverMode_0(btContactSolverInfoAddr self) {
	long_t ret = reinterpret_cast<btContactSolverInfo*>(self)->m_solverMode;
	return ret;
}

void btContactSolverInfo_set_m_solverMode_1(btContactSolverInfoAddr self, long_t arg0) {
	reinterpret_cast<btContactSolverInfo*>(self)->m_solverMode = arg0;
}

bool btContactSolverInfo_get_m_splitImpulse_0(btContactSolverInfoAddr self) {
	bool ret = reinterpret_cast<btContactSolverInfo*>(self)->m_splitImpulse;
	return ret;
}

void btContactSolverInfo_set_m_splitImpulse_1(btContactSolverInfoAddr self, bool arg0) {
	reinterpret_cast<btContactSolverInfo*>(self)->m_splitImpulse = arg0;
}

double btContactSolverInfo_get_m_splitImpulsePenetrationThreshold_0(btContactSolverInfoAddr self) {
	double ret = reinterpret_cast<btContactSolverInfo*>(self)->m_splitImpulsePenetrationThreshold;
	return ret;
}

void btContactSolverInfo_set_m_splitImpulsePenetrationThreshold_1(btContactSolverInfoAddr self, double arg0) {
	reinterpret_cast<btContactSolverInfo*>(self)->m_splitImpulsePenetrationThreshold = arg0;
}

long_t btContactSolverInfo_get_m_numIterations_0(btContactSolverInfoAddr self) {
	long_t ret = reinterpret_cast<btContactSolverInfo*>(self)->m_numIterations;
	return ret;
}

void btContactSolverInfo_set_m_numIterations_1(btContactSolverInfoAddr self, long_t arg0) {
	reinterpret_cast<btContactSolverInfo*>(self)->m_numIterations = arg0;
}

void btContactSolverInfo___destroy___0(btContactSolverInfoAddr self) {
	delete reinterpret_cast<btContactSolverInfo*>(self);
}

// tScalarArray

long_t tScalarArray_size_0(tScalarArrayAddr self) {
	long_t ret = reinterpret_cast<btCollisionWorld::tScalarArray*>(self)->size();
	return ret;
}

double tScalarArray_at_1(tScalarArrayAddr self, long_t arg0) {
	double ret = reinterpret_cast<btCollisionWorld::tScalarArray*>(self)->at(arg0);
	return ret;
}

void tScalarArray_clear_0(tScalarArrayAddr self) {
	reinterpret_cast<btCollisionWorld::tScalarArray*>(self)->clear();
}

void tScalarArray_push_back_1(tScalarArrayAddr self, double arg0) {
	reinterpret_cast<btCollisionWorld::tScalarArray*>(self)->push_back(arg0);
}

void tScalarArray_pop_back_0(tScalarArrayAddr self) {
	reinterpret_cast<btCollisionWorld::tScalarArray*>(self)->pop_back();
}

void tScalarArray___destroy___0(tScalarArrayAddr self) {
	delete reinterpret_cast<btCollisionWorld::tScalarArray*>(self);
}

// btSphereShape

btSphereShapeAddr btSphereShape_btSphereShape_1(double arg0) {
	btSphereShape* newObj = new btSphereShape(arg0);
	return reinterpret_cast<btSphereShapeAddr>(newObj);
}

void btSphereShape_setMargin_1(btSphereShapeAddr self, double arg0) {
	reinterpret_cast<btSphereShape*>(self)->setMargin(arg0);
}

double btSphereShape_getMargin_0(btSphereShapeAddr self) {
	double ret = reinterpret_cast<btSphereShape*>(self)->getMargin();
	return ret;
}

void btSphereShape_setLocalScaling_1(btSphereShapeAddr self, btVector3Addr arg0) {
	const btVector3* pArg0 = reinterpret_cast<const btVector3*>(arg0);

	reinterpret_cast<btSphereShape*>(self)->setLocalScaling(*pArg0);
}

btVector3Addr btSphereShape_getLocalScaling_0(btSphereShapeAddr self) {
	const btVector3& ret = reinterpret_cast<btSphereShape*>(self)->getLocalScaling();
	return reinterpret_cast<btVector3Addr>(&ret);
}

void btSphereShape_calculateLocalInertia_2(btSphereShapeAddr self, double arg0, btVector3Addr arg1) {
	btVector3* pArg1 = reinterpret_cast<btVector3*>(arg1);

	reinterpret_cast<btSphereShape*>(self)->calculateLocalInertia(arg0, *pArg1);
}

void btSphereShape___destroy___0(btSphereShapeAddr self) {
	delete reinterpret_cast<btSphereShape*>(self);
}

// LocalConvexResult

LocalConvexResultAddr LocalConvexResult_LocalConvexResult_5(btCollisionObjectAddr arg0, LocalShapeInfoAddr arg1, btVector3Addr arg2, btVector3Addr arg3, double arg4) {
	const btCollisionObject* pArg0 = reinterpret_cast<const btCollisionObject*>(arg0);
btCollisionWorld::LocalShapeInfo* pArg1 = reinterpret_cast<btCollisionWorld::LocalShapeInfo*>(arg1);
const btVector3* pArg2 = reinterpret_cast<const btVector3*>(arg2);
const btVector3* pArg3 = reinterpret_cast<const btVector3*>(arg3);

	btCollisionWorld::LocalConvexResult* newObj = new btCollisionWorld::LocalConvexResult(pArg0, pArg1, *pArg2, *pArg3, arg4);
	return reinterpret_cast<LocalConvexResultAddr>(newObj);
}

btCollisionObjectAddr LocalConvexResult_get_m_hitCollisionObject_0(LocalConvexResultAddr self) {
	const btCollisionObject* ret = reinterpret_cast<btCollisionWorld::LocalConvexResult*>(self)->m_hitCollisionObject;
	return reinterpret_cast<btCollisionObjectAddr>(ret);
}

void LocalConvexResult_set_m_hitCollisionObject_1(LocalConvexResultAddr self, btCollisionObjectAddr arg0) {
	btCollisionObject* pArg0 = reinterpret_cast<btCollisionObject*>(arg0);

	reinterpret_cast<btCollisionWorld::LocalConvexResult*>(self)->m_hitCollisionObject = pArg0;
}

LocalShapeInfoAddr LocalConvexResult_get_m_localShapeInfo_0(LocalConvexResultAddr self) {
	btCollisionWorld::LocalShapeInfo* ret = reinterpret_cast<btCollisionWorld::LocalConvexResult*>(self)->m_localShapeInfo;
	return reinterpret_cast<LocalShapeInfoAddr>(ret);
}

void LocalConvexResult_set_m_localShapeInfo_1(LocalConvexResultAddr self, LocalShapeInfoAddr arg0) {
	btCollisionWorld::LocalShapeInfo* pArg0 = reinterpret_cast<btCollisionWorld::LocalShapeInfo*>(arg0);

	reinterpret_cast<btCollisionWorld::LocalConvexResult*>(self)->m_localShapeInfo = pArg0;
}

btVector3Addr LocalConvexResult_get_m_hitNormalLocal_0(LocalConvexResultAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::LocalConvexResult*>(self)->m_hitNormalLocal;
	return reinterpret_cast<btVector3Addr>(ret);
}

void LocalConvexResult_set_m_hitNormalLocal_1(LocalConvexResultAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::LocalConvexResult*>(self)->m_hitNormalLocal = *pArg0;
}

btVector3Addr LocalConvexResult_get_m_hitPointLocal_0(LocalConvexResultAddr self) {
	btVector3* ret = &reinterpret_cast<btCollisionWorld::LocalConvexResult*>(self)->m_hitPointLocal;
	return reinterpret_cast<btVector3Addr>(ret);
}

void LocalConvexResult_set_m_hitPointLocal_1(LocalConvexResultAddr self, btVector3Addr arg0) {
	btVector3* pArg0 = reinterpret_cast<btVector3*>(arg0);

	reinterpret_cast<btCollisionWorld::LocalConvexResult*>(self)->m_hitPointLocal = *pArg0;
}

double LocalConvexResult_get_m_hitFraction_0(LocalConvexResultAddr self) {
	double ret = reinterpret_cast<btCollisionWorld::LocalConvexResult*>(self)->m_hitFraction;
	return ret;
}

void LocalConvexResult_set_m_hitFraction_1(LocalConvexResultAddr self, double arg0) {
	reinterpret_cast<btCollisionWorld::LocalConvexResult*>(self)->m_hitFraction = arg0;
}

void LocalConvexResult___destroy___0(LocalConvexResultAddr self) {
	delete reinterpret_cast<btCollisionWorld::LocalConvexResult*>(self);
}

// btConstraintParams
long_t enum_btConstraintParams_BT_CONSTRAINT_ERP() {
  return BT_CONSTRAINT_ERP;
}
 long_t enum_btConstraintParams_BT_CONSTRAINT_STOP_ERP() {
  return BT_CONSTRAINT_STOP_ERP;
}
 long_t enum_btConstraintParams_BT_CONSTRAINT_CFM() {
  return BT_CONSTRAINT_CFM;
}
 long_t enum_btConstraintParams_BT_CONSTRAINT_STOP_CFM() {
  return BT_CONSTRAINT_STOP_CFM;
}
 
// PHY_ScalarType
long_t enum_PHY_ScalarType_PHY_FLOAT() {
  return PHY_FLOAT;
}
 long_t enum_PHY_ScalarType_PHY_DOUBLE() {
  return PHY_DOUBLE;
}
 long_t enum_PHY_ScalarType_PHY_INTEGER() {
  return PHY_INTEGER;
}
 long_t enum_PHY_ScalarType_PHY_SHORT() {
  return PHY_SHORT;
}
 long_t enum_PHY_ScalarType_PHY_FIXEDPOINT88() {
  return PHY_FIXEDPOINT88;
}
 long_t enum_PHY_ScalarType_PHY_UCHAR() {
  return PHY_UCHAR;
}
 
void btGImpactCollisionAlgorithm_RegisterAlgorithm0(btCollisionDispatcherAddr arg0) {
    btCollisionDispatcher* pArg0 = reinterpret_cast<btCollisionDispatcher*>(arg0);
    btGImpactCollisionAlgorithm::registerAlgorithm(pArg0);
}

}

