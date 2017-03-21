#include <Core.h>
#include <PrimitiveManager.h>
#include <PrimitiveInstance.h>
#include "CMatrix4D.h"
#include "CVector4D.h"
#include <Timer.h>

class TestApp : public AppBase {
public:
	TestApp() : AppBase() {}
	void InitVars();
	void CreateAssets();
	void DestroyAssets();

	void OnUpdate();
	void OnDraw();
	void OnInput();

	void OnPause();
	void OnResume();

	void OnReset();

	PrimitiveManager PrimitiveMgr;
	PrimitiveInst	primitiveFigs[10];

	CVector4D		Position;
	CVector4D		Orientation;
	CVector4D		Scaling;
	CVector4D		PositionCamera;
	float			rotationCam = 0;
	CMatrix4D		View;
	CMatrix4D		Projection;
	CMatrix4D		VP;

	Timer			DtTimer;
};