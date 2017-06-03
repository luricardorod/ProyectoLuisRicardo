#include <Core.h>
#include <PrimitiveManager.h>
#include <PrimitiveInstance.h>
#include "CMatrix4D.h"
#include "CVector4D.h"
#include <Timer.h>
#include "btBulletDynamicsCommon.h"
#include <stdio.h>

class TestApp : public AppBase {
public:
	TestApp() : AppBase() {}
	void InitVars();
	void CreateAssets();
	void DestroyAssets();
	void InitWorldBullet();
	void DestroyWorldBullet();
	void UpdateWorldBullet();


	void OnUpdate();
	void OnDraw();
	void OnInput();

	void OnPause();
	void OnResume();

	void OnReset();

	PrimitiveManager PrimitiveMgr;
	PrimitiveInst	primitiveFigs[15];
	PrimitiveInst	figsFisics[31];
	PrimitiveInst	rendertargets[7];

	bool runBullet = false;

	CVector4D		Position;
	CVector4D		PositionLight;

	CVector4D		Orientation;
	CVector4D		Scaling;
	CVector4D		PositionCamera;
	float			rotationCam = 0;
	CMatrix4D		View;
	CMatrix4D		Projection;
	CMatrix4D		VP;
	Lights			worldLights;
	Timer			DtTimer;

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	int				GBufferPass;
	int				DeferredPass;
	int				QuadIndex;
};