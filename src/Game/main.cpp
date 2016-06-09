// Precompiled Header
#include "Game/Stdafx.h"

#include "Platform/Platform.h"
#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/Game.h"
#include "Core/SystemManager.h"
#include "Core/System.h"
#include "Core/Renderer.h"
#include "Core/InputManager.h"
#include "Core/MeshManager.h"

#include <windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	BH::MeshManager::LoadDirectory = "../resources/Models/";
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::Renderer>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::MeshManager>();
	SYSTEM_MANAGER.GameComponentDependency<BH::MeshManager, BH::Renderer>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::InputManager>();
	//GAME.Window.SetTitle("BlackHole Game");
	GAME.Initialise();
	GAME.Run();
	GAME.Shutdown();

	return 0;
}