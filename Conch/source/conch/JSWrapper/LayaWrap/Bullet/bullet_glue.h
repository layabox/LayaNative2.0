#include "BulletDynamics/ConstraintSolver/btTypedConstraint.h"
#include "BulletCollision/CollisionShapes/btConcaveShape.h"
#include "JSbtBindHelper.h"

#include "../../JSInterface/JSInterface.h"
#include <JSObjBase.h>
namespace laya
{
typedef intptr_t long_t;

	class JSLayaMotionStateHandler : public JsObjBase
	{
	public:
		static JsObjClassInfo JSCLSINFO;

		JSLayaMotionStateHandler()
		{
			createJSObj();
		}

		void setNativeGetHandle(JSValueAsParam value)
		{
			m_getHandle.set(0, this, value);
		}

		void setNativeSetHandle(JSValueAsParam value)
		{
			m_setHandle.set(1, this, value);
		}

		void setThisValue(JsValue thisValue)
		{
			m_thisValueHandle.set(2, this, thisValue);
		}

		void callGet(btTransform& worldTrans)
		{
			m_getHandle.Call(m_thisValueHandle.getJsObj(), reinterpret_cast<long_t>(&worldTrans));
		}
		void callSet(const btTransform& worldTrans)
		{
			m_setHandle.Call(m_thisValueHandle.getJsObj(), reinterpret_cast<long_t>(&worldTrans));
		}

	private:
		JsObjHandle m_getHandle;
		JsObjHandle m_setHandle;
		JsObjHandle m_thisValueHandle;
	};

	class LayaMotionState0 : public btMotionState, public JSObjNode
	{
	public:
		void setNativeGetWorldTransform(JSValueAsParam value)
		{
			m_jsHandler.setNativeGetHandle(value);
		}

		void setNativeSetWorldTransform(JSValueAsParam value)
		{
			m_jsHandler.setNativeSetHandle(value);
		}

		void setRigidbody(JsValue thisValue)
		{
			m_jsHandler.setThisValue(thisValue);
		}

		virtual void getWorldTransform(btTransform& worldTrans)
		{
			m_jsHandler.callGet(worldTrans);
		};

		virtual void setWorldTransform(const btTransform& worldTrans)
		{
			m_jsHandler.callSet(worldTrans);
		};

	private:
		JSLayaMotionStateHandler m_jsHandler;
	};
    typedef intptr_t btCollisionShapeAddr;
    typedef intptr_t btCollisionObjectAddr;
    typedef intptr_t btTypedConstraintAddr;
    typedef intptr_t btCollisionWorldAddr;
    typedef intptr_t btConcaveShapeAddr;
    typedef intptr_t btCapsuleShapeAddr;
    typedef intptr_t btGImpactShapeInterfaceAddr;
    typedef intptr_t btDynamicsWorldAddr;
    typedef intptr_t btTriangleMeshShapeAddr;
    typedef intptr_t btGhostObjectAddr;
    typedef intptr_t btConeShapeAddr;
    typedef intptr_t btActionInterfaceAddr;
    typedef intptr_t btVector3Addr;
    typedef intptr_t btVehicleRaycasterAddr;
    typedef intptr_t btQuadWordAddr;
    typedef intptr_t btCylinderShapeAddr;
    typedef intptr_t btConvexShapeAddr;
    typedef intptr_t btDispatcherAddr;
    typedef intptr_t btGeneric6DofConstraintAddr;
    typedef intptr_t btStridingMeshInterfaceAddr;
    typedef intptr_t btMotionStateAddr;
    typedef intptr_t ConvexResultCallbackAddr;
    typedef intptr_t ContactResultCallbackAddr;
    typedef intptr_t RayResultCallbackAddr;
    typedef intptr_t btMatrix3x3Addr;
    typedef intptr_t btDispatcherInfoAddr;
    typedef intptr_t LayaMotionStateAddr;
    typedef intptr_t btWheelInfoConstructionInfoAddr;
    typedef intptr_t btConvexTriangleMeshShapeAddr;
    typedef intptr_t btBroadphaseInterfaceAddr;
    typedef intptr_t btDefaultCollisionConfigurationAddr;
    typedef intptr_t btRigidBodyConstructionInfoAddr;
    typedef intptr_t btCollisionConfigurationAddr;
    typedef intptr_t btPersistentManifoldAddr;
    typedef intptr_t btCompoundShapeAddr;
    typedef intptr_t ClosestConvexResultCallbackAddr;
    typedef intptr_t AllHitsRayResultCallbackAddr;
    typedef intptr_t btDefaultVehicleRaycasterAddr;
    typedef intptr_t btConstraintSettingAddr;
    typedef intptr_t LocalShapeInfoAddr;
    typedef intptr_t btRigidBodyAddr;
    typedef intptr_t btDbvtBroadphaseAddr;
    typedef intptr_t btCollisionDispatcherAddr;
    typedef intptr_t btAxisSweep3Addr;
    typedef intptr_t VoidPtrAddr;
    typedef intptr_t btConeTwistConstraintAddr;
    typedef intptr_t btHingeConstraintAddr;
    typedef intptr_t JSValueAsParamAddr;
    typedef intptr_t btConeShapeZAddr;
    typedef intptr_t btConeShapeXAddr;
    typedef intptr_t btTriangleMeshAddr;
    typedef intptr_t btConvexHullShapeAddr;
    typedef intptr_t btVehicleTuningAddr;
    typedef intptr_t btCollisionObjectWrapperAddr;
    typedef intptr_t btGImpactMeshShapeAddr;
    typedef intptr_t btDefaultMotionStateAddr;
    typedef intptr_t btWheelInfoAddr;
    typedef intptr_t btVector4Addr;
    typedef intptr_t JsValueAddr;
    typedef intptr_t btDefaultCollisionConstructionInfoAddr;
    typedef intptr_t btVehicleRaycasterResultAddr;
    typedef intptr_t btConstraintSolverAddr;
    typedef intptr_t btRaycastVehicleAddr;
    typedef intptr_t tVector3ArrayAddr;
    typedef intptr_t btCylinderShapeXAddr;
    typedef intptr_t btCylinderShapeZAddr;
    typedef intptr_t btSequentialImpulseConstraintSolverAddr;
    typedef intptr_t RaycastInfoAddr;
    typedef intptr_t btHeightfieldTerrainShapeAddr;
    typedef intptr_t btDiscreteDynamicsWorldAddr;
    typedef intptr_t btGhostPairCallbackAddr;
    typedef intptr_t btOverlappingPairCallbackAddr;
    typedef intptr_t btKinematicCharacterControllerAddr;
    typedef intptr_t btStaticPlaneShapeAddr;
    typedef intptr_t btOverlappingPairCacheAddr;
    typedef intptr_t tBtCollisionObjectArrayAddr;
    typedef intptr_t btFixedConstraintAddr;
    typedef intptr_t btTransformAddr;
    typedef intptr_t ClosestRayResultCallbackAddr;
    typedef intptr_t AllConvexResultCallbackAddr;
    typedef intptr_t ConcreteContactResultCallbackAddr;
    typedef intptr_t btBvhTriangleMeshShapeAddr;
    typedef intptr_t btSliderConstraintAddr;
    typedef intptr_t btPairCachingGhostObjectAddr;
    typedef intptr_t btManifoldPointAddr;
    typedef intptr_t btPoint2PointConstraintAddr;
    typedef intptr_t btBroadphaseProxyAddr;
    typedef intptr_t btGeneric6DofSpringConstraintAddr;
    typedef intptr_t btBoxShapeAddr;
    typedef intptr_t btCapsuleShapeXAddr;
    typedef intptr_t btQuaternionAddr;
    typedef intptr_t btCapsuleShapeZAddr;
    typedef intptr_t btContactSolverInfoAddr;
    typedef intptr_t tScalarArrayAddr;
    typedef intptr_t btSphereShapeAddr;
    typedef intptr_t LocalConvexResultAddr;

    // btCollisionShape
  
    void btCollisionShape_setLocalScaling_1(btCollisionShapeAddr self, btVector3Addr arg0);
    btVector3Addr btCollisionShape_getLocalScaling_0(btCollisionShapeAddr self);
    void btCollisionShape_calculateLocalInertia_2(btCollisionShapeAddr self, double arg0, btVector3Addr arg1);
    void btCollisionShape_setMargin_1(btCollisionShapeAddr self, double arg0);
    double btCollisionShape_getMargin_0(btCollisionShapeAddr self);
    void btCollisionShape___destroy___0(btCollisionShapeAddr self);

    // btCollisionObject
  
    btCollisionObjectAddr btCollisionObject_btCollisionObject_0();
    void btCollisionObject_setAnisotropicFriction_2(btCollisionObjectAddr self, btVector3Addr arg0, long_t arg1);
    btCollisionShapeAddr btCollisionObject_getCollisionShape_0(btCollisionObjectAddr self);
    void btCollisionObject_setContactProcessingThreshold_1(btCollisionObjectAddr self, double arg0);
    void btCollisionObject_setActivationState_1(btCollisionObjectAddr self, long_t arg0);
    void btCollisionObject_forceActivationState_1(btCollisionObjectAddr self, long_t arg0);
    void btCollisionObject_activate_0(btCollisionObjectAddr self);
    void btCollisionObject_activate_1(btCollisionObjectAddr self, bool arg0);
    bool btCollisionObject_isActive_0(btCollisionObjectAddr self);
    bool btCollisionObject_isKinematicObject_0(btCollisionObjectAddr self);
    bool btCollisionObject_isStaticObject_0(btCollisionObjectAddr self);
    bool btCollisionObject_isStaticOrKinematicObject_0(btCollisionObjectAddr self);
    double btCollisionObject_getRestitution_0(btCollisionObjectAddr self);
    void btCollisionObject_setRestitution_1(btCollisionObjectAddr self, double arg0);
    double btCollisionObject_getFriction_0(btCollisionObjectAddr self);
    void btCollisionObject_setFriction_1(btCollisionObjectAddr self, double arg0);
    double btCollisionObject_getRollingFriction_0(btCollisionObjectAddr self);
    void btCollisionObject_setRollingFriction_1(btCollisionObjectAddr self, double arg0);
    long_t btCollisionObject_getCollisionFlags_0(btCollisionObjectAddr self);
    void btCollisionObject_setCollisionFlags_1(btCollisionObjectAddr self, long_t arg0);
    btTransformAddr btCollisionObject_getWorldTransform_0(btCollisionObjectAddr self);
    void btCollisionObject_setWorldTransform_1(btCollisionObjectAddr self, btTransformAddr arg0);
    void btCollisionObject_setCollisionShape_1(btCollisionObjectAddr self, btCollisionShapeAddr arg0);
    double btCollisionObject_getCcdMotionThreshold_0(btCollisionObjectAddr self);
    void btCollisionObject_setCcdMotionThreshold_1(btCollisionObjectAddr self, double arg0);
    double btCollisionObject_getCcdSweptSphereRadius_0(btCollisionObjectAddr self);
    void btCollisionObject_setCcdSweptSphereRadius_1(btCollisionObjectAddr self, double arg0);
    long_t btCollisionObject_getUserIndex_0(btCollisionObjectAddr self);
    void btCollisionObject_setUserIndex_1(btCollisionObjectAddr self, long_t arg0);
    VoidPtrAddr btCollisionObject_getUserPointer_0(btCollisionObjectAddr self);
    void btCollisionObject_setUserPointer_1(btCollisionObjectAddr self, VoidPtrAddr arg0);
    btVector3Addr btCollisionObject_getInterpolationAngularVelocity_0(btCollisionObjectAddr self);
    void btCollisionObject_setInterpolationAngularVelocity_1(btCollisionObjectAddr self, btVector3Addr arg0);
    btVector3Addr btCollisionObject_getInterpolationLinearVelocity_0(btCollisionObjectAddr self);
    void btCollisionObject_setInterpolationLinearVelocity_1(btCollisionObjectAddr self, btVector3Addr arg0);
    btBroadphaseProxyAddr btCollisionObject_getBroadphaseHandle_0(btCollisionObjectAddr self);
    long_t btCollisionObject_getActivationState_0(btCollisionObjectAddr self);
    void btCollisionObject___destroy___0(btCollisionObjectAddr self);

    // btTypedConstraint
  
    void btTypedConstraint_enableFeedback_1(btTypedConstraintAddr self, bool arg0);
    double btTypedConstraint_getBreakingImpulseThreshold_0(btTypedConstraintAddr self);
    void btTypedConstraint_setBreakingImpulseThreshold_1(btTypedConstraintAddr self, double arg0);
    double btTypedConstraint_getParam_2(btTypedConstraintAddr self, long_t arg0, long_t arg1);
    void btTypedConstraint_setParam_3(btTypedConstraintAddr self, long_t arg0, double arg1, long_t arg2);
    void btTypedConstraint___destroy___0(btTypedConstraintAddr self);

    // btCollisionWorld
  
    btDispatcherAddr btCollisionWorld_getDispatcher_0(btCollisionWorldAddr self);
    void btCollisionWorld_rayTest_3(btCollisionWorldAddr self, btVector3Addr arg0, btVector3Addr arg1, RayResultCallbackAddr arg2);
    btOverlappingPairCacheAddr btCollisionWorld_getPairCache_0(btCollisionWorldAddr self);
    btDispatcherInfoAddr btCollisionWorld_getDispatchInfo_0(btCollisionWorldAddr self);
    void btCollisionWorld_addCollisionObject_1(btCollisionWorldAddr self, btCollisionObjectAddr arg0);
    void btCollisionWorld_addCollisionObject_2(btCollisionWorldAddr self, btCollisionObjectAddr arg0, long_t arg1);
    void btCollisionWorld_addCollisionObject_3(btCollisionWorldAddr self, btCollisionObjectAddr arg0, long_t arg1, long_t arg2);
    void btCollisionWorld_removeCollisionObject_1(btCollisionWorldAddr self, btCollisionObjectAddr arg0);
    btBroadphaseInterfaceAddr btCollisionWorld_getBroadphase_0(btCollisionWorldAddr self);
    void btCollisionWorld_convexSweepTest_5(btCollisionWorldAddr self, btConvexShapeAddr arg0, btTransformAddr arg1, btTransformAddr arg2, ConvexResultCallbackAddr arg3, double arg4);
    void btCollisionWorld_contactPairTest_3(btCollisionWorldAddr self, btCollisionObjectAddr arg0, btCollisionObjectAddr arg1, ContactResultCallbackAddr arg2);
    void btCollisionWorld_contactTest_2(btCollisionWorldAddr self, btCollisionObjectAddr arg0, ContactResultCallbackAddr arg1);
    void btCollisionWorld_updateSingleAabb_1(btCollisionWorldAddr self, btCollisionObjectAddr arg0);
    void btCollisionWorld___destroy___0(btCollisionWorldAddr self);

    // btConcaveShape
  
    void btConcaveShape_setLocalScaling_1(btConcaveShapeAddr self, btVector3Addr arg0);
    btVector3Addr btConcaveShape_getLocalScaling_0(btConcaveShapeAddr self);
    void btConcaveShape_calculateLocalInertia_2(btConcaveShapeAddr self, double arg0, btVector3Addr arg1);
    void btConcaveShape___destroy___0(btConcaveShapeAddr self);

    // btCapsuleShape
  
    btCapsuleShapeAddr btCapsuleShape_btCapsuleShape_2(double arg0, double arg1);
    void btCapsuleShape_setMargin_1(btCapsuleShapeAddr self, double arg0);
    double btCapsuleShape_getMargin_0(btCapsuleShapeAddr self);
    long_t btCapsuleShape_getUpAxis_0(btCapsuleShapeAddr self);
    double btCapsuleShape_getRadius_0(btCapsuleShapeAddr self);
    double btCapsuleShape_getHalfHeight_0(btCapsuleShapeAddr self);
    void btCapsuleShape_setLocalScaling_1(btCapsuleShapeAddr self, btVector3Addr arg0);
    btVector3Addr btCapsuleShape_getLocalScaling_0(btCapsuleShapeAddr self);
    void btCapsuleShape_calculateLocalInertia_2(btCapsuleShapeAddr self, double arg0, btVector3Addr arg1);
    void btCapsuleShape___destroy___0(btCapsuleShapeAddr self);

    // btGImpactShapeInterface
  
    void btGImpactShapeInterface_updateBound_0(btGImpactShapeInterfaceAddr self);
    void btGImpactShapeInterface_setLocalScaling_1(btGImpactShapeInterfaceAddr self, btVector3Addr arg0);
    btVector3Addr btGImpactShapeInterface_getLocalScaling_0(btGImpactShapeInterfaceAddr self);
    void btGImpactShapeInterface_calculateLocalInertia_2(btGImpactShapeInterfaceAddr self, double arg0, btVector3Addr arg1);
    void btGImpactShapeInterface___destroy___0(btGImpactShapeInterfaceAddr self);

    // btDynamicsWorld
  
    void btDynamicsWorld_addAction_1(btDynamicsWorldAddr self, btActionInterfaceAddr arg0);
    void btDynamicsWorld_removeAction_1(btDynamicsWorldAddr self, btActionInterfaceAddr arg0);
    btContactSolverInfoAddr btDynamicsWorld_getSolverInfo_0(btDynamicsWorldAddr self);
    btDispatcherAddr btDynamicsWorld_getDispatcher_0(btDynamicsWorldAddr self);
    void btDynamicsWorld_rayTest_3(btDynamicsWorldAddr self, btVector3Addr arg0, btVector3Addr arg1, RayResultCallbackAddr arg2);
    btOverlappingPairCacheAddr btDynamicsWorld_getPairCache_0(btDynamicsWorldAddr self);
    btDispatcherInfoAddr btDynamicsWorld_getDispatchInfo_0(btDynamicsWorldAddr self);
    void btDynamicsWorld_addCollisionObject_1(btDynamicsWorldAddr self, btCollisionObjectAddr arg0);
    void btDynamicsWorld_addCollisionObject_2(btDynamicsWorldAddr self, btCollisionObjectAddr arg0, long_t arg1);
    void btDynamicsWorld_addCollisionObject_3(btDynamicsWorldAddr self, btCollisionObjectAddr arg0, long_t arg1, long_t arg2);
    void btDynamicsWorld_removeCollisionObject_1(btDynamicsWorldAddr self, btCollisionObjectAddr arg0);
    btBroadphaseInterfaceAddr btDynamicsWorld_getBroadphase_0(btDynamicsWorldAddr self);
    void btDynamicsWorld_convexSweepTest_5(btDynamicsWorldAddr self, btConvexShapeAddr arg0, btTransformAddr arg1, btTransformAddr arg2, ConvexResultCallbackAddr arg3, double arg4);
    void btDynamicsWorld_contactPairTest_3(btDynamicsWorldAddr self, btCollisionObjectAddr arg0, btCollisionObjectAddr arg1, ContactResultCallbackAddr arg2);
    void btDynamicsWorld_contactTest_2(btDynamicsWorldAddr self, btCollisionObjectAddr arg0, ContactResultCallbackAddr arg1);
    void btDynamicsWorld_updateSingleAabb_1(btDynamicsWorldAddr self, btCollisionObjectAddr arg0);
    void btDynamicsWorld___destroy___0(btDynamicsWorldAddr self);

    // btTriangleMeshShape
  
    void btTriangleMeshShape_setLocalScaling_1(btTriangleMeshShapeAddr self, btVector3Addr arg0);
    btVector3Addr btTriangleMeshShape_getLocalScaling_0(btTriangleMeshShapeAddr self);
    void btTriangleMeshShape_calculateLocalInertia_2(btTriangleMeshShapeAddr self, double arg0, btVector3Addr arg1);
    void btTriangleMeshShape___destroy___0(btTriangleMeshShapeAddr self);

    // btGhostObject
  
    btGhostObjectAddr btGhostObject_btGhostObject_0();
    long_t btGhostObject_getNumOverlappingObjects_0(btGhostObjectAddr self);
    btCollisionObjectAddr btGhostObject_getOverlappingObject_1(btGhostObjectAddr self, long_t arg0);
    void btGhostObject_setAnisotropicFriction_2(btGhostObjectAddr self, btVector3Addr arg0, long_t arg1);
    btCollisionShapeAddr btGhostObject_getCollisionShape_0(btGhostObjectAddr self);
    void btGhostObject_setContactProcessingThreshold_1(btGhostObjectAddr self, double arg0);
    void btGhostObject_setActivationState_1(btGhostObjectAddr self, long_t arg0);
    void btGhostObject_forceActivationState_1(btGhostObjectAddr self, long_t arg0);
    void btGhostObject_activate_0(btGhostObjectAddr self);
    void btGhostObject_activate_1(btGhostObjectAddr self, bool arg0);
    bool btGhostObject_isActive_0(btGhostObjectAddr self);
    bool btGhostObject_isKinematicObject_0(btGhostObjectAddr self);
    bool btGhostObject_isStaticObject_0(btGhostObjectAddr self);
    bool btGhostObject_isStaticOrKinematicObject_0(btGhostObjectAddr self);
    double btGhostObject_getRestitution_0(btGhostObjectAddr self);
    void btGhostObject_setRestitution_1(btGhostObjectAddr self, double arg0);
    double btGhostObject_getFriction_0(btGhostObjectAddr self);
    void btGhostObject_setFriction_1(btGhostObjectAddr self, double arg0);
    double btGhostObject_getRollingFriction_0(btGhostObjectAddr self);
    void btGhostObject_setRollingFriction_1(btGhostObjectAddr self, double arg0);
    long_t btGhostObject_getCollisionFlags_0(btGhostObjectAddr self);
    void btGhostObject_setCollisionFlags_1(btGhostObjectAddr self, long_t arg0);
    btTransformAddr btGhostObject_getWorldTransform_0(btGhostObjectAddr self);
    void btGhostObject_setWorldTransform_1(btGhostObjectAddr self, btTransformAddr arg0);
    void btGhostObject_setCollisionShape_1(btGhostObjectAddr self, btCollisionShapeAddr arg0);
    double btGhostObject_getCcdMotionThreshold_0(btGhostObjectAddr self);
    void btGhostObject_setCcdMotionThreshold_1(btGhostObjectAddr self, double arg0);
    double btGhostObject_getCcdSweptSphereRadius_0(btGhostObjectAddr self);
    void btGhostObject_setCcdSweptSphereRadius_1(btGhostObjectAddr self, double arg0);
    long_t btGhostObject_getUserIndex_0(btGhostObjectAddr self);
    void btGhostObject_setUserIndex_1(btGhostObjectAddr self, long_t arg0);
    VoidPtrAddr btGhostObject_getUserPointer_0(btGhostObjectAddr self);
    void btGhostObject_setUserPointer_1(btGhostObjectAddr self, VoidPtrAddr arg0);
    btVector3Addr btGhostObject_getInterpolationAngularVelocity_0(btGhostObjectAddr self);
    void btGhostObject_setInterpolationAngularVelocity_1(btGhostObjectAddr self, btVector3Addr arg0);
    btVector3Addr btGhostObject_getInterpolationLinearVelocity_0(btGhostObjectAddr self);
    void btGhostObject_setInterpolationLinearVelocity_1(btGhostObjectAddr self, btVector3Addr arg0);
    btBroadphaseProxyAddr btGhostObject_getBroadphaseHandle_0(btGhostObjectAddr self);
    long_t btGhostObject_getActivationState_0(btGhostObjectAddr self);
    void btGhostObject___destroy___0(btGhostObjectAddr self);

    // btConeShape
  
    btConeShapeAddr btConeShape_btConeShape_2(double arg0, double arg1);
    void btConeShape_setLocalScaling_1(btConeShapeAddr self, btVector3Addr arg0);
    btVector3Addr btConeShape_getLocalScaling_0(btConeShapeAddr self);
    void btConeShape_calculateLocalInertia_2(btConeShapeAddr self, double arg0, btVector3Addr arg1);
    void btConeShape___destroy___0(btConeShapeAddr self);

    // btActionInterface
  
    void btActionInterface_updateAction_2(btActionInterfaceAddr self, btCollisionWorldAddr arg0, double arg1);
    void btActionInterface___destroy___0(btActionInterfaceAddr self);

    // btVector3
  
    btVector3Addr btVector3_btVector3_0();
    btVector3Addr btVector3_btVector3_3(double arg0, double arg1, double arg2);
    double btVector3_length_0(btVector3Addr self);
    double btVector3_x_0(btVector3Addr self);
    double btVector3_y_0(btVector3Addr self);
    double btVector3_z_0(btVector3Addr self);
    void btVector3_setX_1(btVector3Addr self, double arg0);
    void btVector3_setY_1(btVector3Addr self, double arg0);
    void btVector3_setZ_1(btVector3Addr self, double arg0);
    void btVector3_setValue_3(btVector3Addr self, double arg0, double arg1, double arg2);
    void btVector3_normalize_0(btVector3Addr self);
    btVector3Addr btVector3_rotate_2(btVector3Addr self, btVector3Addr arg0, double arg1);
    double btVector3_dot_1(btVector3Addr self, btVector3Addr arg0);
    btVector3Addr btVector3_op_mul_1(btVector3Addr self, double arg0);
    btVector3Addr btVector3_op_add_1(btVector3Addr self, btVector3Addr arg0);
    btVector3Addr btVector3_op_sub_1(btVector3Addr self, btVector3Addr arg0);
    void btVector3___destroy___0(btVector3Addr self);

    // btVehicleRaycaster
  
    void btVehicleRaycaster_castRay_3(btVehicleRaycasterAddr self, btVector3Addr arg0, btVector3Addr arg1, btVehicleRaycasterResultAddr arg2);
    void btVehicleRaycaster___destroy___0(btVehicleRaycasterAddr self);

    // btQuadWord
  
    double btQuadWord_x_0(btQuadWordAddr self);
    double btQuadWord_y_0(btQuadWordAddr self);
    double btQuadWord_z_0(btQuadWordAddr self);
    double btQuadWord_w_0(btQuadWordAddr self);
    void btQuadWord_setX_1(btQuadWordAddr self, double arg0);
    void btQuadWord_setY_1(btQuadWordAddr self, double arg0);
    void btQuadWord_setZ_1(btQuadWordAddr self, double arg0);
    void btQuadWord_setW_1(btQuadWordAddr self, double arg0);
    void btQuadWord___destroy___0(btQuadWordAddr self);

    // btCylinderShape
  
    btCylinderShapeAddr btCylinderShape_btCylinderShape_1(btVector3Addr arg0);
    void btCylinderShape_setMargin_1(btCylinderShapeAddr self, double arg0);
    double btCylinderShape_getMargin_0(btCylinderShapeAddr self);
    void btCylinderShape_setLocalScaling_1(btCylinderShapeAddr self, btVector3Addr arg0);
    btVector3Addr btCylinderShape_getLocalScaling_0(btCylinderShapeAddr self);
    void btCylinderShape_calculateLocalInertia_2(btCylinderShapeAddr self, double arg0, btVector3Addr arg1);
    void btCylinderShape___destroy___0(btCylinderShapeAddr self);

    // btConvexShape
  
    void btConvexShape_setLocalScaling_1(btConvexShapeAddr self, btVector3Addr arg0);
    btVector3Addr btConvexShape_getLocalScaling_0(btConvexShapeAddr self);
    void btConvexShape_calculateLocalInertia_2(btConvexShapeAddr self, double arg0, btVector3Addr arg1);
    void btConvexShape_setMargin_1(btConvexShapeAddr self, double arg0);
    double btConvexShape_getMargin_0(btConvexShapeAddr self);
    void btConvexShape___destroy___0(btConvexShapeAddr self);

    // btDispatcher
  
    long_t btDispatcher_getNumManifolds_0(btDispatcherAddr self);
    btPersistentManifoldAddr btDispatcher_getManifoldByIndexInternal_1(btDispatcherAddr self, long_t arg0);
    void btDispatcher___destroy___0(btDispatcherAddr self);

    // btGeneric6DofConstraint
  
    btGeneric6DofConstraintAddr btGeneric6DofConstraint_btGeneric6DofConstraint_3(btRigidBodyAddr arg0, btTransformAddr arg1, bool arg2);
    btGeneric6DofConstraintAddr btGeneric6DofConstraint_btGeneric6DofConstraint_5(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3, bool arg4);
    void btGeneric6DofConstraint_setLinearLowerLimit_1(btGeneric6DofConstraintAddr self, btVector3Addr arg0);
    void btGeneric6DofConstraint_setLinearUpperLimit_1(btGeneric6DofConstraintAddr self, btVector3Addr arg0);
    void btGeneric6DofConstraint_setAngularLowerLimit_1(btGeneric6DofConstraintAddr self, btVector3Addr arg0);
    void btGeneric6DofConstraint_setAngularUpperLimit_1(btGeneric6DofConstraintAddr self, btVector3Addr arg0);
    btTransformAddr btGeneric6DofConstraint_getFrameOffsetA_0(btGeneric6DofConstraintAddr self);
    void btGeneric6DofConstraint_enableFeedback_1(btGeneric6DofConstraintAddr self, bool arg0);
    double btGeneric6DofConstraint_getBreakingImpulseThreshold_0(btGeneric6DofConstraintAddr self);
    void btGeneric6DofConstraint_setBreakingImpulseThreshold_1(btGeneric6DofConstraintAddr self, double arg0);
    double btGeneric6DofConstraint_getParam_2(btGeneric6DofConstraintAddr self, long_t arg0, long_t arg1);
    void btGeneric6DofConstraint_setParam_3(btGeneric6DofConstraintAddr self, long_t arg0, double arg1, long_t arg2);
    void btGeneric6DofConstraint___destroy___0(btGeneric6DofConstraintAddr self);

    // btStridingMeshInterface
  
    void btStridingMeshInterface___destroy___0(btStridingMeshInterfaceAddr self);

    // btMotionState
  
    void btMotionState_getWorldTransform_1(btMotionStateAddr self, btTransformAddr arg0);
    void btMotionState_setWorldTransform_1(btMotionStateAddr self, btTransformAddr arg0);
    void btMotionState___destroy___0(btMotionStateAddr self);

    // ConvexResultCallback
  
    bool ConvexResultCallback_hasHit_0(ConvexResultCallbackAddr self);
    long_t ConvexResultCallback_get_m_collisionFilterGroup_0(ConvexResultCallbackAddr self);
    void ConvexResultCallback_set_m_collisionFilterGroup_1(ConvexResultCallbackAddr self, long_t arg0);
    long_t ConvexResultCallback_get_m_collisionFilterMask_0(ConvexResultCallbackAddr self);
    void ConvexResultCallback_set_m_collisionFilterMask_1(ConvexResultCallbackAddr self, long_t arg0);
    double ConvexResultCallback_get_m_closestHitFraction_0(ConvexResultCallbackAddr self);
    void ConvexResultCallback_set_m_closestHitFraction_1(ConvexResultCallbackAddr self, double arg0);
    void ConvexResultCallback___destroy___0(ConvexResultCallbackAddr self);

    // ContactResultCallback
  
    double ContactResultCallback_addSingleResult_7(ContactResultCallbackAddr self, btManifoldPointAddr arg0, btCollisionObjectWrapperAddr arg1, long_t arg2, long_t arg3, btCollisionObjectWrapperAddr arg4, long_t arg5, long_t arg6);
    void ContactResultCallback___destroy___0(ContactResultCallbackAddr self);

    // RayResultCallback
  
    bool RayResultCallback_hasHit_0(RayResultCallbackAddr self);
    long_t RayResultCallback_get_m_collisionFilterGroup_0(RayResultCallbackAddr self);
    void RayResultCallback_set_m_collisionFilterGroup_1(RayResultCallbackAddr self, long_t arg0);
    long_t RayResultCallback_get_m_collisionFilterMask_0(RayResultCallbackAddr self);
    void RayResultCallback_set_m_collisionFilterMask_1(RayResultCallbackAddr self, long_t arg0);
    double RayResultCallback_get_m_closestHitFraction_0(RayResultCallbackAddr self);
    void RayResultCallback_set_m_closestHitFraction_1(RayResultCallbackAddr self, double arg0);
    btCollisionObjectAddr RayResultCallback_get_m_collisionObject_0(RayResultCallbackAddr self);
    void RayResultCallback_set_m_collisionObject_1(RayResultCallbackAddr self, btCollisionObjectAddr arg0);
    void RayResultCallback___destroy___0(RayResultCallbackAddr self);

    // btMatrix3x3
  
    void btMatrix3x3_setEulerZYX_3(btMatrix3x3Addr self, double arg0, double arg1, double arg2);
    void btMatrix3x3_getRotation_1(btMatrix3x3Addr self, btQuaternionAddr arg0);
    btVector3Addr btMatrix3x3_getRow_1(btMatrix3x3Addr self, long_t arg0);
    void btMatrix3x3___destroy___0(btMatrix3x3Addr self);

    // btDispatcherInfo
  
    double btDispatcherInfo_get_m_timeStep_0(btDispatcherInfoAddr self);
    void btDispatcherInfo_set_m_timeStep_1(btDispatcherInfoAddr self, double arg0);
    long_t btDispatcherInfo_get_m_stepCount_0(btDispatcherInfoAddr self);
    void btDispatcherInfo_set_m_stepCount_1(btDispatcherInfoAddr self, long_t arg0);
    long_t btDispatcherInfo_get_m_dispatchFunc_0(btDispatcherInfoAddr self);
    void btDispatcherInfo_set_m_dispatchFunc_1(btDispatcherInfoAddr self, long_t arg0);
    double btDispatcherInfo_get_m_timeOfImpact_0(btDispatcherInfoAddr self);
    void btDispatcherInfo_set_m_timeOfImpact_1(btDispatcherInfoAddr self, double arg0);
    bool btDispatcherInfo_get_m_useContinuous_0(btDispatcherInfoAddr self);
    void btDispatcherInfo_set_m_useContinuous_1(btDispatcherInfoAddr self, bool arg0);
    bool btDispatcherInfo_get_m_enableSatConvex_0(btDispatcherInfoAddr self);
    void btDispatcherInfo_set_m_enableSatConvex_1(btDispatcherInfoAddr self, bool arg0);
    bool btDispatcherInfo_get_m_enableSPU_0(btDispatcherInfoAddr self);
    void btDispatcherInfo_set_m_enableSPU_1(btDispatcherInfoAddr self, bool arg0);
    bool btDispatcherInfo_get_m_useEpa_0(btDispatcherInfoAddr self);
    void btDispatcherInfo_set_m_useEpa_1(btDispatcherInfoAddr self, bool arg0);
    double btDispatcherInfo_get_m_allowedCcdPenetration_0(btDispatcherInfoAddr self);
    void btDispatcherInfo_set_m_allowedCcdPenetration_1(btDispatcherInfoAddr self, double arg0);
    bool btDispatcherInfo_get_m_useConvexConservativeDistanceUtil_0(btDispatcherInfoAddr self);
    void btDispatcherInfo_set_m_useConvexConservativeDistanceUtil_1(btDispatcherInfoAddr self, bool arg0);
    double btDispatcherInfo_get_m_convexConservativeDistanceThreshold_0(btDispatcherInfoAddr self);
    void btDispatcherInfo_set_m_convexConservativeDistanceThreshold_1(btDispatcherInfoAddr self, double arg0);
    void btDispatcherInfo___destroy___0(btDispatcherInfoAddr self);

    // LayaMotionState
  
    LayaMotionStateAddr LayaMotionState_LayaMotionState_0();
    void LayaMotionState_getWorldTransform_1(LayaMotionStateAddr self, btTransformAddr arg0);
    void LayaMotionState_setWorldTransform_1(LayaMotionStateAddr self, btTransformAddr arg0);
    void LayaMotionState_setNativeGetWorldTransform_1(LayaMotionStateAddr self, JSValueAsParam arg0);
    void LayaMotionState_setNativeSetWorldTransform_1(LayaMotionStateAddr self, JSValueAsParam arg0);
    void LayaMotionState_setRigidbody_1(LayaMotionStateAddr self, JsValue arg0);
    void LayaMotionState___destroy___0(LayaMotionStateAddr self);

    // btWheelInfoConstructionInfo
  
    btVector3Addr btWheelInfoConstructionInfo_get_m_chassisConnectionCS_0(btWheelInfoConstructionInfoAddr self);
    void btWheelInfoConstructionInfo_set_m_chassisConnectionCS_1(btWheelInfoConstructionInfoAddr self, btVector3Addr arg0);
    btVector3Addr btWheelInfoConstructionInfo_get_m_wheelDirectionCS_0(btWheelInfoConstructionInfoAddr self);
    void btWheelInfoConstructionInfo_set_m_wheelDirectionCS_1(btWheelInfoConstructionInfoAddr self, btVector3Addr arg0);
    btVector3Addr btWheelInfoConstructionInfo_get_m_wheelAxleCS_0(btWheelInfoConstructionInfoAddr self);
    void btWheelInfoConstructionInfo_set_m_wheelAxleCS_1(btWheelInfoConstructionInfoAddr self, btVector3Addr arg0);
    double btWheelInfoConstructionInfo_get_m_suspensionRestLength_0(btWheelInfoConstructionInfoAddr self);
    void btWheelInfoConstructionInfo_set_m_suspensionRestLength_1(btWheelInfoConstructionInfoAddr self, double arg0);
    double btWheelInfoConstructionInfo_get_m_maxSuspensionTravelCm_0(btWheelInfoConstructionInfoAddr self);
    void btWheelInfoConstructionInfo_set_m_maxSuspensionTravelCm_1(btWheelInfoConstructionInfoAddr self, double arg0);
    double btWheelInfoConstructionInfo_get_m_wheelRadius_0(btWheelInfoConstructionInfoAddr self);
    void btWheelInfoConstructionInfo_set_m_wheelRadius_1(btWheelInfoConstructionInfoAddr self, double arg0);
    double btWheelInfoConstructionInfo_get_m_suspensionStiffness_0(btWheelInfoConstructionInfoAddr self);
    void btWheelInfoConstructionInfo_set_m_suspensionStiffness_1(btWheelInfoConstructionInfoAddr self, double arg0);
    double btWheelInfoConstructionInfo_get_m_wheelsDampingCompression_0(btWheelInfoConstructionInfoAddr self);
    void btWheelInfoConstructionInfo_set_m_wheelsDampingCompression_1(btWheelInfoConstructionInfoAddr self, double arg0);
    double btWheelInfoConstructionInfo_get_m_wheelsDampingRelaxation_0(btWheelInfoConstructionInfoAddr self);
    void btWheelInfoConstructionInfo_set_m_wheelsDampingRelaxation_1(btWheelInfoConstructionInfoAddr self, double arg0);
    double btWheelInfoConstructionInfo_get_m_frictionSlip_0(btWheelInfoConstructionInfoAddr self);
    void btWheelInfoConstructionInfo_set_m_frictionSlip_1(btWheelInfoConstructionInfoAddr self, double arg0);
    double btWheelInfoConstructionInfo_get_m_maxSuspensionForce_0(btWheelInfoConstructionInfoAddr self);
    void btWheelInfoConstructionInfo_set_m_maxSuspensionForce_1(btWheelInfoConstructionInfoAddr self, double arg0);
    bool btWheelInfoConstructionInfo_get_m_bIsFrontWheel_0(btWheelInfoConstructionInfoAddr self);
    void btWheelInfoConstructionInfo_set_m_bIsFrontWheel_1(btWheelInfoConstructionInfoAddr self, bool arg0);
    void btWheelInfoConstructionInfo___destroy___0(btWheelInfoConstructionInfoAddr self);

    // btConvexTriangleMeshShape
  
    btConvexTriangleMeshShapeAddr btConvexTriangleMeshShape_btConvexTriangleMeshShape_1(btStridingMeshInterfaceAddr arg0);
    btConvexTriangleMeshShapeAddr btConvexTriangleMeshShape_btConvexTriangleMeshShape_2(btStridingMeshInterfaceAddr arg0, bool arg1);
    void btConvexTriangleMeshShape_setLocalScaling_1(btConvexTriangleMeshShapeAddr self, btVector3Addr arg0);
    btVector3Addr btConvexTriangleMeshShape_getLocalScaling_0(btConvexTriangleMeshShapeAddr self);
    void btConvexTriangleMeshShape_calculateLocalInertia_2(btConvexTriangleMeshShapeAddr self, double arg0, btVector3Addr arg1);
    void btConvexTriangleMeshShape_setMargin_1(btConvexTriangleMeshShapeAddr self, double arg0);
    double btConvexTriangleMeshShape_getMargin_0(btConvexTriangleMeshShapeAddr self);
    void btConvexTriangleMeshShape___destroy___0(btConvexTriangleMeshShapeAddr self);

    // btBroadphaseInterface
  
    void btBroadphaseInterface___destroy___0(btBroadphaseInterfaceAddr self);

    // btDefaultCollisionConfiguration
  
    btDefaultCollisionConfigurationAddr btDefaultCollisionConfiguration_btDefaultCollisionConfiguration_0();
    btDefaultCollisionConfigurationAddr btDefaultCollisionConfiguration_btDefaultCollisionConfiguration_1(btDefaultCollisionConstructionInfoAddr arg0);
    void btDefaultCollisionConfiguration___destroy___0(btDefaultCollisionConfigurationAddr self);

    // btRigidBodyConstructionInfo
  
    btRigidBodyConstructionInfoAddr btRigidBodyConstructionInfo_btRigidBodyConstructionInfo_3(double arg0, btMotionStateAddr arg1, btCollisionShapeAddr arg2);
    btRigidBodyConstructionInfoAddr btRigidBodyConstructionInfo_btRigidBodyConstructionInfo_4(double arg0, btMotionStateAddr arg1, btCollisionShapeAddr arg2, btVector3Addr arg3);
    double btRigidBodyConstructionInfo_get_m_linearDamping_0(btRigidBodyConstructionInfoAddr self);
    void btRigidBodyConstructionInfo_set_m_linearDamping_1(btRigidBodyConstructionInfoAddr self, double arg0);
    double btRigidBodyConstructionInfo_get_m_angularDamping_0(btRigidBodyConstructionInfoAddr self);
    void btRigidBodyConstructionInfo_set_m_angularDamping_1(btRigidBodyConstructionInfoAddr self, double arg0);
    double btRigidBodyConstructionInfo_get_m_friction_0(btRigidBodyConstructionInfoAddr self);
    void btRigidBodyConstructionInfo_set_m_friction_1(btRigidBodyConstructionInfoAddr self, double arg0);
    double btRigidBodyConstructionInfo_get_m_rollingFriction_0(btRigidBodyConstructionInfoAddr self);
    void btRigidBodyConstructionInfo_set_m_rollingFriction_1(btRigidBodyConstructionInfoAddr self, double arg0);
    double btRigidBodyConstructionInfo_get_m_restitution_0(btRigidBodyConstructionInfoAddr self);
    void btRigidBodyConstructionInfo_set_m_restitution_1(btRigidBodyConstructionInfoAddr self, double arg0);
    double btRigidBodyConstructionInfo_get_m_linearSleepingThreshold_0(btRigidBodyConstructionInfoAddr self);
    void btRigidBodyConstructionInfo_set_m_linearSleepingThreshold_1(btRigidBodyConstructionInfoAddr self, double arg0);
    double btRigidBodyConstructionInfo_get_m_angularSleepingThreshold_0(btRigidBodyConstructionInfoAddr self);
    void btRigidBodyConstructionInfo_set_m_angularSleepingThreshold_1(btRigidBodyConstructionInfoAddr self, double arg0);
    bool btRigidBodyConstructionInfo_get_m_additionalDamping_0(btRigidBodyConstructionInfoAddr self);
    void btRigidBodyConstructionInfo_set_m_additionalDamping_1(btRigidBodyConstructionInfoAddr self, bool arg0);
    double btRigidBodyConstructionInfo_get_m_additionalDampingFactor_0(btRigidBodyConstructionInfoAddr self);
    void btRigidBodyConstructionInfo_set_m_additionalDampingFactor_1(btRigidBodyConstructionInfoAddr self, double arg0);
    double btRigidBodyConstructionInfo_get_m_additionalLinearDampingThresholdSqr_0(btRigidBodyConstructionInfoAddr self);
    void btRigidBodyConstructionInfo_set_m_additionalLinearDampingThresholdSqr_1(btRigidBodyConstructionInfoAddr self, double arg0);
    double btRigidBodyConstructionInfo_get_m_additionalAngularDampingThresholdSqr_0(btRigidBodyConstructionInfoAddr self);
    void btRigidBodyConstructionInfo_set_m_additionalAngularDampingThresholdSqr_1(btRigidBodyConstructionInfoAddr self, double arg0);
    double btRigidBodyConstructionInfo_get_m_additionalAngularDampingFactor_0(btRigidBodyConstructionInfoAddr self);
    void btRigidBodyConstructionInfo_set_m_additionalAngularDampingFactor_1(btRigidBodyConstructionInfoAddr self, double arg0);
    void btRigidBodyConstructionInfo___destroy___0(btRigidBodyConstructionInfoAddr self);

    // btCollisionConfiguration
  
    void btCollisionConfiguration___destroy___0(btCollisionConfigurationAddr self);

    // btPersistentManifold
  
    btPersistentManifoldAddr btPersistentManifold_btPersistentManifold_0();
    btCollisionObjectAddr btPersistentManifold_getBody0_0(btPersistentManifoldAddr self);
    btCollisionObjectAddr btPersistentManifold_getBody1_0(btPersistentManifoldAddr self);
    long_t btPersistentManifold_getNumContacts_0(btPersistentManifoldAddr self);
    btManifoldPointAddr btPersistentManifold_getContactPoint_1(btPersistentManifoldAddr self, long_t arg0);
    void btPersistentManifold___destroy___0(btPersistentManifoldAddr self);

    // btCompoundShape
  
    btCompoundShapeAddr btCompoundShape_btCompoundShape_0();
    btCompoundShapeAddr btCompoundShape_btCompoundShape_1(bool arg0);
    void btCompoundShape_addChildShape_2(btCompoundShapeAddr self, btTransformAddr arg0, btCollisionShapeAddr arg1);
    void btCompoundShape_removeChildShapeByIndex_1(btCompoundShapeAddr self, long_t arg0);
    long_t btCompoundShape_getNumChildShapes_0(btCompoundShapeAddr self);
    btCollisionShapeAddr btCompoundShape_getChildShape_1(btCompoundShapeAddr self, long_t arg0);
    void btCompoundShape_setMargin_1(btCompoundShapeAddr self, double arg0);
    double btCompoundShape_getMargin_0(btCompoundShapeAddr self);
    void btCompoundShape_updateChildTransform_2(btCompoundShapeAddr self, long_t arg0, btTransformAddr arg1);
    void btCompoundShape_updateChildTransform_3(btCompoundShapeAddr self, long_t arg0, btTransformAddr arg1, bool arg2);
    void btCompoundShape_setLocalScaling_1(btCompoundShapeAddr self, btVector3Addr arg0);
    btVector3Addr btCompoundShape_getLocalScaling_0(btCompoundShapeAddr self);
    void btCompoundShape_calculateLocalInertia_2(btCompoundShapeAddr self, double arg0, btVector3Addr arg1);
    void btCompoundShape___destroy___0(btCompoundShapeAddr self);

    // ClosestConvexResultCallback
  
    ClosestConvexResultCallbackAddr ClosestConvexResultCallback_ClosestConvexResultCallback_2(btVector3Addr arg0, btVector3Addr arg1);
    bool ClosestConvexResultCallback_hasHit_0(ClosestConvexResultCallbackAddr self);
    btVector3Addr ClosestConvexResultCallback_get_m_convexFromWorld_0(ClosestConvexResultCallbackAddr self);
    void ClosestConvexResultCallback_set_m_convexFromWorld_1(ClosestConvexResultCallbackAddr self, btVector3Addr arg0);
    btVector3Addr ClosestConvexResultCallback_get_m_convexToWorld_0(ClosestConvexResultCallbackAddr self);
    void ClosestConvexResultCallback_set_m_convexToWorld_1(ClosestConvexResultCallbackAddr self, btVector3Addr arg0);
    btVector3Addr ClosestConvexResultCallback_get_m_hitNormalWorld_0(ClosestConvexResultCallbackAddr self);
    void ClosestConvexResultCallback_set_m_hitNormalWorld_1(ClosestConvexResultCallbackAddr self, btVector3Addr arg0);
    btVector3Addr ClosestConvexResultCallback_get_m_hitPointWorld_0(ClosestConvexResultCallbackAddr self);
    void ClosestConvexResultCallback_set_m_hitPointWorld_1(ClosestConvexResultCallbackAddr self, btVector3Addr arg0);
    btCollisionObjectAddr ClosestConvexResultCallback_get_m_hitCollisionObject_0(ClosestConvexResultCallbackAddr self);
    void ClosestConvexResultCallback_set_m_hitCollisionObject_1(ClosestConvexResultCallbackAddr self, btCollisionObjectAddr arg0);
    long_t ClosestConvexResultCallback_get_m_collisionFilterGroup_0(ClosestConvexResultCallbackAddr self);
    void ClosestConvexResultCallback_set_m_collisionFilterGroup_1(ClosestConvexResultCallbackAddr self, long_t arg0);
    long_t ClosestConvexResultCallback_get_m_collisionFilterMask_0(ClosestConvexResultCallbackAddr self);
    void ClosestConvexResultCallback_set_m_collisionFilterMask_1(ClosestConvexResultCallbackAddr self, long_t arg0);
    double ClosestConvexResultCallback_get_m_closestHitFraction_0(ClosestConvexResultCallbackAddr self);
    void ClosestConvexResultCallback_set_m_closestHitFraction_1(ClosestConvexResultCallbackAddr self, double arg0);
    void ClosestConvexResultCallback___destroy___0(ClosestConvexResultCallbackAddr self);

    // AllHitsRayResultCallback
  
    AllHitsRayResultCallbackAddr AllHitsRayResultCallback_AllHitsRayResultCallback_2(btVector3Addr arg0, btVector3Addr arg1);
    bool AllHitsRayResultCallback_hasHit_0(AllHitsRayResultCallbackAddr self);
    tBtCollisionObjectArrayAddr AllHitsRayResultCallback_get_m_collisionObjects_0(AllHitsRayResultCallbackAddr self);
    void AllHitsRayResultCallback_set_m_collisionObjects_1(AllHitsRayResultCallbackAddr self, tBtCollisionObjectArrayAddr arg0);
    btVector3Addr AllHitsRayResultCallback_get_m_rayFromWorld_0(AllHitsRayResultCallbackAddr self);
    void AllHitsRayResultCallback_set_m_rayFromWorld_1(AllHitsRayResultCallbackAddr self, btVector3Addr arg0);
    btVector3Addr AllHitsRayResultCallback_get_m_rayToWorld_0(AllHitsRayResultCallbackAddr self);
    void AllHitsRayResultCallback_set_m_rayToWorld_1(AllHitsRayResultCallbackAddr self, btVector3Addr arg0);
    tVector3ArrayAddr AllHitsRayResultCallback_get_m_hitNormalWorld_0(AllHitsRayResultCallbackAddr self);
    void AllHitsRayResultCallback_set_m_hitNormalWorld_1(AllHitsRayResultCallbackAddr self, tVector3ArrayAddr arg0);
    tVector3ArrayAddr AllHitsRayResultCallback_get_m_hitPointWorld_0(AllHitsRayResultCallbackAddr self);
    void AllHitsRayResultCallback_set_m_hitPointWorld_1(AllHitsRayResultCallbackAddr self, tVector3ArrayAddr arg0);
    tScalarArrayAddr AllHitsRayResultCallback_get_m_hitFractions_0(AllHitsRayResultCallbackAddr self);
    void AllHitsRayResultCallback_set_m_hitFractions_1(AllHitsRayResultCallbackAddr self, tScalarArrayAddr arg0);
    long_t AllHitsRayResultCallback_get_m_collisionFilterGroup_0(AllHitsRayResultCallbackAddr self);
    void AllHitsRayResultCallback_set_m_collisionFilterGroup_1(AllHitsRayResultCallbackAddr self, long_t arg0);
    long_t AllHitsRayResultCallback_get_m_collisionFilterMask_0(AllHitsRayResultCallbackAddr self);
    void AllHitsRayResultCallback_set_m_collisionFilterMask_1(AllHitsRayResultCallbackAddr self, long_t arg0);
    double AllHitsRayResultCallback_get_m_closestHitFraction_0(AllHitsRayResultCallbackAddr self);
    void AllHitsRayResultCallback_set_m_closestHitFraction_1(AllHitsRayResultCallbackAddr self, double arg0);
    btCollisionObjectAddr AllHitsRayResultCallback_get_m_collisionObject_0(AllHitsRayResultCallbackAddr self);
    void AllHitsRayResultCallback_set_m_collisionObject_1(AllHitsRayResultCallbackAddr self, btCollisionObjectAddr arg0);
    void AllHitsRayResultCallback___destroy___0(AllHitsRayResultCallbackAddr self);

    // btDefaultVehicleRaycaster
  
    btDefaultVehicleRaycasterAddr btDefaultVehicleRaycaster_btDefaultVehicleRaycaster_1(btDynamicsWorldAddr arg0);
    void btDefaultVehicleRaycaster_castRay_3(btDefaultVehicleRaycasterAddr self, btVector3Addr arg0, btVector3Addr arg1, btVehicleRaycasterResultAddr arg2);
    void btDefaultVehicleRaycaster___destroy___0(btDefaultVehicleRaycasterAddr self);

    // btConstraintSetting
  
    btConstraintSettingAddr btConstraintSetting_btConstraintSetting_0();
    double btConstraintSetting_get_m_tau_0(btConstraintSettingAddr self);
    void btConstraintSetting_set_m_tau_1(btConstraintSettingAddr self, double arg0);
    double btConstraintSetting_get_m_damping_0(btConstraintSettingAddr self);
    void btConstraintSetting_set_m_damping_1(btConstraintSettingAddr self, double arg0);
    double btConstraintSetting_get_m_impulseClamp_0(btConstraintSettingAddr self);
    void btConstraintSetting_set_m_impulseClamp_1(btConstraintSettingAddr self, double arg0);
    void btConstraintSetting___destroy___0(btConstraintSettingAddr self);

    // LocalShapeInfo
  
    long_t LocalShapeInfo_get_m_shapePart_0(LocalShapeInfoAddr self);
    void LocalShapeInfo_set_m_shapePart_1(LocalShapeInfoAddr self, long_t arg0);
    long_t LocalShapeInfo_get_m_triangleIndex_0(LocalShapeInfoAddr self);
    void LocalShapeInfo_set_m_triangleIndex_1(LocalShapeInfoAddr self, long_t arg0);
    void LocalShapeInfo___destroy___0(LocalShapeInfoAddr self);

    // btRigidBody
  
    btRigidBodyAddr btRigidBody_btRigidBody_1(btRigidBodyConstructionInfoAddr arg0);
    btTransformAddr btRigidBody_getCenterOfMassTransform_0(btRigidBodyAddr self);
    void btRigidBody_setCenterOfMassTransform_1(btRigidBodyAddr self, btTransformAddr arg0);
    void btRigidBody_setSleepingThresholds_2(btRigidBodyAddr self, double arg0, double arg1);
    double btRigidBody_getLinearSleepingThreshold_0(btRigidBodyAddr self);
    double btRigidBody_getAngularSleepingThreshold_0(btRigidBodyAddr self);
    void btRigidBody_setDamping_2(btRigidBodyAddr self, double arg0, double arg1);
    void btRigidBody_setMassProps_2(btRigidBodyAddr self, double arg0, btVector3Addr arg1);
    void btRigidBody_setLinearFactor_1(btRigidBodyAddr self, btVector3Addr arg0);
    void btRigidBody_applyTorque_1(btRigidBodyAddr self, btVector3Addr arg0);
    void btRigidBody_applyForce_2(btRigidBodyAddr self, btVector3Addr arg0, btVector3Addr arg1);
    void btRigidBody_applyCentralForce_1(btRigidBodyAddr self, btVector3Addr arg0);
    void btRigidBody_applyTorqueImpulse_1(btRigidBodyAddr self, btVector3Addr arg0);
    void btRigidBody_applyImpulse_2(btRigidBodyAddr self, btVector3Addr arg0, btVector3Addr arg1);
    void btRigidBody_applyCentralImpulse_1(btRigidBodyAddr self, btVector3Addr arg0);
    void btRigidBody_updateInertiaTensor_0(btRigidBodyAddr self);
    btVector3Addr btRigidBody_getLinearVelocity_0(btRigidBodyAddr self);
    btVector3Addr btRigidBody_getAngularVelocity_0(btRigidBodyAddr self);
    void btRigidBody_setLinearVelocity_1(btRigidBodyAddr self, btVector3Addr arg0);
    void btRigidBody_setAngularVelocity_1(btRigidBodyAddr self, btVector3Addr arg0);
    btMotionStateAddr btRigidBody_getMotionState_0(btRigidBodyAddr self);
    void btRigidBody_setMotionState_1(btRigidBodyAddr self, btMotionStateAddr arg0);
    void btRigidBody_setAngularFactor_1(btRigidBodyAddr self, btVector3Addr arg0);
    btRigidBodyAddr btRigidBody_upcast_1(btRigidBodyAddr self, btCollisionObjectAddr arg0);
    void btRigidBody_getAabb_2(btRigidBodyAddr self, btVector3Addr arg0, btVector3Addr arg1);
    void btRigidBody_applyGravity_0(btRigidBodyAddr self);
    btVector3Addr btRigidBody_getGravity_0(btRigidBodyAddr self);
    void btRigidBody_setGravity_1(btRigidBodyAddr self, btVector3Addr arg0);
    btVector3Addr btRigidBody_getTotalForce_0(btRigidBodyAddr self);
    btVector3Addr btRigidBody_getTotalTorque_0(btRigidBodyAddr self);
    long_t btRigidBody_getFlags_0(btRigidBodyAddr self);
    void btRigidBody_setFlags_1(btRigidBodyAddr self, long_t arg0);
    void btRigidBody_clearForces_0(btRigidBodyAddr self);
    void btRigidBody_setAnisotropicFriction_2(btRigidBodyAddr self, btVector3Addr arg0, long_t arg1);
    btCollisionShapeAddr btRigidBody_getCollisionShape_0(btRigidBodyAddr self);
    void btRigidBody_setContactProcessingThreshold_1(btRigidBodyAddr self, double arg0);
    void btRigidBody_setActivationState_1(btRigidBodyAddr self, long_t arg0);
    void btRigidBody_forceActivationState_1(btRigidBodyAddr self, long_t arg0);
    void btRigidBody_activate_0(btRigidBodyAddr self);
    void btRigidBody_activate_1(btRigidBodyAddr self, bool arg0);
    bool btRigidBody_isActive_0(btRigidBodyAddr self);
    bool btRigidBody_isKinematicObject_0(btRigidBodyAddr self);
    bool btRigidBody_isStaticObject_0(btRigidBodyAddr self);
    bool btRigidBody_isStaticOrKinematicObject_0(btRigidBodyAddr self);
    double btRigidBody_getRestitution_0(btRigidBodyAddr self);
    void btRigidBody_setRestitution_1(btRigidBodyAddr self, double arg0);
    double btRigidBody_getFriction_0(btRigidBodyAddr self);
    void btRigidBody_setFriction_1(btRigidBodyAddr self, double arg0);
    double btRigidBody_getRollingFriction_0(btRigidBodyAddr self);
    void btRigidBody_setRollingFriction_1(btRigidBodyAddr self, double arg0);
    long_t btRigidBody_getCollisionFlags_0(btRigidBodyAddr self);
    void btRigidBody_setCollisionFlags_1(btRigidBodyAddr self, long_t arg0);
    btTransformAddr btRigidBody_getWorldTransform_0(btRigidBodyAddr self);
    void btRigidBody_setWorldTransform_1(btRigidBodyAddr self, btTransformAddr arg0);
    void btRigidBody_setCollisionShape_1(btRigidBodyAddr self, btCollisionShapeAddr arg0);
    double btRigidBody_getCcdMotionThreshold_0(btRigidBodyAddr self);
    void btRigidBody_setCcdMotionThreshold_1(btRigidBodyAddr self, double arg0);
    double btRigidBody_getCcdSweptSphereRadius_0(btRigidBodyAddr self);
    void btRigidBody_setCcdSweptSphereRadius_1(btRigidBodyAddr self, double arg0);
    long_t btRigidBody_getUserIndex_0(btRigidBodyAddr self);
    void btRigidBody_setUserIndex_1(btRigidBodyAddr self, long_t arg0);
    VoidPtrAddr btRigidBody_getUserPointer_0(btRigidBodyAddr self);
    void btRigidBody_setUserPointer_1(btRigidBodyAddr self, VoidPtrAddr arg0);
    btVector3Addr btRigidBody_getInterpolationAngularVelocity_0(btRigidBodyAddr self);
    void btRigidBody_setInterpolationAngularVelocity_1(btRigidBodyAddr self, btVector3Addr arg0);
    btVector3Addr btRigidBody_getInterpolationLinearVelocity_0(btRigidBodyAddr self);
    void btRigidBody_setInterpolationLinearVelocity_1(btRigidBodyAddr self, btVector3Addr arg0);
    btBroadphaseProxyAddr btRigidBody_getBroadphaseHandle_0(btRigidBodyAddr self);
    long_t btRigidBody_getActivationState_0(btRigidBodyAddr self);
    void btRigidBody___destroy___0(btRigidBodyAddr self);

    // btDbvtBroadphase
  
    btDbvtBroadphaseAddr btDbvtBroadphase_btDbvtBroadphase_0();
    btOverlappingPairCacheAddr btDbvtBroadphase_getOverlappingPairCache_0(btDbvtBroadphaseAddr self);
    void btDbvtBroadphase___destroy___0(btDbvtBroadphaseAddr self);

    // btCollisionDispatcher
  
    btCollisionDispatcherAddr btCollisionDispatcher_btCollisionDispatcher_1(btDefaultCollisionConfigurationAddr arg0);
    long_t btCollisionDispatcher_getNumManifolds_0(btCollisionDispatcherAddr self);
    btPersistentManifoldAddr btCollisionDispatcher_getManifoldByIndexInternal_1(btCollisionDispatcherAddr self, long_t arg0);
    void btCollisionDispatcher___destroy___0(btCollisionDispatcherAddr self);

    // btAxisSweep3
  
    btAxisSweep3Addr btAxisSweep3_btAxisSweep3_2(btVector3Addr arg0, btVector3Addr arg1);
    btAxisSweep3Addr btAxisSweep3_btAxisSweep3_3(btVector3Addr arg0, btVector3Addr arg1, long_t arg2);
    btAxisSweep3Addr btAxisSweep3_btAxisSweep3_4(btVector3Addr arg0, btVector3Addr arg1, long_t arg2, btOverlappingPairCacheAddr arg3);
    btAxisSweep3Addr btAxisSweep3_btAxisSweep3_5(btVector3Addr arg0, btVector3Addr arg1, long_t arg2, btOverlappingPairCacheAddr arg3, bool arg4);
    void btAxisSweep3___destroy___0(btAxisSweep3Addr self);

    // VoidPtr
  
    void VoidPtr___destroy___0(VoidPtrAddr self);

    // btConeTwistConstraint
  
    btConeTwistConstraintAddr btConeTwistConstraint_btConeTwistConstraint_2(btRigidBodyAddr arg0, btTransformAddr arg1);
    btConeTwistConstraintAddr btConeTwistConstraint_btConeTwistConstraint_4(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3);
    void btConeTwistConstraint_setLimit_2(btConeTwistConstraintAddr self, long_t arg0, double arg1);
    void btConeTwistConstraint_setAngularOnly_1(btConeTwistConstraintAddr self, bool arg0);
    void btConeTwistConstraint_setDamping_1(btConeTwistConstraintAddr self, double arg0);
    void btConeTwistConstraint_enableMotor_1(btConeTwistConstraintAddr self, bool arg0);
    void btConeTwistConstraint_setMaxMotorImpulse_1(btConeTwistConstraintAddr self, double arg0);
    void btConeTwistConstraint_setMaxMotorImpulseNormalized_1(btConeTwistConstraintAddr self, double arg0);
    void btConeTwistConstraint_setMotorTarget_1(btConeTwistConstraintAddr self, btQuaternionAddr arg0);
    void btConeTwistConstraint_setMotorTargetInConstraintSpace_1(btConeTwistConstraintAddr self, btQuaternionAddr arg0);
    void btConeTwistConstraint_enableFeedback_1(btConeTwistConstraintAddr self, bool arg0);
    double btConeTwistConstraint_getBreakingImpulseThreshold_0(btConeTwistConstraintAddr self);
    void btConeTwistConstraint_setBreakingImpulseThreshold_1(btConeTwistConstraintAddr self, double arg0);
    double btConeTwistConstraint_getParam_2(btConeTwistConstraintAddr self, long_t arg0, long_t arg1);
    void btConeTwistConstraint_setParam_3(btConeTwistConstraintAddr self, long_t arg0, double arg1, long_t arg2);
    void btConeTwistConstraint___destroy___0(btConeTwistConstraintAddr self);

    // btHingeConstraint
  
    btHingeConstraintAddr btHingeConstraint_btHingeConstraint_2(btRigidBodyAddr arg0, btTransformAddr arg1);
    btHingeConstraintAddr btHingeConstraint_btHingeConstraint_3(btRigidBodyAddr arg0, btTransformAddr arg1, bool arg2);
    btHingeConstraintAddr btHingeConstraint_btHingeConstraint_4(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3);
    btHingeConstraintAddr btHingeConstraint_btHingeConstraint_5(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3, bool arg4);
    btHingeConstraintAddr btHingeConstraint_btHingeConstraint_6(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btVector3Addr arg2, btVector3Addr arg3, btVector3Addr arg4, btVector3Addr arg5);
    btHingeConstraintAddr btHingeConstraint_btHingeConstraint_7(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btVector3Addr arg2, btVector3Addr arg3, btVector3Addr arg4, btVector3Addr arg5, bool arg6);
    void btHingeConstraint_setLimit_4(btHingeConstraintAddr self, double arg0, double arg1, double arg2, double arg3);
    void btHingeConstraint_setLimit_5(btHingeConstraintAddr self, double arg0, double arg1, double arg2, double arg3, double arg4);
    void btHingeConstraint_enableAngularMotor_3(btHingeConstraintAddr self, bool arg0, double arg1, double arg2);
    void btHingeConstraint_setAngularOnly_1(btHingeConstraintAddr self, bool arg0);
    void btHingeConstraint_enableMotor_1(btHingeConstraintAddr self, bool arg0);
    void btHingeConstraint_setMaxMotorImpulse_1(btHingeConstraintAddr self, double arg0);
    void btHingeConstraint_setMotorTarget_2(btHingeConstraintAddr self, double arg0, double arg1);
    void btHingeConstraint_enableFeedback_1(btHingeConstraintAddr self, bool arg0);
    double btHingeConstraint_getBreakingImpulseThreshold_0(btHingeConstraintAddr self);
    void btHingeConstraint_setBreakingImpulseThreshold_1(btHingeConstraintAddr self, double arg0);
    double btHingeConstraint_getParam_2(btHingeConstraintAddr self, long_t arg0, long_t arg1);
    void btHingeConstraint_setParam_3(btHingeConstraintAddr self, long_t arg0, double arg1, long_t arg2);
    void btHingeConstraint___destroy___0(btHingeConstraintAddr self);

    // JSValueAsParam
  

    // btConeShapeZ
  
    btConeShapeZAddr btConeShapeZ_btConeShapeZ_2(double arg0, double arg1);
    void btConeShapeZ_setLocalScaling_1(btConeShapeZAddr self, btVector3Addr arg0);
    btVector3Addr btConeShapeZ_getLocalScaling_0(btConeShapeZAddr self);
    void btConeShapeZ_calculateLocalInertia_2(btConeShapeZAddr self, double arg0, btVector3Addr arg1);
    void btConeShapeZ___destroy___0(btConeShapeZAddr self);

    // btConeShapeX
  
    btConeShapeXAddr btConeShapeX_btConeShapeX_2(double arg0, double arg1);
    void btConeShapeX_setLocalScaling_1(btConeShapeXAddr self, btVector3Addr arg0);
    btVector3Addr btConeShapeX_getLocalScaling_0(btConeShapeXAddr self);
    void btConeShapeX_calculateLocalInertia_2(btConeShapeXAddr self, double arg0, btVector3Addr arg1);
    void btConeShapeX___destroy___0(btConeShapeXAddr self);

    // btTriangleMesh
  
    btTriangleMeshAddr btTriangleMesh_btTriangleMesh_0();
    btTriangleMeshAddr btTriangleMesh_btTriangleMesh_1(bool arg0);
    btTriangleMeshAddr btTriangleMesh_btTriangleMesh_2(bool arg0, bool arg1);
    void btTriangleMesh_addTriangle_3(btTriangleMeshAddr self, btVector3Addr arg0, btVector3Addr arg1, btVector3Addr arg2);
    void btTriangleMesh_addTriangle_4(btTriangleMeshAddr self, btVector3Addr arg0, btVector3Addr arg1, btVector3Addr arg2, bool arg3);
    void btTriangleMesh___destroy___0(btTriangleMeshAddr self);

    // btConvexHullShape
  
    btConvexHullShapeAddr btConvexHullShape_btConvexHullShape_0();
    void btConvexHullShape_addPoint_1(btConvexHullShapeAddr self, btVector3Addr arg0);
    void btConvexHullShape_addPoint_2(btConvexHullShapeAddr self, btVector3Addr arg0, bool arg1);
    void btConvexHullShape_setMargin_1(btConvexHullShapeAddr self, double arg0);
    double btConvexHullShape_getMargin_0(btConvexHullShapeAddr self);
    void btConvexHullShape_setLocalScaling_1(btConvexHullShapeAddr self, btVector3Addr arg0);
    btVector3Addr btConvexHullShape_getLocalScaling_0(btConvexHullShapeAddr self);
    void btConvexHullShape_calculateLocalInertia_2(btConvexHullShapeAddr self, double arg0, btVector3Addr arg1);
    void btConvexHullShape___destroy___0(btConvexHullShapeAddr self);

    // btVehicleTuning
  
    btVehicleTuningAddr btVehicleTuning_btVehicleTuning_0();
    double btVehicleTuning_get_m_suspensionStiffness_0(btVehicleTuningAddr self);
    void btVehicleTuning_set_m_suspensionStiffness_1(btVehicleTuningAddr self, double arg0);
    double btVehicleTuning_get_m_suspensionCompression_0(btVehicleTuningAddr self);
    void btVehicleTuning_set_m_suspensionCompression_1(btVehicleTuningAddr self, double arg0);
    double btVehicleTuning_get_m_suspensionDamping_0(btVehicleTuningAddr self);
    void btVehicleTuning_set_m_suspensionDamping_1(btVehicleTuningAddr self, double arg0);
    double btVehicleTuning_get_m_maxSuspensionTravelCm_0(btVehicleTuningAddr self);
    void btVehicleTuning_set_m_maxSuspensionTravelCm_1(btVehicleTuningAddr self, double arg0);
    double btVehicleTuning_get_m_frictionSlip_0(btVehicleTuningAddr self);
    void btVehicleTuning_set_m_frictionSlip_1(btVehicleTuningAddr self, double arg0);
    double btVehicleTuning_get_m_maxSuspensionForce_0(btVehicleTuningAddr self);
    void btVehicleTuning_set_m_maxSuspensionForce_1(btVehicleTuningAddr self, double arg0);

    // btCollisionObjectWrapper
  

    // btGImpactMeshShape
  
    btGImpactMeshShapeAddr btGImpactMeshShape_btGImpactMeshShape_1(btStridingMeshInterfaceAddr arg0);
    void btGImpactMeshShape_setLocalScaling_1(btGImpactMeshShapeAddr self, btVector3Addr arg0);
    btVector3Addr btGImpactMeshShape_getLocalScaling_0(btGImpactMeshShapeAddr self);
    void btGImpactMeshShape_calculateLocalInertia_2(btGImpactMeshShapeAddr self, double arg0, btVector3Addr arg1);
    void btGImpactMeshShape_updateBound_0(btGImpactMeshShapeAddr self);
    void btGImpactMeshShape___destroy___0(btGImpactMeshShapeAddr self);

    // btDefaultMotionState
  
    btDefaultMotionStateAddr btDefaultMotionState_btDefaultMotionState_0();
    btDefaultMotionStateAddr btDefaultMotionState_btDefaultMotionState_1(btTransformAddr arg0);
    btDefaultMotionStateAddr btDefaultMotionState_btDefaultMotionState_2(btTransformAddr arg0, btTransformAddr arg1);
    void btDefaultMotionState_getWorldTransform_1(btDefaultMotionStateAddr self, btTransformAddr arg0);
    void btDefaultMotionState_setWorldTransform_1(btDefaultMotionStateAddr self, btTransformAddr arg0);
    btTransformAddr btDefaultMotionState_get_m_graphicsWorldTrans_0(btDefaultMotionStateAddr self);
    void btDefaultMotionState_set_m_graphicsWorldTrans_1(btDefaultMotionStateAddr self, btTransformAddr arg0);
    void btDefaultMotionState___destroy___0(btDefaultMotionStateAddr self);

    // btWheelInfo
  
    btWheelInfoAddr btWheelInfo_btWheelInfo_1(btWheelInfoConstructionInfoAddr arg0);
    double btWheelInfo_getSuspensionRestLength_0(btWheelInfoAddr self);
    void btWheelInfo_updateWheel_2(btWheelInfoAddr self, btRigidBodyAddr arg0, RaycastInfoAddr arg1);
    double btWheelInfo_get_m_suspensionStiffness_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_suspensionStiffness_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_frictionSlip_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_frictionSlip_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_engineForce_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_engineForce_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_rollInfluence_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_rollInfluence_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_suspensionRestLength1_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_suspensionRestLength1_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_wheelsRadius_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_wheelsRadius_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_wheelsDampingCompression_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_wheelsDampingCompression_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_wheelsDampingRelaxation_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_wheelsDampingRelaxation_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_steering_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_steering_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_maxSuspensionForce_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_maxSuspensionForce_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_maxSuspensionTravelCm_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_maxSuspensionTravelCm_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_wheelsSuspensionForce_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_wheelsSuspensionForce_1(btWheelInfoAddr self, double arg0);
    bool btWheelInfo_get_m_bIsFrontWheel_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_bIsFrontWheel_1(btWheelInfoAddr self, bool arg0);
    RaycastInfoAddr btWheelInfo_get_m_raycastInfo_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_raycastInfo_1(btWheelInfoAddr self, RaycastInfoAddr arg0);
    btVector3Addr btWheelInfo_get_m_chassisConnectionPointCS_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_chassisConnectionPointCS_1(btWheelInfoAddr self, btVector3Addr arg0);
    btTransformAddr btWheelInfo_get_m_worldTransform_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_worldTransform_1(btWheelInfoAddr self, btTransformAddr arg0);
    btVector3Addr btWheelInfo_get_m_wheelDirectionCS_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_wheelDirectionCS_1(btWheelInfoAddr self, btVector3Addr arg0);
    btVector3Addr btWheelInfo_get_m_wheelAxleCS_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_wheelAxleCS_1(btWheelInfoAddr self, btVector3Addr arg0);
    double btWheelInfo_get_m_rotation_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_rotation_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_deltaRotation_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_deltaRotation_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_brake_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_brake_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_clippedInvContactDotSuspension_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_clippedInvContactDotSuspension_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_suspensionRelativeVelocity_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_suspensionRelativeVelocity_1(btWheelInfoAddr self, double arg0);
    double btWheelInfo_get_m_skidInfo_0(btWheelInfoAddr self);
    void btWheelInfo_set_m_skidInfo_1(btWheelInfoAddr self, double arg0);
    void btWheelInfo___destroy___0(btWheelInfoAddr self);

    // btVector4
  
    btVector4Addr btVector4_btVector4_0();
    btVector4Addr btVector4_btVector4_4(double arg0, double arg1, double arg2, double arg3);
    double btVector4_w_0(btVector4Addr self);
    void btVector4_setValue_4(btVector4Addr self, double arg0, double arg1, double arg2, double arg3);
    double btVector4_length_0(btVector4Addr self);
    double btVector4_x_0(btVector4Addr self);
    double btVector4_y_0(btVector4Addr self);
    double btVector4_z_0(btVector4Addr self);
    void btVector4_setX_1(btVector4Addr self, double arg0);
    void btVector4_setY_1(btVector4Addr self, double arg0);
    void btVector4_setZ_1(btVector4Addr self, double arg0);
    void btVector4_normalize_0(btVector4Addr self);
    btVector3Addr btVector4_rotate_2(btVector4Addr self, btVector3Addr arg0, double arg1);
    double btVector4_dot_1(btVector4Addr self, btVector3Addr arg0);
    btVector3Addr btVector4_op_mul_1(btVector4Addr self, double arg0);
    btVector3Addr btVector4_op_add_1(btVector4Addr self, btVector3Addr arg0);
    btVector3Addr btVector4_op_sub_1(btVector4Addr self, btVector3Addr arg0);
    void btVector4___destroy___0(btVector4Addr self);

    // JsValue
  

    // btDefaultCollisionConstructionInfo
  
    btDefaultCollisionConstructionInfoAddr btDefaultCollisionConstructionInfo_btDefaultCollisionConstructionInfo_0();
    void btDefaultCollisionConstructionInfo___destroy___0(btDefaultCollisionConstructionInfoAddr self);

    // btVehicleRaycasterResult
  
    btVector3Addr btVehicleRaycasterResult_get_m_hitPointInWorld_0(btVehicleRaycasterResultAddr self);
    void btVehicleRaycasterResult_set_m_hitPointInWorld_1(btVehicleRaycasterResultAddr self, btVector3Addr arg0);
    btVector3Addr btVehicleRaycasterResult_get_m_hitNormalInWorld_0(btVehicleRaycasterResultAddr self);
    void btVehicleRaycasterResult_set_m_hitNormalInWorld_1(btVehicleRaycasterResultAddr self, btVector3Addr arg0);
    double btVehicleRaycasterResult_get_m_distFraction_0(btVehicleRaycasterResultAddr self);
    void btVehicleRaycasterResult_set_m_distFraction_1(btVehicleRaycasterResultAddr self, double arg0);
    void btVehicleRaycasterResult___destroy___0(btVehicleRaycasterResultAddr self);

    // btConstraintSolver
  
    void btConstraintSolver___destroy___0(btConstraintSolverAddr self);

    // btRaycastVehicle
  
    btRaycastVehicleAddr btRaycastVehicle_btRaycastVehicle_3(btVehicleTuningAddr arg0, btRigidBodyAddr arg1, btVehicleRaycasterAddr arg2);
    void btRaycastVehicle_applyEngineForce_2(btRaycastVehicleAddr self, double arg0, long_t arg1);
    void btRaycastVehicle_setSteeringValue_2(btRaycastVehicleAddr self, double arg0, long_t arg1);
    btTransformAddr btRaycastVehicle_getWheelTransformWS_1(btRaycastVehicleAddr self, long_t arg0);
    void btRaycastVehicle_updateWheelTransform_2(btRaycastVehicleAddr self, long_t arg0, bool arg1);
    btWheelInfoAddr btRaycastVehicle_addWheel_7(btRaycastVehicleAddr self, btVector3Addr arg0, btVector3Addr arg1, btVector3Addr arg2, double arg3, double arg4, btVehicleTuningAddr arg5, bool arg6);
    long_t btRaycastVehicle_getNumWheels_0(btRaycastVehicleAddr self);
    btRigidBodyAddr btRaycastVehicle_getRigidBody_0(btRaycastVehicleAddr self);
    btWheelInfoAddr btRaycastVehicle_getWheelInfo_1(btRaycastVehicleAddr self, long_t arg0);
    void btRaycastVehicle_setBrake_2(btRaycastVehicleAddr self, double arg0, long_t arg1);
    void btRaycastVehicle_setCoordinateSystem_3(btRaycastVehicleAddr self, long_t arg0, long_t arg1, long_t arg2);
    double btRaycastVehicle_getCurrentSpeedKmHour_0(btRaycastVehicleAddr self);
    btTransformAddr btRaycastVehicle_getChassisWorldTransform_0(btRaycastVehicleAddr self);
    double btRaycastVehicle_rayCast_1(btRaycastVehicleAddr self, btWheelInfoAddr arg0);
    void btRaycastVehicle_updateVehicle_1(btRaycastVehicleAddr self, double arg0);
    void btRaycastVehicle_resetSuspension_0(btRaycastVehicleAddr self);
    double btRaycastVehicle_getSteeringValue_1(btRaycastVehicleAddr self, long_t arg0);
    void btRaycastVehicle_updateWheelTransformsWS_1(btRaycastVehicleAddr self, btWheelInfoAddr arg0);
    void btRaycastVehicle_updateWheelTransformsWS_2(btRaycastVehicleAddr self, btWheelInfoAddr arg0, bool arg1);
    void btRaycastVehicle_setPitchControl_1(btRaycastVehicleAddr self, double arg0);
    void btRaycastVehicle_updateSuspension_1(btRaycastVehicleAddr self, double arg0);
    void btRaycastVehicle_updateFriction_1(btRaycastVehicleAddr self, double arg0);
    long_t btRaycastVehicle_getRightAxis_0(btRaycastVehicleAddr self);
    long_t btRaycastVehicle_getUpAxis_0(btRaycastVehicleAddr self);
    long_t btRaycastVehicle_getForwardAxis_0(btRaycastVehicleAddr self);
    btVector3Addr btRaycastVehicle_getForwardVector_0(btRaycastVehicleAddr self);
    long_t btRaycastVehicle_getUserConstraintType_0(btRaycastVehicleAddr self);
    void btRaycastVehicle_setUserConstraintType_1(btRaycastVehicleAddr self, long_t arg0);
    void btRaycastVehicle_setUserConstraintId_1(btRaycastVehicleAddr self, long_t arg0);
    long_t btRaycastVehicle_getUserConstraintId_0(btRaycastVehicleAddr self);
    void btRaycastVehicle_updateAction_2(btRaycastVehicleAddr self, btCollisionWorldAddr arg0, double arg1);
    void btRaycastVehicle___destroy___0(btRaycastVehicleAddr self);

    // tVector3Array
  
    long_t tVector3Array_size_0(tVector3ArrayAddr self);
    btVector3Addr tVector3Array_at_1(tVector3ArrayAddr self, long_t arg0);
    void tVector3Array_clear_0(tVector3ArrayAddr self);
    void tVector3Array_push_back_1(tVector3ArrayAddr self, btVector3Addr arg0);
    void tVector3Array_pop_back_0(tVector3ArrayAddr self);
    void tVector3Array___destroy___0(tVector3ArrayAddr self);

    // btCylinderShapeX
  
    btCylinderShapeXAddr btCylinderShapeX_btCylinderShapeX_1(btVector3Addr arg0);
    void btCylinderShapeX_setMargin_1(btCylinderShapeXAddr self, double arg0);
    double btCylinderShapeX_getMargin_0(btCylinderShapeXAddr self);
    void btCylinderShapeX_setLocalScaling_1(btCylinderShapeXAddr self, btVector3Addr arg0);
    btVector3Addr btCylinderShapeX_getLocalScaling_0(btCylinderShapeXAddr self);
    void btCylinderShapeX_calculateLocalInertia_2(btCylinderShapeXAddr self, double arg0, btVector3Addr arg1);
    void btCylinderShapeX___destroy___0(btCylinderShapeXAddr self);

    // btCylinderShapeZ
  
    btCylinderShapeZAddr btCylinderShapeZ_btCylinderShapeZ_1(btVector3Addr arg0);
    void btCylinderShapeZ_setMargin_1(btCylinderShapeZAddr self, double arg0);
    double btCylinderShapeZ_getMargin_0(btCylinderShapeZAddr self);
    void btCylinderShapeZ_setLocalScaling_1(btCylinderShapeZAddr self, btVector3Addr arg0);
    btVector3Addr btCylinderShapeZ_getLocalScaling_0(btCylinderShapeZAddr self);
    void btCylinderShapeZ_calculateLocalInertia_2(btCylinderShapeZAddr self, double arg0, btVector3Addr arg1);
    void btCylinderShapeZ___destroy___0(btCylinderShapeZAddr self);

    // btSequentialImpulseConstraintSolver
  
    btSequentialImpulseConstraintSolverAddr btSequentialImpulseConstraintSolver_btSequentialImpulseConstraintSolver_0();
    void btSequentialImpulseConstraintSolver___destroy___0(btSequentialImpulseConstraintSolverAddr self);

    // RaycastInfo
  
    btVector3Addr RaycastInfo_get_m_contactNormalWS_0(RaycastInfoAddr self);
    void RaycastInfo_set_m_contactNormalWS_1(RaycastInfoAddr self, btVector3Addr arg0);
    btVector3Addr RaycastInfo_get_m_contactPointWS_0(RaycastInfoAddr self);
    void RaycastInfo_set_m_contactPointWS_1(RaycastInfoAddr self, btVector3Addr arg0);
    double RaycastInfo_get_m_suspensionLength_0(RaycastInfoAddr self);
    void RaycastInfo_set_m_suspensionLength_1(RaycastInfoAddr self, double arg0);
    btVector3Addr RaycastInfo_get_m_hardPointWS_0(RaycastInfoAddr self);
    void RaycastInfo_set_m_hardPointWS_1(RaycastInfoAddr self, btVector3Addr arg0);
    btVector3Addr RaycastInfo_get_m_wheelDirectionWS_0(RaycastInfoAddr self);
    void RaycastInfo_set_m_wheelDirectionWS_1(RaycastInfoAddr self, btVector3Addr arg0);
    btVector3Addr RaycastInfo_get_m_wheelAxleWS_0(RaycastInfoAddr self);
    void RaycastInfo_set_m_wheelAxleWS_1(RaycastInfoAddr self, btVector3Addr arg0);
    bool RaycastInfo_get_m_isInContact_0(RaycastInfoAddr self);
    void RaycastInfo_set_m_isInContact_1(RaycastInfoAddr self, bool arg0);
    VoidPtrAddr RaycastInfo_get_m_groundObject_0(RaycastInfoAddr self);
    void RaycastInfo_set_m_groundObject_1(RaycastInfoAddr self, VoidPtrAddr arg0);
    void RaycastInfo___destroy___0(RaycastInfoAddr self);

    // btHeightfieldTerrainShape
  
    btHeightfieldTerrainShapeAddr btHeightfieldTerrainShape_btHeightfieldTerrainShape_9(long_t arg0, long_t arg1, VoidPtrAddr arg2, double arg3, double arg4, double arg5, long_t arg6, long_t arg7, bool arg8);
    void btHeightfieldTerrainShape_setMargin_1(btHeightfieldTerrainShapeAddr self, double arg0);
    double btHeightfieldTerrainShape_getMargin_0(btHeightfieldTerrainShapeAddr self);
    void btHeightfieldTerrainShape_setLocalScaling_1(btHeightfieldTerrainShapeAddr self, btVector3Addr arg0);
    btVector3Addr btHeightfieldTerrainShape_getLocalScaling_0(btHeightfieldTerrainShapeAddr self);
    void btHeightfieldTerrainShape_calculateLocalInertia_2(btHeightfieldTerrainShapeAddr self, double arg0, btVector3Addr arg1);
    void btHeightfieldTerrainShape___destroy___0(btHeightfieldTerrainShapeAddr self);

    // btDiscreteDynamicsWorld
  
    btDiscreteDynamicsWorldAddr btDiscreteDynamicsWorld_btDiscreteDynamicsWorld_4(btDispatcherAddr arg0, btBroadphaseInterfaceAddr arg1, btConstraintSolverAddr arg2, btCollisionConfigurationAddr arg3);
    void btDiscreteDynamicsWorld_setGravity_1(btDiscreteDynamicsWorldAddr self, btVector3Addr arg0);
    btVector3Addr btDiscreteDynamicsWorld_getGravity_0(btDiscreteDynamicsWorldAddr self);
    void btDiscreteDynamicsWorld_addRigidBody_1(btDiscreteDynamicsWorldAddr self, btRigidBodyAddr arg0);
    void btDiscreteDynamicsWorld_addRigidBody_3(btDiscreteDynamicsWorldAddr self, btRigidBodyAddr arg0, long_t arg1, long_t arg2);
    void btDiscreteDynamicsWorld_removeRigidBody_1(btDiscreteDynamicsWorldAddr self, btRigidBodyAddr arg0);
    void btDiscreteDynamicsWorld_addConstraint_1(btDiscreteDynamicsWorldAddr self, btTypedConstraintAddr arg0);
    void btDiscreteDynamicsWorld_addConstraint_2(btDiscreteDynamicsWorldAddr self, btTypedConstraintAddr arg0, bool arg1);
    void btDiscreteDynamicsWorld_removeConstraint_1(btDiscreteDynamicsWorldAddr self, btTypedConstraintAddr arg0);
    long_t btDiscreteDynamicsWorld_stepSimulation_1(btDiscreteDynamicsWorldAddr self, double arg0);
    long_t btDiscreteDynamicsWorld_stepSimulation_2(btDiscreteDynamicsWorldAddr self, double arg0, long_t arg1);
    long_t btDiscreteDynamicsWorld_stepSimulation_3(btDiscreteDynamicsWorldAddr self, double arg0, long_t arg1, double arg2);
    void btDiscreteDynamicsWorld_clearForces_0(btDiscreteDynamicsWorldAddr self);
    void btDiscreteDynamicsWorld_setApplySpeculativeContactRestitution_1(btDiscreteDynamicsWorldAddr self, bool arg0);
    bool btDiscreteDynamicsWorld_getApplySpeculativeContactRestitution_0(btDiscreteDynamicsWorldAddr self);
    btDispatcherAddr btDiscreteDynamicsWorld_getDispatcher_0(btDiscreteDynamicsWorldAddr self);
    void btDiscreteDynamicsWorld_rayTest_3(btDiscreteDynamicsWorldAddr self, btVector3Addr arg0, btVector3Addr arg1, RayResultCallbackAddr arg2);
    btOverlappingPairCacheAddr btDiscreteDynamicsWorld_getPairCache_0(btDiscreteDynamicsWorldAddr self);
    btDispatcherInfoAddr btDiscreteDynamicsWorld_getDispatchInfo_0(btDiscreteDynamicsWorldAddr self);
    void btDiscreteDynamicsWorld_addCollisionObject_1(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0);
    void btDiscreteDynamicsWorld_addCollisionObject_2(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0, long_t arg1);
    void btDiscreteDynamicsWorld_addCollisionObject_3(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0, long_t arg1, long_t arg2);
    void btDiscreteDynamicsWorld_removeCollisionObject_1(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0);
    btBroadphaseInterfaceAddr btDiscreteDynamicsWorld_getBroadphase_0(btDiscreteDynamicsWorldAddr self);
    void btDiscreteDynamicsWorld_convexSweepTest_5(btDiscreteDynamicsWorldAddr self, btConvexShapeAddr arg0, btTransformAddr arg1, btTransformAddr arg2, ConvexResultCallbackAddr arg3, double arg4);
    void btDiscreteDynamicsWorld_contactPairTest_3(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0, btCollisionObjectAddr arg1, ContactResultCallbackAddr arg2);
    void btDiscreteDynamicsWorld_contactTest_2(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0, ContactResultCallbackAddr arg1);
    void btDiscreteDynamicsWorld_updateSingleAabb_1(btDiscreteDynamicsWorldAddr self, btCollisionObjectAddr arg0);
    void btDiscreteDynamicsWorld_addAction_1(btDiscreteDynamicsWorldAddr self, btActionInterfaceAddr arg0);
    void btDiscreteDynamicsWorld_removeAction_1(btDiscreteDynamicsWorldAddr self, btActionInterfaceAddr arg0);
    btContactSolverInfoAddr btDiscreteDynamicsWorld_getSolverInfo_0(btDiscreteDynamicsWorldAddr self);
    void btDiscreteDynamicsWorld___destroy___0(btDiscreteDynamicsWorldAddr self);

    // btGhostPairCallback
  
    btGhostPairCallbackAddr btGhostPairCallback_btGhostPairCallback_0();
    void btGhostPairCallback___destroy___0(btGhostPairCallbackAddr self);

    // btOverlappingPairCallback
  
    void btOverlappingPairCallback___destroy___0(btOverlappingPairCallbackAddr self);

    // btKinematicCharacterController
  
    btKinematicCharacterControllerAddr btKinematicCharacterController_btKinematicCharacterController_4(btPairCachingGhostObjectAddr arg0, btConvexShapeAddr arg1, double arg2, btVector3Addr arg3);
    void btKinematicCharacterController_setUp_1(btKinematicCharacterControllerAddr self, btVector3Addr arg0);
    void btKinematicCharacterController_setWalkDirection_1(btKinematicCharacterControllerAddr self, btVector3Addr arg0);
    void btKinematicCharacterController_setVelocityForTimeInterval_2(btKinematicCharacterControllerAddr self, btVector3Addr arg0, double arg1);
    void btKinematicCharacterController_warp_1(btKinematicCharacterControllerAddr self, btVector3Addr arg0);
    void btKinematicCharacterController_preStep_1(btKinematicCharacterControllerAddr self, btCollisionWorldAddr arg0);
    void btKinematicCharacterController_playerStep_2(btKinematicCharacterControllerAddr self, btCollisionWorldAddr arg0, double arg1);
    void btKinematicCharacterController_setFallSpeed_1(btKinematicCharacterControllerAddr self, double arg0);
    void btKinematicCharacterController_setJumpSpeed_1(btKinematicCharacterControllerAddr self, double arg0);
    void btKinematicCharacterController_setMaxJumpHeight_1(btKinematicCharacterControllerAddr self, double arg0);
    bool btKinematicCharacterController_canJump_0(btKinematicCharacterControllerAddr self);
    void btKinematicCharacterController_jump_1(btKinematicCharacterControllerAddr self, btVector3Addr arg0);
    void btKinematicCharacterController_setGravity_1(btKinematicCharacterControllerAddr self, btVector3Addr arg0);
    btVector3Addr btKinematicCharacterController_getGravity_0(btKinematicCharacterControllerAddr self);
    void btKinematicCharacterController_setMaxSlope_1(btKinematicCharacterControllerAddr self, double arg0);
    double btKinematicCharacterController_getMaxSlope_0(btKinematicCharacterControllerAddr self);
    btPairCachingGhostObjectAddr btKinematicCharacterController_getGhostObject_0(btKinematicCharacterControllerAddr self);
    void btKinematicCharacterController_setUseGhostSweepTest_1(btKinematicCharacterControllerAddr self, bool arg0);
    bool btKinematicCharacterController_onGround_0(btKinematicCharacterControllerAddr self);
    void btKinematicCharacterController_setUpInterpolate_1(btKinematicCharacterControllerAddr self, bool arg0);
    void btKinematicCharacterController_updateAction_2(btKinematicCharacterControllerAddr self, btCollisionWorldAddr arg0, double arg1);
    void btKinematicCharacterController___destroy___0(btKinematicCharacterControllerAddr self);

    // btStaticPlaneShape
  
    btStaticPlaneShapeAddr btStaticPlaneShape_btStaticPlaneShape_2(btVector3Addr arg0, double arg1);
    void btStaticPlaneShape_setLocalScaling_1(btStaticPlaneShapeAddr self, btVector3Addr arg0);
    btVector3Addr btStaticPlaneShape_getLocalScaling_0(btStaticPlaneShapeAddr self);
    void btStaticPlaneShape_calculateLocalInertia_2(btStaticPlaneShapeAddr self, double arg0, btVector3Addr arg1);
    void btStaticPlaneShape___destroy___0(btStaticPlaneShapeAddr self);

    // btOverlappingPairCache
  
    void btOverlappingPairCache_setInternalGhostPairCallback_1(btOverlappingPairCacheAddr self, btOverlappingPairCallbackAddr arg0);
    void btOverlappingPairCache___destroy___0(btOverlappingPairCacheAddr self);

    // tBtCollisionObjectArray
  
    long_t tBtCollisionObjectArray_size_0(tBtCollisionObjectArrayAddr self);
    btCollisionObjectAddr tBtCollisionObjectArray_at_1(tBtCollisionObjectArrayAddr self, long_t arg0);
    void tBtCollisionObjectArray_clear_0(tBtCollisionObjectArrayAddr self);
    void tBtCollisionObjectArray_push_back_1(tBtCollisionObjectArrayAddr self, btCollisionObjectAddr arg0);
    void tBtCollisionObjectArray_pop_back_0(tBtCollisionObjectArrayAddr self);
    void tBtCollisionObjectArray___destroy___0(tBtCollisionObjectArrayAddr self);

    // btFixedConstraint
  
    btFixedConstraintAddr btFixedConstraint_btFixedConstraint_4(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3);
    void btFixedConstraint_enableFeedback_1(btFixedConstraintAddr self, bool arg0);
    double btFixedConstraint_getBreakingImpulseThreshold_0(btFixedConstraintAddr self);
    void btFixedConstraint_setBreakingImpulseThreshold_1(btFixedConstraintAddr self, double arg0);
    double btFixedConstraint_getParam_2(btFixedConstraintAddr self, long_t arg0, long_t arg1);
    void btFixedConstraint_setParam_3(btFixedConstraintAddr self, long_t arg0, double arg1, long_t arg2);
    void btFixedConstraint___destroy___0(btFixedConstraintAddr self);

    // btTransform
  
    btTransformAddr btTransform_btTransform_0();
    btTransformAddr btTransform_btTransform_2(btQuaternionAddr arg0, btVector3Addr arg1);
    void btTransform_setIdentity_0(btTransformAddr self);
    void btTransform_setOrigin_1(btTransformAddr self, btVector3Addr arg0);
    void btTransform_setRotation_1(btTransformAddr self, btQuaternionAddr arg0);
    btVector3Addr btTransform_getOrigin_0(btTransformAddr self);
    btQuaternionAddr btTransform_getRotation_0(btTransformAddr self);
    btMatrix3x3Addr btTransform_getBasis_0(btTransformAddr self);
    void btTransform_setFromOpenGLMatrix_1(btTransformAddr self, double* arg0);
    void btTransform___destroy___0(btTransformAddr self);

    // ClosestRayResultCallback
  
    ClosestRayResultCallbackAddr ClosestRayResultCallback_ClosestRayResultCallback_2(btVector3Addr arg0, btVector3Addr arg1);
    bool ClosestRayResultCallback_hasHit_0(ClosestRayResultCallbackAddr self);
    btVector3Addr ClosestRayResultCallback_get_m_rayFromWorld_0(ClosestRayResultCallbackAddr self);
    void ClosestRayResultCallback_set_m_rayFromWorld_1(ClosestRayResultCallbackAddr self, btVector3Addr arg0);
    btVector3Addr ClosestRayResultCallback_get_m_rayToWorld_0(ClosestRayResultCallbackAddr self);
    void ClosestRayResultCallback_set_m_rayToWorld_1(ClosestRayResultCallbackAddr self, btVector3Addr arg0);
    btVector3Addr ClosestRayResultCallback_get_m_hitNormalWorld_0(ClosestRayResultCallbackAddr self);
    void ClosestRayResultCallback_set_m_hitNormalWorld_1(ClosestRayResultCallbackAddr self, btVector3Addr arg0);
    btVector3Addr ClosestRayResultCallback_get_m_hitPointWorld_0(ClosestRayResultCallbackAddr self);
    void ClosestRayResultCallback_set_m_hitPointWorld_1(ClosestRayResultCallbackAddr self, btVector3Addr arg0);
    long_t ClosestRayResultCallback_get_m_collisionFilterGroup_0(ClosestRayResultCallbackAddr self);
    void ClosestRayResultCallback_set_m_collisionFilterGroup_1(ClosestRayResultCallbackAddr self, long_t arg0);
    long_t ClosestRayResultCallback_get_m_collisionFilterMask_0(ClosestRayResultCallbackAddr self);
    void ClosestRayResultCallback_set_m_collisionFilterMask_1(ClosestRayResultCallbackAddr self, long_t arg0);
    double ClosestRayResultCallback_get_m_closestHitFraction_0(ClosestRayResultCallbackAddr self);
    void ClosestRayResultCallback_set_m_closestHitFraction_1(ClosestRayResultCallbackAddr self, double arg0);
    btCollisionObjectAddr ClosestRayResultCallback_get_m_collisionObject_0(ClosestRayResultCallbackAddr self);
    void ClosestRayResultCallback_set_m_collisionObject_1(ClosestRayResultCallbackAddr self, btCollisionObjectAddr arg0);
    void ClosestRayResultCallback___destroy___0(ClosestRayResultCallbackAddr self);

    // AllConvexResultCallback
  
    AllConvexResultCallbackAddr AllConvexResultCallback_AllConvexResultCallback_2(btVector3Addr arg0, btVector3Addr arg1);
    bool AllConvexResultCallback_hasHit_0(AllConvexResultCallbackAddr self);
    btVector3Addr AllConvexResultCallback_get_m_convexFromWorld_0(AllConvexResultCallbackAddr self);
    void AllConvexResultCallback_set_m_convexFromWorld_1(AllConvexResultCallbackAddr self, btVector3Addr arg0);
    btVector3Addr AllConvexResultCallback_get_m_convexToWorld_0(AllConvexResultCallbackAddr self);
    void AllConvexResultCallback_set_m_convexToWorld_1(AllConvexResultCallbackAddr self, btVector3Addr arg0);
    tVector3ArrayAddr AllConvexResultCallback_get_m_hitNormalWorld_0(AllConvexResultCallbackAddr self);
    void AllConvexResultCallback_set_m_hitNormalWorld_1(AllConvexResultCallbackAddr self, tVector3ArrayAddr arg0);
    tVector3ArrayAddr AllConvexResultCallback_get_m_hitPointWorld_0(AllConvexResultCallbackAddr self);
    void AllConvexResultCallback_set_m_hitPointWorld_1(AllConvexResultCallbackAddr self, tVector3ArrayAddr arg0);
    tScalarArrayAddr AllConvexResultCallback_get_m_hitFractions_0(AllConvexResultCallbackAddr self);
    void AllConvexResultCallback_set_m_hitFractions_1(AllConvexResultCallbackAddr self, tScalarArrayAddr arg0);
    tBtCollisionObjectArrayAddr AllConvexResultCallback_get_m_collisionObjects_0(AllConvexResultCallbackAddr self);
    void AllConvexResultCallback_set_m_collisionObjects_1(AllConvexResultCallbackAddr self, tBtCollisionObjectArrayAddr arg0);
    long_t AllConvexResultCallback_get_m_collisionFilterGroup_0(AllConvexResultCallbackAddr self);
    void AllConvexResultCallback_set_m_collisionFilterGroup_1(AllConvexResultCallbackAddr self, long_t arg0);
    long_t AllConvexResultCallback_get_m_collisionFilterMask_0(AllConvexResultCallbackAddr self);
    void AllConvexResultCallback_set_m_collisionFilterMask_1(AllConvexResultCallbackAddr self, long_t arg0);
    double AllConvexResultCallback_get_m_closestHitFraction_0(AllConvexResultCallbackAddr self);
    void AllConvexResultCallback_set_m_closestHitFraction_1(AllConvexResultCallbackAddr self, double arg0);
    void AllConvexResultCallback___destroy___0(AllConvexResultCallbackAddr self);

    // ConcreteContactResultCallback
  
    ConcreteContactResultCallbackAddr ConcreteContactResultCallback_ConcreteContactResultCallback_0();
    double ConcreteContactResultCallback_addSingleResult_7(ConcreteContactResultCallbackAddr self, btManifoldPointAddr arg0, btCollisionObjectWrapperAddr arg1, long_t arg2, long_t arg3, btCollisionObjectWrapperAddr arg4, long_t arg5, long_t arg6);
    void ConcreteContactResultCallback___destroy___0(ConcreteContactResultCallbackAddr self);

    // btBvhTriangleMeshShape
  
    btBvhTriangleMeshShapeAddr btBvhTriangleMeshShape_btBvhTriangleMeshShape_2(btStridingMeshInterfaceAddr arg0, bool arg1);
    btBvhTriangleMeshShapeAddr btBvhTriangleMeshShape_btBvhTriangleMeshShape_3(btStridingMeshInterfaceAddr arg0, bool arg1, bool arg2);
    void btBvhTriangleMeshShape_setLocalScaling_1(btBvhTriangleMeshShapeAddr self, btVector3Addr arg0);
    btVector3Addr btBvhTriangleMeshShape_getLocalScaling_0(btBvhTriangleMeshShapeAddr self);
    void btBvhTriangleMeshShape_calculateLocalInertia_2(btBvhTriangleMeshShapeAddr self, double arg0, btVector3Addr arg1);
    void btBvhTriangleMeshShape___destroy___0(btBvhTriangleMeshShapeAddr self);

    // btSliderConstraint
  
    btSliderConstraintAddr btSliderConstraint_btSliderConstraint_3(btRigidBodyAddr arg0, btTransformAddr arg1, bool arg2);
    btSliderConstraintAddr btSliderConstraint_btSliderConstraint_5(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3, bool arg4);
    void btSliderConstraint_setLowerLinLimit_1(btSliderConstraintAddr self, double arg0);
    void btSliderConstraint_setUpperLinLimit_1(btSliderConstraintAddr self, double arg0);
    void btSliderConstraint_setLowerAngLimit_1(btSliderConstraintAddr self, double arg0);
    void btSliderConstraint_setUpperAngLimit_1(btSliderConstraintAddr self, double arg0);
    void btSliderConstraint_enableFeedback_1(btSliderConstraintAddr self, bool arg0);
    double btSliderConstraint_getBreakingImpulseThreshold_0(btSliderConstraintAddr self);
    void btSliderConstraint_setBreakingImpulseThreshold_1(btSliderConstraintAddr self, double arg0);
    double btSliderConstraint_getParam_2(btSliderConstraintAddr self, long_t arg0, long_t arg1);
    void btSliderConstraint_setParam_3(btSliderConstraintAddr self, long_t arg0, double arg1, long_t arg2);
    void btSliderConstraint___destroy___0(btSliderConstraintAddr self);

    // btPairCachingGhostObject
  
    btPairCachingGhostObjectAddr btPairCachingGhostObject_btPairCachingGhostObject_0();
    void btPairCachingGhostObject_setAnisotropicFriction_2(btPairCachingGhostObjectAddr self, btVector3Addr arg0, long_t arg1);
    btCollisionShapeAddr btPairCachingGhostObject_getCollisionShape_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_setContactProcessingThreshold_1(btPairCachingGhostObjectAddr self, double arg0);
    void btPairCachingGhostObject_setActivationState_1(btPairCachingGhostObjectAddr self, long_t arg0);
    void btPairCachingGhostObject_forceActivationState_1(btPairCachingGhostObjectAddr self, long_t arg0);
    void btPairCachingGhostObject_activate_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_activate_1(btPairCachingGhostObjectAddr self, bool arg0);
    bool btPairCachingGhostObject_isActive_0(btPairCachingGhostObjectAddr self);
    bool btPairCachingGhostObject_isKinematicObject_0(btPairCachingGhostObjectAddr self);
    bool btPairCachingGhostObject_isStaticObject_0(btPairCachingGhostObjectAddr self);
    bool btPairCachingGhostObject_isStaticOrKinematicObject_0(btPairCachingGhostObjectAddr self);
    double btPairCachingGhostObject_getRestitution_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_setRestitution_1(btPairCachingGhostObjectAddr self, double arg0);
    double btPairCachingGhostObject_getFriction_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_setFriction_1(btPairCachingGhostObjectAddr self, double arg0);
    double btPairCachingGhostObject_getRollingFriction_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_setRollingFriction_1(btPairCachingGhostObjectAddr self, double arg0);
    long_t btPairCachingGhostObject_getCollisionFlags_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_setCollisionFlags_1(btPairCachingGhostObjectAddr self, long_t arg0);
    btTransformAddr btPairCachingGhostObject_getWorldTransform_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_setWorldTransform_1(btPairCachingGhostObjectAddr self, btTransformAddr arg0);
    void btPairCachingGhostObject_setCollisionShape_1(btPairCachingGhostObjectAddr self, btCollisionShapeAddr arg0);
    double btPairCachingGhostObject_getCcdMotionThreshold_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_setCcdMotionThreshold_1(btPairCachingGhostObjectAddr self, double arg0);
    double btPairCachingGhostObject_getCcdSweptSphereRadius_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_setCcdSweptSphereRadius_1(btPairCachingGhostObjectAddr self, double arg0);
    long_t btPairCachingGhostObject_getUserIndex_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_setUserIndex_1(btPairCachingGhostObjectAddr self, long_t arg0);
    VoidPtrAddr btPairCachingGhostObject_getUserPointer_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_setUserPointer_1(btPairCachingGhostObjectAddr self, VoidPtrAddr arg0);
    btVector3Addr btPairCachingGhostObject_getInterpolationAngularVelocity_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_setInterpolationAngularVelocity_1(btPairCachingGhostObjectAddr self, btVector3Addr arg0);
    btVector3Addr btPairCachingGhostObject_getInterpolationLinearVelocity_0(btPairCachingGhostObjectAddr self);
    void btPairCachingGhostObject_setInterpolationLinearVelocity_1(btPairCachingGhostObjectAddr self, btVector3Addr arg0);
    btBroadphaseProxyAddr btPairCachingGhostObject_getBroadphaseHandle_0(btPairCachingGhostObjectAddr self);
    long_t btPairCachingGhostObject_getActivationState_0(btPairCachingGhostObjectAddr self);
    long_t btPairCachingGhostObject_getNumOverlappingObjects_0(btPairCachingGhostObjectAddr self);
    btCollisionObjectAddr btPairCachingGhostObject_getOverlappingObject_1(btPairCachingGhostObjectAddr self, long_t arg0);
    void btPairCachingGhostObject___destroy___0(btPairCachingGhostObjectAddr self);

    // btManifoldPoint
  
    btVector3Addr btManifoldPoint_getPositionWorldOnA_0(btManifoldPointAddr self);
    btVector3Addr btManifoldPoint_getPositionWorldOnB_0(btManifoldPointAddr self);
    double btManifoldPoint_getAppliedImpulse_0(btManifoldPointAddr self);
    double btManifoldPoint_getDistance_0(btManifoldPointAddr self);
    btVector3Addr btManifoldPoint_get_m_localPointA_0(btManifoldPointAddr self);
    void btManifoldPoint_set_m_localPointA_1(btManifoldPointAddr self, btVector3Addr arg0);
    btVector3Addr btManifoldPoint_get_m_localPointB_0(btManifoldPointAddr self);
    void btManifoldPoint_set_m_localPointB_1(btManifoldPointAddr self, btVector3Addr arg0);
    btVector3Addr btManifoldPoint_get_m_positionWorldOnB_0(btManifoldPointAddr self);
    void btManifoldPoint_set_m_positionWorldOnB_1(btManifoldPointAddr self, btVector3Addr arg0);
    btVector3Addr btManifoldPoint_get_m_positionWorldOnA_0(btManifoldPointAddr self);
    void btManifoldPoint_set_m_positionWorldOnA_1(btManifoldPointAddr self, btVector3Addr arg0);
    btVector3Addr btManifoldPoint_get_m_normalWorldOnB_0(btManifoldPointAddr self);
    void btManifoldPoint_set_m_normalWorldOnB_1(btManifoldPointAddr self, btVector3Addr arg0);
    void btManifoldPoint___destroy___0(btManifoldPointAddr self);

    // btPoint2PointConstraint
  
    btPoint2PointConstraintAddr btPoint2PointConstraint_btPoint2PointConstraint_2(btRigidBodyAddr arg0, btVector3Addr arg1);
    btPoint2PointConstraintAddr btPoint2PointConstraint_btPoint2PointConstraint_4(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btVector3Addr arg2, btVector3Addr arg3);
    void btPoint2PointConstraint_setPivotA_1(btPoint2PointConstraintAddr self, btVector3Addr arg0);
    void btPoint2PointConstraint_setPivotB_1(btPoint2PointConstraintAddr self, btVector3Addr arg0);
    btVector3Addr btPoint2PointConstraint_getPivotInA_0(btPoint2PointConstraintAddr self);
    btVector3Addr btPoint2PointConstraint_getPivotInB_0(btPoint2PointConstraintAddr self);
    void btPoint2PointConstraint_enableFeedback_1(btPoint2PointConstraintAddr self, bool arg0);
    double btPoint2PointConstraint_getBreakingImpulseThreshold_0(btPoint2PointConstraintAddr self);
    void btPoint2PointConstraint_setBreakingImpulseThreshold_1(btPoint2PointConstraintAddr self, double arg0);
    double btPoint2PointConstraint_getParam_2(btPoint2PointConstraintAddr self, long_t arg0, long_t arg1);
    void btPoint2PointConstraint_setParam_3(btPoint2PointConstraintAddr self, long_t arg0, double arg1, long_t arg2);
    btConstraintSettingAddr btPoint2PointConstraint_get_m_setting_0(btPoint2PointConstraintAddr self);
    void btPoint2PointConstraint_set_m_setting_1(btPoint2PointConstraintAddr self, btConstraintSettingAddr arg0);
    void btPoint2PointConstraint___destroy___0(btPoint2PointConstraintAddr self);

    // btBroadphaseProxy
  
    long_t btBroadphaseProxy_get_m_collisionFilterGroup_0(btBroadphaseProxyAddr self);
    void btBroadphaseProxy_set_m_collisionFilterGroup_1(btBroadphaseProxyAddr self, long_t arg0);
    long_t btBroadphaseProxy_get_m_collisionFilterMask_0(btBroadphaseProxyAddr self);
    void btBroadphaseProxy_set_m_collisionFilterMask_1(btBroadphaseProxyAddr self, long_t arg0);
    void btBroadphaseProxy___destroy___0(btBroadphaseProxyAddr self);

    // btGeneric6DofSpringConstraint
  
    btGeneric6DofSpringConstraintAddr btGeneric6DofSpringConstraint_btGeneric6DofSpringConstraint_3(btRigidBodyAddr arg0, btTransformAddr arg1, bool arg2);
    btGeneric6DofSpringConstraintAddr btGeneric6DofSpringConstraint_btGeneric6DofSpringConstraint_5(btRigidBodyAddr arg0, btRigidBodyAddr arg1, btTransformAddr arg2, btTransformAddr arg3, bool arg4);
    void btGeneric6DofSpringConstraint_enableSpring_2(btGeneric6DofSpringConstraintAddr self, long_t arg0, bool arg1);
    void btGeneric6DofSpringConstraint_setStiffness_2(btGeneric6DofSpringConstraintAddr self, long_t arg0, double arg1);
    void btGeneric6DofSpringConstraint_setDamping_2(btGeneric6DofSpringConstraintAddr self, long_t arg0, double arg1);
    void btGeneric6DofSpringConstraint_setLinearLowerLimit_1(btGeneric6DofSpringConstraintAddr self, btVector3Addr arg0);
    void btGeneric6DofSpringConstraint_setLinearUpperLimit_1(btGeneric6DofSpringConstraintAddr self, btVector3Addr arg0);
    void btGeneric6DofSpringConstraint_setAngularLowerLimit_1(btGeneric6DofSpringConstraintAddr self, btVector3Addr arg0);
    void btGeneric6DofSpringConstraint_setAngularUpperLimit_1(btGeneric6DofSpringConstraintAddr self, btVector3Addr arg0);
    btTransformAddr btGeneric6DofSpringConstraint_getFrameOffsetA_0(btGeneric6DofSpringConstraintAddr self);
    void btGeneric6DofSpringConstraint_enableFeedback_1(btGeneric6DofSpringConstraintAddr self, bool arg0);
    double btGeneric6DofSpringConstraint_getBreakingImpulseThreshold_0(btGeneric6DofSpringConstraintAddr self);
    void btGeneric6DofSpringConstraint_setBreakingImpulseThreshold_1(btGeneric6DofSpringConstraintAddr self, double arg0);
    double btGeneric6DofSpringConstraint_getParam_2(btGeneric6DofSpringConstraintAddr self, long_t arg0, long_t arg1);
    void btGeneric6DofSpringConstraint_setParam_3(btGeneric6DofSpringConstraintAddr self, long_t arg0, double arg1, long_t arg2);
    void btGeneric6DofSpringConstraint___destroy___0(btGeneric6DofSpringConstraintAddr self);

    // btBoxShape
  
    btBoxShapeAddr btBoxShape_btBoxShape_1(btVector3Addr arg0);
    void btBoxShape_setMargin_1(btBoxShapeAddr self, double arg0);
    double btBoxShape_getMargin_0(btBoxShapeAddr self);
    void btBoxShape_setLocalScaling_1(btBoxShapeAddr self, btVector3Addr arg0);
    btVector3Addr btBoxShape_getLocalScaling_0(btBoxShapeAddr self);
    void btBoxShape_calculateLocalInertia_2(btBoxShapeAddr self, double arg0, btVector3Addr arg1);
    void btBoxShape___destroy___0(btBoxShapeAddr self);

    // btCapsuleShapeX
  
    btCapsuleShapeXAddr btCapsuleShapeX_btCapsuleShapeX_2(double arg0, double arg1);
    void btCapsuleShapeX_setMargin_1(btCapsuleShapeXAddr self, double arg0);
    double btCapsuleShapeX_getMargin_0(btCapsuleShapeXAddr self);
    long_t btCapsuleShapeX_getUpAxis_0(btCapsuleShapeXAddr self);
    double btCapsuleShapeX_getRadius_0(btCapsuleShapeXAddr self);
    double btCapsuleShapeX_getHalfHeight_0(btCapsuleShapeXAddr self);
    void btCapsuleShapeX_setLocalScaling_1(btCapsuleShapeXAddr self, btVector3Addr arg0);
    btVector3Addr btCapsuleShapeX_getLocalScaling_0(btCapsuleShapeXAddr self);
    void btCapsuleShapeX_calculateLocalInertia_2(btCapsuleShapeXAddr self, double arg0, btVector3Addr arg1);
    void btCapsuleShapeX___destroy___0(btCapsuleShapeXAddr self);

    // btQuaternion
  
    btQuaternionAddr btQuaternion_btQuaternion_4(double arg0, double arg1, double arg2, double arg3);
    void btQuaternion_setValue_4(btQuaternionAddr self, double arg0, double arg1, double arg2, double arg3);
    void btQuaternion_setEulerZYX_3(btQuaternionAddr self, double arg0, double arg1, double arg2);
    void btQuaternion_setRotation_2(btQuaternionAddr self, btVector3Addr arg0, double arg1);
    void btQuaternion_normalize_0(btQuaternionAddr self);
    double btQuaternion_length2_0(btQuaternionAddr self);
    double btQuaternion_length_0(btQuaternionAddr self);
    double btQuaternion_dot_1(btQuaternionAddr self, btQuaternionAddr arg0);
    btQuaternionAddr btQuaternion_normalized_0(btQuaternionAddr self);
    btVector3Addr btQuaternion_getAxis_0(btQuaternionAddr self);
    btQuaternionAddr btQuaternion_inverse_0(btQuaternionAddr self);
    double btQuaternion_getAngle_0(btQuaternionAddr self);
    double btQuaternion_getAngleShortestPath_0(btQuaternionAddr self);
    double btQuaternion_angle_1(btQuaternionAddr self, btQuaternionAddr arg0);
    double btQuaternion_angleShortestPath_1(btQuaternionAddr self, btQuaternionAddr arg0);
    btQuaternionAddr btQuaternion_op_add_1(btQuaternionAddr self, btQuaternionAddr arg0);
    btQuaternionAddr btQuaternion_op_sub_1(btQuaternionAddr self, btQuaternionAddr arg0);
    btQuaternionAddr btQuaternion_op_mul_1(btQuaternionAddr self, double arg0);
    btQuaternionAddr btQuaternion_op_mulq_1(btQuaternionAddr self, btQuaternionAddr arg0);
    btQuaternionAddr btQuaternion_op_div_1(btQuaternionAddr self, double arg0);
    double btQuaternion_x_0(btQuaternionAddr self);
    double btQuaternion_y_0(btQuaternionAddr self);
    double btQuaternion_z_0(btQuaternionAddr self);
    double btQuaternion_w_0(btQuaternionAddr self);
    void btQuaternion_setX_1(btQuaternionAddr self, double arg0);
    void btQuaternion_setY_1(btQuaternionAddr self, double arg0);
    void btQuaternion_setZ_1(btQuaternionAddr self, double arg0);
    void btQuaternion_setW_1(btQuaternionAddr self, double arg0);
    void btQuaternion___destroy___0(btQuaternionAddr self);

    // btCapsuleShapeZ
  
    btCapsuleShapeZAddr btCapsuleShapeZ_btCapsuleShapeZ_2(double arg0, double arg1);
    void btCapsuleShapeZ_setMargin_1(btCapsuleShapeZAddr self, double arg0);
    double btCapsuleShapeZ_getMargin_0(btCapsuleShapeZAddr self);
    long_t btCapsuleShapeZ_getUpAxis_0(btCapsuleShapeZAddr self);
    double btCapsuleShapeZ_getRadius_0(btCapsuleShapeZAddr self);
    double btCapsuleShapeZ_getHalfHeight_0(btCapsuleShapeZAddr self);
    void btCapsuleShapeZ_setLocalScaling_1(btCapsuleShapeZAddr self, btVector3Addr arg0);
    btVector3Addr btCapsuleShapeZ_getLocalScaling_0(btCapsuleShapeZAddr self);
    void btCapsuleShapeZ_calculateLocalInertia_2(btCapsuleShapeZAddr self, double arg0, btVector3Addr arg1);
    void btCapsuleShapeZ___destroy___0(btCapsuleShapeZAddr self);

    // btContactSolverInfo
  
    long_t btContactSolverInfo_get_m_solverMode_0(btContactSolverInfoAddr self);
    void btContactSolverInfo_set_m_solverMode_1(btContactSolverInfoAddr self, long_t arg0);
    bool btContactSolverInfo_get_m_splitImpulse_0(btContactSolverInfoAddr self);
    void btContactSolverInfo_set_m_splitImpulse_1(btContactSolverInfoAddr self, bool arg0);
    double btContactSolverInfo_get_m_splitImpulsePenetrationThreshold_0(btContactSolverInfoAddr self);
    void btContactSolverInfo_set_m_splitImpulsePenetrationThreshold_1(btContactSolverInfoAddr self, double arg0);
    long_t btContactSolverInfo_get_m_numIterations_0(btContactSolverInfoAddr self);
    void btContactSolverInfo_set_m_numIterations_1(btContactSolverInfoAddr self, long_t arg0);
    void btContactSolverInfo___destroy___0(btContactSolverInfoAddr self);

    // tScalarArray
  
    long_t tScalarArray_size_0(tScalarArrayAddr self);
    double tScalarArray_at_1(tScalarArrayAddr self, long_t arg0);
    void tScalarArray_clear_0(tScalarArrayAddr self);
    void tScalarArray_push_back_1(tScalarArrayAddr self, double arg0);
    void tScalarArray_pop_back_0(tScalarArrayAddr self);
    void tScalarArray___destroy___0(tScalarArrayAddr self);

    // btSphereShape
  
    btSphereShapeAddr btSphereShape_btSphereShape_1(double arg0);
    void btSphereShape_setMargin_1(btSphereShapeAddr self, double arg0);
    double btSphereShape_getMargin_0(btSphereShapeAddr self);
    void btSphereShape_setLocalScaling_1(btSphereShapeAddr self, btVector3Addr arg0);
    btVector3Addr btSphereShape_getLocalScaling_0(btSphereShapeAddr self);
    void btSphereShape_calculateLocalInertia_2(btSphereShapeAddr self, double arg0, btVector3Addr arg1);
    void btSphereShape___destroy___0(btSphereShapeAddr self);

    // LocalConvexResult
  
    LocalConvexResultAddr LocalConvexResult_LocalConvexResult_5(btCollisionObjectAddr arg0, LocalShapeInfoAddr arg1, btVector3Addr arg2, btVector3Addr arg3, double arg4);
    btCollisionObjectAddr LocalConvexResult_get_m_hitCollisionObject_0(LocalConvexResultAddr self);
    void LocalConvexResult_set_m_hitCollisionObject_1(LocalConvexResultAddr self, btCollisionObjectAddr arg0);
    LocalShapeInfoAddr LocalConvexResult_get_m_localShapeInfo_0(LocalConvexResultAddr self);
    void LocalConvexResult_set_m_localShapeInfo_1(LocalConvexResultAddr self, LocalShapeInfoAddr arg0);
    btVector3Addr LocalConvexResult_get_m_hitNormalLocal_0(LocalConvexResultAddr self);
    void LocalConvexResult_set_m_hitNormalLocal_1(LocalConvexResultAddr self, btVector3Addr arg0);
    btVector3Addr LocalConvexResult_get_m_hitPointLocal_0(LocalConvexResultAddr self);
    void LocalConvexResult_set_m_hitPointLocal_1(LocalConvexResultAddr self, btVector3Addr arg0);
    double LocalConvexResult_get_m_hitFraction_0(LocalConvexResultAddr self);
    void LocalConvexResult_set_m_hitFraction_1(LocalConvexResultAddr self, double arg0);
    void LocalConvexResult___destroy___0(LocalConvexResultAddr self);
	// btConstraintParams

    long_t enum_btConstraintParams_BT_CONSTRAINT_ERP();
    long_t enum_btConstraintParams_BT_CONSTRAINT_STOP_ERP();
    long_t enum_btConstraintParams_BT_CONSTRAINT_CFM();
    long_t enum_btConstraintParams_BT_CONSTRAINT_STOP_CFM();
	// PHY_ScalarType

    long_t enum_PHY_ScalarType_PHY_FLOAT();
    long_t enum_PHY_ScalarType_PHY_DOUBLE();
    long_t enum_PHY_ScalarType_PHY_INTEGER();
    long_t enum_PHY_ScalarType_PHY_SHORT();
    long_t enum_PHY_ScalarType_PHY_FIXEDPOINT88();
    long_t enum_PHY_ScalarType_PHY_UCHAR();

    void btGImpactCollisionAlgorithm_RegisterAlgorithm0(btCollisionDispatcherAddr arg0);



#define ExportJS_bullet() \
    /* btCollisionShape */ \
    JSbt_Bind_Global_Func(btCollisionShape_setLocalScaling_1, void, btCollisionShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCollisionShape_getLocalScaling_0, btVector3Addr, btCollisionShapeAddr); \
    JSbt_Bind_Global_Func(btCollisionShape_calculateLocalInertia_2, void, btCollisionShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btCollisionShape_setMargin_1, void, btCollisionShapeAddr, double); \
    JSbt_Bind_Global_Func(btCollisionShape_getMargin_0, double, btCollisionShapeAddr); \
    JSbt_Bind_Global_Func(btCollisionShape___destroy___0, void, btCollisionShapeAddr); \
    /* btCollisionObject */ \
    JSbt_Bind_Global_Func(btCollisionObject_btCollisionObject_0, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setAnisotropicFriction_2, void, btCollisionObjectAddr, btVector3Addr, long_t); \
    JSbt_Bind_Global_Func(btCollisionObject_getCollisionShape_0, btCollisionShapeAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setContactProcessingThreshold_1, void, btCollisionObjectAddr, double); \
    JSbt_Bind_Global_Func(btCollisionObject_setActivationState_1, void, btCollisionObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btCollisionObject_forceActivationState_1, void, btCollisionObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btCollisionObject_activate_0, void, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_activate_1, void, btCollisionObjectAddr, bool); \
    JSbt_Bind_Global_Func(btCollisionObject_isActive_0, bool, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_isKinematicObject_0, bool, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_isStaticObject_0, bool, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_isStaticOrKinematicObject_0, bool, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_getRestitution_0, double, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setRestitution_1, void, btCollisionObjectAddr, double); \
    JSbt_Bind_Global_Func(btCollisionObject_getFriction_0, double, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setFriction_1, void, btCollisionObjectAddr, double); \
    JSbt_Bind_Global_Func(btCollisionObject_getRollingFriction_0, double, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setRollingFriction_1, void, btCollisionObjectAddr, double); \
    JSbt_Bind_Global_Func(btCollisionObject_getCollisionFlags_0, long_t, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setCollisionFlags_1, void, btCollisionObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btCollisionObject_getWorldTransform_0, btTransformAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setWorldTransform_1, void, btCollisionObjectAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setCollisionShape_1, void, btCollisionObjectAddr, btCollisionShapeAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_getCcdMotionThreshold_0, double, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setCcdMotionThreshold_1, void, btCollisionObjectAddr, double); \
    JSbt_Bind_Global_Func(btCollisionObject_getCcdSweptSphereRadius_0, double, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setCcdSweptSphereRadius_1, void, btCollisionObjectAddr, double); \
    JSbt_Bind_Global_Func(btCollisionObject_getUserIndex_0, long_t, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setUserIndex_1, void, btCollisionObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btCollisionObject_getUserPointer_0, VoidPtrAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setUserPointer_1, void, btCollisionObjectAddr, VoidPtrAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_getInterpolationAngularVelocity_0, btVector3Addr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setInterpolationAngularVelocity_1, void, btCollisionObjectAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCollisionObject_getInterpolationLinearVelocity_0, btVector3Addr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_setInterpolationLinearVelocity_1, void, btCollisionObjectAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCollisionObject_getBroadphaseHandle_0, btBroadphaseProxyAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject_getActivationState_0, long_t, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionObject___destroy___0, void, btCollisionObjectAddr); \
    /* btTypedConstraint */ \
    JSbt_Bind_Global_Func(btTypedConstraint_enableFeedback_1, void, btTypedConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btTypedConstraint_getBreakingImpulseThreshold_0, double, btTypedConstraintAddr); \
    JSbt_Bind_Global_Func(btTypedConstraint_setBreakingImpulseThreshold_1, void, btTypedConstraintAddr, double); \
    JSbt_Bind_Global_Func(btTypedConstraint_getParam_2, double, btTypedConstraintAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(btTypedConstraint_setParam_3, void, btTypedConstraintAddr, long_t, double, long_t); \
    JSbt_Bind_Global_Func(btTypedConstraint___destroy___0, void, btTypedConstraintAddr); \
    /* btCollisionWorld */ \
    JSbt_Bind_Global_Func(btCollisionWorld_getDispatcher_0, btDispatcherAddr, btCollisionWorldAddr); \
    JSbt_Bind_Global_Func(btCollisionWorld_rayTest_3, void, btCollisionWorldAddr, btVector3Addr, btVector3Addr, RayResultCallbackAddr); \
    JSbt_Bind_Global_Func(btCollisionWorld_getPairCache_0, btOverlappingPairCacheAddr, btCollisionWorldAddr); \
    JSbt_Bind_Global_Func(btCollisionWorld_getDispatchInfo_0, btDispatcherInfoAddr, btCollisionWorldAddr); \
    JSbt_Bind_Global_Func(btCollisionWorld_addCollisionObject_1, void, btCollisionWorldAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionWorld_addCollisionObject_2, void, btCollisionWorldAddr, btCollisionObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btCollisionWorld_addCollisionObject_3, void, btCollisionWorldAddr, btCollisionObjectAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(btCollisionWorld_removeCollisionObject_1, void, btCollisionWorldAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionWorld_getBroadphase_0, btBroadphaseInterfaceAddr, btCollisionWorldAddr); \
    JSbt_Bind_Global_Func(btCollisionWorld_convexSweepTest_5, void, btCollisionWorldAddr, btConvexShapeAddr, btTransformAddr, btTransformAddr, ConvexResultCallbackAddr, double); \
    JSbt_Bind_Global_Func(btCollisionWorld_contactPairTest_3, void, btCollisionWorldAddr, btCollisionObjectAddr, btCollisionObjectAddr, ContactResultCallbackAddr); \
    JSbt_Bind_Global_Func(btCollisionWorld_contactTest_2, void, btCollisionWorldAddr, btCollisionObjectAddr, ContactResultCallbackAddr); \
    JSbt_Bind_Global_Func(btCollisionWorld_updateSingleAabb_1, void, btCollisionWorldAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btCollisionWorld___destroy___0, void, btCollisionWorldAddr); \
    /* btConcaveShape */ \
    JSbt_Bind_Global_Func(btConcaveShape_setLocalScaling_1, void, btConcaveShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btConcaveShape_getLocalScaling_0, btVector3Addr, btConcaveShapeAddr); \
    JSbt_Bind_Global_Func(btConcaveShape_calculateLocalInertia_2, void, btConcaveShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btConcaveShape___destroy___0, void, btConcaveShapeAddr); \
    /* btCapsuleShape */ \
    JSbt_Bind_Global_Func(btCapsuleShape_btCapsuleShape_2, btCapsuleShapeAddr, double, double); \
    JSbt_Bind_Global_Func(btCapsuleShape_setMargin_1, void, btCapsuleShapeAddr, double); \
    JSbt_Bind_Global_Func(btCapsuleShape_getMargin_0, double, btCapsuleShapeAddr); \
    JSbt_Bind_Global_Func(btCapsuleShape_getUpAxis_0, long_t, btCapsuleShapeAddr); \
    JSbt_Bind_Global_Func(btCapsuleShape_getRadius_0, double, btCapsuleShapeAddr); \
    JSbt_Bind_Global_Func(btCapsuleShape_getHalfHeight_0, double, btCapsuleShapeAddr); \
    JSbt_Bind_Global_Func(btCapsuleShape_setLocalScaling_1, void, btCapsuleShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCapsuleShape_getLocalScaling_0, btVector3Addr, btCapsuleShapeAddr); \
    JSbt_Bind_Global_Func(btCapsuleShape_calculateLocalInertia_2, void, btCapsuleShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btCapsuleShape___destroy___0, void, btCapsuleShapeAddr); \
    /* btGImpactShapeInterface */ \
    JSbt_Bind_Global_Func(btGImpactShapeInterface_updateBound_0, void, btGImpactShapeInterfaceAddr); \
    JSbt_Bind_Global_Func(btGImpactShapeInterface_setLocalScaling_1, void, btGImpactShapeInterfaceAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btGImpactShapeInterface_getLocalScaling_0, btVector3Addr, btGImpactShapeInterfaceAddr); \
    JSbt_Bind_Global_Func(btGImpactShapeInterface_calculateLocalInertia_2, void, btGImpactShapeInterfaceAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btGImpactShapeInterface___destroy___0, void, btGImpactShapeInterfaceAddr); \
    /* btDynamicsWorld */ \
    JSbt_Bind_Global_Func(btDynamicsWorld_addAction_1, void, btDynamicsWorldAddr, btActionInterfaceAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld_removeAction_1, void, btDynamicsWorldAddr, btActionInterfaceAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld_getSolverInfo_0, btContactSolverInfoAddr, btDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld_getDispatcher_0, btDispatcherAddr, btDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld_rayTest_3, void, btDynamicsWorldAddr, btVector3Addr, btVector3Addr, RayResultCallbackAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld_getPairCache_0, btOverlappingPairCacheAddr, btDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld_getDispatchInfo_0, btDispatcherInfoAddr, btDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld_addCollisionObject_1, void, btDynamicsWorldAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld_addCollisionObject_2, void, btDynamicsWorldAddr, btCollisionObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btDynamicsWorld_addCollisionObject_3, void, btDynamicsWorldAddr, btCollisionObjectAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(btDynamicsWorld_removeCollisionObject_1, void, btDynamicsWorldAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld_getBroadphase_0, btBroadphaseInterfaceAddr, btDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld_convexSweepTest_5, void, btDynamicsWorldAddr, btConvexShapeAddr, btTransformAddr, btTransformAddr, ConvexResultCallbackAddr, double); \
    JSbt_Bind_Global_Func(btDynamicsWorld_contactPairTest_3, void, btDynamicsWorldAddr, btCollisionObjectAddr, btCollisionObjectAddr, ContactResultCallbackAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld_contactTest_2, void, btDynamicsWorldAddr, btCollisionObjectAddr, ContactResultCallbackAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld_updateSingleAabb_1, void, btDynamicsWorldAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btDynamicsWorld___destroy___0, void, btDynamicsWorldAddr); \
    /* btTriangleMeshShape */ \
    JSbt_Bind_Global_Func(btTriangleMeshShape_setLocalScaling_1, void, btTriangleMeshShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btTriangleMeshShape_getLocalScaling_0, btVector3Addr, btTriangleMeshShapeAddr); \
    JSbt_Bind_Global_Func(btTriangleMeshShape_calculateLocalInertia_2, void, btTriangleMeshShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btTriangleMeshShape___destroy___0, void, btTriangleMeshShapeAddr); \
    /* btGhostObject */ \
    JSbt_Bind_Global_Func(btGhostObject_btGhostObject_0, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_getNumOverlappingObjects_0, long_t, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_getOverlappingObject_1, btCollisionObjectAddr, btGhostObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btGhostObject_setAnisotropicFriction_2, void, btGhostObjectAddr, btVector3Addr, long_t); \
    JSbt_Bind_Global_Func(btGhostObject_getCollisionShape_0, btCollisionShapeAddr, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setContactProcessingThreshold_1, void, btGhostObjectAddr, double); \
    JSbt_Bind_Global_Func(btGhostObject_setActivationState_1, void, btGhostObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btGhostObject_forceActivationState_1, void, btGhostObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btGhostObject_activate_0, void, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_activate_1, void, btGhostObjectAddr, bool); \
    JSbt_Bind_Global_Func(btGhostObject_isActive_0, bool, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_isKinematicObject_0, bool, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_isStaticObject_0, bool, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_isStaticOrKinematicObject_0, bool, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_getRestitution_0, double, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setRestitution_1, void, btGhostObjectAddr, double); \
    JSbt_Bind_Global_Func(btGhostObject_getFriction_0, double, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setFriction_1, void, btGhostObjectAddr, double); \
    JSbt_Bind_Global_Func(btGhostObject_getRollingFriction_0, double, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setRollingFriction_1, void, btGhostObjectAddr, double); \
    JSbt_Bind_Global_Func(btGhostObject_getCollisionFlags_0, long_t, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setCollisionFlags_1, void, btGhostObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btGhostObject_getWorldTransform_0, btTransformAddr, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setWorldTransform_1, void, btGhostObjectAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setCollisionShape_1, void, btGhostObjectAddr, btCollisionShapeAddr); \
    JSbt_Bind_Global_Func(btGhostObject_getCcdMotionThreshold_0, double, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setCcdMotionThreshold_1, void, btGhostObjectAddr, double); \
    JSbt_Bind_Global_Func(btGhostObject_getCcdSweptSphereRadius_0, double, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setCcdSweptSphereRadius_1, void, btGhostObjectAddr, double); \
    JSbt_Bind_Global_Func(btGhostObject_getUserIndex_0, long_t, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setUserIndex_1, void, btGhostObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btGhostObject_getUserPointer_0, VoidPtrAddr, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setUserPointer_1, void, btGhostObjectAddr, VoidPtrAddr); \
    JSbt_Bind_Global_Func(btGhostObject_getInterpolationAngularVelocity_0, btVector3Addr, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setInterpolationAngularVelocity_1, void, btGhostObjectAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btGhostObject_getInterpolationLinearVelocity_0, btVector3Addr, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_setInterpolationLinearVelocity_1, void, btGhostObjectAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btGhostObject_getBroadphaseHandle_0, btBroadphaseProxyAddr, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject_getActivationState_0, long_t, btGhostObjectAddr); \
    JSbt_Bind_Global_Func(btGhostObject___destroy___0, void, btGhostObjectAddr); \
    /* btConeShape */ \
    JSbt_Bind_Global_Func(btConeShape_btConeShape_2, btConeShapeAddr, double, double); \
    JSbt_Bind_Global_Func(btConeShape_setLocalScaling_1, void, btConeShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btConeShape_getLocalScaling_0, btVector3Addr, btConeShapeAddr); \
    JSbt_Bind_Global_Func(btConeShape_calculateLocalInertia_2, void, btConeShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btConeShape___destroy___0, void, btConeShapeAddr); \
    /* btActionInterface */ \
    JSbt_Bind_Global_Func(btActionInterface_updateAction_2, void, btActionInterfaceAddr, btCollisionWorldAddr, double); \
    JSbt_Bind_Global_Func(btActionInterface___destroy___0, void, btActionInterfaceAddr); \
    /* btVector3 */ \
    JSbt_Bind_Global_Func(btVector3_btVector3_0, btVector3Addr); \
    JSbt_Bind_Global_Func(btVector3_btVector3_3, btVector3Addr, double, double, double); \
    JSbt_Bind_Global_Func(btVector3_length_0, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btVector3_x_0, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btVector3_y_0, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btVector3_z_0, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btVector3_setX_1, void, btVector3Addr, double); \
    JSbt_Bind_Global_Func(btVector3_setY_1, void, btVector3Addr, double); \
    JSbt_Bind_Global_Func(btVector3_setZ_1, void, btVector3Addr, double); \
    JSbt_Bind_Global_Func(btVector3_setValue_3, void, btVector3Addr, double, double, double); \
    JSbt_Bind_Global_Func(btVector3_normalize_0, void, btVector3Addr); \
    JSbt_Bind_Global_Func(btVector3_rotate_2, btVector3Addr, btVector3Addr, btVector3Addr, double); \
    JSbt_Bind_Global_Func(btVector3_dot_1, double, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btVector3_op_mul_1, btVector3Addr, btVector3Addr, double); \
    JSbt_Bind_Global_Func(btVector3_op_add_1, btVector3Addr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btVector3_op_sub_1, btVector3Addr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btVector3___destroy___0, void, btVector3Addr); \
    /* btVehicleRaycaster */ \
    JSbt_Bind_Global_Func(btVehicleRaycaster_castRay_3, void, btVehicleRaycasterAddr, btVector3Addr, btVector3Addr, btVehicleRaycasterResultAddr); \
    JSbt_Bind_Global_Func(btVehicleRaycaster___destroy___0, void, btVehicleRaycasterAddr); \
    /* btQuadWord */ \
    JSbt_Bind_Global_Func(btQuadWord_x_0, double, btQuadWordAddr); \
    JSbt_Bind_Global_Func(btQuadWord_y_0, double, btQuadWordAddr); \
    JSbt_Bind_Global_Func(btQuadWord_z_0, double, btQuadWordAddr); \
    JSbt_Bind_Global_Func(btQuadWord_w_0, double, btQuadWordAddr); \
    JSbt_Bind_Global_Func(btQuadWord_setX_1, void, btQuadWordAddr, double); \
    JSbt_Bind_Global_Func(btQuadWord_setY_1, void, btQuadWordAddr, double); \
    JSbt_Bind_Global_Func(btQuadWord_setZ_1, void, btQuadWordAddr, double); \
    JSbt_Bind_Global_Func(btQuadWord_setW_1, void, btQuadWordAddr, double); \
    JSbt_Bind_Global_Func(btQuadWord___destroy___0, void, btQuadWordAddr); \
    /* btCylinderShape */ \
    JSbt_Bind_Global_Func(btCylinderShape_btCylinderShape_1, btCylinderShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCylinderShape_setMargin_1, void, btCylinderShapeAddr, double); \
    JSbt_Bind_Global_Func(btCylinderShape_getMargin_0, double, btCylinderShapeAddr); \
    JSbt_Bind_Global_Func(btCylinderShape_setLocalScaling_1, void, btCylinderShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCylinderShape_getLocalScaling_0, btVector3Addr, btCylinderShapeAddr); \
    JSbt_Bind_Global_Func(btCylinderShape_calculateLocalInertia_2, void, btCylinderShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btCylinderShape___destroy___0, void, btCylinderShapeAddr); \
    /* btConvexShape */ \
    JSbt_Bind_Global_Func(btConvexShape_setLocalScaling_1, void, btConvexShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btConvexShape_getLocalScaling_0, btVector3Addr, btConvexShapeAddr); \
    JSbt_Bind_Global_Func(btConvexShape_calculateLocalInertia_2, void, btConvexShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btConvexShape_setMargin_1, void, btConvexShapeAddr, double); \
    JSbt_Bind_Global_Func(btConvexShape_getMargin_0, double, btConvexShapeAddr); \
    JSbt_Bind_Global_Func(btConvexShape___destroy___0, void, btConvexShapeAddr); \
    /* btDispatcher */ \
    JSbt_Bind_Global_Func(btDispatcher_getNumManifolds_0, long_t, btDispatcherAddr); \
    JSbt_Bind_Global_Func(btDispatcher_getManifoldByIndexInternal_1, btPersistentManifoldAddr, btDispatcherAddr, long_t); \
    JSbt_Bind_Global_Func(btDispatcher___destroy___0, void, btDispatcherAddr); \
    /* btGeneric6DofConstraint */ \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint_btGeneric6DofConstraint_3, btGeneric6DofConstraintAddr, btRigidBodyAddr, btTransformAddr, bool); \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint_btGeneric6DofConstraint_5, btGeneric6DofConstraintAddr, btRigidBodyAddr, btRigidBodyAddr, btTransformAddr, btTransformAddr, bool); \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint_setLinearLowerLimit_1, void, btGeneric6DofConstraintAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint_setLinearUpperLimit_1, void, btGeneric6DofConstraintAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint_setAngularLowerLimit_1, void, btGeneric6DofConstraintAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint_setAngularUpperLimit_1, void, btGeneric6DofConstraintAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint_getFrameOffsetA_0, btTransformAddr, btGeneric6DofConstraintAddr); \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint_enableFeedback_1, void, btGeneric6DofConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint_getBreakingImpulseThreshold_0, double, btGeneric6DofConstraintAddr); \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint_setBreakingImpulseThreshold_1, void, btGeneric6DofConstraintAddr, double); \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint_getParam_2, double, btGeneric6DofConstraintAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint_setParam_3, void, btGeneric6DofConstraintAddr, long_t, double, long_t); \
    JSbt_Bind_Global_Func(btGeneric6DofConstraint___destroy___0, void, btGeneric6DofConstraintAddr); \
    /* btStridingMeshInterface */ \
    JSbt_Bind_Global_Func(btStridingMeshInterface___destroy___0, void, btStridingMeshInterfaceAddr); \
    /* btMotionState */ \
    JSbt_Bind_Global_Func(btMotionState_getWorldTransform_1, void, btMotionStateAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btMotionState_setWorldTransform_1, void, btMotionStateAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btMotionState___destroy___0, void, btMotionStateAddr); \
    /* ConvexResultCallback */ \
    JSbt_Bind_Global_Func(ConvexResultCallback_hasHit_0, bool, ConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ConvexResultCallback_get_m_collisionFilterGroup_0, long_t, ConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ConvexResultCallback_set_m_collisionFilterGroup_1, void, ConvexResultCallbackAddr, long_t); \
    JSbt_Bind_Global_Func(ConvexResultCallback_get_m_collisionFilterMask_0, long_t, ConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ConvexResultCallback_set_m_collisionFilterMask_1, void, ConvexResultCallbackAddr, long_t); \
    JSbt_Bind_Global_Func(ConvexResultCallback_get_m_closestHitFraction_0, double, ConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ConvexResultCallback_set_m_closestHitFraction_1, void, ConvexResultCallbackAddr, double); \
    JSbt_Bind_Global_Func(ConvexResultCallback___destroy___0, void, ConvexResultCallbackAddr); \
    /* ContactResultCallback */ \
    JSbt_Bind_Global_Func(ContactResultCallback_addSingleResult_7, double, ContactResultCallbackAddr, btManifoldPointAddr, btCollisionObjectWrapperAddr, long_t, long_t, btCollisionObjectWrapperAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(ContactResultCallback___destroy___0, void, ContactResultCallbackAddr); \
    /* RayResultCallback */ \
    JSbt_Bind_Global_Func(RayResultCallback_hasHit_0, bool, RayResultCallbackAddr); \
    JSbt_Bind_Global_Func(RayResultCallback_get_m_collisionFilterGroup_0, long_t, RayResultCallbackAddr); \
    JSbt_Bind_Global_Func(RayResultCallback_set_m_collisionFilterGroup_1, void, RayResultCallbackAddr, long_t); \
    JSbt_Bind_Global_Func(RayResultCallback_get_m_collisionFilterMask_0, long_t, RayResultCallbackAddr); \
    JSbt_Bind_Global_Func(RayResultCallback_set_m_collisionFilterMask_1, void, RayResultCallbackAddr, long_t); \
    JSbt_Bind_Global_Func(RayResultCallback_get_m_closestHitFraction_0, double, RayResultCallbackAddr); \
    JSbt_Bind_Global_Func(RayResultCallback_set_m_closestHitFraction_1, void, RayResultCallbackAddr, double); \
    JSbt_Bind_Global_Func(RayResultCallback_get_m_collisionObject_0, btCollisionObjectAddr, RayResultCallbackAddr); \
    JSbt_Bind_Global_Func(RayResultCallback_set_m_collisionObject_1, void, RayResultCallbackAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(RayResultCallback___destroy___0, void, RayResultCallbackAddr); \
    /* btMatrix3x3 */ \
    JSbt_Bind_Global_Func(btMatrix3x3_setEulerZYX_3, void, btMatrix3x3Addr, double, double, double); \
    JSbt_Bind_Global_Func(btMatrix3x3_getRotation_1, void, btMatrix3x3Addr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btMatrix3x3_getRow_1, btVector3Addr, btMatrix3x3Addr, long_t); \
    JSbt_Bind_Global_Func(btMatrix3x3___destroy___0, void, btMatrix3x3Addr); \
    /* btDispatcherInfo */ \
    JSbt_Bind_Global_Func(btDispatcherInfo_get_m_timeStep_0, double, btDispatcherInfoAddr); \
    JSbt_Bind_Global_Func(btDispatcherInfo_set_m_timeStep_1, void, btDispatcherInfoAddr, double); \
    JSbt_Bind_Global_Func(btDispatcherInfo_get_m_stepCount_0, long_t, btDispatcherInfoAddr); \
    JSbt_Bind_Global_Func(btDispatcherInfo_set_m_stepCount_1, void, btDispatcherInfoAddr, long_t); \
    JSbt_Bind_Global_Func(btDispatcherInfo_get_m_dispatchFunc_0, long_t, btDispatcherInfoAddr); \
    JSbt_Bind_Global_Func(btDispatcherInfo_set_m_dispatchFunc_1, void, btDispatcherInfoAddr, long_t); \
    JSbt_Bind_Global_Func(btDispatcherInfo_get_m_timeOfImpact_0, double, btDispatcherInfoAddr); \
    JSbt_Bind_Global_Func(btDispatcherInfo_set_m_timeOfImpact_1, void, btDispatcherInfoAddr, double); \
    JSbt_Bind_Global_Func(btDispatcherInfo_get_m_useContinuous_0, bool, btDispatcherInfoAddr); \
    JSbt_Bind_Global_Func(btDispatcherInfo_set_m_useContinuous_1, void, btDispatcherInfoAddr, bool); \
    JSbt_Bind_Global_Func(btDispatcherInfo_get_m_enableSatConvex_0, bool, btDispatcherInfoAddr); \
    JSbt_Bind_Global_Func(btDispatcherInfo_set_m_enableSatConvex_1, void, btDispatcherInfoAddr, bool); \
    JSbt_Bind_Global_Func(btDispatcherInfo_get_m_enableSPU_0, bool, btDispatcherInfoAddr); \
    JSbt_Bind_Global_Func(btDispatcherInfo_set_m_enableSPU_1, void, btDispatcherInfoAddr, bool); \
    JSbt_Bind_Global_Func(btDispatcherInfo_get_m_useEpa_0, bool, btDispatcherInfoAddr); \
    JSbt_Bind_Global_Func(btDispatcherInfo_set_m_useEpa_1, void, btDispatcherInfoAddr, bool); \
    JSbt_Bind_Global_Func(btDispatcherInfo_get_m_allowedCcdPenetration_0, double, btDispatcherInfoAddr); \
    JSbt_Bind_Global_Func(btDispatcherInfo_set_m_allowedCcdPenetration_1, void, btDispatcherInfoAddr, double); \
    JSbt_Bind_Global_Func(btDispatcherInfo_get_m_useConvexConservativeDistanceUtil_0, bool, btDispatcherInfoAddr); \
    JSbt_Bind_Global_Func(btDispatcherInfo_set_m_useConvexConservativeDistanceUtil_1, void, btDispatcherInfoAddr, bool); \
    JSbt_Bind_Global_Func(btDispatcherInfo_get_m_convexConservativeDistanceThreshold_0, double, btDispatcherInfoAddr); \
    JSbt_Bind_Global_Func(btDispatcherInfo_set_m_convexConservativeDistanceThreshold_1, void, btDispatcherInfoAddr, double); \
    JSbt_Bind_Global_Func(btDispatcherInfo___destroy___0, void, btDispatcherInfoAddr); \
    /* LayaMotionState */ \
    JSbt_Bind_Global_Func(LayaMotionState_LayaMotionState_0, LayaMotionStateAddr); \
    JSbt_Bind_Global_Func(LayaMotionState_getWorldTransform_1, void, LayaMotionStateAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(LayaMotionState_setWorldTransform_1, void, LayaMotionStateAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(LayaMotionState_setNativeGetWorldTransform_1, void, LayaMotionStateAddr, JSValueAsParam); \
    JSbt_Bind_Global_Func(LayaMotionState_setNativeSetWorldTransform_1, void, LayaMotionStateAddr, JSValueAsParam); \
    JSbt_Bind_Global_Func(LayaMotionState_setRigidbody_1, void, LayaMotionStateAddr, JsValue); \
    JSbt_Bind_Global_Func(LayaMotionState___destroy___0, void, LayaMotionStateAddr); \
    /* btWheelInfoConstructionInfo */ \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_get_m_chassisConnectionCS_0, btVector3Addr, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_set_m_chassisConnectionCS_1, void, btWheelInfoConstructionInfoAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_get_m_wheelDirectionCS_0, btVector3Addr, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_set_m_wheelDirectionCS_1, void, btWheelInfoConstructionInfoAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_get_m_wheelAxleCS_0, btVector3Addr, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_set_m_wheelAxleCS_1, void, btWheelInfoConstructionInfoAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_get_m_suspensionRestLength_0, double, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_set_m_suspensionRestLength_1, void, btWheelInfoConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_get_m_maxSuspensionTravelCm_0, double, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_set_m_maxSuspensionTravelCm_1, void, btWheelInfoConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_get_m_wheelRadius_0, double, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_set_m_wheelRadius_1, void, btWheelInfoConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_get_m_suspensionStiffness_0, double, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_set_m_suspensionStiffness_1, void, btWheelInfoConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_get_m_wheelsDampingCompression_0, double, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_set_m_wheelsDampingCompression_1, void, btWheelInfoConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_get_m_wheelsDampingRelaxation_0, double, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_set_m_wheelsDampingRelaxation_1, void, btWheelInfoConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_get_m_frictionSlip_0, double, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_set_m_frictionSlip_1, void, btWheelInfoConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_get_m_maxSuspensionForce_0, double, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_set_m_maxSuspensionForce_1, void, btWheelInfoConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_get_m_bIsFrontWheel_0, bool, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo_set_m_bIsFrontWheel_1, void, btWheelInfoConstructionInfoAddr, bool); \
    JSbt_Bind_Global_Func(btWheelInfoConstructionInfo___destroy___0, void, btWheelInfoConstructionInfoAddr); \
    /* btConvexTriangleMeshShape */ \
    JSbt_Bind_Global_Func(btConvexTriangleMeshShape_btConvexTriangleMeshShape_1, btConvexTriangleMeshShapeAddr, btStridingMeshInterfaceAddr); \
    JSbt_Bind_Global_Func(btConvexTriangleMeshShape_btConvexTriangleMeshShape_2, btConvexTriangleMeshShapeAddr, btStridingMeshInterfaceAddr, bool); \
    JSbt_Bind_Global_Func(btConvexTriangleMeshShape_setLocalScaling_1, void, btConvexTriangleMeshShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btConvexTriangleMeshShape_getLocalScaling_0, btVector3Addr, btConvexTriangleMeshShapeAddr); \
    JSbt_Bind_Global_Func(btConvexTriangleMeshShape_calculateLocalInertia_2, void, btConvexTriangleMeshShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btConvexTriangleMeshShape_setMargin_1, void, btConvexTriangleMeshShapeAddr, double); \
    JSbt_Bind_Global_Func(btConvexTriangleMeshShape_getMargin_0, double, btConvexTriangleMeshShapeAddr); \
    JSbt_Bind_Global_Func(btConvexTriangleMeshShape___destroy___0, void, btConvexTriangleMeshShapeAddr); \
    /* btBroadphaseInterface */ \
    JSbt_Bind_Global_Func(btBroadphaseInterface___destroy___0, void, btBroadphaseInterfaceAddr); \
    /* btDefaultCollisionConfiguration */ \
    JSbt_Bind_Global_Func(btDefaultCollisionConfiguration_btDefaultCollisionConfiguration_0, btDefaultCollisionConfigurationAddr); \
    JSbt_Bind_Global_Func(btDefaultCollisionConfiguration_btDefaultCollisionConfiguration_1, btDefaultCollisionConfigurationAddr, btDefaultCollisionConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btDefaultCollisionConfiguration___destroy___0, void, btDefaultCollisionConfigurationAddr); \
    /* btRigidBodyConstructionInfo */ \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_btRigidBodyConstructionInfo_3, btRigidBodyConstructionInfoAddr, double, btMotionStateAddr, btCollisionShapeAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_btRigidBodyConstructionInfo_4, btRigidBodyConstructionInfoAddr, double, btMotionStateAddr, btCollisionShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_get_m_linearDamping_0, double, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_set_m_linearDamping_1, void, btRigidBodyConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_get_m_angularDamping_0, double, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_set_m_angularDamping_1, void, btRigidBodyConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_get_m_friction_0, double, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_set_m_friction_1, void, btRigidBodyConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_get_m_rollingFriction_0, double, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_set_m_rollingFriction_1, void, btRigidBodyConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_get_m_restitution_0, double, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_set_m_restitution_1, void, btRigidBodyConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_get_m_linearSleepingThreshold_0, double, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_set_m_linearSleepingThreshold_1, void, btRigidBodyConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_get_m_angularSleepingThreshold_0, double, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_set_m_angularSleepingThreshold_1, void, btRigidBodyConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_get_m_additionalDamping_0, bool, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_set_m_additionalDamping_1, void, btRigidBodyConstructionInfoAddr, bool); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_get_m_additionalDampingFactor_0, double, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_set_m_additionalDampingFactor_1, void, btRigidBodyConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_get_m_additionalLinearDampingThresholdSqr_0, double, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_set_m_additionalLinearDampingThresholdSqr_1, void, btRigidBodyConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_get_m_additionalAngularDampingThresholdSqr_0, double, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_set_m_additionalAngularDampingThresholdSqr_1, void, btRigidBodyConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_get_m_additionalAngularDampingFactor_0, double, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo_set_m_additionalAngularDampingFactor_1, void, btRigidBodyConstructionInfoAddr, double); \
    JSbt_Bind_Global_Func(btRigidBodyConstructionInfo___destroy___0, void, btRigidBodyConstructionInfoAddr); \
    /* btCollisionConfiguration */ \
    JSbt_Bind_Global_Func(btCollisionConfiguration___destroy___0, void, btCollisionConfigurationAddr); \
    /* btPersistentManifold */ \
    JSbt_Bind_Global_Func(btPersistentManifold_btPersistentManifold_0, btPersistentManifoldAddr); \
    JSbt_Bind_Global_Func(btPersistentManifold_getBody0_0, btCollisionObjectAddr, btPersistentManifoldAddr); \
    JSbt_Bind_Global_Func(btPersistentManifold_getBody1_0, btCollisionObjectAddr, btPersistentManifoldAddr); \
    JSbt_Bind_Global_Func(btPersistentManifold_getNumContacts_0, long_t, btPersistentManifoldAddr); \
    JSbt_Bind_Global_Func(btPersistentManifold_getContactPoint_1, btManifoldPointAddr, btPersistentManifoldAddr, long_t); \
    JSbt_Bind_Global_Func(btPersistentManifold___destroy___0, void, btPersistentManifoldAddr); \
    /* btCompoundShape */ \
    JSbt_Bind_Global_Func(btCompoundShape_btCompoundShape_0, btCompoundShapeAddr); \
    JSbt_Bind_Global_Func(btCompoundShape_btCompoundShape_1, btCompoundShapeAddr, bool); \
    JSbt_Bind_Global_Func(btCompoundShape_addChildShape_2, void, btCompoundShapeAddr, btTransformAddr, btCollisionShapeAddr); \
    JSbt_Bind_Global_Func(btCompoundShape_removeChildShapeByIndex_1, void, btCompoundShapeAddr, long_t); \
    JSbt_Bind_Global_Func(btCompoundShape_getNumChildShapes_0, long_t, btCompoundShapeAddr); \
    JSbt_Bind_Global_Func(btCompoundShape_getChildShape_1, btCollisionShapeAddr, btCompoundShapeAddr, long_t); \
    JSbt_Bind_Global_Func(btCompoundShape_setMargin_1, void, btCompoundShapeAddr, double); \
    JSbt_Bind_Global_Func(btCompoundShape_getMargin_0, double, btCompoundShapeAddr); \
    JSbt_Bind_Global_Func(btCompoundShape_updateChildTransform_2, void, btCompoundShapeAddr, long_t, btTransformAddr); \
    JSbt_Bind_Global_Func(btCompoundShape_updateChildTransform_3, void, btCompoundShapeAddr, long_t, btTransformAddr, bool); \
    JSbt_Bind_Global_Func(btCompoundShape_setLocalScaling_1, void, btCompoundShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCompoundShape_getLocalScaling_0, btVector3Addr, btCompoundShapeAddr); \
    JSbt_Bind_Global_Func(btCompoundShape_calculateLocalInertia_2, void, btCompoundShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btCompoundShape___destroy___0, void, btCompoundShapeAddr); \
    /* ClosestConvexResultCallback */ \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_ClosestConvexResultCallback_2, ClosestConvexResultCallbackAddr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_hasHit_0, bool, ClosestConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_get_m_convexFromWorld_0, btVector3Addr, ClosestConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_set_m_convexFromWorld_1, void, ClosestConvexResultCallbackAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_get_m_convexToWorld_0, btVector3Addr, ClosestConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_set_m_convexToWorld_1, void, ClosestConvexResultCallbackAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_get_m_hitNormalWorld_0, btVector3Addr, ClosestConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_set_m_hitNormalWorld_1, void, ClosestConvexResultCallbackAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_get_m_hitPointWorld_0, btVector3Addr, ClosestConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_set_m_hitPointWorld_1, void, ClosestConvexResultCallbackAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_get_m_hitCollisionObject_0, btCollisionObjectAddr, ClosestConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_set_m_hitCollisionObject_1, void, ClosestConvexResultCallbackAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_get_m_collisionFilterGroup_0, long_t, ClosestConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_set_m_collisionFilterGroup_1, void, ClosestConvexResultCallbackAddr, long_t); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_get_m_collisionFilterMask_0, long_t, ClosestConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_set_m_collisionFilterMask_1, void, ClosestConvexResultCallbackAddr, long_t); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_get_m_closestHitFraction_0, double, ClosestConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback_set_m_closestHitFraction_1, void, ClosestConvexResultCallbackAddr, double); \
    JSbt_Bind_Global_Func(ClosestConvexResultCallback___destroy___0, void, ClosestConvexResultCallbackAddr); \
    /* AllHitsRayResultCallback */ \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_AllHitsRayResultCallback_2, AllHitsRayResultCallbackAddr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_hasHit_0, bool, AllHitsRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_get_m_collisionObjects_0, tBtCollisionObjectArrayAddr, AllHitsRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_set_m_collisionObjects_1, void, AllHitsRayResultCallbackAddr, tBtCollisionObjectArrayAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_get_m_rayFromWorld_0, btVector3Addr, AllHitsRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_set_m_rayFromWorld_1, void, AllHitsRayResultCallbackAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_get_m_rayToWorld_0, btVector3Addr, AllHitsRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_set_m_rayToWorld_1, void, AllHitsRayResultCallbackAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_get_m_hitNormalWorld_0, tVector3ArrayAddr, AllHitsRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_set_m_hitNormalWorld_1, void, AllHitsRayResultCallbackAddr, tVector3ArrayAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_get_m_hitPointWorld_0, tVector3ArrayAddr, AllHitsRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_set_m_hitPointWorld_1, void, AllHitsRayResultCallbackAddr, tVector3ArrayAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_get_m_hitFractions_0, tScalarArrayAddr, AllHitsRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_set_m_hitFractions_1, void, AllHitsRayResultCallbackAddr, tScalarArrayAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_get_m_collisionFilterGroup_0, long_t, AllHitsRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_set_m_collisionFilterGroup_1, void, AllHitsRayResultCallbackAddr, long_t); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_get_m_collisionFilterMask_0, long_t, AllHitsRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_set_m_collisionFilterMask_1, void, AllHitsRayResultCallbackAddr, long_t); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_get_m_closestHitFraction_0, double, AllHitsRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_set_m_closestHitFraction_1, void, AllHitsRayResultCallbackAddr, double); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_get_m_collisionObject_0, btCollisionObjectAddr, AllHitsRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback_set_m_collisionObject_1, void, AllHitsRayResultCallbackAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(AllHitsRayResultCallback___destroy___0, void, AllHitsRayResultCallbackAddr); \
    /* btDefaultVehicleRaycaster */ \
    JSbt_Bind_Global_Func(btDefaultVehicleRaycaster_btDefaultVehicleRaycaster_1, btDefaultVehicleRaycasterAddr, btDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDefaultVehicleRaycaster_castRay_3, void, btDefaultVehicleRaycasterAddr, btVector3Addr, btVector3Addr, btVehicleRaycasterResultAddr); \
    JSbt_Bind_Global_Func(btDefaultVehicleRaycaster___destroy___0, void, btDefaultVehicleRaycasterAddr); \
    /* btConstraintSetting */ \
    JSbt_Bind_Global_Func(btConstraintSetting_btConstraintSetting_0, btConstraintSettingAddr); \
    JSbt_Bind_Global_Func(btConstraintSetting_get_m_tau_0, double, btConstraintSettingAddr); \
    JSbt_Bind_Global_Func(btConstraintSetting_set_m_tau_1, void, btConstraintSettingAddr, double); \
    JSbt_Bind_Global_Func(btConstraintSetting_get_m_damping_0, double, btConstraintSettingAddr); \
    JSbt_Bind_Global_Func(btConstraintSetting_set_m_damping_1, void, btConstraintSettingAddr, double); \
    JSbt_Bind_Global_Func(btConstraintSetting_get_m_impulseClamp_0, double, btConstraintSettingAddr); \
    JSbt_Bind_Global_Func(btConstraintSetting_set_m_impulseClamp_1, void, btConstraintSettingAddr, double); \
    JSbt_Bind_Global_Func(btConstraintSetting___destroy___0, void, btConstraintSettingAddr); \
    /* LocalShapeInfo */ \
    JSbt_Bind_Global_Func(LocalShapeInfo_get_m_shapePart_0, long_t, LocalShapeInfoAddr); \
    JSbt_Bind_Global_Func(LocalShapeInfo_set_m_shapePart_1, void, LocalShapeInfoAddr, long_t); \
    JSbt_Bind_Global_Func(LocalShapeInfo_get_m_triangleIndex_0, long_t, LocalShapeInfoAddr); \
    JSbt_Bind_Global_Func(LocalShapeInfo_set_m_triangleIndex_1, void, LocalShapeInfoAddr, long_t); \
    JSbt_Bind_Global_Func(LocalShapeInfo___destroy___0, void, LocalShapeInfoAddr); \
    /* btRigidBody */ \
    JSbt_Bind_Global_Func(btRigidBody_btRigidBody_1, btRigidBodyAddr, btRigidBodyConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btRigidBody_getCenterOfMassTransform_0, btTransformAddr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setCenterOfMassTransform_1, void, btRigidBodyAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setSleepingThresholds_2, void, btRigidBodyAddr, double, double); \
    JSbt_Bind_Global_Func(btRigidBody_getLinearSleepingThreshold_0, double, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_getAngularSleepingThreshold_0, double, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setDamping_2, void, btRigidBodyAddr, double, double); \
    JSbt_Bind_Global_Func(btRigidBody_setMassProps_2, void, btRigidBodyAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_setLinearFactor_1, void, btRigidBodyAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_applyTorque_1, void, btRigidBodyAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_applyForce_2, void, btRigidBodyAddr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_applyCentralForce_1, void, btRigidBodyAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_applyTorqueImpulse_1, void, btRigidBodyAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_applyImpulse_2, void, btRigidBodyAddr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_applyCentralImpulse_1, void, btRigidBodyAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_updateInertiaTensor_0, void, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_getLinearVelocity_0, btVector3Addr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_getAngularVelocity_0, btVector3Addr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setLinearVelocity_1, void, btRigidBodyAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_setAngularVelocity_1, void, btRigidBodyAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_getMotionState_0, btMotionStateAddr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setMotionState_1, void, btRigidBodyAddr, btMotionStateAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setAngularFactor_1, void, btRigidBodyAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_upcast_1, btRigidBodyAddr, btRigidBodyAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btRigidBody_getAabb_2, void, btRigidBodyAddr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_applyGravity_0, void, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_getGravity_0, btVector3Addr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setGravity_1, void, btRigidBodyAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_getTotalForce_0, btVector3Addr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_getTotalTorque_0, btVector3Addr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_getFlags_0, long_t, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setFlags_1, void, btRigidBodyAddr, long_t); \
    JSbt_Bind_Global_Func(btRigidBody_clearForces_0, void, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setAnisotropicFriction_2, void, btRigidBodyAddr, btVector3Addr, long_t); \
    JSbt_Bind_Global_Func(btRigidBody_getCollisionShape_0, btCollisionShapeAddr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setContactProcessingThreshold_1, void, btRigidBodyAddr, double); \
    JSbt_Bind_Global_Func(btRigidBody_setActivationState_1, void, btRigidBodyAddr, long_t); \
    JSbt_Bind_Global_Func(btRigidBody_forceActivationState_1, void, btRigidBodyAddr, long_t); \
    JSbt_Bind_Global_Func(btRigidBody_activate_0, void, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_activate_1, void, btRigidBodyAddr, bool); \
    JSbt_Bind_Global_Func(btRigidBody_isActive_0, bool, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_isKinematicObject_0, bool, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_isStaticObject_0, bool, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_isStaticOrKinematicObject_0, bool, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_getRestitution_0, double, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setRestitution_1, void, btRigidBodyAddr, double); \
    JSbt_Bind_Global_Func(btRigidBody_getFriction_0, double, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setFriction_1, void, btRigidBodyAddr, double); \
    JSbt_Bind_Global_Func(btRigidBody_getRollingFriction_0, double, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setRollingFriction_1, void, btRigidBodyAddr, double); \
    JSbt_Bind_Global_Func(btRigidBody_getCollisionFlags_0, long_t, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setCollisionFlags_1, void, btRigidBodyAddr, long_t); \
    JSbt_Bind_Global_Func(btRigidBody_getWorldTransform_0, btTransformAddr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setWorldTransform_1, void, btRigidBodyAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setCollisionShape_1, void, btRigidBodyAddr, btCollisionShapeAddr); \
    JSbt_Bind_Global_Func(btRigidBody_getCcdMotionThreshold_0, double, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setCcdMotionThreshold_1, void, btRigidBodyAddr, double); \
    JSbt_Bind_Global_Func(btRigidBody_getCcdSweptSphereRadius_0, double, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setCcdSweptSphereRadius_1, void, btRigidBodyAddr, double); \
    JSbt_Bind_Global_Func(btRigidBody_getUserIndex_0, long_t, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setUserIndex_1, void, btRigidBodyAddr, long_t); \
    JSbt_Bind_Global_Func(btRigidBody_getUserPointer_0, VoidPtrAddr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setUserPointer_1, void, btRigidBodyAddr, VoidPtrAddr); \
    JSbt_Bind_Global_Func(btRigidBody_getInterpolationAngularVelocity_0, btVector3Addr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setInterpolationAngularVelocity_1, void, btRigidBodyAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_getInterpolationLinearVelocity_0, btVector3Addr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_setInterpolationLinearVelocity_1, void, btRigidBodyAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btRigidBody_getBroadphaseHandle_0, btBroadphaseProxyAddr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody_getActivationState_0, long_t, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btRigidBody___destroy___0, void, btRigidBodyAddr); \
    /* btDbvtBroadphase */ \
    JSbt_Bind_Global_Func(btDbvtBroadphase_btDbvtBroadphase_0, btDbvtBroadphaseAddr); \
    JSbt_Bind_Global_Func(btDbvtBroadphase_getOverlappingPairCache_0, btOverlappingPairCacheAddr, btDbvtBroadphaseAddr); \
    JSbt_Bind_Global_Func(btDbvtBroadphase___destroy___0, void, btDbvtBroadphaseAddr); \
    /* btCollisionDispatcher */ \
    JSbt_Bind_Global_Func(btCollisionDispatcher_btCollisionDispatcher_1, btCollisionDispatcherAddr, btDefaultCollisionConfigurationAddr); \
    JSbt_Bind_Global_Func(btCollisionDispatcher_getNumManifolds_0, long_t, btCollisionDispatcherAddr); \
    JSbt_Bind_Global_Func(btCollisionDispatcher_getManifoldByIndexInternal_1, btPersistentManifoldAddr, btCollisionDispatcherAddr, long_t); \
    JSbt_Bind_Global_Func(btCollisionDispatcher___destroy___0, void, btCollisionDispatcherAddr); \
    /* btAxisSweep3 */ \
    JSbt_Bind_Global_Func(btAxisSweep3_btAxisSweep3_2, btAxisSweep3Addr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btAxisSweep3_btAxisSweep3_3, btAxisSweep3Addr, btVector3Addr, btVector3Addr, long_t); \
    JSbt_Bind_Global_Func(btAxisSweep3_btAxisSweep3_4, btAxisSweep3Addr, btVector3Addr, btVector3Addr, long_t, btOverlappingPairCacheAddr); \
    JSbt_Bind_Global_Func(btAxisSweep3_btAxisSweep3_5, btAxisSweep3Addr, btVector3Addr, btVector3Addr, long_t, btOverlappingPairCacheAddr, bool); \
    JSbt_Bind_Global_Func(btAxisSweep3___destroy___0, void, btAxisSweep3Addr); \
    /* VoidPtr */ \
    JSbt_Bind_Global_Func(VoidPtr___destroy___0, void, VoidPtrAddr); \
    /* btConeTwistConstraint */ \
    JSbt_Bind_Global_Func(btConeTwistConstraint_btConeTwistConstraint_2, btConeTwistConstraintAddr, btRigidBodyAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_btConeTwistConstraint_4, btConeTwistConstraintAddr, btRigidBodyAddr, btRigidBodyAddr, btTransformAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_setLimit_2, void, btConeTwistConstraintAddr, long_t, double); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_setAngularOnly_1, void, btConeTwistConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_setDamping_1, void, btConeTwistConstraintAddr, double); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_enableMotor_1, void, btConeTwistConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_setMaxMotorImpulse_1, void, btConeTwistConstraintAddr, double); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_setMaxMotorImpulseNormalized_1, void, btConeTwistConstraintAddr, double); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_setMotorTarget_1, void, btConeTwistConstraintAddr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_setMotorTargetInConstraintSpace_1, void, btConeTwistConstraintAddr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_enableFeedback_1, void, btConeTwistConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_getBreakingImpulseThreshold_0, double, btConeTwistConstraintAddr); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_setBreakingImpulseThreshold_1, void, btConeTwistConstraintAddr, double); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_getParam_2, double, btConeTwistConstraintAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(btConeTwistConstraint_setParam_3, void, btConeTwistConstraintAddr, long_t, double, long_t); \
    JSbt_Bind_Global_Func(btConeTwistConstraint___destroy___0, void, btConeTwistConstraintAddr); \
    /* btHingeConstraint */ \
    JSbt_Bind_Global_Func(btHingeConstraint_btHingeConstraint_2, btHingeConstraintAddr, btRigidBodyAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btHingeConstraint_btHingeConstraint_3, btHingeConstraintAddr, btRigidBodyAddr, btTransformAddr, bool); \
    JSbt_Bind_Global_Func(btHingeConstraint_btHingeConstraint_4, btHingeConstraintAddr, btRigidBodyAddr, btRigidBodyAddr, btTransformAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btHingeConstraint_btHingeConstraint_5, btHingeConstraintAddr, btRigidBodyAddr, btRigidBodyAddr, btTransformAddr, btTransformAddr, bool); \
    JSbt_Bind_Global_Func(btHingeConstraint_btHingeConstraint_6, btHingeConstraintAddr, btRigidBodyAddr, btRigidBodyAddr, btVector3Addr, btVector3Addr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btHingeConstraint_btHingeConstraint_7, btHingeConstraintAddr, btRigidBodyAddr, btRigidBodyAddr, btVector3Addr, btVector3Addr, btVector3Addr, btVector3Addr, bool); \
    JSbt_Bind_Global_Func(btHingeConstraint_setLimit_4, void, btHingeConstraintAddr, double, double, double, double); \
    JSbt_Bind_Global_Func(btHingeConstraint_setLimit_5, void, btHingeConstraintAddr, double, double, double, double, double); \
    JSbt_Bind_Global_Func(btHingeConstraint_enableAngularMotor_3, void, btHingeConstraintAddr, bool, double, double); \
    JSbt_Bind_Global_Func(btHingeConstraint_setAngularOnly_1, void, btHingeConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btHingeConstraint_enableMotor_1, void, btHingeConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btHingeConstraint_setMaxMotorImpulse_1, void, btHingeConstraintAddr, double); \
    JSbt_Bind_Global_Func(btHingeConstraint_setMotorTarget_2, void, btHingeConstraintAddr, double, double); \
    JSbt_Bind_Global_Func(btHingeConstraint_enableFeedback_1, void, btHingeConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btHingeConstraint_getBreakingImpulseThreshold_0, double, btHingeConstraintAddr); \
    JSbt_Bind_Global_Func(btHingeConstraint_setBreakingImpulseThreshold_1, void, btHingeConstraintAddr, double); \
    JSbt_Bind_Global_Func(btHingeConstraint_getParam_2, double, btHingeConstraintAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(btHingeConstraint_setParam_3, void, btHingeConstraintAddr, long_t, double, long_t); \
    JSbt_Bind_Global_Func(btHingeConstraint___destroy___0, void, btHingeConstraintAddr); \
    /* JSValueAsParam */ \
    /* btConeShapeZ */ \
    JSbt_Bind_Global_Func(btConeShapeZ_btConeShapeZ_2, btConeShapeZAddr, double, double); \
    JSbt_Bind_Global_Func(btConeShapeZ_setLocalScaling_1, void, btConeShapeZAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btConeShapeZ_getLocalScaling_0, btVector3Addr, btConeShapeZAddr); \
    JSbt_Bind_Global_Func(btConeShapeZ_calculateLocalInertia_2, void, btConeShapeZAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btConeShapeZ___destroy___0, void, btConeShapeZAddr); \
    /* btConeShapeX */ \
    JSbt_Bind_Global_Func(btConeShapeX_btConeShapeX_2, btConeShapeXAddr, double, double); \
    JSbt_Bind_Global_Func(btConeShapeX_setLocalScaling_1, void, btConeShapeXAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btConeShapeX_getLocalScaling_0, btVector3Addr, btConeShapeXAddr); \
    JSbt_Bind_Global_Func(btConeShapeX_calculateLocalInertia_2, void, btConeShapeXAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btConeShapeX___destroy___0, void, btConeShapeXAddr); \
    /* btTriangleMesh */ \
    JSbt_Bind_Global_Func(btTriangleMesh_btTriangleMesh_0, btTriangleMeshAddr); \
    JSbt_Bind_Global_Func(btTriangleMesh_btTriangleMesh_1, btTriangleMeshAddr, bool); \
    JSbt_Bind_Global_Func(btTriangleMesh_btTriangleMesh_2, btTriangleMeshAddr, bool, bool); \
    JSbt_Bind_Global_Func(btTriangleMesh_addTriangle_3, void, btTriangleMeshAddr, btVector3Addr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btTriangleMesh_addTriangle_4, void, btTriangleMeshAddr, btVector3Addr, btVector3Addr, btVector3Addr, bool); \
    JSbt_Bind_Global_Func(btTriangleMesh___destroy___0, void, btTriangleMeshAddr); \
    /* btConvexHullShape */ \
    JSbt_Bind_Global_Func(btConvexHullShape_btConvexHullShape_0, btConvexHullShapeAddr); \
    JSbt_Bind_Global_Func(btConvexHullShape_addPoint_1, void, btConvexHullShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btConvexHullShape_addPoint_2, void, btConvexHullShapeAddr, btVector3Addr, bool); \
    JSbt_Bind_Global_Func(btConvexHullShape_setMargin_1, void, btConvexHullShapeAddr, double); \
    JSbt_Bind_Global_Func(btConvexHullShape_getMargin_0, double, btConvexHullShapeAddr); \
    JSbt_Bind_Global_Func(btConvexHullShape_setLocalScaling_1, void, btConvexHullShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btConvexHullShape_getLocalScaling_0, btVector3Addr, btConvexHullShapeAddr); \
    JSbt_Bind_Global_Func(btConvexHullShape_calculateLocalInertia_2, void, btConvexHullShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btConvexHullShape___destroy___0, void, btConvexHullShapeAddr); \
    /* btVehicleTuning */ \
    JSbt_Bind_Global_Func(btVehicleTuning_btVehicleTuning_0, btVehicleTuningAddr); \
    JSbt_Bind_Global_Func(btVehicleTuning_get_m_suspensionStiffness_0, double, btVehicleTuningAddr); \
    JSbt_Bind_Global_Func(btVehicleTuning_set_m_suspensionStiffness_1, void, btVehicleTuningAddr, double); \
    JSbt_Bind_Global_Func(btVehicleTuning_get_m_suspensionCompression_0, double, btVehicleTuningAddr); \
    JSbt_Bind_Global_Func(btVehicleTuning_set_m_suspensionCompression_1, void, btVehicleTuningAddr, double); \
    JSbt_Bind_Global_Func(btVehicleTuning_get_m_suspensionDamping_0, double, btVehicleTuningAddr); \
    JSbt_Bind_Global_Func(btVehicleTuning_set_m_suspensionDamping_1, void, btVehicleTuningAddr, double); \
    JSbt_Bind_Global_Func(btVehicleTuning_get_m_maxSuspensionTravelCm_0, double, btVehicleTuningAddr); \
    JSbt_Bind_Global_Func(btVehicleTuning_set_m_maxSuspensionTravelCm_1, void, btVehicleTuningAddr, double); \
    JSbt_Bind_Global_Func(btVehicleTuning_get_m_frictionSlip_0, double, btVehicleTuningAddr); \
    JSbt_Bind_Global_Func(btVehicleTuning_set_m_frictionSlip_1, void, btVehicleTuningAddr, double); \
    JSbt_Bind_Global_Func(btVehicleTuning_get_m_maxSuspensionForce_0, double, btVehicleTuningAddr); \
    JSbt_Bind_Global_Func(btVehicleTuning_set_m_maxSuspensionForce_1, void, btVehicleTuningAddr, double); \
    /* btCollisionObjectWrapper */ \
    /* btGImpactMeshShape */ \
    JSbt_Bind_Global_Func(btGImpactMeshShape_btGImpactMeshShape_1, btGImpactMeshShapeAddr, btStridingMeshInterfaceAddr); \
    JSbt_Bind_Global_Func(btGImpactMeshShape_setLocalScaling_1, void, btGImpactMeshShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btGImpactMeshShape_getLocalScaling_0, btVector3Addr, btGImpactMeshShapeAddr); \
    JSbt_Bind_Global_Func(btGImpactMeshShape_calculateLocalInertia_2, void, btGImpactMeshShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btGImpactMeshShape_updateBound_0, void, btGImpactMeshShapeAddr); \
    JSbt_Bind_Global_Func(btGImpactMeshShape___destroy___0, void, btGImpactMeshShapeAddr); \
    /* btDefaultMotionState */ \
    JSbt_Bind_Global_Func(btDefaultMotionState_btDefaultMotionState_0, btDefaultMotionStateAddr); \
    JSbt_Bind_Global_Func(btDefaultMotionState_btDefaultMotionState_1, btDefaultMotionStateAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btDefaultMotionState_btDefaultMotionState_2, btDefaultMotionStateAddr, btTransformAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btDefaultMotionState_getWorldTransform_1, void, btDefaultMotionStateAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btDefaultMotionState_setWorldTransform_1, void, btDefaultMotionStateAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btDefaultMotionState_get_m_graphicsWorldTrans_0, btTransformAddr, btDefaultMotionStateAddr); \
    JSbt_Bind_Global_Func(btDefaultMotionState_set_m_graphicsWorldTrans_1, void, btDefaultMotionStateAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btDefaultMotionState___destroy___0, void, btDefaultMotionStateAddr); \
    /* btWheelInfo */ \
    JSbt_Bind_Global_Func(btWheelInfo_btWheelInfo_1, btWheelInfoAddr, btWheelInfoConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_getSuspensionRestLength_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_updateWheel_2, void, btWheelInfoAddr, btRigidBodyAddr, RaycastInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_suspensionStiffness_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_suspensionStiffness_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_frictionSlip_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_frictionSlip_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_engineForce_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_engineForce_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_rollInfluence_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_rollInfluence_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_suspensionRestLength1_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_suspensionRestLength1_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_wheelsRadius_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_wheelsRadius_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_wheelsDampingCompression_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_wheelsDampingCompression_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_wheelsDampingRelaxation_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_wheelsDampingRelaxation_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_steering_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_steering_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_maxSuspensionForce_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_maxSuspensionForce_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_maxSuspensionTravelCm_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_maxSuspensionTravelCm_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_wheelsSuspensionForce_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_wheelsSuspensionForce_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_bIsFrontWheel_0, bool, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_bIsFrontWheel_1, void, btWheelInfoAddr, bool); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_raycastInfo_0, RaycastInfoAddr, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_raycastInfo_1, void, btWheelInfoAddr, RaycastInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_chassisConnectionPointCS_0, btVector3Addr, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_chassisConnectionPointCS_1, void, btWheelInfoAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_worldTransform_0, btTransformAddr, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_worldTransform_1, void, btWheelInfoAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_wheelDirectionCS_0, btVector3Addr, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_wheelDirectionCS_1, void, btWheelInfoAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_wheelAxleCS_0, btVector3Addr, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_wheelAxleCS_1, void, btWheelInfoAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_rotation_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_rotation_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_deltaRotation_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_deltaRotation_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_brake_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_brake_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_clippedInvContactDotSuspension_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_clippedInvContactDotSuspension_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_suspensionRelativeVelocity_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_suspensionRelativeVelocity_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo_get_m_skidInfo_0, double, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btWheelInfo_set_m_skidInfo_1, void, btWheelInfoAddr, double); \
    JSbt_Bind_Global_Func(btWheelInfo___destroy___0, void, btWheelInfoAddr); \
    /* btVector4 */ \
    JSbt_Bind_Global_Func(btVector4_btVector4_0, btVector4Addr); \
    JSbt_Bind_Global_Func(btVector4_btVector4_4, btVector4Addr, double, double, double, double); \
    JSbt_Bind_Global_Func(btVector4_w_0, double, btVector4Addr); \
    JSbt_Bind_Global_Func(btVector4_setValue_4, void, btVector4Addr, double, double, double, double); \
    JSbt_Bind_Global_Func(btVector4_length_0, double, btVector4Addr); \
    JSbt_Bind_Global_Func(btVector4_x_0, double, btVector4Addr); \
    JSbt_Bind_Global_Func(btVector4_y_0, double, btVector4Addr); \
    JSbt_Bind_Global_Func(btVector4_z_0, double, btVector4Addr); \
    JSbt_Bind_Global_Func(btVector4_setX_1, void, btVector4Addr, double); \
    JSbt_Bind_Global_Func(btVector4_setY_1, void, btVector4Addr, double); \
    JSbt_Bind_Global_Func(btVector4_setZ_1, void, btVector4Addr, double); \
    JSbt_Bind_Global_Func(btVector4_normalize_0, void, btVector4Addr); \
    JSbt_Bind_Global_Func(btVector4_rotate_2, btVector3Addr, btVector4Addr, btVector3Addr, double); \
    JSbt_Bind_Global_Func(btVector4_dot_1, double, btVector4Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btVector4_op_mul_1, btVector3Addr, btVector4Addr, double); \
    JSbt_Bind_Global_Func(btVector4_op_add_1, btVector3Addr, btVector4Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btVector4_op_sub_1, btVector3Addr, btVector4Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btVector4___destroy___0, void, btVector4Addr); \
    /* JsValue */ \
    /* btDefaultCollisionConstructionInfo */ \
    JSbt_Bind_Global_Func(btDefaultCollisionConstructionInfo_btDefaultCollisionConstructionInfo_0, btDefaultCollisionConstructionInfoAddr); \
    JSbt_Bind_Global_Func(btDefaultCollisionConstructionInfo___destroy___0, void, btDefaultCollisionConstructionInfoAddr); \
    /* btVehicleRaycasterResult */ \
    JSbt_Bind_Global_Func(btVehicleRaycasterResult_get_m_hitPointInWorld_0, btVector3Addr, btVehicleRaycasterResultAddr); \
    JSbt_Bind_Global_Func(btVehicleRaycasterResult_set_m_hitPointInWorld_1, void, btVehicleRaycasterResultAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btVehicleRaycasterResult_get_m_hitNormalInWorld_0, btVector3Addr, btVehicleRaycasterResultAddr); \
    JSbt_Bind_Global_Func(btVehicleRaycasterResult_set_m_hitNormalInWorld_1, void, btVehicleRaycasterResultAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btVehicleRaycasterResult_get_m_distFraction_0, double, btVehicleRaycasterResultAddr); \
    JSbt_Bind_Global_Func(btVehicleRaycasterResult_set_m_distFraction_1, void, btVehicleRaycasterResultAddr, double); \
    JSbt_Bind_Global_Func(btVehicleRaycasterResult___destroy___0, void, btVehicleRaycasterResultAddr); \
    /* btConstraintSolver */ \
    JSbt_Bind_Global_Func(btConstraintSolver___destroy___0, void, btConstraintSolverAddr); \
    /* btRaycastVehicle */ \
    JSbt_Bind_Global_Func(btRaycastVehicle_btRaycastVehicle_3, btRaycastVehicleAddr, btVehicleTuningAddr, btRigidBodyAddr, btVehicleRaycasterAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_applyEngineForce_2, void, btRaycastVehicleAddr, double, long_t); \
    JSbt_Bind_Global_Func(btRaycastVehicle_setSteeringValue_2, void, btRaycastVehicleAddr, double, long_t); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getWheelTransformWS_1, btTransformAddr, btRaycastVehicleAddr, long_t); \
    JSbt_Bind_Global_Func(btRaycastVehicle_updateWheelTransform_2, void, btRaycastVehicleAddr, long_t, bool); \
    JSbt_Bind_Global_Func(btRaycastVehicle_addWheel_7, btWheelInfoAddr, btRaycastVehicleAddr, btVector3Addr, btVector3Addr, btVector3Addr, double, double, btVehicleTuningAddr, bool); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getNumWheels_0, long_t, btRaycastVehicleAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getRigidBody_0, btRigidBodyAddr, btRaycastVehicleAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getWheelInfo_1, btWheelInfoAddr, btRaycastVehicleAddr, long_t); \
    JSbt_Bind_Global_Func(btRaycastVehicle_setBrake_2, void, btRaycastVehicleAddr, double, long_t); \
    JSbt_Bind_Global_Func(btRaycastVehicle_setCoordinateSystem_3, void, btRaycastVehicleAddr, long_t, long_t, long_t); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getCurrentSpeedKmHour_0, double, btRaycastVehicleAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getChassisWorldTransform_0, btTransformAddr, btRaycastVehicleAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_rayCast_1, double, btRaycastVehicleAddr, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_updateVehicle_1, void, btRaycastVehicleAddr, double); \
    JSbt_Bind_Global_Func(btRaycastVehicle_resetSuspension_0, void, btRaycastVehicleAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getSteeringValue_1, double, btRaycastVehicleAddr, long_t); \
    JSbt_Bind_Global_Func(btRaycastVehicle_updateWheelTransformsWS_1, void, btRaycastVehicleAddr, btWheelInfoAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_updateWheelTransformsWS_2, void, btRaycastVehicleAddr, btWheelInfoAddr, bool); \
    JSbt_Bind_Global_Func(btRaycastVehicle_setPitchControl_1, void, btRaycastVehicleAddr, double); \
    JSbt_Bind_Global_Func(btRaycastVehicle_updateSuspension_1, void, btRaycastVehicleAddr, double); \
    JSbt_Bind_Global_Func(btRaycastVehicle_updateFriction_1, void, btRaycastVehicleAddr, double); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getRightAxis_0, long_t, btRaycastVehicleAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getUpAxis_0, long_t, btRaycastVehicleAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getForwardAxis_0, long_t, btRaycastVehicleAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getForwardVector_0, btVector3Addr, btRaycastVehicleAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getUserConstraintType_0, long_t, btRaycastVehicleAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_setUserConstraintType_1, void, btRaycastVehicleAddr, long_t); \
    JSbt_Bind_Global_Func(btRaycastVehicle_setUserConstraintId_1, void, btRaycastVehicleAddr, long_t); \
    JSbt_Bind_Global_Func(btRaycastVehicle_getUserConstraintId_0, long_t, btRaycastVehicleAddr); \
    JSbt_Bind_Global_Func(btRaycastVehicle_updateAction_2, void, btRaycastVehicleAddr, btCollisionWorldAddr, double); \
    JSbt_Bind_Global_Func(btRaycastVehicle___destroy___0, void, btRaycastVehicleAddr); \
    /* tVector3Array */ \
    JSbt_Bind_Global_Func(tVector3Array_size_0, long_t, tVector3ArrayAddr); \
    JSbt_Bind_Global_Func(tVector3Array_at_1, btVector3Addr, tVector3ArrayAddr, long_t); \
    JSbt_Bind_Global_Func(tVector3Array_clear_0, void, tVector3ArrayAddr); \
    JSbt_Bind_Global_Func(tVector3Array_push_back_1, void, tVector3ArrayAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(tVector3Array_pop_back_0, void, tVector3ArrayAddr); \
    JSbt_Bind_Global_Func(tVector3Array___destroy___0, void, tVector3ArrayAddr); \
    /* btCylinderShapeX */ \
    JSbt_Bind_Global_Func(btCylinderShapeX_btCylinderShapeX_1, btCylinderShapeXAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCylinderShapeX_setMargin_1, void, btCylinderShapeXAddr, double); \
    JSbt_Bind_Global_Func(btCylinderShapeX_getMargin_0, double, btCylinderShapeXAddr); \
    JSbt_Bind_Global_Func(btCylinderShapeX_setLocalScaling_1, void, btCylinderShapeXAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCylinderShapeX_getLocalScaling_0, btVector3Addr, btCylinderShapeXAddr); \
    JSbt_Bind_Global_Func(btCylinderShapeX_calculateLocalInertia_2, void, btCylinderShapeXAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btCylinderShapeX___destroy___0, void, btCylinderShapeXAddr); \
    /* btCylinderShapeZ */ \
    JSbt_Bind_Global_Func(btCylinderShapeZ_btCylinderShapeZ_1, btCylinderShapeZAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCylinderShapeZ_setMargin_1, void, btCylinderShapeZAddr, double); \
    JSbt_Bind_Global_Func(btCylinderShapeZ_getMargin_0, double, btCylinderShapeZAddr); \
    JSbt_Bind_Global_Func(btCylinderShapeZ_setLocalScaling_1, void, btCylinderShapeZAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCylinderShapeZ_getLocalScaling_0, btVector3Addr, btCylinderShapeZAddr); \
    JSbt_Bind_Global_Func(btCylinderShapeZ_calculateLocalInertia_2, void, btCylinderShapeZAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btCylinderShapeZ___destroy___0, void, btCylinderShapeZAddr); \
    /* btSequentialImpulseConstraintSolver */ \
    JSbt_Bind_Global_Func(btSequentialImpulseConstraintSolver_btSequentialImpulseConstraintSolver_0, btSequentialImpulseConstraintSolverAddr); \
    JSbt_Bind_Global_Func(btSequentialImpulseConstraintSolver___destroy___0, void, btSequentialImpulseConstraintSolverAddr); \
    /* RaycastInfo */ \
    JSbt_Bind_Global_Func(RaycastInfo_get_m_contactNormalWS_0, btVector3Addr, RaycastInfoAddr); \
    JSbt_Bind_Global_Func(RaycastInfo_set_m_contactNormalWS_1, void, RaycastInfoAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(RaycastInfo_get_m_contactPointWS_0, btVector3Addr, RaycastInfoAddr); \
    JSbt_Bind_Global_Func(RaycastInfo_set_m_contactPointWS_1, void, RaycastInfoAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(RaycastInfo_get_m_suspensionLength_0, double, RaycastInfoAddr); \
    JSbt_Bind_Global_Func(RaycastInfo_set_m_suspensionLength_1, void, RaycastInfoAddr, double); \
    JSbt_Bind_Global_Func(RaycastInfo_get_m_hardPointWS_0, btVector3Addr, RaycastInfoAddr); \
    JSbt_Bind_Global_Func(RaycastInfo_set_m_hardPointWS_1, void, RaycastInfoAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(RaycastInfo_get_m_wheelDirectionWS_0, btVector3Addr, RaycastInfoAddr); \
    JSbt_Bind_Global_Func(RaycastInfo_set_m_wheelDirectionWS_1, void, RaycastInfoAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(RaycastInfo_get_m_wheelAxleWS_0, btVector3Addr, RaycastInfoAddr); \
    JSbt_Bind_Global_Func(RaycastInfo_set_m_wheelAxleWS_1, void, RaycastInfoAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(RaycastInfo_get_m_isInContact_0, bool, RaycastInfoAddr); \
    JSbt_Bind_Global_Func(RaycastInfo_set_m_isInContact_1, void, RaycastInfoAddr, bool); \
    JSbt_Bind_Global_Func(RaycastInfo_get_m_groundObject_0, VoidPtrAddr, RaycastInfoAddr); \
    JSbt_Bind_Global_Func(RaycastInfo_set_m_groundObject_1, void, RaycastInfoAddr, VoidPtrAddr); \
    JSbt_Bind_Global_Func(RaycastInfo___destroy___0, void, RaycastInfoAddr); \
    /* btHeightfieldTerrainShape */ \
    JSbt_Bind_Global_Func(btHeightfieldTerrainShape_btHeightfieldTerrainShape_9, btHeightfieldTerrainShapeAddr, long_t, long_t, VoidPtrAddr, double, double, double, long_t, long_t, bool); \
    JSbt_Bind_Global_Func(btHeightfieldTerrainShape_setMargin_1, void, btHeightfieldTerrainShapeAddr, double); \
    JSbt_Bind_Global_Func(btHeightfieldTerrainShape_getMargin_0, double, btHeightfieldTerrainShapeAddr); \
    JSbt_Bind_Global_Func(btHeightfieldTerrainShape_setLocalScaling_1, void, btHeightfieldTerrainShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btHeightfieldTerrainShape_getLocalScaling_0, btVector3Addr, btHeightfieldTerrainShapeAddr); \
    JSbt_Bind_Global_Func(btHeightfieldTerrainShape_calculateLocalInertia_2, void, btHeightfieldTerrainShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btHeightfieldTerrainShape___destroy___0, void, btHeightfieldTerrainShapeAddr); \
    /* btDiscreteDynamicsWorld */ \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_btDiscreteDynamicsWorld_4, btDiscreteDynamicsWorldAddr, btDispatcherAddr, btBroadphaseInterfaceAddr, btConstraintSolverAddr, btCollisionConfigurationAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_setGravity_1, void, btDiscreteDynamicsWorldAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_getGravity_0, btVector3Addr, btDiscreteDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_addRigidBody_1, void, btDiscreteDynamicsWorldAddr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_addRigidBody_3, void, btDiscreteDynamicsWorldAddr, btRigidBodyAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_removeRigidBody_1, void, btDiscreteDynamicsWorldAddr, btRigidBodyAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_addConstraint_1, void, btDiscreteDynamicsWorldAddr, btTypedConstraintAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_addConstraint_2, void, btDiscreteDynamicsWorldAddr, btTypedConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_removeConstraint_1, void, btDiscreteDynamicsWorldAddr, btTypedConstraintAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_stepSimulation_1, long_t, btDiscreteDynamicsWorldAddr, double); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_stepSimulation_2, long_t, btDiscreteDynamicsWorldAddr, double, long_t); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_stepSimulation_3, long_t, btDiscreteDynamicsWorldAddr, double, long_t, double); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_clearForces_0, void, btDiscreteDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_setApplySpeculativeContactRestitution_1, void, btDiscreteDynamicsWorldAddr, bool); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_getApplySpeculativeContactRestitution_0, bool, btDiscreteDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_getDispatcher_0, btDispatcherAddr, btDiscreteDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_rayTest_3, void, btDiscreteDynamicsWorldAddr, btVector3Addr, btVector3Addr, RayResultCallbackAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_getPairCache_0, btOverlappingPairCacheAddr, btDiscreteDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_getDispatchInfo_0, btDispatcherInfoAddr, btDiscreteDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_addCollisionObject_1, void, btDiscreteDynamicsWorldAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_addCollisionObject_2, void, btDiscreteDynamicsWorldAddr, btCollisionObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_addCollisionObject_3, void, btDiscreteDynamicsWorldAddr, btCollisionObjectAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_removeCollisionObject_1, void, btDiscreteDynamicsWorldAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_getBroadphase_0, btBroadphaseInterfaceAddr, btDiscreteDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_convexSweepTest_5, void, btDiscreteDynamicsWorldAddr, btConvexShapeAddr, btTransformAddr, btTransformAddr, ConvexResultCallbackAddr, double); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_contactPairTest_3, void, btDiscreteDynamicsWorldAddr, btCollisionObjectAddr, btCollisionObjectAddr, ContactResultCallbackAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_contactTest_2, void, btDiscreteDynamicsWorldAddr, btCollisionObjectAddr, ContactResultCallbackAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_updateSingleAabb_1, void, btDiscreteDynamicsWorldAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_addAction_1, void, btDiscreteDynamicsWorldAddr, btActionInterfaceAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_removeAction_1, void, btDiscreteDynamicsWorldAddr, btActionInterfaceAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld_getSolverInfo_0, btContactSolverInfoAddr, btDiscreteDynamicsWorldAddr); \
    JSbt_Bind_Global_Func(btDiscreteDynamicsWorld___destroy___0, void, btDiscreteDynamicsWorldAddr); \
    /* btGhostPairCallback */ \
    JSbt_Bind_Global_Func(btGhostPairCallback_btGhostPairCallback_0, btGhostPairCallbackAddr); \
    JSbt_Bind_Global_Func(btGhostPairCallback___destroy___0, void, btGhostPairCallbackAddr); \
    /* btOverlappingPairCallback */ \
    JSbt_Bind_Global_Func(btOverlappingPairCallback___destroy___0, void, btOverlappingPairCallbackAddr); \
    /* btKinematicCharacterController */ \
    JSbt_Bind_Global_Func(btKinematicCharacterController_btKinematicCharacterController_4, btKinematicCharacterControllerAddr, btPairCachingGhostObjectAddr, btConvexShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_setUp_1, void, btKinematicCharacterControllerAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_setWalkDirection_1, void, btKinematicCharacterControllerAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_setVelocityForTimeInterval_2, void, btKinematicCharacterControllerAddr, btVector3Addr, double); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_warp_1, void, btKinematicCharacterControllerAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_preStep_1, void, btKinematicCharacterControllerAddr, btCollisionWorldAddr); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_playerStep_2, void, btKinematicCharacterControllerAddr, btCollisionWorldAddr, double); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_setFallSpeed_1, void, btKinematicCharacterControllerAddr, double); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_setJumpSpeed_1, void, btKinematicCharacterControllerAddr, double); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_setMaxJumpHeight_1, void, btKinematicCharacterControllerAddr, double); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_canJump_0, bool, btKinematicCharacterControllerAddr); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_jump_1, void, btKinematicCharacterControllerAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_setGravity_1, void, btKinematicCharacterControllerAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_getGravity_0, btVector3Addr, btKinematicCharacterControllerAddr); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_setMaxSlope_1, void, btKinematicCharacterControllerAddr, double); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_getMaxSlope_0, double, btKinematicCharacterControllerAddr); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_getGhostObject_0, btPairCachingGhostObjectAddr, btKinematicCharacterControllerAddr); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_setUseGhostSweepTest_1, void, btKinematicCharacterControllerAddr, bool); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_onGround_0, bool, btKinematicCharacterControllerAddr); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_setUpInterpolate_1, void, btKinematicCharacterControllerAddr, bool); \
    JSbt_Bind_Global_Func(btKinematicCharacterController_updateAction_2, void, btKinematicCharacterControllerAddr, btCollisionWorldAddr, double); \
    JSbt_Bind_Global_Func(btKinematicCharacterController___destroy___0, void, btKinematicCharacterControllerAddr); \
    /* btStaticPlaneShape */ \
    JSbt_Bind_Global_Func(btStaticPlaneShape_btStaticPlaneShape_2, btStaticPlaneShapeAddr, btVector3Addr, double); \
    JSbt_Bind_Global_Func(btStaticPlaneShape_setLocalScaling_1, void, btStaticPlaneShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btStaticPlaneShape_getLocalScaling_0, btVector3Addr, btStaticPlaneShapeAddr); \
    JSbt_Bind_Global_Func(btStaticPlaneShape_calculateLocalInertia_2, void, btStaticPlaneShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btStaticPlaneShape___destroy___0, void, btStaticPlaneShapeAddr); \
    /* btOverlappingPairCache */ \
    JSbt_Bind_Global_Func(btOverlappingPairCache_setInternalGhostPairCallback_1, void, btOverlappingPairCacheAddr, btOverlappingPairCallbackAddr); \
    JSbt_Bind_Global_Func(btOverlappingPairCache___destroy___0, void, btOverlappingPairCacheAddr); \
    /* tBtCollisionObjectArray */ \
    JSbt_Bind_Global_Func(tBtCollisionObjectArray_size_0, long_t, tBtCollisionObjectArrayAddr); \
    JSbt_Bind_Global_Func(tBtCollisionObjectArray_at_1, btCollisionObjectAddr, tBtCollisionObjectArrayAddr, long_t); \
    JSbt_Bind_Global_Func(tBtCollisionObjectArray_clear_0, void, tBtCollisionObjectArrayAddr); \
    JSbt_Bind_Global_Func(tBtCollisionObjectArray_push_back_1, void, tBtCollisionObjectArrayAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(tBtCollisionObjectArray_pop_back_0, void, tBtCollisionObjectArrayAddr); \
    JSbt_Bind_Global_Func(tBtCollisionObjectArray___destroy___0, void, tBtCollisionObjectArrayAddr); \
    /* btFixedConstraint */ \
    JSbt_Bind_Global_Func(btFixedConstraint_btFixedConstraint_4, btFixedConstraintAddr, btRigidBodyAddr, btRigidBodyAddr, btTransformAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btFixedConstraint_enableFeedback_1, void, btFixedConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btFixedConstraint_getBreakingImpulseThreshold_0, double, btFixedConstraintAddr); \
    JSbt_Bind_Global_Func(btFixedConstraint_setBreakingImpulseThreshold_1, void, btFixedConstraintAddr, double); \
    JSbt_Bind_Global_Func(btFixedConstraint_getParam_2, double, btFixedConstraintAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(btFixedConstraint_setParam_3, void, btFixedConstraintAddr, long_t, double, long_t); \
    JSbt_Bind_Global_Func(btFixedConstraint___destroy___0, void, btFixedConstraintAddr); \
    /* btTransform */ \
    JSbt_Bind_Global_Func(btTransform_btTransform_0, btTransformAddr); \
    JSbt_Bind_Global_Func(btTransform_btTransform_2, btTransformAddr, btQuaternionAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btTransform_setIdentity_0, void, btTransformAddr); \
    JSbt_Bind_Global_Func(btTransform_setOrigin_1, void, btTransformAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btTransform_setRotation_1, void, btTransformAddr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btTransform_getOrigin_0, btVector3Addr, btTransformAddr); \
    JSbt_Bind_Global_Func(btTransform_getRotation_0, btQuaternionAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btTransform_getBasis_0, btMatrix3x3Addr, btTransformAddr); \
    JSbt_Bind_Global_Func(btTransform_setFromOpenGLMatrix_1, void, btTransformAddr, double*); \
    JSbt_Bind_Global_Func(btTransform___destroy___0, void, btTransformAddr); \
    /* ClosestRayResultCallback */ \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_ClosestRayResultCallback_2, ClosestRayResultCallbackAddr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_hasHit_0, bool, ClosestRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_get_m_rayFromWorld_0, btVector3Addr, ClosestRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_set_m_rayFromWorld_1, void, ClosestRayResultCallbackAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_get_m_rayToWorld_0, btVector3Addr, ClosestRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_set_m_rayToWorld_1, void, ClosestRayResultCallbackAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_get_m_hitNormalWorld_0, btVector3Addr, ClosestRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_set_m_hitNormalWorld_1, void, ClosestRayResultCallbackAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_get_m_hitPointWorld_0, btVector3Addr, ClosestRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_set_m_hitPointWorld_1, void, ClosestRayResultCallbackAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_get_m_collisionFilterGroup_0, long_t, ClosestRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_set_m_collisionFilterGroup_1, void, ClosestRayResultCallbackAddr, long_t); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_get_m_collisionFilterMask_0, long_t, ClosestRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_set_m_collisionFilterMask_1, void, ClosestRayResultCallbackAddr, long_t); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_get_m_closestHitFraction_0, double, ClosestRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_set_m_closestHitFraction_1, void, ClosestRayResultCallbackAddr, double); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_get_m_collisionObject_0, btCollisionObjectAddr, ClosestRayResultCallbackAddr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback_set_m_collisionObject_1, void, ClosestRayResultCallbackAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(ClosestRayResultCallback___destroy___0, void, ClosestRayResultCallbackAddr); \
    /* AllConvexResultCallback */ \
    JSbt_Bind_Global_Func(AllConvexResultCallback_AllConvexResultCallback_2, AllConvexResultCallbackAddr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_hasHit_0, bool, AllConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_get_m_convexFromWorld_0, btVector3Addr, AllConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_set_m_convexFromWorld_1, void, AllConvexResultCallbackAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_get_m_convexToWorld_0, btVector3Addr, AllConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_set_m_convexToWorld_1, void, AllConvexResultCallbackAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_get_m_hitNormalWorld_0, tVector3ArrayAddr, AllConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_set_m_hitNormalWorld_1, void, AllConvexResultCallbackAddr, tVector3ArrayAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_get_m_hitPointWorld_0, tVector3ArrayAddr, AllConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_set_m_hitPointWorld_1, void, AllConvexResultCallbackAddr, tVector3ArrayAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_get_m_hitFractions_0, tScalarArrayAddr, AllConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_set_m_hitFractions_1, void, AllConvexResultCallbackAddr, tScalarArrayAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_get_m_collisionObjects_0, tBtCollisionObjectArrayAddr, AllConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_set_m_collisionObjects_1, void, AllConvexResultCallbackAddr, tBtCollisionObjectArrayAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_get_m_collisionFilterGroup_0, long_t, AllConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_set_m_collisionFilterGroup_1, void, AllConvexResultCallbackAddr, long_t); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_get_m_collisionFilterMask_0, long_t, AllConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_set_m_collisionFilterMask_1, void, AllConvexResultCallbackAddr, long_t); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_get_m_closestHitFraction_0, double, AllConvexResultCallbackAddr); \
    JSbt_Bind_Global_Func(AllConvexResultCallback_set_m_closestHitFraction_1, void, AllConvexResultCallbackAddr, double); \
    JSbt_Bind_Global_Func(AllConvexResultCallback___destroy___0, void, AllConvexResultCallbackAddr); \
    /* ConcreteContactResultCallback */ \
    JSbt_Bind_Global_Func(ConcreteContactResultCallback_ConcreteContactResultCallback_0, ConcreteContactResultCallbackAddr); \
    JSbt_Bind_Global_Func(ConcreteContactResultCallback_addSingleResult_7, double, ConcreteContactResultCallbackAddr, btManifoldPointAddr, btCollisionObjectWrapperAddr, long_t, long_t, btCollisionObjectWrapperAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(ConcreteContactResultCallback___destroy___0, void, ConcreteContactResultCallbackAddr); \
    /* btBvhTriangleMeshShape */ \
    JSbt_Bind_Global_Func(btBvhTriangleMeshShape_btBvhTriangleMeshShape_2, btBvhTriangleMeshShapeAddr, btStridingMeshInterfaceAddr, bool); \
    JSbt_Bind_Global_Func(btBvhTriangleMeshShape_btBvhTriangleMeshShape_3, btBvhTriangleMeshShapeAddr, btStridingMeshInterfaceAddr, bool, bool); \
    JSbt_Bind_Global_Func(btBvhTriangleMeshShape_setLocalScaling_1, void, btBvhTriangleMeshShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btBvhTriangleMeshShape_getLocalScaling_0, btVector3Addr, btBvhTriangleMeshShapeAddr); \
    JSbt_Bind_Global_Func(btBvhTriangleMeshShape_calculateLocalInertia_2, void, btBvhTriangleMeshShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btBvhTriangleMeshShape___destroy___0, void, btBvhTriangleMeshShapeAddr); \
    /* btSliderConstraint */ \
    JSbt_Bind_Global_Func(btSliderConstraint_btSliderConstraint_3, btSliderConstraintAddr, btRigidBodyAddr, btTransformAddr, bool); \
    JSbt_Bind_Global_Func(btSliderConstraint_btSliderConstraint_5, btSliderConstraintAddr, btRigidBodyAddr, btRigidBodyAddr, btTransformAddr, btTransformAddr, bool); \
    JSbt_Bind_Global_Func(btSliderConstraint_setLowerLinLimit_1, void, btSliderConstraintAddr, double); \
    JSbt_Bind_Global_Func(btSliderConstraint_setUpperLinLimit_1, void, btSliderConstraintAddr, double); \
    JSbt_Bind_Global_Func(btSliderConstraint_setLowerAngLimit_1, void, btSliderConstraintAddr, double); \
    JSbt_Bind_Global_Func(btSliderConstraint_setUpperAngLimit_1, void, btSliderConstraintAddr, double); \
    JSbt_Bind_Global_Func(btSliderConstraint_enableFeedback_1, void, btSliderConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btSliderConstraint_getBreakingImpulseThreshold_0, double, btSliderConstraintAddr); \
    JSbt_Bind_Global_Func(btSliderConstraint_setBreakingImpulseThreshold_1, void, btSliderConstraintAddr, double); \
    JSbt_Bind_Global_Func(btSliderConstraint_getParam_2, double, btSliderConstraintAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(btSliderConstraint_setParam_3, void, btSliderConstraintAddr, long_t, double, long_t); \
    JSbt_Bind_Global_Func(btSliderConstraint___destroy___0, void, btSliderConstraintAddr); \
    /* btPairCachingGhostObject */ \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_btPairCachingGhostObject_0, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setAnisotropicFriction_2, void, btPairCachingGhostObjectAddr, btVector3Addr, long_t); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getCollisionShape_0, btCollisionShapeAddr, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setContactProcessingThreshold_1, void, btPairCachingGhostObjectAddr, double); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setActivationState_1, void, btPairCachingGhostObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_forceActivationState_1, void, btPairCachingGhostObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_activate_0, void, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_activate_1, void, btPairCachingGhostObjectAddr, bool); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_isActive_0, bool, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_isKinematicObject_0, bool, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_isStaticObject_0, bool, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_isStaticOrKinematicObject_0, bool, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getRestitution_0, double, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setRestitution_1, void, btPairCachingGhostObjectAddr, double); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getFriction_0, double, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setFriction_1, void, btPairCachingGhostObjectAddr, double); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getRollingFriction_0, double, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setRollingFriction_1, void, btPairCachingGhostObjectAddr, double); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getCollisionFlags_0, long_t, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setCollisionFlags_1, void, btPairCachingGhostObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getWorldTransform_0, btTransformAddr, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setWorldTransform_1, void, btPairCachingGhostObjectAddr, btTransformAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setCollisionShape_1, void, btPairCachingGhostObjectAddr, btCollisionShapeAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getCcdMotionThreshold_0, double, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setCcdMotionThreshold_1, void, btPairCachingGhostObjectAddr, double); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getCcdSweptSphereRadius_0, double, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setCcdSweptSphereRadius_1, void, btPairCachingGhostObjectAddr, double); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getUserIndex_0, long_t, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setUserIndex_1, void, btPairCachingGhostObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getUserPointer_0, VoidPtrAddr, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setUserPointer_1, void, btPairCachingGhostObjectAddr, VoidPtrAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getInterpolationAngularVelocity_0, btVector3Addr, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setInterpolationAngularVelocity_1, void, btPairCachingGhostObjectAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getInterpolationLinearVelocity_0, btVector3Addr, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_setInterpolationLinearVelocity_1, void, btPairCachingGhostObjectAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getBroadphaseHandle_0, btBroadphaseProxyAddr, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getActivationState_0, long_t, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getNumOverlappingObjects_0, long_t, btPairCachingGhostObjectAddr); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject_getOverlappingObject_1, btCollisionObjectAddr, btPairCachingGhostObjectAddr, long_t); \
    JSbt_Bind_Global_Func(btPairCachingGhostObject___destroy___0, void, btPairCachingGhostObjectAddr); \
    /* btManifoldPoint */ \
    JSbt_Bind_Global_Func(btManifoldPoint_getPositionWorldOnA_0, btVector3Addr, btManifoldPointAddr); \
    JSbt_Bind_Global_Func(btManifoldPoint_getPositionWorldOnB_0, btVector3Addr, btManifoldPointAddr); \
    JSbt_Bind_Global_Func(btManifoldPoint_getAppliedImpulse_0, double, btManifoldPointAddr); \
    JSbt_Bind_Global_Func(btManifoldPoint_getDistance_0, double, btManifoldPointAddr); \
    JSbt_Bind_Global_Func(btManifoldPoint_get_m_localPointA_0, btVector3Addr, btManifoldPointAddr); \
    JSbt_Bind_Global_Func(btManifoldPoint_set_m_localPointA_1, void, btManifoldPointAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btManifoldPoint_get_m_localPointB_0, btVector3Addr, btManifoldPointAddr); \
    JSbt_Bind_Global_Func(btManifoldPoint_set_m_localPointB_1, void, btManifoldPointAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btManifoldPoint_get_m_positionWorldOnB_0, btVector3Addr, btManifoldPointAddr); \
    JSbt_Bind_Global_Func(btManifoldPoint_set_m_positionWorldOnB_1, void, btManifoldPointAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btManifoldPoint_get_m_positionWorldOnA_0, btVector3Addr, btManifoldPointAddr); \
    JSbt_Bind_Global_Func(btManifoldPoint_set_m_positionWorldOnA_1, void, btManifoldPointAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btManifoldPoint_get_m_normalWorldOnB_0, btVector3Addr, btManifoldPointAddr); \
    JSbt_Bind_Global_Func(btManifoldPoint_set_m_normalWorldOnB_1, void, btManifoldPointAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btManifoldPoint___destroy___0, void, btManifoldPointAddr); \
    /* btPoint2PointConstraint */ \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_btPoint2PointConstraint_2, btPoint2PointConstraintAddr, btRigidBodyAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_btPoint2PointConstraint_4, btPoint2PointConstraintAddr, btRigidBodyAddr, btRigidBodyAddr, btVector3Addr, btVector3Addr); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_setPivotA_1, void, btPoint2PointConstraintAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_setPivotB_1, void, btPoint2PointConstraintAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_getPivotInA_0, btVector3Addr, btPoint2PointConstraintAddr); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_getPivotInB_0, btVector3Addr, btPoint2PointConstraintAddr); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_enableFeedback_1, void, btPoint2PointConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_getBreakingImpulseThreshold_0, double, btPoint2PointConstraintAddr); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_setBreakingImpulseThreshold_1, void, btPoint2PointConstraintAddr, double); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_getParam_2, double, btPoint2PointConstraintAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_setParam_3, void, btPoint2PointConstraintAddr, long_t, double, long_t); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_get_m_setting_0, btConstraintSettingAddr, btPoint2PointConstraintAddr); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint_set_m_setting_1, void, btPoint2PointConstraintAddr, btConstraintSettingAddr); \
    JSbt_Bind_Global_Func(btPoint2PointConstraint___destroy___0, void, btPoint2PointConstraintAddr); \
    /* btBroadphaseProxy */ \
    JSbt_Bind_Global_Func(btBroadphaseProxy_get_m_collisionFilterGroup_0, long_t, btBroadphaseProxyAddr); \
    JSbt_Bind_Global_Func(btBroadphaseProxy_set_m_collisionFilterGroup_1, void, btBroadphaseProxyAddr, long_t); \
    JSbt_Bind_Global_Func(btBroadphaseProxy_get_m_collisionFilterMask_0, long_t, btBroadphaseProxyAddr); \
    JSbt_Bind_Global_Func(btBroadphaseProxy_set_m_collisionFilterMask_1, void, btBroadphaseProxyAddr, long_t); \
    JSbt_Bind_Global_Func(btBroadphaseProxy___destroy___0, void, btBroadphaseProxyAddr); \
    /* btGeneric6DofSpringConstraint */ \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_btGeneric6DofSpringConstraint_3, btGeneric6DofSpringConstraintAddr, btRigidBodyAddr, btTransformAddr, bool); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_btGeneric6DofSpringConstraint_5, btGeneric6DofSpringConstraintAddr, btRigidBodyAddr, btRigidBodyAddr, btTransformAddr, btTransformAddr, bool); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_enableSpring_2, void, btGeneric6DofSpringConstraintAddr, long_t, bool); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_setStiffness_2, void, btGeneric6DofSpringConstraintAddr, long_t, double); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_setDamping_2, void, btGeneric6DofSpringConstraintAddr, long_t, double); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_setLinearLowerLimit_1, void, btGeneric6DofSpringConstraintAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_setLinearUpperLimit_1, void, btGeneric6DofSpringConstraintAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_setAngularLowerLimit_1, void, btGeneric6DofSpringConstraintAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_setAngularUpperLimit_1, void, btGeneric6DofSpringConstraintAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_getFrameOffsetA_0, btTransformAddr, btGeneric6DofSpringConstraintAddr); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_enableFeedback_1, void, btGeneric6DofSpringConstraintAddr, bool); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_getBreakingImpulseThreshold_0, double, btGeneric6DofSpringConstraintAddr); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_setBreakingImpulseThreshold_1, void, btGeneric6DofSpringConstraintAddr, double); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_getParam_2, double, btGeneric6DofSpringConstraintAddr, long_t, long_t); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint_setParam_3, void, btGeneric6DofSpringConstraintAddr, long_t, double, long_t); \
    JSbt_Bind_Global_Func(btGeneric6DofSpringConstraint___destroy___0, void, btGeneric6DofSpringConstraintAddr); \
    /* btBoxShape */ \
    JSbt_Bind_Global_Func(btBoxShape_btBoxShape_1, btBoxShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btBoxShape_setMargin_1, void, btBoxShapeAddr, double); \
    JSbt_Bind_Global_Func(btBoxShape_getMargin_0, double, btBoxShapeAddr); \
    JSbt_Bind_Global_Func(btBoxShape_setLocalScaling_1, void, btBoxShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btBoxShape_getLocalScaling_0, btVector3Addr, btBoxShapeAddr); \
    JSbt_Bind_Global_Func(btBoxShape_calculateLocalInertia_2, void, btBoxShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btBoxShape___destroy___0, void, btBoxShapeAddr); \
    /* btCapsuleShapeX */ \
    JSbt_Bind_Global_Func(btCapsuleShapeX_btCapsuleShapeX_2, btCapsuleShapeXAddr, double, double); \
    JSbt_Bind_Global_Func(btCapsuleShapeX_setMargin_1, void, btCapsuleShapeXAddr, double); \
    JSbt_Bind_Global_Func(btCapsuleShapeX_getMargin_0, double, btCapsuleShapeXAddr); \
    JSbt_Bind_Global_Func(btCapsuleShapeX_getUpAxis_0, long_t, btCapsuleShapeXAddr); \
    JSbt_Bind_Global_Func(btCapsuleShapeX_getRadius_0, double, btCapsuleShapeXAddr); \
    JSbt_Bind_Global_Func(btCapsuleShapeX_getHalfHeight_0, double, btCapsuleShapeXAddr); \
    JSbt_Bind_Global_Func(btCapsuleShapeX_setLocalScaling_1, void, btCapsuleShapeXAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCapsuleShapeX_getLocalScaling_0, btVector3Addr, btCapsuleShapeXAddr); \
    JSbt_Bind_Global_Func(btCapsuleShapeX_calculateLocalInertia_2, void, btCapsuleShapeXAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btCapsuleShapeX___destroy___0, void, btCapsuleShapeXAddr); \
    /* btQuaternion */ \
    JSbt_Bind_Global_Func(btQuaternion_btQuaternion_4, btQuaternionAddr, double, double, double, double); \
    JSbt_Bind_Global_Func(btQuaternion_setValue_4, void, btQuaternionAddr, double, double, double, double); \
    JSbt_Bind_Global_Func(btQuaternion_setEulerZYX_3, void, btQuaternionAddr, double, double, double); \
    JSbt_Bind_Global_Func(btQuaternion_setRotation_2, void, btQuaternionAddr, btVector3Addr, double); \
    JSbt_Bind_Global_Func(btQuaternion_normalize_0, void, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_length2_0, double, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_length_0, double, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_dot_1, double, btQuaternionAddr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_normalized_0, btQuaternionAddr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_getAxis_0, btVector3Addr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_inverse_0, btQuaternionAddr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_getAngle_0, double, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_getAngleShortestPath_0, double, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_angle_1, double, btQuaternionAddr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_angleShortestPath_1, double, btQuaternionAddr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_op_add_1, btQuaternionAddr, btQuaternionAddr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_op_sub_1, btQuaternionAddr, btQuaternionAddr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_op_mul_1, btQuaternionAddr, btQuaternionAddr, double); \
    JSbt_Bind_Global_Func(btQuaternion_op_mulq_1, btQuaternionAddr, btQuaternionAddr, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_op_div_1, btQuaternionAddr, btQuaternionAddr, double); \
    JSbt_Bind_Global_Func(btQuaternion_x_0, double, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_y_0, double, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_z_0, double, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_w_0, double, btQuaternionAddr); \
    JSbt_Bind_Global_Func(btQuaternion_setX_1, void, btQuaternionAddr, double); \
    JSbt_Bind_Global_Func(btQuaternion_setY_1, void, btQuaternionAddr, double); \
    JSbt_Bind_Global_Func(btQuaternion_setZ_1, void, btQuaternionAddr, double); \
    JSbt_Bind_Global_Func(btQuaternion_setW_1, void, btQuaternionAddr, double); \
    JSbt_Bind_Global_Func(btQuaternion___destroy___0, void, btQuaternionAddr); \
    /* btCapsuleShapeZ */ \
    JSbt_Bind_Global_Func(btCapsuleShapeZ_btCapsuleShapeZ_2, btCapsuleShapeZAddr, double, double); \
    JSbt_Bind_Global_Func(btCapsuleShapeZ_setMargin_1, void, btCapsuleShapeZAddr, double); \
    JSbt_Bind_Global_Func(btCapsuleShapeZ_getMargin_0, double, btCapsuleShapeZAddr); \
    JSbt_Bind_Global_Func(btCapsuleShapeZ_getUpAxis_0, long_t, btCapsuleShapeZAddr); \
    JSbt_Bind_Global_Func(btCapsuleShapeZ_getRadius_0, double, btCapsuleShapeZAddr); \
    JSbt_Bind_Global_Func(btCapsuleShapeZ_getHalfHeight_0, double, btCapsuleShapeZAddr); \
    JSbt_Bind_Global_Func(btCapsuleShapeZ_setLocalScaling_1, void, btCapsuleShapeZAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btCapsuleShapeZ_getLocalScaling_0, btVector3Addr, btCapsuleShapeZAddr); \
    JSbt_Bind_Global_Func(btCapsuleShapeZ_calculateLocalInertia_2, void, btCapsuleShapeZAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btCapsuleShapeZ___destroy___0, void, btCapsuleShapeZAddr); \
    /* btContactSolverInfo */ \
    JSbt_Bind_Global_Func(btContactSolverInfo_get_m_solverMode_0, long_t, btContactSolverInfoAddr); \
    JSbt_Bind_Global_Func(btContactSolverInfo_set_m_solverMode_1, void, btContactSolverInfoAddr, long_t); \
    JSbt_Bind_Global_Func(btContactSolverInfo_get_m_splitImpulse_0, bool, btContactSolverInfoAddr); \
    JSbt_Bind_Global_Func(btContactSolverInfo_set_m_splitImpulse_1, void, btContactSolverInfoAddr, bool); \
    JSbt_Bind_Global_Func(btContactSolverInfo_get_m_splitImpulsePenetrationThreshold_0, double, btContactSolverInfoAddr); \
    JSbt_Bind_Global_Func(btContactSolverInfo_set_m_splitImpulsePenetrationThreshold_1, void, btContactSolverInfoAddr, double); \
    JSbt_Bind_Global_Func(btContactSolverInfo_get_m_numIterations_0, long_t, btContactSolverInfoAddr); \
    JSbt_Bind_Global_Func(btContactSolverInfo_set_m_numIterations_1, void, btContactSolverInfoAddr, long_t); \
    JSbt_Bind_Global_Func(btContactSolverInfo___destroy___0, void, btContactSolverInfoAddr); \
    /* tScalarArray */ \
    JSbt_Bind_Global_Func(tScalarArray_size_0, long_t, tScalarArrayAddr); \
    JSbt_Bind_Global_Func(tScalarArray_at_1, double, tScalarArrayAddr, long_t); \
    JSbt_Bind_Global_Func(tScalarArray_clear_0, void, tScalarArrayAddr); \
    JSbt_Bind_Global_Func(tScalarArray_push_back_1, void, tScalarArrayAddr, double); \
    JSbt_Bind_Global_Func(tScalarArray_pop_back_0, void, tScalarArrayAddr); \
    JSbt_Bind_Global_Func(tScalarArray___destroy___0, void, tScalarArrayAddr); \
    /* btSphereShape */ \
    JSbt_Bind_Global_Func(btSphereShape_btSphereShape_1, btSphereShapeAddr, double); \
    JSbt_Bind_Global_Func(btSphereShape_setMargin_1, void, btSphereShapeAddr, double); \
    JSbt_Bind_Global_Func(btSphereShape_getMargin_0, double, btSphereShapeAddr); \
    JSbt_Bind_Global_Func(btSphereShape_setLocalScaling_1, void, btSphereShapeAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(btSphereShape_getLocalScaling_0, btVector3Addr, btSphereShapeAddr); \
    JSbt_Bind_Global_Func(btSphereShape_calculateLocalInertia_2, void, btSphereShapeAddr, double, btVector3Addr); \
    JSbt_Bind_Global_Func(btSphereShape___destroy___0, void, btSphereShapeAddr); \
    /* LocalConvexResult */ \
    JSbt_Bind_Global_Func(LocalConvexResult_LocalConvexResult_5, LocalConvexResultAddr, btCollisionObjectAddr, LocalShapeInfoAddr, btVector3Addr, btVector3Addr, double); \
    JSbt_Bind_Global_Func(LocalConvexResult_get_m_hitCollisionObject_0, btCollisionObjectAddr, LocalConvexResultAddr); \
    JSbt_Bind_Global_Func(LocalConvexResult_set_m_hitCollisionObject_1, void, LocalConvexResultAddr, btCollisionObjectAddr); \
    JSbt_Bind_Global_Func(LocalConvexResult_get_m_localShapeInfo_0, LocalShapeInfoAddr, LocalConvexResultAddr); \
    JSbt_Bind_Global_Func(LocalConvexResult_set_m_localShapeInfo_1, void, LocalConvexResultAddr, LocalShapeInfoAddr); \
    JSbt_Bind_Global_Func(LocalConvexResult_get_m_hitNormalLocal_0, btVector3Addr, LocalConvexResultAddr); \
    JSbt_Bind_Global_Func(LocalConvexResult_set_m_hitNormalLocal_1, void, LocalConvexResultAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(LocalConvexResult_get_m_hitPointLocal_0, btVector3Addr, LocalConvexResultAddr); \
    JSbt_Bind_Global_Func(LocalConvexResult_set_m_hitPointLocal_1, void, LocalConvexResultAddr, btVector3Addr); \
    JSbt_Bind_Global_Func(LocalConvexResult_get_m_hitFraction_0, double, LocalConvexResultAddr); \
    JSbt_Bind_Global_Func(LocalConvexResult_set_m_hitFraction_1, void, LocalConvexResultAddr, double); \
    JSbt_Bind_Global_Func(LocalConvexResult___destroy___0, void, LocalConvexResultAddr); \
	/* btConstraintParams */\
	JSbt_Bind_Global_Func(enum_btConstraintParams_BT_CONSTRAINT_ERP, long_t);\
	JSbt_Bind_Global_Func(enum_btConstraintParams_BT_CONSTRAINT_STOP_ERP, long_t);\
	JSbt_Bind_Global_Func(enum_btConstraintParams_BT_CONSTRAINT_CFM, long_t);\
	JSbt_Bind_Global_Func(enum_btConstraintParams_BT_CONSTRAINT_STOP_CFM, long_t);\
	/* PHY_ScalarType */\
	JSbt_Bind_Global_Func(enum_PHY_ScalarType_PHY_FLOAT, long_t);\
	JSbt_Bind_Global_Func(enum_PHY_ScalarType_PHY_DOUBLE, long_t);\
	JSbt_Bind_Global_Func(enum_PHY_ScalarType_PHY_INTEGER, long_t);\
	JSbt_Bind_Global_Func(enum_PHY_ScalarType_PHY_SHORT, long_t);\
	JSbt_Bind_Global_Func(enum_PHY_ScalarType_PHY_FIXEDPOINT88, long_t);\
	JSbt_Bind_Global_Func(enum_PHY_ScalarType_PHY_UCHAR, long_t);\
    JSP_ADD_GLOBAL_FUNCTION(btGImpactCollisionAlgorithm_RegisterAlgorithm, btGImpactCollisionAlgorithm_RegisterAlgorithm0, void, btCollisionDispatcherAddr);
}