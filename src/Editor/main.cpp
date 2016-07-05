#include "Platform/Platform.h"
#include "Platform/PlatformWindow.h"
#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/Game.h"
#include "Core/SystemManager.h"
#include "Core/System.h"
#include "Core/Renderer.h"
#include "Core/InputManager.h"
#include "Core/ModelManager.h"
#include "Core/TextureManager.h"
#include "Core/HDRTextureManager.h"
#include "Core/AnimationControllerManager.h"
#include "Core/GameStateManager.h"
#include "Core/LevelManager.h"
#include "Core/MaterialManager.h"
#include "Core/XMLDeserialiser.h"
#include "Core/CollisionManager.h"

#include "Script/CommonFunctions.h"

#include "Game/ScriptLoader.h"
#include "Game/GameConfig.h"
#include "Game/DebugCamera.h"
#include "Game/CollisionSolver.h"

#include "Editor/EditorConfig.h"
#include "Editor/EditorApplication.h"
#include "Editor/MaterialEditor.h"
#include "Editor/MaterialEditorCamera.h"

#if defined( BH_DEBUG ) && ( GAME_LEAK_CHECK == BH_ENABLED )
	#pragma comment ( lib, "../Lib/vld.lib" )
	#include "vld.h"
#endif

int main()
{
	// @cleanup: Clean up this messy portion
	BH::ModelManager::LoadDirectory = "../resources/Models/";
	BH::TextureManager::LoadDirectory = "../resources/Textures/";
	BH::ScriptLoader::LoadDirectory = "../scripts/";
	BH::LevelManager::LoadDirectory = "../resources/Levels/";
	BH::MaterialManager::LoadFileName = "../resources/Material.xml";
	BH::HDRTextureManager::LoadDirectory = "../resources/HDR/";

	SYSTEM_MANAGER.AddGameComponentToSystem<BH::InputManager>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::Renderer>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::ModelManager>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::TextureManager>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::HDRTextureManager>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::AnimationControllerManager>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::ScriptLoader>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::GameStateManager>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::LevelManager>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::MaterialManager>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::CollisionManager>();
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::CollisionSolver>();
	SYSTEM_MANAGER.GameComponentDependency<BH::ModelManager, BH::Renderer>();
	SYSTEM_MANAGER.GameComponentDependency<BH::TextureManager, BH::Renderer>();
	SYSTEM_MANAGER.GameComponentDependency<BH::HDRTextureManager, BH::Renderer>();

	// For interop
	SetRenderer( SYSTEM_MANAGER.GetGameComponentFromSystem<BH::Renderer>() );
	SetModelManager( SYSTEM_MANAGER.GetGameComponentFromSystem<BH::ModelManager>() );
	SetAnimationControllerManager( SYSTEM_MANAGER.GetGameComponentFromSystem<BH::AnimationControllerManager>() );
	SetTextureManager( SYSTEM_MANAGER.GetGameComponentFromSystem<BH::TextureManager>() );
	SetMaterialManager( SYSTEM_MANAGER.GetGameComponentFromSystem<BH::MaterialManager>() );
	SetCollisionManager( SYSTEM_MANAGER.GetGameComponentFromSystem<BH::CollisionManager>() );

	// Debug
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::DebugCamera>();
	SYSTEM_MANAGER.GameComponentDependency<BH::DebugCamera, BH::Renderer>();

#if GAME_EDITOR_MODE
	SYSTEM_MANAGER.AddGameComponentToSystem<BH::EditorSystem>();
	SYSTEM_MANAGER.GameComponentDependency<BH::EditorSystem, BH::ScriptLoader>();
	SYSTEM_MANAGER.GameComponentDependency<BH::EditorSystem, BH::Renderer>();
	SYSTEM_MANAGER.GameComponentDependency<BH::EditorSystem, BH::InputManager>();

	SYSTEM_MANAGER.AddGameComponentToSystem<BH::MaterialEditor>();
	SYSTEM_MANAGER.GameComponentDependency<BH::MaterialEditor, BH::MaterialManager>();
	SYSTEM_MANAGER.GameComponentDependency<BH::MaterialEditor, BH::EditorSystem>();

	SYSTEM_MANAGER.AddGameComponentToSystem<BH::MaterialEditorCamera>();
	SYSTEM_MANAGER.GameComponentDependency<BH::MaterialEditorCamera, BH::Renderer>();
	SYSTEM_MANAGER.GameComponentDependency<BH::MaterialEditorCamera, BH::MaterialEditor>();
#endif

	//GAME.Window.SetTitle( "BlackHole Editor" );
	GAME.Window.GetSettings().w = 1280;
	GAME.Window.GetSettings().h = 720;
	GAME.Initialise();

	// Settings
	GAME.ObjectManager.LoadArcheTypes<BH::XMLDeserialiser>( "../resources/Prefabs.xml" );
	SYSTEM_MANAGER.GetGameComponentFromSystem<BH::LevelManager>()->SetLevel( "SandBox" );

	// @debug
	SYSTEM_MANAGER.GetGameComponentFromSystem<BH::Renderer>()->EnableAmbientOcculsion = false;

	GAME.Run();
	GAME.Shutdown();

	return 0;
}