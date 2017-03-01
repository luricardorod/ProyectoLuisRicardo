#include "Application.h"
#include <SDL\SDL_mouse.h>

void TestApp::InitVars() {
	DtTimer.Init();
	Position	= CVector4D(0.0f, 0.0f, 0.0f, 0);
	Orientation = CVector4D(0.0f, 0.0f, 1.0f, 0);
	Scaling		= CVector4D(1.0f, 1.0f, 1.0f, 0);
}

void TestApp::CreateAssets() {	
	PrimitiveMgr.SetVP(&VP);

	int indexCerdo = PrimitiveMgr.CreateObject3D("NuCroc.X");
	int indexBatman = PrimitiveMgr.CreateObject3D("NuBatman.X");

	//int cubo = PrimitiveMgr.CreateCube();
	primitiveFigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &VP);
	primitiveFigs[1].CreateInstance(PrimitiveMgr.GetPrimitive(indexBatman), &VP);

	Position.x = 0;
	Position.y = 0;
	Position.z = 0;
	Position.w = 0;

	PositionCamera.x = 0;
	PositionCamera.y = 0.0f;
	PositionCamera.z = -500.0f;
	PositionCamera.w = 0;
	
	CMatrix4D View;
	Up = CVector4D(0.0f, 1.0f, 0.0f, 0);
	CVector4D LookAt = PositionCamera + Normalize(Orientation) * 10;

	View = LookAtRH(PositionCamera, LookAt, Up);
	proj = PerspectiveFovRH(45*3.1416/180, 1280.0f / 720.0f, 0.1f, 10000.0f);
	VP = View * proj;

}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();
	OnInput();
	PositionCamera = PositionCamera + Position.z * Normalize(Orientation);
	CVector4D proyeccion = Normalize(Orientation) *  -1*Position.x;
	proyeccion.y = 0;
	float temp = proyeccion.x;
	proyeccion.x = proyeccion.z;
	proyeccion.z = -temp;
	PositionCamera = PositionCamera + proyeccion;
	CMatrix4D View;
	CVector4D LookAt = PositionCamera + Normalize(Orientation) * 10;

	View = LookAtRH(PositionCamera, LookAt, Up);
	proj = PerspectiveFovRH(45 * 3.1416 / 180, 1280.0f / 720.0f, 0.1f, 10000.0f);
	VP = View * proj;
	Position = CVector4D(0.0f, 0.0f, 0.0f, 0);

	//primitiveFigs[0].TranslateAbsolute(Position.x, Position.y, Position.z);
	//primitiveFigs[0].RotateXAbsolute(Orientation.x);
	//primitiveFigs[0].RotateYAbsolute(Orientation.y);
	//primitiveFigs[0].RotateZAbsolute(Orientation.z);
	//primitiveFigs[0].ScaleAbsolute(Scaling.x);
	//primitiveFigs[0].Update();
	//primitiveFigs[1].TranslateAbsolute(Position.x+1, Position.y, Position.z);
	//primitiveFigs[1].RotateXAbsolute(Orientation.x);
	//primitiveFigs[1].RotateYAbsolute(Orientation.y);
	//primitiveFigs[1].RotateZAbsolute(Orientation.z);
	//primitiveFigs[1].ScaleAbsolute(Scaling.x);
	//primitiveFigs[1].Update();
	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	
	primitiveFigs[0].Draw();
	primitiveFigs[1].Draw();
	pFramework->pVideoDriver->SwapBuffers();
}

void TestApp::OnInput() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	printf("%d/n", x);

	if (x < 620)
	{
		Orientation  = RotationY( DtTimer.GetDTSecs() *.001f * (620-x)) * Orientation;
	}
	if (x > 660)
	{
		Orientation = RotationY(-1 * DtTimer.GetDTSecs() * .001f * (x - 660)) * Orientation;
	}
	if (y < 340)
	{
		Orientation = RotationX(-1 * DtTimer.GetDTSecs() *.01f * (340 - y)) * Orientation;
	}
	if (y > 380)
	{
		Orientation = RotationX(DtTimer.GetDTSecs() * .01f * (y - 340)) * Orientation;
	}

	if (IManager.PressedKey(SDLK_UP)) {
		Position.z += 100*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_DOWN)) {
		Position.z -= 100*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_LEFT)) {
		Position.x -= 100*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_RIGHT)) {
		Position.x += 100*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_z)) {
		Position.z -= 100*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_x)) {
		Position.z += 100*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_PLUS)) {
		Scaling.x += 1.0f*DtTimer.GetDTSecs();
		Scaling.y += 1.0f*DtTimer.GetDTSecs();
		Scaling.z += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_MINUS)) {
		Scaling.x -= 1.0f*DtTimer.GetDTSecs();
		Scaling.y -= 1.0f*DtTimer.GetDTSecs();
		Scaling.z -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP5)) {
		Orientation.x -= 60.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP6)) {
		Orientation.x += 60.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP2)) {
		Orientation.y -= 60.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP3)) {
		Orientation.y += 60.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP0)) {
		Orientation.z -= 60.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_PERIOD)) {
		Orientation.z += 60.0f*DtTimer.GetDTSecs();
	}

	
}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}