#define BULLET_NATIVE
#include <stdio.h>
#include <vector>

#ifdef BULLET_NATIVE
#include "LayaBulletExport.h"
namespace laya
{
#else
typedef int pointer_t;
#include "../bullet/src/btBulletDynamicsCommon.h"
#include "../bullet/src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include "../bullet/src/BulletCollision/Gimpact/btGImpactShape.h"
#include "../bullet/src/BulletCollision/CollisionDispatch/btGhostObject.h"
#include "../bullet/src/BulletDynamics/Character/btKinematicCharacterController.h"
#define WASM_EXP __attribute__((visibility("default")))
#define __BTWASM_SYSCALL_NAME(name) \
	__attribute__((__import_module__("LayaAirInteractive"), __import_name__(#name)))

extern "C"
{
	int __cxa_begin_catch(int a) { return 0; }
	void layaMotionStateGetWorldTransform(pointer_t rigidBodyID, pointer_t worldTrans) __BTWASM_SYSCALL_NAME(getWorldTransform);
	void layaMotionStateSetWorldTransform(pointer_t rigidBodyID, const pointer_t worldTrans) __BTWASM_SYSCALL_NAME(setWorldTransform);
}

class LayaMotionState : public btMotionState
{
public:
	pointer_t rigidBodyID;
	virtual void getWorldTransform(btTransform &centerOfMassWorldTrans)
	{
		layaMotionStateGetWorldTransform(rigidBodyID, (pointer_t)&centerOfMassWorldTrans);
	}
	virtual void setWorldTransform(const btTransform &centerOfMassWorldTrans)
	{
		layaMotionStateSetWorldTransform(rigidBodyID, (pointer_t)&centerOfMassWorldTrans);
	}
};

extern "C"
{
#endif
btVector3 tempbtVector3;
btQuaternion tempbtQuaternion;
void WASM_EXP btGImpactCollisionAlgorithm_RegisterAlgorithm(pointer_t dispatcher)
{
	btGImpactCollisionAlgorithm::registerAlgorithm((btCollisionDispatcher *)dispatcher);
}

//btVector3
pointer_t WASM_EXP btVector3_create(btScalar x, btScalar y, btScalar z)
{
	return (pointer_t) new btVector3(x, y, z);
}

void WASM_EXP btVector3_setValue(pointer_t ptr, btScalar x, btScalar y, btScalar z)
{
	btVector3 *vec3 = (btVector3 *)ptr;
	vec3->setValue(x, y, z);
}

btScalar WASM_EXP btVector3_x(pointer_t ptr)
{
	btVector3 *vec3 = (btVector3 *)ptr;
	return vec3->x();
}

btScalar WASM_EXP btVector3_y(pointer_t ptr)
{
	btVector3 *vec3 = (btVector3 *)ptr;
	return vec3->y();
}

btScalar WASM_EXP btVector3_z(pointer_t ptr)
{
	btVector3 *vec3 = (btVector3 *)ptr;
	return vec3->z();
}

// //btVector4
// int WASM_EXP btVector4_create(float x, float y, float z, float w)
// {
// 	return (int)new btVector4(x, y, z, w);
// }

// void WASM_EXP btVector4_setValue(int ptr, float x, float y, float z, float w)
// {
// 	btVector4 *vec4 = (btVector4 *)ptr;
// 	vec4->setValue(x, y, z, w);
// }

//btQuaternion
pointer_t WASM_EXP btQuaternion_create(btScalar x, btScalar y, btScalar z, btScalar w)
{
	return (pointer_t) new btQuaternion(x, y, z, w);
}

void WASM_EXP btQuaternion_setValue(pointer_t ptr, btScalar x, btScalar y, btScalar z, btScalar w)
{
	btQuaternion *qua = (btQuaternion *)ptr;
	qua->setValue(x, y, z, w);
}

btScalar WASM_EXP btQuaternion_x(pointer_t ptr)
{
	btQuaternion *qua = (btQuaternion *)ptr;
	return qua->x();
}

btScalar WASM_EXP btQuaternion_y(pointer_t ptr)
{
	btQuaternion *qua = (btQuaternion *)ptr;
	return qua->y();
}

btScalar WASM_EXP btQuaternion_z(pointer_t ptr)
{
	btQuaternion *qua = (btQuaternion *)ptr;
	return qua->z();
}

btScalar WASM_EXP btQuaternion_w(pointer_t ptr)
{
	btQuaternion *qua = (btQuaternion *)ptr;
	return qua->w();
}

//btTransform
pointer_t WASM_EXP btTransform_create()
{
	return (pointer_t) new btTransform();
}

void WASM_EXP btTransform_setOrigin(pointer_t ptr, pointer_t origin)
{
	btTransform *trans = (btTransform *)ptr;
	trans->setOrigin(*(btVector3 *)origin);
}

void WASM_EXP btTransform_setRotation(pointer_t ptr, pointer_t q)
{
	btTransform *trans = (btTransform *)ptr;
	trans->setRotation(*(btQuaternion *)q);
}

pointer_t WASM_EXP btTransform_getOrigin(pointer_t ptr)
{
	btTransform *trans = (btTransform *)ptr;
	return (pointer_t)&trans->getOrigin();
}

pointer_t WASM_EXP btTransform_getRotation(pointer_t ptr)
{
	btTransform *trans = (btTransform *)ptr;
	tempbtQuaternion = trans->getRotation();
	return (pointer_t)&tempbtQuaternion;
}

void WASM_EXP btTransform_setIdentity(pointer_t ptr)
{
	btTransform *trans = (btTransform *)ptr;
	trans->setIdentity();
}


void WASM_EXP btTransform_equal(pointer_t ptr,pointer_t other)
{
	btTransform *trans = (btTransform *)ptr;
	btTransform *otherTrans = (btTransform *)other;
	trans=otherTrans;
}



//btMotionState
void WASM_EXP btMotionState_destroy(pointer_t ptr)
{
	btMotionState *state = (btMotionState *)ptr;
	delete state;
}

//LayaMotionState
pointer_t WASM_EXP layaMotionState_create()
{
	return (pointer_t) new LayaMotionState();
}

void WASM_EXP layaMotionState_set_rigidBodyID(pointer_t ptr, int rigidBodyID)
{
	LayaMotionState *state = (LayaMotionState *)ptr;
	state->rigidBodyID = rigidBodyID;
}

//btCollisionObject
pointer_t WASM_EXP btCollisionObject_create()
{
	return (pointer_t) new btCollisionObject();
}

void WASM_EXP btCollisionObject_setContactProcessingThreshold(pointer_t ptr, btScalar contactProcessingThreshold)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->setContactProcessingThreshold(contactProcessingThreshold);
}

void WASM_EXP btCollisionObject_setActivationState(pointer_t ptr, int newState)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->setActivationState(newState);
}

void WASM_EXP btCollisionObject_forceActivationState(pointer_t ptr, int newState)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->forceActivationState(newState);
}

void WASM_EXP btCollisionObject_activate(pointer_t ptr, bool forceActivation)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->activate(forceActivation);
}

bool WASM_EXP btCollisionObject_isActive(pointer_t ptr)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	return colObj->isActive();
}

void WASM_EXP btCollisionObject_setRestitution(pointer_t ptr, btScalar rest)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	return colObj->setRestitution(rest);
}

void WASM_EXP btCollisionObject_setFriction(pointer_t ptr, btScalar frict)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	return colObj->setFriction(frict);
}

void WASM_EXP btCollisionObject_setRollingFriction(pointer_t ptr, btScalar frict)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	return colObj->setRollingFriction(frict);
}

int WASM_EXP btCollisionObject_getCollisionFlags(pointer_t ptr)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	return colObj->getCollisionFlags();
}

void WASM_EXP btCollisionObject_setCollisionFlags(pointer_t ptr, int flags)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->setCollisionFlags(flags);
}

pointer_t WASM_EXP btCollisionObject_getWorldTransform(pointer_t ptr)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	return (pointer_t)&colObj->getWorldTransform();
}

void WASM_EXP btCollisionObject_setWorldTransform(pointer_t ptr,pointer_t worldTrans)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->setWorldTransform(*(btTransform *)worldTrans);
}

void WASM_EXP btCollisionObject_setInterpolationWorldTransform(pointer_t ptr,pointer_t worldTrans)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->setInterpolationWorldTransform(*(btTransform *)worldTrans);
}

void WASM_EXP btCollisionObject_setCollisionShape(pointer_t ptr, pointer_t collisionShape)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->setCollisionShape((btCollisionShape *)collisionShape);
}

btScalar WASM_EXP btCollisionObject_getCcdMotionThreshold(pointer_t ptr)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	return colObj->getCcdMotionThreshold();
}

void WASM_EXP btCollisionObject_setCcdMotionThreshold(pointer_t ptr, btScalar ccdMotionThreshold)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->setCcdMotionThreshold(ccdMotionThreshold);
}

btScalar WASM_EXP btCollisionObject_getCcdSweptSphereRadius(pointer_t ptr)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	return colObj->getCcdSweptSphereRadius();
}

void WASM_EXP btCollisionObject_setCcdSweptSphereRadius(pointer_t ptr, btScalar radius)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->setCcdSweptSphereRadius(radius);
}

int WASM_EXP btCollisionObject_getUserIndex(pointer_t ptr)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	return colObj->getUserIndex();
}

void WASM_EXP btCollisionObject_setUserIndex(pointer_t ptr, int index)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->setUserIndex(index);
}

int WASM_EXP btCollisionObject_getActivationState(pointer_t ptr)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	return colObj->getActivationState();
}

void WASM_EXP btCollisionObject_setInterpolationAngularVelocity(pointer_t ptr, pointer_t angvel)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->setInterpolationAngularVelocity(*(btVector3 *)angvel);
}

void WASM_EXP btCollisionObject_setInterpolationLinearVelocity(pointer_t ptr, pointer_t linvel)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	colObj->setInterpolationLinearVelocity(*(btVector3 *)linvel);
}

void WASM_EXP btCollisionObject_destroy(pointer_t ptr)
{
	btCollisionObject *colObj = (btCollisionObject *)ptr;
	delete colObj;
}
void WASM_EXP RayResultCallback_set_m_flags(pointer_t ptr, int flags)
{
	btCollisionWorld::RayResultCallback *callBack = (btCollisionWorld::RayResultCallback *)ptr;
	callBack->m_flags = flags;
}
bool WASM_EXP RayResultCallback_hasHit(pointer_t ptr)
{
	btCollisionWorld::RayResultCallback *callBack = (btCollisionWorld::RayResultCallback *)ptr;
	return callBack->hasHit();
}

void WASM_EXP RayResultCallback_set_m_collisionFilterGroup(pointer_t ptr, int group)
{
	btCollisionWorld::RayResultCallback *callBack = (btCollisionWorld::RayResultCallback *)ptr;
	callBack->m_collisionFilterGroup = group;
}

void WASM_EXP RayResultCallback_set_m_collisionFilterMask(pointer_t ptr, int mask)
{
	btCollisionWorld::RayResultCallback *callBack = (btCollisionWorld::RayResultCallback *)ptr;
	callBack->m_collisionFilterMask = mask;
}

btScalar WASM_EXP RayResultCallback_get_m_closestHitFraction(pointer_t ptr)
{
	btCollisionWorld::RayResultCallback *callBack = (btCollisionWorld::RayResultCallback *)ptr;
	return callBack->m_closestHitFraction;
}

void WASM_EXP RayResultCallback_set_m_closestHitFraction(pointer_t ptr, btScalar fraction)
{
	btCollisionWorld::RayResultCallback *callBack = (btCollisionWorld::RayResultCallback *)ptr;
	callBack->m_closestHitFraction = fraction;
}

pointer_t WASM_EXP RayResultCallback_get_m_collisionObject(pointer_t ptr)
{
	btCollisionWorld::RayResultCallback *callBack = (btCollisionWorld::RayResultCallback *)ptr;
	return (pointer_t)callBack->m_collisionObject;
}

void WASM_EXP RayResultCallback_set_m_collisionObject(pointer_t ptr, pointer_t collisionObject)
{
	btCollisionWorld::RayResultCallback *callBack = (btCollisionWorld::RayResultCallback *)ptr;
	callBack->m_collisionObject = (btCollisionObject *)collisionObject;
}

//ClosestRayResultCallbackrayToWorld
pointer_t WASM_EXP ClosestRayResultCallback_create(pointer_t rayFromWorld, pointer_t rayToWorld)
{
	return (pointer_t) new btCollisionWorld::ClosestRayResultCallback(*(btVector3 *)rayFromWorld, *(btVector3 *)rayToWorld);
}

pointer_t WASM_EXP ClosestRayResultCallback_get_m_rayFromWorld(pointer_t ptr)
{
	btCollisionWorld::ClosestRayResultCallback *result = (btCollisionWorld::ClosestRayResultCallback *)ptr;
	return (pointer_t)&result->m_rayFromWorld;
}

void WASM_EXP ClosestRayResultCallback_set_m_rayFromWorld(pointer_t ptr, pointer_t rayFromWorld)
{
	btCollisionWorld::ClosestRayResultCallback *result = (btCollisionWorld::ClosestRayResultCallback *)ptr;
	result->m_rayFromWorld = *(btVector3 *)rayFromWorld;
}

pointer_t WASM_EXP ClosestRayResultCallback_get_m_rayToWorld(pointer_t ptr)
{
	btCollisionWorld::ClosestRayResultCallback *result = (btCollisionWorld::ClosestRayResultCallback *)ptr;
	return (pointer_t)&result->m_rayToWorld;
}

void WASM_EXP ClosestRayResultCallback_set_m_rayToWorld(pointer_t ptr, pointer_t rayToWorld)
{
	btCollisionWorld::ClosestRayResultCallback *result = (btCollisionWorld::ClosestRayResultCallback *)ptr;
	result->m_rayToWorld = *(btVector3 *)rayToWorld;
}

pointer_t WASM_EXP ClosestRayResultCallback_get_m_hitNormalWorld(pointer_t ptr)
{
	btCollisionWorld::ClosestRayResultCallback *result = (btCollisionWorld::ClosestRayResultCallback *)ptr;
	return (pointer_t)&result->m_hitNormalWorld;
}

pointer_t WASM_EXP ClosestRayResultCallback_get_m_hitPointWorld(pointer_t ptr)
{
	btCollisionWorld::ClosestRayResultCallback *result = (btCollisionWorld::ClosestRayResultCallback *)ptr;
	return (pointer_t)&result->m_hitPointWorld;
}

//tBtCollisionObjectArray
int WASM_EXP tBtCollisionObjectArray_size(pointer_t ptr)
{
	btCollisionWorld::tBtCollisionObjectArray *colArray = (btCollisionWorld::tBtCollisionObjectArray *)ptr;
	return colArray->size();
}

pointer_t WASM_EXP tBtCollisionObjectArray_at(pointer_t ptr, int n)
{
	btCollisionWorld::tBtCollisionObjectArray *colArray = (btCollisionWorld::tBtCollisionObjectArray *)ptr;
	return (pointer_t)colArray->at(n);
}

void WASM_EXP tBtCollisionObjectArray_clear(pointer_t ptr)
{
	btCollisionWorld::tBtCollisionObjectArray *colArray = (btCollisionWorld::tBtCollisionObjectArray *)ptr;
	colArray->clear();
}

//tVector3Array
pointer_t WASM_EXP tVector3Array_at(pointer_t ptr, int n)
{
	btCollisionWorld::tVector3Array *vec3Array = (btCollisionWorld::tVector3Array *)ptr;
	return (pointer_t)&vec3Array->at(n);
}

void WASM_EXP tVector3Array_clear(pointer_t ptr)
{
	btCollisionWorld::tVector3Array *vec3Array = (btCollisionWorld::tVector3Array *)ptr;
	vec3Array->clear();
}

//tScalarArray
btScalar WASM_EXP tScalarArray_at(pointer_t ptr, int n)
{
	btCollisionWorld::tScalarArray *scalarArray = (btCollisionWorld::tScalarArray *)ptr;
	return scalarArray->at(n);
}

void WASM_EXP tScalarArray_clear(pointer_t ptr)
{
	btCollisionWorld::tScalarArray *scalarArray = (btCollisionWorld::tScalarArray *)ptr;
	scalarArray->clear();
}

//AllHitsRayResultCallback
pointer_t WASM_EXP AllHitsRayResultCallback_create(pointer_t rayFromWorld, pointer_t rayToWorld)
{
	return (pointer_t) new btCollisionWorld::AllHitsRayResultCallback(*(btVector3 *)rayFromWorld, *(btVector3 *)rayToWorld);
}

pointer_t WASM_EXP AllHitsRayResultCallback_get_m_rayFromWorld(pointer_t ptr)
{
	btCollisionWorld::AllHitsRayResultCallback *result = (btCollisionWorld::AllHitsRayResultCallback *)ptr;
	return (pointer_t)&result->m_rayFromWorld;
}

void WASM_EXP AllHitsRayResultCallback_set_m_rayFromWorld(pointer_t ptr, pointer_t rayFromWorld)
{
	btCollisionWorld::AllHitsRayResultCallback *result = (btCollisionWorld::AllHitsRayResultCallback *)ptr;
	result->m_rayFromWorld = *(btVector3 *)rayFromWorld;
}

pointer_t WASM_EXP AllHitsRayResultCallback_get_m_rayToWorld(pointer_t ptr)
{
	btCollisionWorld::AllHitsRayResultCallback *result = (btCollisionWorld::AllHitsRayResultCallback *)ptr;
	return (pointer_t)&result->m_rayToWorld;
}

void WASM_EXP AllHitsRayResultCallback_set_m_rayToWorld(pointer_t ptr, pointer_t rayToWorld)
{
	btCollisionWorld::AllHitsRayResultCallback *result = (btCollisionWorld::AllHitsRayResultCallback *)ptr;
	result->m_rayToWorld = *(btVector3 *)rayToWorld;
}

pointer_t WASM_EXP AllHitsRayResultCallback_get_m_hitPointWorld(pointer_t ptr)
{
	btCollisionWorld::AllHitsRayResultCallback *result = (btCollisionWorld::AllHitsRayResultCallback *)ptr;
	return (pointer_t)&result->m_hitPointWorld;
}

pointer_t WASM_EXP AllHitsRayResultCallback_get_m_hitNormalWorld(pointer_t ptr)
{
	btCollisionWorld::AllHitsRayResultCallback *result = (btCollisionWorld::AllHitsRayResultCallback *)ptr;
	return (pointer_t)&result->m_hitNormalWorld;
}

pointer_t WASM_EXP AllHitsRayResultCallback_get_m_collisionObjects(pointer_t ptr)
{
	btCollisionWorld::AllHitsRayResultCallback *result = (btCollisionWorld::AllHitsRayResultCallback *)ptr;
	return (pointer_t)&result->m_collisionObjects;
}

pointer_t WASM_EXP AllHitsRayResultCallback_get_m_hitFractions(pointer_t ptr)
{
	btCollisionWorld::AllHitsRayResultCallback *result = (btCollisionWorld::AllHitsRayResultCallback *)ptr;
	return (pointer_t)&result->m_hitFractions;
}

//btManifoldPoint
pointer_t WASM_EXP btManifoldPoint_get_m_positionWorldOnA(pointer_t ptr)
{
	btManifoldPoint *point = (btManifoldPoint *)ptr;
	return (pointer_t)&point->m_positionWorldOnA;
}

pointer_t WASM_EXP btManifoldPoint_get_m_positionWorldOnB(pointer_t ptr)
{
	btManifoldPoint *point = (btManifoldPoint *)ptr;
	return (pointer_t)&point->m_positionWorldOnB;
}

pointer_t WASM_EXP btManifoldPoint_get_m_normalWorldOnB(pointer_t ptr)
{
	btManifoldPoint *point = (btManifoldPoint *)ptr;
	return (pointer_t)&point->m_normalWorldOnB;
}

btScalar WASM_EXP btManifoldPoint_getDistance(pointer_t ptr)
{
	btManifoldPoint *point = (btManifoldPoint *)ptr;
	return point->getDistance();
}

//ConvexResultCallback
bool WASM_EXP ConvexResultCallback_hasHit(pointer_t ptr)
{
	btCollisionWorld::ConvexResultCallback *callBack = (btCollisionWorld::ConvexResultCallback *)ptr;
	return callBack->hasHit();
}

void WASM_EXP ConvexResultCallback_set_m_collisionFilterGroup(pointer_t ptr, int group)
{
	btCollisionWorld::ConvexResultCallback *callBack = (btCollisionWorld::ConvexResultCallback *)ptr;
	callBack->m_collisionFilterGroup = group;
}

void WASM_EXP ConvexResultCallback_set_m_collisionFilterMask(pointer_t ptr, int mask)
{
	btCollisionWorld::ConvexResultCallback *callBack = (btCollisionWorld::ConvexResultCallback *)ptr;
	callBack->m_collisionFilterMask = mask;
}

btScalar WASM_EXP ConvexResultCallback_get_m_closestHitFraction(pointer_t ptr)
{
	btCollisionWorld::ConvexResultCallback *callBack = (btCollisionWorld::ConvexResultCallback *)ptr;
	return callBack->m_closestHitFraction;
}

void WASM_EXP ConvexResultCallback_set_m_closestHitFraction(pointer_t ptr, btScalar fraction)
{
	btCollisionWorld::ConvexResultCallback *callBack = (btCollisionWorld::ConvexResultCallback *)ptr;
	callBack->m_closestHitFraction = fraction;
}

//ClosestConvexResultCallback
pointer_t WASM_EXP ClosestConvexResultCallback_create(pointer_t convexFromWorld, pointer_t convexToWorld)
{
	return (pointer_t) new btCollisionWorld::ClosestConvexResultCallback(*(btVector3 *)convexFromWorld, *(btVector3 *)convexToWorld);
}

pointer_t WASM_EXP ClosestConvexResultCallback_get_m_hitNormalWorld(pointer_t ptr)
{
	btCollisionWorld::ClosestConvexResultCallback *result = (btCollisionWorld::ClosestConvexResultCallback *)ptr;
	return (pointer_t)&result->m_hitNormalWorld;
}

pointer_t WASM_EXP ClosestConvexResultCallback_get_m_hitPointWorld(pointer_t ptr)
{
	btCollisionWorld::ClosestConvexResultCallback *result = (btCollisionWorld::ClosestConvexResultCallback *)ptr;
	return (pointer_t)&result->m_hitPointWorld;
}

pointer_t WASM_EXP ClosestConvexResultCallback_get_m_hitCollisionObject(pointer_t ptr)
{
	btCollisionWorld::ClosestConvexResultCallback *callBack = (btCollisionWorld::ClosestConvexResultCallback *)ptr;
	return (pointer_t)callBack->m_hitCollisionObject;
}

void WASM_EXP ClosestConvexResultCallback_set_m_hitCollisionObject(pointer_t ptr, pointer_t hitCollisionObject)
{
	btCollisionWorld::ClosestConvexResultCallback *callBack = (btCollisionWorld::ClosestConvexResultCallback *)ptr;
	callBack->m_hitCollisionObject = (btCollisionObject *)hitCollisionObject;
}

//AllConvexResultCallback
pointer_t WASM_EXP AllConvexResultCallback_create(pointer_t convexFromWorld, pointer_t convexToWorld)
{
	return (pointer_t) new btCollisionWorld::AllConvexResultCallback(*(btVector3 *)convexFromWorld, *(btVector3 *)convexToWorld);
}

pointer_t WASM_EXP AllConvexResultCallback_get_m_hitNormalWorld(pointer_t ptr)
{
	btCollisionWorld::AllConvexResultCallback *result = (btCollisionWorld::AllConvexResultCallback *)ptr;
	return (pointer_t)&result->m_hitNormalWorld;
}

pointer_t WASM_EXP AllConvexResultCallback_get_m_hitPointWorld(pointer_t ptr)
{
	btCollisionWorld::AllConvexResultCallback *result = (btCollisionWorld::AllConvexResultCallback *)ptr;
	return (pointer_t)&result->m_hitPointWorld;
}

pointer_t WASM_EXP AllConvexResultCallback_get_m_hitFractions(pointer_t ptr)
{
	btCollisionWorld::AllConvexResultCallback *result = (btCollisionWorld::AllConvexResultCallback *)ptr;
	return (pointer_t)&result->m_hitFractions;
}

pointer_t WASM_EXP AllConvexResultCallback_get_m_collisionObjects(pointer_t ptr)
{
	btCollisionWorld::AllConvexResultCallback *result = (btCollisionWorld::AllConvexResultCallback *)ptr;
	return (pointer_t)&result->m_collisionObjects;
}

//btCollisionShape
pointer_t WASM_EXP btCollisionShape_getLocalScaling(pointer_t ptr)
{
	btCollisionShape *shape = (btCollisionShape *)ptr;
	return (pointer_t)&shape->getLocalScaling();
}

void WASM_EXP btCollisionShape_setLocalScaling(pointer_t ptr, pointer_t scaling)
{
	btCollisionShape *shape = (btCollisionShape *)ptr;
	shape->setLocalScaling(*(btVector3 *)scaling);
}

void WASM_EXP btCollisionShape_calculateLocalInertia(pointer_t ptr, btScalar mass, pointer_t inertia)
{
	btCollisionShape *shape = (btCollisionShape *)ptr;
	shape->calculateLocalInertia(mass, *(btVector3 *)inertia);
}

void WASM_EXP btCollisionShape_destroy(pointer_t ptr)
{
	btCollisionShape *shape = (btCollisionShape *)ptr;
	delete shape;
}

//btBoxShape
pointer_t WASM_EXP btBoxShape_create(pointer_t boxHalfExtents)
{
	return (pointer_t) new btBoxShape(*(btVector3 *)boxHalfExtents);
}



//btCapsuleShape
pointer_t WASM_EXP btCapsuleShape_create(btScalar radius, btScalar height)
{
	return (pointer_t) new btCapsuleShape(radius, height);
}

//btCapsuleShapeX
pointer_t WASM_EXP btCapsuleShapeX_create(btScalar radius, btScalar height)
{
	return (pointer_t) new btCapsuleShapeX(radius, height);
}

//btCapsuleShapeZ
pointer_t WASM_EXP btCapsuleShapeZ_create(btScalar radius, btScalar height)
{
	return (pointer_t) new btCapsuleShapeZ(radius, height);
}

//btCylinderShape
pointer_t WASM_EXP btCylinderShape_create(pointer_t halfExtents)
{
	return (pointer_t) new btCylinderShape(*(btVector3 *)halfExtents);
}

//btCylinderShapeX
pointer_t WASM_EXP btCylinderShapeX_create(pointer_t halfExtents)
{
	return (pointer_t) new btCylinderShapeX(*(btVector3 *)halfExtents);
}

//btCylinderShapeZ
pointer_t WASM_EXP btCylinderShapeZ_create(pointer_t halfExtents)
{
	return (pointer_t) new btCylinderShapeZ(*(btVector3 *)halfExtents);
}

//btSphereShape
pointer_t WASM_EXP btSphereShape_create(btScalar radius)
{
	return (pointer_t) new btSphereShape(radius);
}

//btConeShape
pointer_t WASM_EXP btConeShape_create(btScalar radius, btScalar height)
{
	return (pointer_t) new btConeShape(radius, height);
}

//btConeShapeX
pointer_t WASM_EXP btConeShapeX_create(btScalar radius, btScalar height)
{
	return (pointer_t) new btConeShapeX(radius, height);
}

//btConeShapeZ
pointer_t WASM_EXP btConeShapeZ_create(btScalar radius, btScalar height)
{
	return (pointer_t) new btConeShapeZ(radius, height);
}

//btStaticPlaneShape
pointer_t WASM_EXP btStaticPlaneShape_create(pointer_t planeNormal, btScalar planeConstant)
{
	return (pointer_t) new btStaticPlaneShape(*(btVector3 *)planeNormal, planeConstant);
}

//btGImpactShapeInterface
void WASM_EXP btGImpactShapeInterface_updateBound(pointer_t ptr)
{
	btGImpactShapeInterface *shape = (btGImpactShapeInterface *)ptr;
	shape->updateBound();
}

//btGImpactMeshShape
pointer_t WASM_EXP btGImpactMeshShape_create(pointer_t meshInterface)
{
	return (pointer_t) new btGImpactMeshShape((btStridingMeshInterface *)meshInterface);
}

//btCompoundShape
pointer_t WASM_EXP btCompoundShape_create()
{
	return (pointer_t) new btCompoundShape();
}

void WASM_EXP btCompoundShape_addChildShape(pointer_t ptr, pointer_t localTransform, pointer_t shape)
{
	btCompoundShape *compShape = (btCompoundShape *)ptr;
	compShape->addChildShape(*(btTransform *)localTransform, (btCollisionShape *)shape);
}

void WASM_EXP btCompoundShape_removeChildShapeByIndex(pointer_t ptr, int childShapeIndex)
{
	btCompoundShape *compShape = (btCompoundShape *)ptr;
	compShape->removeChildShapeByIndex(childShapeIndex);
}

pointer_t WASM_EXP btCompoundShape_getChildShape(pointer_t ptr, int index)
{
	btCompoundShape *compShape = (btCompoundShape *)ptr;
	return (pointer_t)compShape->getChildShape(index);
}

void WASM_EXP btCompoundShape_updateChildTransform(pointer_t ptr, int index, pointer_t newChildTransform, bool shouldRecalculateLocalAabb)
{
	btCompoundShape *compShape = (btCompoundShape *)ptr;
	compShape->updateChildTransform(index, *(btTransform *)newChildTransform, shouldRecalculateLocalAabb);
}

//btStridingMeshInterface
void WASM_EXP btStridingMeshInterface_destroy(pointer_t ptr)
{
	btStridingMeshInterface *mesh = (btStridingMeshInterface *)ptr;
	delete mesh;
}

//btTriangleMesh
pointer_t WASM_EXP btTriangleMesh_create()
{
	return (pointer_t) new btTriangleMesh();
}

void WASM_EXP btTriangleMesh_addTriangle(pointer_t ptr, pointer_t vertex1, pointer_t vertex2, pointer_t vertex3, bool removeDuplicateVertices)
{
	btTriangleMesh *mesh = (btTriangleMesh *)ptr;
	mesh->addTriangle(*(btVector3 *)vertex1, *(btVector3 *)vertex2, *(btVector3 *)vertex3, removeDuplicateVertices);
}

//btDefaultCollisionConfiguration
pointer_t WASM_EXP btDefaultCollisionConfiguration_create()
{
	return (pointer_t) new btDefaultCollisionConfiguration();
}

void WASM_EXP btDefaultCollisionConfiguration_destroy(pointer_t ptr)
{
	btDefaultCollisionConfiguration *config = (btDefaultCollisionConfiguration *)ptr;
	delete config;
}

//btPersistentManifold
pointer_t WASM_EXP btPersistentManifold_getBody0(pointer_t ptr)
{
	btPersistentManifold *manifold = (btPersistentManifold *)ptr;
	return (pointer_t)manifold->getBody0();
}

pointer_t WASM_EXP btPersistentManifold_getBody1(pointer_t ptr)
{
	btPersistentManifold *manifold = (btPersistentManifold *)ptr;
	return (pointer_t)manifold->getBody1();
}

int WASM_EXP btPersistentManifold_getNumContacts(pointer_t ptr)
{
	btPersistentManifold *manifold = (btPersistentManifold *)ptr;
	return manifold->getNumContacts();
}

pointer_t WASM_EXP btPersistentManifold_getContactPoint(pointer_t ptr, int index)
{
	btPersistentManifold *manifold = (btPersistentManifold *)ptr;
	return (pointer_t)&manifold->getContactPoint(index);
}

//btDispatcher
int WASM_EXP btDispatcher_getNumManifolds(pointer_t ptr)
{
	btDispatcher *dispatcher = (btDispatcher *)ptr;
	return dispatcher->getNumManifolds();
}

pointer_t WASM_EXP btDispatcher_getManifoldByIndexInternal(pointer_t ptr, int index)
{
	btDispatcher *dispatcher = (btDispatcher *)ptr;
	return (pointer_t)dispatcher->getManifoldByIndexInternal(index);
}

//btCollisionDispatcher
pointer_t WASM_EXP btCollisionDispatcher_create(pointer_t collisionConfiguration)
{
	return (pointer_t) new btCollisionDispatcher((btDefaultCollisionConfiguration *)collisionConfiguration);
}

void WASM_EXP btCollisionDispatcher_destroy(pointer_t ptr)
{
	btCollisionDispatcher *dispatcher = (btCollisionDispatcher *)ptr;
	delete dispatcher;
}

//btOverlappingPairCache
void WASM_EXP btOverlappingPairCache_setInternalGhostPairCallback(pointer_t ptr, pointer_t ghostPairCallback)
{
	btOverlappingPairCache *pairCache = (btOverlappingPairCache *)ptr;
	btOverlappingPairCallback *callBack = (btOverlappingPairCallback *)ghostPairCallback;
	pairCache->setInternalGhostPairCallback(callBack);
};

//btDbvtBroadphase
pointer_t WASM_EXP btDbvtBroadphase_create()
{
	return (pointer_t) new btDbvtBroadphase();
}

pointer_t WASM_EXP btDbvtBroadphase_getOverlappingPairCache(pointer_t ptr)
{
	btDbvtBroadphase *phase = (btDbvtBroadphase *)ptr;
	return (pointer_t)phase->getOverlappingPairCache();
}

void WASM_EXP btDbvtBroadphase_destroy(pointer_t ptr)
{
	btDbvtBroadphase *phase = (btDbvtBroadphase *)ptr;
	delete phase;
}

//btRigidBodyConstructionInfo
pointer_t WASM_EXP btRigidBodyConstructionInfo_create(btScalar mass, pointer_t motionState, pointer_t collisionShape, pointer_t localInertia)
{
	return (pointer_t) new btRigidBody::btRigidBodyConstructionInfo(mass, (btMotionState *)motionState, (btCollisionShape *)collisionShape, *(btVector3 *)localInertia);
}

void WASM_EXP btRigidBodyConstructionInfo_destroy(pointer_t ptr)
{
	btRigidBody::btRigidBodyConstructionInfo *info = (btRigidBody::btRigidBodyConstructionInfo *)ptr;
	delete info;
}

//btRigidBody
pointer_t WASM_EXP btRigidBody_create(pointer_t constructionInfo)
{
	return (pointer_t) new btRigidBody(*(btRigidBody::btRigidBodyConstructionInfo *)constructionInfo);
}

void WASM_EXP btRigidBody_setCenterOfMassTransform(pointer_t ptr, pointer_t xform)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->setCenterOfMassTransform(*(btTransform *)xform);
}

void WASM_EXP btRigidBody_setSleepingThresholds(pointer_t ptr, btScalar linear, btScalar angular)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->setSleepingThresholds(linear, angular);
}

btScalar WASM_EXP btRigidBody_getLinearSleepingThreshold(pointer_t ptr)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	return rigid->getLinearSleepingThreshold();
}

btScalar WASM_EXP btRigidBody_getAngularSleepingThreshold(pointer_t ptr)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	return rigid->getAngularSleepingThreshold();
}

void WASM_EXP btRigidBody_setDamping(pointer_t ptr, btScalar lin_damping, btScalar ang_damping)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->setDamping(lin_damping, ang_damping);
}

void WASM_EXP btRigidBody_setMassProps(pointer_t ptr, btScalar mass, pointer_t inertia)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->setMassProps(mass, *(btVector3 *)inertia);
}

void WASM_EXP btRigidBody_setLinearFactor(pointer_t ptr, pointer_t linearFactor)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->setLinearFactor(*(btVector3 *)linearFactor);
}

void WASM_EXP btRigidBody_applyTorque(pointer_t ptr, pointer_t torque)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->applyTorque(*(btVector3 *)torque);
}

void WASM_EXP btRigidBody_applyForce(pointer_t ptr, pointer_t force, pointer_t rel_pos)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->applyForce(*(btVector3 *)force, *(btVector3 *)rel_pos);
}

void WASM_EXP btRigidBody_applyCentralForce(pointer_t ptr, pointer_t force)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->applyCentralForce(*(btVector3 *)force);
}

void WASM_EXP btRigidBody_applyTorqueImpulse(pointer_t ptr, pointer_t torque)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->applyTorqueImpulse(*(btVector3 *)torque);
}

void WASM_EXP btRigidBody_applyImpulse(pointer_t ptr, pointer_t impulse, pointer_t rel_pos)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->applyImpulse(*(btVector3 *)impulse, *(btVector3 *)rel_pos);
}

void WASM_EXP btRigidBody_applyCentralImpulse(pointer_t ptr, pointer_t impulse)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->applyCentralImpulse(*(btVector3 *)impulse);
}

void WASM_EXP btRigidBody_updateInertiaTensor(pointer_t ptr)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->updateInertiaTensor();
}

pointer_t WASM_EXP btRigidBody_getLinearVelocity(pointer_t ptr)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	return (pointer_t)&rigid->getLinearVelocity();
}

pointer_t WASM_EXP btRigidBody_getAngularVelocity(pointer_t ptr)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	return (pointer_t)&rigid->getAngularVelocity();
}

void WASM_EXP btRigidBody_setLinearVelocity(pointer_t ptr, pointer_t lin_vel)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->setLinearVelocity(*(btVector3 *)lin_vel);
}

void WASM_EXP btRigidBody_setAngularVelocity(pointer_t ptr, pointer_t ang_vel)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->setAngularVelocity(*(btVector3 *)ang_vel);
}

void WASM_EXP btRigidBody_setAngularFactor(pointer_t ptr, pointer_t angularFactor)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->setAngularFactor(*(btVector3 *)angularFactor);
}

pointer_t WASM_EXP btRigidBody_getGravity(pointer_t ptr)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	return (pointer_t)&rigid->getGravity();
}

void WASM_EXP btRigidBody_setGravity(pointer_t ptr, pointer_t acceleration)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->setGravity(*(btVector3 *)acceleration);
}

pointer_t WASM_EXP btRigidBody_getTotalForce(pointer_t ptr)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	return (pointer_t)&rigid->getTotalForce();
}

pointer_t WASM_EXP btRigidBody_getTotalTorque(pointer_t ptr)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	return (pointer_t)&rigid->getTotalTorque();
}

int WASM_EXP btRigidBody_getFlags(pointer_t ptr)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	return rigid->getFlags();
}

void WASM_EXP btRigidBody_setFlags(pointer_t ptr, int flags)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->setFlags(flags);
}

void WASM_EXP btRigidBody_clearForces(pointer_t ptr)
{
	btRigidBody *rigid = (btRigidBody *)ptr;
	rigid->clearForces();
}

//btSequentialImpulseConstraintSolver
pointer_t WASM_EXP btSequentialImpulseConstraintSolver_create()
{
	return (pointer_t) new btSequentialImpulseConstraintSolver();
}

//btDispatcherInfo
bool WASM_EXP btCollisionWorld_get_m_useContinuous(pointer_t ptr)
{
	btDispatcherInfo *info = (btDispatcherInfo *)ptr;
	return info->m_useContinuous;
}

void WASM_EXP btCollisionWorld_set_m_useContinuous(pointer_t ptr, bool useContinuous)
{
	btDispatcherInfo *info = (btDispatcherInfo *)ptr;
	info->m_useContinuous = useContinuous;
}

//btCollisionWorld
void WASM_EXP btCollisionWorld_rayTest(pointer_t ptr, pointer_t rayFromWorld, pointer_t rayToWorld, pointer_t resultCallback)
{
	btCollisionWorld *world = (btCollisionWorld *)ptr;
	world->rayTest(*(btVector3 *)rayFromWorld, *(btVector3 *)rayToWorld, *(btCollisionWorld::RayResultCallback *)resultCallback);
}

pointer_t WASM_EXP btCollisionWorld_getDispatchInfo(pointer_t ptr)
{
	btCollisionWorld *world = (btCollisionWorld *)ptr;
	return (pointer_t)&world->getDispatchInfo();
}

void WASM_EXP btCollisionWorld_addCollisionObject(pointer_t ptr, pointer_t collisionObject, int collisionFilterGroup, int collisionFilterMask)
{
	btCollisionWorld *world = (btCollisionWorld *)ptr;
	world->addCollisionObject((btCollisionObject *)collisionObject, collisionFilterGroup, collisionFilterMask);
}

void WASM_EXP btCollisionWorld_removeCollisionObject(pointer_t ptr, pointer_t collisionObject)
{
	btCollisionWorld *world = (btCollisionWorld *)ptr;
	world->removeCollisionObject((btCollisionObject *)collisionObject);
}

void WASM_EXP btCollisionWorld_convexSweepTest(pointer_t ptr, pointer_t castShape, pointer_t from, pointer_t to, pointer_t resultCallback, float allowedCcdPenetration)
{
	btCollisionWorld *world = (btCollisionWorld *)ptr;
	world->convexSweepTest((btConvexShape *)castShape, *(btTransform *)from, *(btTransform *)to, *(btCollisionWorld::ConvexResultCallback *)resultCallback, allowedCcdPenetration);
}

void WASM_EXP btCollisionWorld_destroy(pointer_t ptr)
{
	btCollisionWorld *world = (btCollisionWorld *)ptr;
	delete world;
}

//btDynamicsWorld
void WASM_EXP btDynamicsWorld_addAction(pointer_t ptr, pointer_t action)
{
	btDynamicsWorld *world = (btDynamicsWorld *)ptr;
	world->addAction((btActionInterface *)action);
}
void WASM_EXP btDynamicsWorld_removeAction(pointer_t ptr, pointer_t action)
{
	btDynamicsWorld *world = (btDynamicsWorld *)ptr;
	world->removeAction((btActionInterface *)action);
}
pointer_t WASM_EXP btDynamicsWorld_getSolverInfo(pointer_t ptr)
{
	btDynamicsWorld *world = (btDynamicsWorld *)ptr;
	return (pointer_t)&world->getSolverInfo();
}

//btDiscreteDynamicsWorld
pointer_t WASM_EXP btDiscreteDynamicsWorld_create(pointer_t dispatcher, pointer_t pairCache, pointer_t constraintSolver, pointer_t collisionConfiguration)
{
	return (pointer_t) new btDiscreteDynamicsWorld((btDispatcher *)dispatcher, (btBroadphaseInterface *)pairCache, (btConstraintSolver *)constraintSolver, (btCollisionConfiguration *)collisionConfiguration);
}

void WASM_EXP btDiscreteDynamicsWorld_setGravity(pointer_t ptr, pointer_t gravity)
{
	btDiscreteDynamicsWorld *world = (btDiscreteDynamicsWorld *)ptr;
	world->setGravity(*(btVector3 *)gravity);
}

pointer_t WASM_EXP btDiscreteDynamicsWorld_getGravity(pointer_t ptr)
{
	btDiscreteDynamicsWorld *world = (btDiscreteDynamicsWorld *)ptr;
	tempbtVector3 = world->getGravity();
	return (pointer_t)&tempbtVector3;
}

void WASM_EXP btDiscreteDynamicsWorld_addRigidBody(pointer_t ptr, pointer_t body, int group, int mask) //!!!!!!!!!!!!
{
	btDiscreteDynamicsWorld *world = (btDiscreteDynamicsWorld *)ptr;
	return world->addRigidBody((btRigidBody *)body, group, mask);
}

void WASM_EXP btDiscreteDynamicsWorld_removeRigidBody(pointer_t ptr, pointer_t body)
{
	btDiscreteDynamicsWorld *world = (btDiscreteDynamicsWorld *)ptr;
	return world->removeRigidBody((btRigidBody *)body);
}

void WASM_EXP btDiscreteDynamicsWorld_stepSimulation(pointer_t ptr, btScalar timeStep, int maxSubSteps, btScalar fixedTimeStep)
{
	btDiscreteDynamicsWorld *world = (btDiscreteDynamicsWorld *)ptr;
	world->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);
}

void WASM_EXP btDiscreteDynamicsWorld_clearForces(pointer_t ptr)
{
	btDiscreteDynamicsWorld *world = (btDiscreteDynamicsWorld *)ptr;
	world->clearForces();
}

void WASM_EXP btDiscreteDynamicsWorld_setApplySpeculativeContactRestitution(pointer_t ptr, bool enable)
{
	btDiscreteDynamicsWorld *world = (btDiscreteDynamicsWorld *)ptr;
	world->setApplySpeculativeContactRestitution(enable);
}

bool WASM_EXP btDiscreteDynamicsWorld_getApplySpeculativeContactRestitution(pointer_t ptr)
{
	btDiscreteDynamicsWorld *world = (btDiscreteDynamicsWorld *)ptr;
	return world->getApplySpeculativeContactRestitution();
}

//btKinematicCharacterController
pointer_t WASM_EXP btKinematicCharacterController_create(pointer_t ghostObject, pointer_t convexShape, btScalar stepHeight, pointer_t up)
{
	return (pointer_t) new btKinematicCharacterController((btPairCachingGhostObject *)ghostObject, (btConvexShape *)convexShape, stepHeight, *(btVector3 *)up);
}

void WASM_EXP btKinematicCharacterController_setWalkDirection(pointer_t ptr, pointer_t walkDirection)
{
	btKinematicCharacterController *character = (btKinematicCharacterController *)ptr;
	character->setWalkDirection(*(btVector3 *)walkDirection);
}

void WASM_EXP btKinematicCharacterController_setFallSpeed(pointer_t ptr, btScalar fallSpeed)
{
	btKinematicCharacterController *character = (btKinematicCharacterController *)ptr;
	character->setFallSpeed(fallSpeed);
}

void WASM_EXP btKinematicCharacterController_setJumpSpeed(pointer_t ptr, btScalar jumpSpeed)
{
	btKinematicCharacterController *character = (btKinematicCharacterController *)ptr;
	character->setJumpSpeed(jumpSpeed);
}

void WASM_EXP btKinematicCharacterController_setMaxSlope(pointer_t ptr, btScalar slopeRadians)
{
	btKinematicCharacterController *character = (btKinematicCharacterController *)ptr;
	character->setMaxSlope(slopeRadians);
}

bool WASM_EXP btKinematicCharacterController_onGround(pointer_t ptr)
{
	btKinematicCharacterController *character = (btKinematicCharacterController *)ptr;
	return character->onGround();
}

void WASM_EXP btKinematicCharacterController_jump(pointer_t ptr, pointer_t v)
{
	btKinematicCharacterController *character = (btKinematicCharacterController *)ptr;
	character->jump(*(btVector3 *)v);
}

void WASM_EXP btKinematicCharacterController_setGravity(pointer_t ptr, pointer_t gravity)
{
	btKinematicCharacterController *character = (btKinematicCharacterController *)ptr;
	character->setGravity(*(btVector3 *)gravity);
}

void WASM_EXP btKinematicCharacterController_setUp(pointer_t ptr, pointer_t up)
{
	btKinematicCharacterController *character = (btKinematicCharacterController *)ptr;
	character->setUp(*(btVector3 *)up);
}

void WASM_EXP btKinematicCharacterController_setStepHeight(pointer_t ptr, btScalar h)
{
	btKinematicCharacterController *character = (btKinematicCharacterController *)ptr;
	character->setStepHeight(h);
}

void WASM_EXP btKinematicCharacterController_destroy(pointer_t ptr)
{
	btKinematicCharacterController *character = (btKinematicCharacterController *)ptr;
	delete character;
}

//btPairCachingGhostObject
pointer_t WASM_EXP btPairCachingGhostObject_create()
{
	return (pointer_t) new btPairCachingGhostObject();
}

//btGhostPairCallback
pointer_t WASM_EXP btGhostPairCallback_create()
{
	return (pointer_t) new btGhostPairCallback();
}


//btTypedConstraint interface
void WASM_EXP btTypedConstraint_setEnabled(pointer_t constraintptr, bool enabled)
{
	btTypedConstraint* constraint = (btTypedConstraint *)constraintptr;
	constraint->setEnabled(enabled);
}

void WASM_EXP btCollisionWorld_addConstraint(pointer_t ptr, pointer_t constraintptr, bool disableCollisionsBetweenLinkedBodies)
{
	btDiscreteDynamicsWorld *world = (btDiscreteDynamicsWorld *)ptr;
	btTypedConstraint* constraint = (btTypedConstraint *)constraintptr;
	world->addConstraint(constraint, disableCollisionsBetweenLinkedBodies);
}
void WASM_EXP btCollisionWorld_removeConstraint(pointer_t ptr, pointer_t constraintptr)
{
	btDiscreteDynamicsWorld *world = (btDiscreteDynamicsWorld *)ptr;
	btTypedConstraint* constraint = (btTypedConstraint *)constraintptr;
	world->removeConstraint(constraint);
}


pointer_t WASM_EXP btJointFeedback_create()
{
	btJointFeedback* jointFeedback = new btJointFeedback();
	return (pointer_t) jointFeedback;
}

void WASM_EXP btJointFeedback_destroy(pointer_t jointFeedbackptr)
{
	btJointFeedback* jointFeedback = (btJointFeedback *)jointFeedbackptr;
	delete jointFeedback;
}

void WASM_EXP btTypedConstraint_setJointFeedback(pointer_t constraintptr, pointer_t jointFeedbackptr)
{
	btTypedConstraint* constraint = (btTypedConstraint *)constraintptr;
	btJointFeedback* jointFeedback = (btJointFeedback *)jointFeedbackptr;
	constraint->setJointFeedback(jointFeedback);	
}

pointer_t WASM_EXP btTypedConstraint_getJointFeedback(pointer_t constraintptr)
{
	btTypedConstraint* constraint = (btTypedConstraint *)constraintptr;
	btJointFeedback* jointFeedback = constraint->getJointFeedback();
	return (pointer_t)jointFeedback;
}

void WASM_EXP btTypedConstraint_enableFeedback(pointer_t constraintptr, bool needsFeedback)
{
	btTypedConstraint* constraint = (btTypedConstraint *)constraintptr;
	constraint->enableFeedback(needsFeedback);
}

void WASM_EXP btTypedConstraint_setParam(pointer_t constraintptr, int axis1, int constraintParams, btScalar value)
{
	btTypedConstraint * btConstraint = (btTypedConstraint *)constraintptr;
	btConstraint->setParam((btConstraintParams)constraintParams, value, axis1);
}

void WASM_EXP btTypedConstraint_setOverrideNumSolverIterations(pointer_t constraintptr, int overideNumIterations)
{
	btTypedConstraint * btConstraint = (btTypedConstraint *)constraintptr;
	btConstraint->setOverrideNumSolverIterations(overideNumIterations);
}

void WASM_EXP btTypedConstraint_destroy(pointer_t constraintptr)
{
	btTypedConstraint* constraint = (btTypedConstraint *)constraintptr;
	delete constraint;
}

pointer_t WASM_EXP btJointFeedback_getAppliedForceBodyA(pointer_t jointFeedbackptr)
{
	btJointFeedback* jointFeedback = (btJointFeedback *)jointFeedbackptr;
	return (pointer_t) &(jointFeedback->m_appliedForceBodyA);
}
pointer_t WASM_EXP btJointFeedback_getAppliedForceBodyB(pointer_t jointFeedbackptr)
{
	btJointFeedback* jointFeedback = (btJointFeedback *)jointFeedbackptr;
	return (pointer_t) &(jointFeedback->m_appliedForceBodyB);
}

pointer_t WASM_EXP btJointFeedback_getAppliedTorqueBodyA(pointer_t jointFeedbackptr)
{
	btJointFeedback* jointFeedback = (btJointFeedback *)jointFeedbackptr;
	return (pointer_t) &(jointFeedback->m_appliedTorqueBodyA);
}
pointer_t WASM_EXP btJointFeedback_getAppliedTorqueBodyB(pointer_t jointFeedbackptr)
{
	btJointFeedback* jointFeedback = (btJointFeedback *)jointFeedbackptr;
	return (pointer_t) &(jointFeedback->m_appliedTorqueBodyB);
}


pointer_t WASM_EXP btFixedConstraint_create(pointer_t rigidBodyA, pointer_t frameInAptr,pointer_t rigidBodyB, pointer_t frameInBptr)
{
	btRigidBody *rigidA = (btRigidBody *)rigidBodyA;
	btRigidBody *rigidB = (btRigidBody *)rigidBodyB;
	btTransform * frameInA = (btTransform *)frameInAptr;
	btTransform * frameInB = (btTransform *)frameInBptr;
	btFixedConstraint * fixCon = new btFixedConstraint(*rigidA, *rigidB, *frameInA, *frameInB);
	return (pointer_t) fixCon;
}


//btGeneric6DofSpring2Constraint interface
pointer_t WASM_EXP btGeneric6DofSpring2Constraint_create(pointer_t rigidBodyAptr, pointer_t frameInAptr, pointer_t rigidBodyBptr, pointer_t frameInBptr, int rotOrder)
{
	btRigidBody * rigidBodyA = (btRigidBody *)rigidBodyAptr;
	btTransform * frameInA = (btTransform *)frameInAptr;
	btRigidBody * rigidBodyB = (btRigidBody *)rigidBodyBptr;
	btTransform * frameInB = (btTransform *)frameInBptr;
	btGeneric6DofSpring2Constraint * btConstraintPtr = new btGeneric6DofSpring2Constraint(*rigidBodyA, *rigidBodyB, *frameInA, *frameInB, (RotateOrder)rotOrder);
	btConstraintPtr->setOverrideNumSolverIterations(80);
	btConstraintPtr->setParam(BT_CONSTRAINT_STOP_ERP, 0.80, 0);
	btConstraintPtr->setParam(BT_CONSTRAINT_STOP_CFM, 0.0, 0);
	btConstraintPtr->setParam(BT_CONSTRAINT_STOP_ERP, 0.80, 1);
	btConstraintPtr->setParam(BT_CONSTRAINT_STOP_CFM, 0.0, 1);
	btConstraintPtr->setParam(BT_CONSTRAINT_STOP_ERP, 0.80, 2);
	btConstraintPtr->setParam(BT_CONSTRAINT_STOP_CFM, 0.0, 2);
	btConstraintPtr->setParam(BT_CONSTRAINT_STOP_ERP, 0.80, 3);
	btConstraintPtr->setParam(BT_CONSTRAINT_STOP_CFM, 0.0, 3);
	btConstraintPtr->setParam(BT_CONSTRAINT_STOP_ERP, 0.80, 4);
	btConstraintPtr->setParam(BT_CONSTRAINT_STOP_CFM, 0.0, 4);
	btConstraintPtr->setParam(BT_CONSTRAINT_STOP_ERP, 0.80, 5);
	btConstraintPtr->setParam(BT_CONSTRAINT_STOP_CFM, 0.0, 5);
	return (pointer_t)btConstraintPtr;
}

void WASM_EXP btGeneric6DofSpring2Constraint_setAxis(pointer_t g6ds2Constraintptr, pointer_t axis1, pointer_t axis2)
{
	btVector3 * axisVec1 = (btVector3 *)axis1;
	btVector3 * axisVec2 = (btVector3 *)axis2;
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btConstraint->setAxis(*axisVec1, *axisVec2);
}
void WASM_EXP btGeneric6DofSpring2Constraint_setLimit(pointer_t g6ds2Constraintptr, int axis, btScalar lo, btScalar hi)
{
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btConstraint->setLimit(axis, lo, hi);
}
void WASM_EXP btGeneric6DofSpring2Constraint_enableSpring(pointer_t g6ds2Constraintptr, int index, bool enableSpring)
{
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btConstraint->enableSpring(index, enableSpring);
}
void WASM_EXP btGeneric6DofSpring2Constraint_setBounce(pointer_t g6ds2Constraintptr, int index, btScalar bounce)
{
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btConstraint->setBounce(index, bounce);
}
void WASM_EXP btGeneric6DofSpring2Constraint_setStiffness(pointer_t g6ds2Constraintptr, int index, btScalar stiffness, bool limitIfNeeded)
{
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btConstraint->setStiffness(index, stiffness, limitIfNeeded);
}
void WASM_EXP btGeneric6DofSpring2Constraint_setDamping(pointer_t g6ds2Constraintptr, int index, btScalar damping, bool limitIfNeeded)
{
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btConstraint->setDamping(index, damping, limitIfNeeded);
}
void WASM_EXP btGeneric6DofSpring2Constraint_setEquilibriumPoint(pointer_t g6ds2Constraintptr, int index, btScalar val)
{
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btConstraint->setEquilibriumPoint(index, val);
}
void WASM_EXP btGeneric6DofSpring2Constraint_enableMotor(pointer_t g6ds2Constraintptr, int index, bool onOff)
{
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btConstraint->enableMotor(index, onOff);
}
void WASM_EXP btGeneric6DofSpring2Constraint_setServo(pointer_t g6ds2Constraintptr, int index, bool onOff)
{
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btConstraint->setServo(index, onOff);
}
void WASM_EXP btGeneric6DofSpring2Constraint_setTargetVelocity(pointer_t g6ds2Constraintptr, int index, btScalar velocity)
{
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btConstraint->setTargetVelocity(index, velocity);
}
void WASM_EXP btGeneric6DofSpring2Constraint_setServoTarget(pointer_t g6ds2Constraintptr, int index, btScalar target)
{
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btConstraint->setServoTarget(index, target);
}

void WASM_EXP btGeneric6DofSpring2Constraint_setMaxMotorForce(pointer_t g6ds2Constraintptr, int index, btScalar force)
{
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btConstraint->setMaxMotorForce(index, force);
}

void WASM_EXP btGeneric6DofSpring2Constraint_setFrames(pointer_t g6ds2Constraintptr, pointer_t frameAptr, pointer_t frameBptr)
{
	btGeneric6DofSpring2Constraint * btConstraint = (btGeneric6DofSpring2Constraint *)g6ds2Constraintptr;
	btTransform *  frameA = (btTransform *)frameAptr;
	btTransform *  frameB = (btTransform *)frameBptr;
	btConstraint->setFrames(*frameA, *frameB);
}
}
