#include "Win32Framework.h"
#ifdef USING_OPENGL_ES
#include "GLDriver.h"
#elif defined(USING_D3D11)
#include "D3DXDriver.h"
#endif
// SDL
#include <SDL/SDL.h>
// Windows 
#include <windows.h>
#include <mmsystem.h>

void Win32Framework::InitGlobalVars() {

	
}

void Win32Framework::OnCreateApplication(){
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("UAD Framework Ricardo", 0);
	int flags = SDL_HWSURFACE;
	//flags |= SDL_FULLSCREEN;
	//flags |= SDL_RESIZABLE;
	int width = 1280;
	int height = 720;
	SDL_SetVideoMode(width, height, 32, flags);

#ifdef USING_OPENGL_ES
	pVideoDriver = new GLDriver;
#elif defined(USING_D3D11)
	pVideoDriver = new D3DXDriver;
	pVideoDriver->SetDimensions(width, height);
#endif
	pVideoDriver->SetWindow(0);
	pVideoDriver->InitDriver();

	pBaseApp->InitVars();
	pBaseApp->CreateAssets();
}
void Win32Framework::OnDestroyApplication() {
	pBaseApp->DestroyAssets();
	pVideoDriver->DestroyDriver();
	delete pVideoDriver;
	SDL_Quit();
}
void Win32Framework::OnInterruptApplication() {
}
void Win32Framework::OnResumeApplication() {
}
void Win32Framework::UpdateApplication() {
	while (m_alive) {
		ProcessInput();
		pBaseApp->OnUpdate();
	}
}
void Win32Framework::ProcessInput() {
	SDL_Event       evento;
	while (SDL_PollEvent(&evento)) {
		switch (evento.type) {
			case SDL_KEYDOWN: {
				if (evento.key.keysym.sym == SDLK_q) {
					m_alive = false;
				}
				pBaseApp->IManager.KeyStates[0][evento.key.keysym.sym] = true;
			}break;
			case SDL_KEYUP: {
				pBaseApp->IManager.KeyStates[0][evento.key.keysym.sym] = false;
				pBaseApp->IManager.KeyStates[1][evento.key.keysym.sym] = false;
			}break;
		}
	}
}
void Win32Framework::ResetApplication() {
}