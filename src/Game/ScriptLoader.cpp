#include "Platform/Platform.h"

#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/Game.h"
#include "Core/SystemManager.h"
#include "Core/Renderer.h"
#include "Core/System.h"
#include "Core/ObjectManager.h"
#include "Core/TypeCreator.h"
#include "Core/InputManager.h"

#include "Script/Script.h"
#include "Script/ScriptComponentTypeCreator.h"
#include "Script/ScriptEventManager.h"

#include "Game/ScriptLoader.h"

namespace BH
{
	String ScriptLoader::LoadDirectory = "";

	ScriptLoader::ScriptLoader()
		: mScriptEventManager( nullptr )
	{
	}

	ScriptLoader::~ScriptLoader()
	{
	}

	void ScriptLoader::Initialise()
	{
		LOG("Initialising Script Loader...");
		PrepareScript();
		BH::Script::BuildScripts( ENTITY_MANAGER.Types );

		if ( !ENTITY_MANAGER.Types.Get( TypeCName<ScriptComponent>() ) )
			TypeCreator<ScriptComponent>::RegisterType();

		ScriptType::SetChangingObject( &Object::ChangingObject );
		ScriptComponent::SetTypeDB( &ENTITY_MANAGER.Types );
		ScriptComponent::SetInitialiseCallback( std::bind( std::mem_fn( &ScriptLoader::ScriptInitialiseCallback ), this, std::placeholders::_1 ) );
		ScriptComponent::SetShutdownCallback( std::bind( std::mem_fn( &ScriptLoader::ScriptShutdownCallback ), this, std::placeholders::_1 ) );

		mScriptEventManager = new ScriptEventManager();
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyTriggeredEvent( 
			Delegate<void( Key::KeyCode )>::FromMethod<E_METHOD( ScriptEventManager, RaiseScriptKeyTriggeredEvent )>( mScriptEventManager ) );
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyUpEvent( 
			Delegate<void( Key::KeyCode )>::FromMethod<E_METHOD( ScriptEventManager, RaiseScriptKeyReleasedEvent )>( mScriptEventManager ) );
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyDownEvent( 
			Delegate<void( Key::KeyCode )>::FromMethod<E_METHOD( ScriptEventManager, RaiseScriptKeyPressedEvent )>( mScriptEventManager ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseTriggeredEvent( 
			Delegate<void( s32, s32, Mouse::Button )>::FromMethod<E_METHOD( ScriptEventManager, RaiseScriptMouseTriggeredEvent )>( mScriptEventManager ) );
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseUpEvent( 
			Delegate<void( s32, s32, Mouse::Button )>::FromMethod<E_METHOD( ScriptEventManager, RaiseScriptMouseReleasedEvent )>( mScriptEventManager ) );
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseDownEvent( 
			Delegate<void( s32, s32, Mouse::Button )>::FromMethod<E_METHOD( ScriptEventManager, RaiseScriptMousePressedEvent )>( mScriptEventManager ) );

		LOG("Script Loader Initialised");
	}

	void ScriptLoader::Shutdown()
	{
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyTriggeredEvent( 
			Delegate<void( Key::KeyCode )>::FromMethod<E_METHOD( ScriptEventManager, RaiseScriptKeyTriggeredEvent )>( mScriptEventManager ) );
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyUpEvent(
			Delegate<void( Key::KeyCode )>::FromMethod<E_METHOD( ScriptEventManager, RaiseScriptKeyReleasedEvent )>( mScriptEventManager ) );
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyDownEvent(
			Delegate<void( Key::KeyCode )>::FromMethod<E_METHOD( ScriptEventManager, RaiseScriptKeyPressedEvent )>( mScriptEventManager ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseTriggeredEvent(
			Delegate<void( s32, s32, Mouse::Button )>::FromMethod<E_METHOD( ScriptEventManager, RaiseScriptMouseTriggeredEvent )>( mScriptEventManager ) );
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseUpEvent(
			Delegate<void( s32, s32, Mouse::Button )>::FromMethod<E_METHOD( ScriptEventManager, RaiseScriptMouseReleasedEvent )>( mScriptEventManager ) );
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseDownEvent(
			Delegate<void( s32, s32, Mouse::Button )>::FromMethod<E_METHOD( ScriptEventManager, RaiseScriptMousePressedEvent )>( mScriptEventManager ) );

		delete mScriptEventManager;
		mScriptEventManager = nullptr;
	}

	void ScriptLoader::ReInitialise()
	{
		Shutdown();

		//std::cout << "[Before Unbuild!]" << std::endl;
		//ENTITY_MANAGER.Types.PrintAllTypes();
		//std::cout << "------------------------" << std::endl;

		// Un build script
		BH::Script::UnBuildScript( ENTITY_MANAGER.Types );

		//std::cout << "[After Unbuild!]" << std::endl;
		//ENTITY_MANAGER.Types.PrintAllTypes();
		//std::cout << "------------------------" << std::endl;

		PrepareScript();

		// Recompile script
		BH::Script::ReBuildScript( ENTITY_MANAGER.Types );

		//std::cout << "[After Rebuild!]" << std::endl;
		//ENTITY_MANAGER.Types.PrintAllTypes();
		//std::cout << "------------------------" << std::endl;

		//// Update type change
		//for( auto & i : mScriptList )
		//{
		//	i->SetScriptReflectionType( ENTITY_MANAGER.Types.Get( CName( i->GetScript().mType.c_str() ) ) );
		//	i->ResetObject();
		//}
	}

	void ScriptLoader::LoadAllScripts()
	{
		// Add all scripts not in folders
		FileInfo::List files = FileSystem::GetFileList( LoadDirectory.c_str(), false );

		for ( const auto & i : files )
		{
			// Look for cs
			if ( i.GetExtension() != "cs" )
				continue;

			BH::Script::AddScript( ( LoadDirectory + i.GetFullFile() ).c_str() );
		}
	}

	void ScriptLoader::PrepareScript()
	{
		// @test
		//BH::Script::AddScript( "TestDependencies.cs" );
		//BH::Script::AddScript( "Test.cs" );

		LoadAllScripts();
	}

	void ScriptLoader::ScriptInitialiseCallback( ScriptComponent * component )
	{
		ScriptList::iterator it = std::find( mScriptList.begin(), mScriptList.end(), component );

		// Script included
		if ( it != mScriptList.end() )
			return;

		mScriptList.push_back( component );
		mScriptObjects[component->GetObjectName()][component->GetScript().mType] = component;
	}

	void ScriptLoader::ScriptShutdownCallback( ScriptComponent * component )
	{
		ScriptList::iterator it = std::find( mScriptList.begin(), mScriptList.end(), component );

		// Script not included
		if ( it == mScriptList.end() )
			return;

		mScriptList.erase( it );

		Components::iterator cit = mScriptObjects[component->GetObjectName()].find( component->GetScript().mType );

		if ( cit != mScriptObjects[component->GetObjectName()].end() )
			mScriptObjects[component->GetObjectName()].erase( cit );
	}

	ScriptComponent * ScriptLoader::GetScriptByObjectAndType( const String & object, const String & type )
	{
		Objects::iterator oit = mScriptObjects.find( object );
		if ( oit != mScriptObjects.end() )
		{
			Components::iterator cit = oit->second.find( type );
			if ( cit != oit->second.end() )
				return cit->second;
		}

		return nullptr;
	}

	template<>
	void * TypeDB::NewEntity<ScriptComponent>()
	{
		BH_STATIC_ASSERT( ( std::is_base_of<ObjectComponent, ScriptComponent>::value == true ), 
							"SYSTEM_ASSIGN_OBJCOMPONENT - ObjectComponentType is not a derived class of ObjectComponent!" );

		ScriptComponent * t = ::BH::ObjectAllocator<ScriptComponent>::Instance().New();
		t->mAddToSystem = &ObjectComponent::AddToAssignedSystem<ScriptComponent>;
		t->mRemoveFromSystem = &ObjectComponent::RemoveFromAssignedSystem<ScriptComponent>;
		return t;
	}
}