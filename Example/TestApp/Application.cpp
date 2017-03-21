#include "Application.h"
#include <SDL\SDL_mouse.h>
enum {
	SCENE = 0,
	CROC,
	LINK,
	HOUSE_L,
	HOUSE_R,
	DRONE,
	CERDO,
	CERDOLIGHT,
	TOTAL_INSTANCES
};
void TestApp::InitVars() {
	DtTimer.Init();
	Position	= CVector4D(0.0f, 0.0f, 0.0f, 0);
	PositionLight = CVector4D(0.0f, 0.0f, 0.0f, 0);
	Orientation = CVector4D(0.0f, 0.0f, 10.0f, 0);
	Scaling		= CVector4D(1.0f, 1.0f, 1.0f, 0);
	rotationCam = 0;
	worldLights.dirGlobal = CVector4D(1, 0, 0, 0);
	worldLights.colorGlobal = CVector4D(.2, 0, 0, 0);
	worldLights.colorPoint = CVector4D(0, 0, .5, 0);
	worldLights.posPoint = CVector4D(50, 100, 10, 0);
}

void TestApp::CreateAssets() {	
	PrimitiveMgr.SetVP(&VP);


	int index = PrimitiveMgr.CreateObject3D("Models/Scene.X");
	primitiveFigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateObject3D("Models/NuCroc.X");
	primitiveFigs[1].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateObject3D("Models/Link.X");
	primitiveFigs[2].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	index = PrimitiveMgr.CreateObject3D("Models/House.X");
	primitiveFigs[3].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	primitiveFigs[4].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	index = PrimitiveMgr.CreateObject3D("Models/Drone.X");
	primitiveFigs[5].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	index = PrimitiveMgr.CreateObject3D("Models/Cerdo.X");
	primitiveFigs[6].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	primitiveFigs[7].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	
	for (int i = 0; i < TOTAL_INSTANCES; i++) {
		primitiveFigs[i].lights = &worldLights;
	}
	
	CMatrix4D View;
	PositionCamera = CVector4D(0.0f, 10.0f, 100.0f, 0);
	CVector4D Up = CVector4D(0.0f, 1.0f, 0.0f, 0);
	CVector4D LookAt = PositionCamera - Normalize(Orientation) * 10;

	View = LookAtRH(PositionCamera, LookAt, Up);
	CMatrix4D proj = PerspectiveFovRH(45*3.1416/180, 1280.0f / 720.0f, 0.1f, 10000.0f);
	VP = View * proj;


	primitiveFigs[CROC].TranslateAbsolute(-43.871941f, 0.064795f, -58.153839f);
	primitiveFigs[CROC].RotateXAbsolute(0.0f);
	primitiveFigs[CROC].RotateYAbsolute(207.731613f);
	primitiveFigs[CROC].RotateXAbsolute(0.0f);
	primitiveFigs[CROC].ScaleAbsolute(0.151794f);
	primitiveFigs[CROC].Update();

	primitiveFigs[LINK].TranslateAbsolute(12.499269f, -0.079694f, -63.019135f);
	primitiveFigs[LINK].RotateXAbsolute(0.0f);
	primitiveFigs[LINK].RotateYAbsolute(-410.563721f);
	primitiveFigs[LINK].RotateXAbsolute(0.0f);
	primitiveFigs[LINK].ScaleAbsolute(0.114174f);
	primitiveFigs[LINK].Update();
	primitiveFigs[HOUSE_L].TranslateAbsolute(56.104416f, 0.574123f, 29.808973f);
	primitiveFigs[HOUSE_L].RotateXAbsolute(0.0f);
	primitiveFigs[HOUSE_L].RotateYAbsolute(141.33f);
	primitiveFigs[HOUSE_L].RotateXAbsolute(0.0f);
	primitiveFigs[HOUSE_L].ScaleAbsolute(0.670580f);
	primitiveFigs[HOUSE_L].Update();
	primitiveFigs[HOUSE_R].TranslateAbsolute(-82.823868f, 0.443788f, 34.599747f);
	primitiveFigs[HOUSE_R].RotateXAbsolute(0.0f);
	primitiveFigs[HOUSE_R].RotateYAbsolute(51.43101f);
	primitiveFigs[HOUSE_R].RotateXAbsolute(0.0f);
	primitiveFigs[HOUSE_R].ScaleAbsolute(0.670580f);
	primitiveFigs[HOUSE_R].Update();
	primitiveFigs[CERDO].TranslateAbsolute(-14.064236f, -3.514139f, -29.351925f);
	primitiveFigs[CERDO].RotateXAbsolute(0.0f);
	primitiveFigs[CERDO].RotateYAbsolute(0.0f);
	primitiveFigs[CERDO].RotateXAbsolute(0.0f);
	primitiveFigs[CERDO].ScaleAbsolute(27.208776f);
	primitiveFigs[CERDO].Update();
	worldLights.posPoint = CVector4D(primitiveFigs[7].position.m30, primitiveFigs[7].position.m31, primitiveFigs[7].position.m32,0);
	PositionLight = worldLights.posPoint;
}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();

	OnInput();
	Orientation = RotationY(rotationCam) * Orientation;
	PositionCamera = PositionCamera + Position.z * Normalize(Orientation)*30;
	CVector4D proyeccion = Normalize(Orientation) * Position.x * 30;
	proyeccion.y = 0;
	float temp = proyeccion.x;
	proyeccion.x = proyeccion.z;
	proyeccion.z = -temp;
	PositionCamera = PositionCamera + proyeccion;
	CMatrix4D View;
	CVector4D Up = CVector4D(0.0f, 1.0f, 0.0f, 0);

	CVector4D LookAt = PositionCamera - Normalize(Orientation) * 10;

	View = LookAtRH(PositionCamera, LookAt, Up);
	CMatrix4D proj = PerspectiveFovRH(45 * 3.1416 / 180, 1280.0f / 720.0f, 0.1f, 10000.0f);
	VP = View * proj;

	primitiveFigs[0].Update();

	primitiveFigs[7].TranslateAbsolute(PositionLight.x, PositionLight.y, PositionLight.z);
	primitiveFigs[7].Update();
	worldLights.posPoint = CVector4D(primitiveFigs[7].position.m30, primitiveFigs[7].position.m31, primitiveFigs[7].position.m32,0);
	OnDraw();
	Orientation = CVector4D(0.0f, 0.0f, 10.0f, 0);
	Position = CVector4D(0.0f, 0.0f, 0.0f, 0);
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	
	for (int i = 0; i < TOTAL_INSTANCES; i++) {
		primitiveFigs[i].Draw();
	}
	pFramework->pVideoDriver->SwapBuffers();
}

void TestApp::OnInput() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (x < 100)
	{
		rotationCam += 0.5f*DtTimer.GetDTSecs();
	}
	if (x > 1180)
	{
		rotationCam -= 0.5f*DtTimer.GetDTSecs();
	}
	Orientation.x = (640 - x)/50.0f;
	Orientation.y = (y - 360)/20.0f;

	if (IManager.PressedKey(SDLK_UP)) {
		Position.z -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_DOWN)) {
		Position.z += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_LEFT)) {
		Position.x -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_RIGHT)) {
		Position.x += 1.0f*DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_w)) {
		PositionLight.z -= 20.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_s)) {
		PositionLight.z += 20.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_a)) {
		PositionLight.x -= 20.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_d)) {
		PositionLight.x += 20.0f*DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_z)) {
		PositionLight.y += 20.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_x)) {
		PositionLight.y -= 20.0f*DtTimer.GetDTSecs();
	}
}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}