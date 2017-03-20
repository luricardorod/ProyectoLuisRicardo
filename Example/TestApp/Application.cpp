#include "Application.h"

void TestApp::InitVars() {
	DtTimer.Init();
	Position	= CVector4D(0.0f, 0.0f, 0.0f, 0);
	Orientation = CVector4D(0.0f, 0.0f, 0.0f, 0);
	Scaling		= CVector4D(1.0f, 1.0f, 1.0f, 0);
}

void TestApp::CreateAssets() {	
	PrimitiveMgr.SetVP(&VP);

	int indexCerdo = PrimitiveMgr.CreateObject3D("Models/NuCroc.X");
	primitiveFigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &VP);


	CMatrix4D View;
	CVector4D Pos = CVector4D(0.0f, 1.0f, 5.0f, 0);
	CVector4D Up = CVector4D(0.0f, 1.0f, 0.0f, 0);
	CVector4D LookAt = CVector4D(0.0001f, 0.0001f, 0.0001f, 0) - Pos;

	View = LookAtRH(Pos, LookAt, Up);
	CMatrix4D proj = PerspectiveFovRH(45*3.1416/180, 1280.0f / 720.0f, 0.1f, 1000.0f);
	VP = View * proj;

}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();

	OnInput();


	primitiveFigs[0].TranslateAbsolute(Position.x, Position.y, Position.z);
	primitiveFigs[0].RotateXAbsolute(Orientation.x);
	primitiveFigs[0].RotateYAbsolute(Orientation.y);
	primitiveFigs[0].RotateZAbsolute(Orientation.z);
	primitiveFigs[0].ScaleAbsolute(Scaling.x);
	primitiveFigs[0].Update();

	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	
	primitiveFigs[0].Draw();
	pFramework->pVideoDriver->SwapBuffers();
}

void TestApp::OnInput() {
	
	if (IManager.PressedKey(SDLK_UP)) {
		Position.y += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_DOWN)) {
		Position.y -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_LEFT)) {
		Position.x -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_RIGHT)) {
		Position.x += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_z)) {
		Position.z -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_x)) {
		Position.z += 1.0f*DtTimer.GetDTSecs();
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