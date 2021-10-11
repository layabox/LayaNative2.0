#ifndef __LayaBulletExport_H__
#define __LayaBulletExport_H__

#include "../../../JCScriptRuntime.h"
#include "../../JSInterface/JSInterface.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#define  WASM_EXP JSLayaConchBullet::
#define __BTWASM_SYSCALL_NAME(name)
typedef intptr_t pointer_t;
namespace laya
{
    inline void layaMotionStateGetWorldTransform(int32_t rigidBodyID, pointer_t worldTrans)
    {
        JCScriptRuntime* pScriptRuntime = JCScriptRuntime::s_JSRT;
        if (pScriptRuntime)
        {
            pScriptRuntime->m_bJSBulletGetWorldTransformHandle.Call(rigidBodyID, worldTrans);
        }
    }
    inline void layaMotionStateSetWorldTransform(int32_t rigidBodyID, const pointer_t worldTrans)
    {
        JCScriptRuntime* pScriptRuntime = JCScriptRuntime::s_JSRT;
        if (pScriptRuntime)
        {
            pScriptRuntime->m_bJSBulletSetWorldTransformHandle.Call(rigidBodyID, worldTrans);
        }
    }


    class LayaMotionState : public btMotionState
    {
    public:
		int32_t rigidBodyID;
        virtual void getWorldTransform(btTransform &centerOfMassWorldTrans)
        {
            layaMotionStateGetWorldTransform(rigidBodyID, (pointer_t)&centerOfMassWorldTrans);
        }
        virtual void setWorldTransform(const btTransform &centerOfMassWorldTrans)
        {
            layaMotionStateSetWorldTransform(rigidBodyID, (pointer_t)&centerOfMassWorldTrans);
        }
    };
    class JSLayaConchBullet : public JsObjBase, public JSObjNode
    {
    private:
        static JSLayaConchBullet* ms_pInstance;
    public:

        static JsObjClassInfo JSCLSINFO;

        static JSLayaConchBullet* getInstance();

        static void exportJS();

        JSLayaConchBullet();

        ~JSLayaConchBullet();

        void  btGImpactCollisionAlgorithm_RegisterAlgorithm(pointer_t dispatcher);
        pointer_t  btVector3_create(btScalar x, btScalar y, btScalar z);
        void  btVector3_setValue(pointer_t ptr, btScalar x, btScalar y, btScalar z);
        btScalar  btVector3_x(pointer_t ptr);
        btScalar  btVector3_y(pointer_t ptr);
        btScalar  btVector3_z(pointer_t ptr);
        pointer_t  btQuaternion_create(btScalar x, btScalar y, btScalar z, btScalar w);
        void  btQuaternion_setValue(pointer_t ptr, btScalar x, btScalar y, btScalar z, btScalar w);
        btScalar  btQuaternion_x(pointer_t ptr);
        btScalar  btQuaternion_y(pointer_t ptr);
        btScalar  btQuaternion_z(pointer_t ptr);
        btScalar  btQuaternion_w(pointer_t ptr);
        pointer_t  btTransform_create();
        void  btTransform_setOrigin(pointer_t ptr, pointer_t origin);
        void  btTransform_setRotation(pointer_t ptr, pointer_t q);
        pointer_t  btTransform_getOrigin(pointer_t ptr);
        pointer_t  btTransform_getRotation(pointer_t ptr);
        void btTransform_setIdentity(pointer_t ptr);
        void  btMotionState_destroy(pointer_t ptr);
        pointer_t  layaMotionState_create();
        void  layaMotionState_set_rigidBodyID(pointer_t ptr, int rigidBodyID);
        pointer_t  btCollisionObject_create();
        void  btCollisionObject_setContactProcessingThreshold(pointer_t ptr, btScalar contactProcessingThreshold);
        void  btCollisionObject_setActivationState(pointer_t ptr, int newState);
        void  btCollisionObject_forceActivationState(pointer_t ptr, int newState);
        void  btCollisionObject_activate(pointer_t ptr, bool forceActivation);
        bool  btCollisionObject_isActive(pointer_t ptr);
        void  btCollisionObject_setRestitution(pointer_t ptr, btScalar rest);
        void  btCollisionObject_setFriction(pointer_t ptr, btScalar frict);
        void  btCollisionObject_setRollingFriction(pointer_t ptr, btScalar frict);
        int  btCollisionObject_getCollisionFlags(pointer_t ptr);
        void  btCollisionObject_setCollisionFlags(pointer_t ptr, int flags);
        pointer_t  btCollisionObject_getWorldTransform(pointer_t ptr);
        void  btCollisionObject_setCollisionShape(pointer_t ptr, pointer_t collisionShape);
        btScalar  btCollisionObject_getCcdMotionThreshold(pointer_t ptr);
        void  btCollisionObject_setCcdMotionThreshold(pointer_t ptr, btScalar ccdMotionThreshold);
        btScalar  btCollisionObject_getCcdSweptSphereRadius(pointer_t ptr);
        void  btCollisionObject_setCcdSweptSphereRadius(pointer_t ptr, btScalar radius);
        int  btCollisionObject_getUserIndex(pointer_t ptr);
        void  btCollisionObject_setUserIndex(pointer_t ptr, int index);
        int  btCollisionObject_getActivationState(pointer_t ptr);
        void  btCollisionObject_setInterpolationAngularVelocity(pointer_t ptr, pointer_t angvel);
        void  btCollisionObject_setInterpolationLinearVelocity(pointer_t ptr, pointer_t linvel);
        void  btCollisionObject_destroy(pointer_t ptr);
		void RayResultCallback_set_m_flags(pointer_t ptr, int flags);
        bool  RayResultCallback_hasHit(pointer_t ptr);
        void  RayResultCallback_set_m_collisionFilterGroup(pointer_t ptr, int group);
        void  RayResultCallback_set_m_collisionFilterMask(pointer_t ptr, int mask);
        btScalar  RayResultCallback_get_m_closestHitFraction(pointer_t ptr);
        void  RayResultCallback_set_m_closestHitFraction(pointer_t ptr, btScalar fraction);
        pointer_t  RayResultCallback_get_m_collisionObject(pointer_t ptr);
        void  RayResultCallback_set_m_collisionObject(pointer_t ptr, pointer_t collisionObject);
        pointer_t  ClosestRayResultCallback_create(pointer_t rayFromWorld, pointer_t rayToWorld);
        pointer_t  ClosestRayResultCallback_get_m_rayFromWorld(pointer_t ptr);
        void  ClosestRayResultCallback_set_m_rayFromWorld(pointer_t ptr, pointer_t rayFromWorld);
        pointer_t  ClosestRayResultCallback_get_m_rayToWorld(pointer_t ptr);
        void  ClosestRayResultCallback_set_m_rayToWorld(pointer_t ptr, pointer_t rayToWorld);
        pointer_t  ClosestRayResultCallback_get_m_hitNormalWorld(pointer_t ptr);
        pointer_t  ClosestRayResultCallback_get_m_hitPointWorld(pointer_t ptr);
        int  tBtCollisionObjectArray_size(pointer_t ptr);
        pointer_t  tBtCollisionObjectArray_at(pointer_t ptr, int n);
        void  tBtCollisionObjectArray_clear(pointer_t ptr);
        pointer_t  tVector3Array_at(pointer_t ptr, int n);
        void  tVector3Array_clear(pointer_t ptr);
        btScalar  tScalarArray_at(pointer_t ptr, int n);
        void  tScalarArray_clear(pointer_t ptr);
        pointer_t  AllHitsRayResultCallback_create(pointer_t rayFromWorld, pointer_t rayToWorld);
        pointer_t  AllHitsRayResultCallback_get_m_rayFromWorld(pointer_t ptr);
        void  AllHitsRayResultCallback_set_m_rayFromWorld(pointer_t ptr, pointer_t rayFromWorld);
        pointer_t  AllHitsRayResultCallback_get_m_rayToWorld(pointer_t ptr);
        void  AllHitsRayResultCallback_set_m_rayToWorld(pointer_t ptr, pointer_t rayToWorld);
        pointer_t  AllHitsRayResultCallback_get_m_hitPointWorld(pointer_t ptr);
        pointer_t  AllHitsRayResultCallback_get_m_hitNormalWorld(pointer_t ptr);
        pointer_t  AllHitsRayResultCallback_get_m_collisionObjects(pointer_t ptr);
        pointer_t  AllHitsRayResultCallback_get_m_hitFractions(pointer_t ptr);
        pointer_t  btManifoldPoint_get_m_positionWorldOnA(pointer_t ptr);
        pointer_t  btManifoldPoint_get_m_positionWorldOnB(pointer_t ptr);
        pointer_t  btManifoldPoint_get_m_normalWorldOnB(pointer_t ptr);
        btScalar  btManifoldPoint_getDistance(pointer_t ptr);
        bool  ConvexResultCallback_hasHit(pointer_t ptr);
        void  ConvexResultCallback_set_m_collisionFilterGroup(pointer_t ptr, int group);
        void  ConvexResultCallback_set_m_collisionFilterMask(pointer_t ptr, int mask);
        btScalar  ConvexResultCallback_get_m_closestHitFraction(pointer_t ptr);
        void  ConvexResultCallback_set_m_closestHitFraction(pointer_t ptr, btScalar fraction);
        pointer_t  ClosestConvexResultCallback_create(pointer_t convexFromWorld, pointer_t convexToWorld);
        pointer_t  ClosestConvexResultCallback_get_m_hitNormalWorld(pointer_t ptr);
        pointer_t  ClosestConvexResultCallback_get_m_hitPointWorld(pointer_t ptr);
        pointer_t  ClosestConvexResultCallback_get_m_hitCollisionObject(pointer_t ptr);
        void  ClosestConvexResultCallback_set_m_hitCollisionObject(pointer_t ptr, pointer_t hitCollisionObject);
        pointer_t  AllConvexResultCallback_create(pointer_t convexFromWorld, pointer_t convexToWorld);
        pointer_t  AllConvexResultCallback_get_m_hitNormalWorld(pointer_t ptr);
        pointer_t  AllConvexResultCallback_get_m_hitPointWorld(pointer_t ptr);
        pointer_t  AllConvexResultCallback_get_m_hitFractions(pointer_t ptr);
        pointer_t  AllConvexResultCallback_get_m_collisionObjects(pointer_t ptr);

        int btCollisionShape_getShapeType(pointer_t ptr);
        pointer_t  btCollisionShape_getLocalScaling(pointer_t ptr);
        void  btCollisionShape_setLocalScaling(pointer_t ptr, pointer_t scaling);
        void  btCollisionShape_calculateLocalInertia(pointer_t ptr, btScalar mass, pointer_t inertia);
        void  btCollisionShape_destroy(pointer_t ptr);
        pointer_t  btBoxShape_create(pointer_t boxHalfExtents);
        void  btBoxShape_getAabb(pointer_t ptr, pointer_t t, pointer_t aabbMin, pointer_t aabbMax);
        pointer_t  btBoxShape_getHalfExtentsWithMargin(pointer_t ptr, pointer_t t, pointer_t aabbMin, pointer_t aabbMax);
        pointer_t  btCapsuleShape_create(btScalar radius, btScalar height);
        pointer_t  btCapsuleShapeX_create(btScalar radius, btScalar height);
        pointer_t  btCapsuleShapeZ_create(btScalar radius, btScalar height);
        pointer_t  btCylinderShape_create(pointer_t halfExtents);
        pointer_t  btCylinderShapeX_create(pointer_t halfExtents);
        pointer_t  btCylinderShapeZ_create(pointer_t halfExtents);
        pointer_t  btSphereShape_create(btScalar radius);
        pointer_t  btConeShape_create(btScalar radius, btScalar height);
        pointer_t  btConeShapeX_create(btScalar radius, btScalar height);
        pointer_t  btConeShapeZ_create(btScalar radius, btScalar height);
        pointer_t  btStaticPlaneShape_create(pointer_t planeNormal, btScalar planeConstant);
        void  btGImpactShapeInterface_updateBound(pointer_t ptr);
        pointer_t  btGImpactMeshShape_create(pointer_t meshInterface);
        pointer_t  btCompoundShape_create();
        void  btCompoundShape_addChildShape(pointer_t ptr, pointer_t localTransform, pointer_t shape);
        void  btCompoundShape_removeChildShapeByIndex(pointer_t ptr, int childShapeIndex);
        pointer_t  btCompoundShape_getChildShape(pointer_t ptr, int index);
        void  btCompoundShape_updateChildTransform(pointer_t ptr, int index, pointer_t newChildTransform, bool shouldRecalculateLocalAabb);
        void  btStridingMeshInterface_destroy(pointer_t ptr);
        pointer_t  btTriangleMesh_create();
        void  btTriangleMesh_addTriangle(pointer_t ptr, pointer_t vertex1, pointer_t vertex2, pointer_t vertex3, bool removeDuplicateVertices);
        pointer_t  btDefaultCollisionConfiguration_create();
        void  btDefaultCollisionConfiguration_destroy(pointer_t ptr);
        pointer_t  btPersistentManifold_getBody0(pointer_t ptr);
        pointer_t  btPersistentManifold_getBody1(pointer_t ptr);
        int  btPersistentManifold_getNumContacts(pointer_t ptr);
        pointer_t  btPersistentManifold_getContactPoint(pointer_t ptr, int index);
        int  btDispatcher_getNumManifolds(pointer_t ptr);
        pointer_t  btDispatcher_getManifoldByIndexInternal(pointer_t ptr, int index);
        pointer_t  btCollisionDispatcher_create(pointer_t collisionConfiguration);
        void  btCollisionDispatcher_destroy(pointer_t ptr);
        void  btOverlappingPairCache_setInternalGhostPairCallback(pointer_t ptr, pointer_t ghostPairCallback);
        pointer_t  btDbvtBroadphase_create();
        pointer_t  btDbvtBroadphase_getOverlappingPairCache(pointer_t ptr);
        void  btDbvtBroadphase_destroy(pointer_t ptr);
        pointer_t  btRigidBodyConstructionInfo_create(btScalar mass, pointer_t motionState, pointer_t collisionShape, pointer_t localInertia);
        void  btRigidBodyConstructionInfo_destroy(pointer_t ptr);
        pointer_t  btRigidBody_create(pointer_t constructionInfo);
        void  btRigidBody_setCenterOfMassTransform(pointer_t ptr, pointer_t xform);
        void  btRigidBody_setSleepingThresholds(pointer_t ptr, btScalar linear, btScalar angular);
        btScalar  btRigidBody_getLinearSleepingThreshold(pointer_t ptr);
        btScalar  btRigidBody_getAngularSleepingThreshold(pointer_t ptr);
        void  btRigidBody_setDamping(pointer_t ptr, btScalar lin_damping, btScalar ang_damping);
        void  btRigidBody_setMassProps(pointer_t ptr, btScalar mass, pointer_t inertia);
        void  btRigidBody_setLinearFactor(pointer_t ptr, pointer_t linearFactor);
        void  btRigidBody_applyTorque(pointer_t ptr, pointer_t torque);
        void  btRigidBody_applyForce(pointer_t ptr, pointer_t force, pointer_t rel_pos);
        void  btRigidBody_applyCentralForce(pointer_t ptr, pointer_t force);
        void  btRigidBody_applyTorqueImpulse(pointer_t ptr, pointer_t torque);
        void  btRigidBody_applyImpulse(pointer_t ptr, pointer_t impulse, pointer_t rel_pos);
        void  btRigidBody_applyCentralImpulse(pointer_t ptr, pointer_t impulse);
        void  btRigidBody_updateInertiaTensor(pointer_t ptr);
        pointer_t  btRigidBody_getLinearVelocity(pointer_t ptr);
        pointer_t  btRigidBody_getAngularVelocity(pointer_t ptr);
        void  btRigidBody_setLinearVelocity(pointer_t ptr, pointer_t lin_vel);
        void  btRigidBody_setAngularVelocity(pointer_t ptr, pointer_t ang_vel);
        void  btRigidBody_setAngularFactor(pointer_t ptr, pointer_t angularFactor);
        pointer_t  btRigidBody_getGravity(pointer_t ptr);
        void  btRigidBody_setGravity(pointer_t ptr, pointer_t acceleration);
        void  btKinematicCharacterController_setUp(pointer_t ptr, pointer_t up);
        void  btKinematicCharacterController_setStepHeight(pointer_t ptr, btScalar h);
        void  btKinematicCharacterController_setMaxPenetrationDepth(pointer_t ptr, btScalar d);
        btScalar btKinematicCharacterController_getMaxPenetrationDepth(pointer_t ptr);
        void  btCollisionObject_setInterpolationWorldTransform(pointer_t ptr, pointer_t worldTrans);
        void  btCollisionObject_setWorldTransform(pointer_t ptr, pointer_t worldTrans);
        pointer_t  btRigidBody_getTotalForce(pointer_t ptr);
        pointer_t  btRigidBody_getTotalTorque(pointer_t ptr);
        int  btRigidBody_getFlags(pointer_t ptr);
        void  btRigidBody_setFlags(pointer_t ptr, int flags);
        void  btRigidBody_clearForces(pointer_t ptr);
        pointer_t  btSequentialImpulseConstraintSolver_create();
        bool  btCollisionWorld_get_m_useContinuous(pointer_t ptr);
        void  btCollisionWorld_set_m_useContinuous(pointer_t ptr, bool useContinuous);
        void  btCollisionWorld_rayTest(pointer_t ptr, pointer_t rayFromWorld, pointer_t rayToWorld, pointer_t resultCallback);
        pointer_t  btCollisionWorld_getDispatchInfo(pointer_t ptr);
        void  btCollisionWorld_addCollisionObject(pointer_t ptr, pointer_t collisionObject, int collisionFilterGroup, int collisionFilterMask);
        void  btCollisionWorld_removeCollisionObject(pointer_t ptr, pointer_t collisionObject);
        void  btCollisionWorld_convexSweepTest(pointer_t ptr, pointer_t castShape, pointer_t from, pointer_t to, pointer_t resultCallback, float allowedCcdPenetration);
        void  btCollisionWorld_destroy(pointer_t ptr);
        void  btDynamicsWorld_addAction(pointer_t ptr, pointer_t action);
        void  btDynamicsWorld_removeAction(pointer_t ptr, pointer_t action);
        pointer_t  btDynamicsWorld_getSolverInfo(pointer_t ptr);
        pointer_t  btDiscreteDynamicsWorld_create(pointer_t dispatcher, pointer_t pairCache, pointer_t constraintSolver, pointer_t collisionConfiguration);
        void  btDiscreteDynamicsWorld_setGravity(pointer_t ptr, pointer_t gravity);
        pointer_t  btDiscreteDynamicsWorld_getGravity(pointer_t ptr);
        void  btDiscreteDynamicsWorld_addRigidBody(pointer_t ptr, pointer_t body, int group, int mask);
        void  btDiscreteDynamicsWorld_removeRigidBody(pointer_t ptr, pointer_t body);
        void  btDiscreteDynamicsWorld_stepSimulation(pointer_t ptr, btScalar timeStep, int maxSubSteps, btScalar fixedTimeStep);
        void  btDiscreteDynamicsWorld_clearForces(pointer_t ptr);
        void  btDiscreteDynamicsWorld_setApplySpeculativeContactRestitution(pointer_t ptr, bool enable);
        bool  btDiscreteDynamicsWorld_getApplySpeculativeContactRestitution(pointer_t ptr);
        pointer_t  btKinematicCharacterController_create(pointer_t ghostObject, pointer_t convexShape, btScalar stepHeight, pointer_t up);
        void  btKinematicCharacterController_setWalkDirection(pointer_t ptr, pointer_t walkDirection);
        void  btKinematicCharacterController_setFallSpeed(pointer_t ptr, btScalar fallSpeed);
        void  btKinematicCharacterController_setJumpSpeed(pointer_t ptr, btScalar jumpSpeed);
        void  btKinematicCharacterController_setMaxSlope(pointer_t ptr, btScalar slopeRadians);
        bool  btKinematicCharacterController_onGround(pointer_t ptr);
        void  btKinematicCharacterController_jump(pointer_t ptr, pointer_t v);
        void  btKinematicCharacterController_setGravity(pointer_t ptr, pointer_t gravity);
        void  btKinematicCharacterController_destroy(pointer_t ptr);
        pointer_t  btPairCachingGhostObject_create();
        pointer_t  btGhostPairCallback_create();
        void  btTransform_equal(pointer_t ptr, pointer_t other);

        void btTypedConstraint_setEnabled(pointer_t constraintptr, bool enabled);
        void btCollisionWorld_addConstraint(pointer_t ptr, pointer_t constraintptr, bool disableCollisionsBetweenLinkedBodies);
        void btCollisionWorld_removeConstraint(pointer_t ptr, pointer_t constraintptr);
        pointer_t btJointFeedback_create();
        void btJointFeedback_destroy(pointer_t jointFeedbackptr);
        void btTypedConstraint_setJointFeedback(pointer_t constraintptr, pointer_t jointFeedbackptr);
        pointer_t btTypedConstraint_getJointFeedback(pointer_t constraintptr);
        void btTypedConstraint_enableFeedback(pointer_t constraintptr, bool needsFeedback);
        void btTypedConstraint_setParam(pointer_t constraintptr, int axis1, int constraintParams, btScalar value); 
        void btTypedConstraint_setOverrideNumSolverIterations(pointer_t constraintptr, int overideNumIterations);
        void btTypedConstraint_destroy(pointer_t constraintptr);
        pointer_t btJointFeedback_getAppliedForceBodyA(pointer_t jointFeedbackptr);
        pointer_t btJointFeedback_getAppliedForceBodyB(pointer_t jointFeedbackptr);
        pointer_t btJointFeedback_getAppliedTorqueBodyA(pointer_t jointFeedbackptr);
        pointer_t btJointFeedback_getAppliedTorqueBodyB(pointer_t jointFeedbackptr);
        pointer_t btFixedConstraint_create(pointer_t rigidBodyA, pointer_t frameInAptr, pointer_t rigidBodyB, pointer_t frameInBptr);
        pointer_t btGeneric6DofSpring2Constraint_create(pointer_t rigidBodyAptr, pointer_t frameInAptr, pointer_t rigidBodyBptr, pointer_t frameInBptr, int rotOrder = 0);
        void btGeneric6DofSpring2Constraint_setAxis(pointer_t g6ds2Constraintptr, pointer_t axis1, pointer_t axis2);
        void btGeneric6DofSpring2Constraint_setLimit(pointer_t g6ds2Constraintptr, int axis, btScalar lo, btScalar hi);
        void btGeneric6DofSpring2Constraint_enableSpring(pointer_t g6ds2Constraintptr, int index, bool enableSpring);
        void btGeneric6DofSpring2Constraint_setBounce(pointer_t g6ds2Constraintptr, int index, btScalar bounce);
        void btGeneric6DofSpring2Constraint_setStiffness(pointer_t g6ds2Constraintptr, int index, btScalar stiffness, bool limitIfNeeded = true);
        void btGeneric6DofSpring2Constraint_setDamping(pointer_t g6ds2Constraintptr, int index, btScalar damping, bool limitIfNeeded = true);
        void btGeneric6DofSpring2Constraint_setEquilibriumPoint(pointer_t g6ds2Constraintptr, int index, btScalar val);
        void btGeneric6DofSpring2Constraint_enableMotor(pointer_t g6ds2Constraintptr, int index, bool onOff);
        void btGeneric6DofSpring2Constraint_setServo(pointer_t g6ds2Constraintptr, int index, bool onOff);
        void btGeneric6DofSpring2Constraint_setTargetVelocity(pointer_t g6ds2Constraintptr, int index, btScalar velocity);
        void btGeneric6DofSpring2Constraint_setServoTarget(pointer_t g6ds2Constraintptr, int index, btScalar target);
        void btGeneric6DofSpring2Constraint_setMaxMotorForce(pointer_t g6ds2Constraintptr, int index, btScalar force);
        void btGeneric6DofSpring2Constraint_setFrames(pointer_t g6ds2Constraintptr, pointer_t frameAptr, pointer_t frameBptr);
    };
}
#endif
