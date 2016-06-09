#ifndef BH_SYSTEM_MANAGER_H
#define BH_SYSTEM_MANAGER_H

#include "Core/Uncopyable.h"
#include "Core/Singleton.h"
#include "Core/CName.h"
#include "Core/LogManager.h"

#include "Platform/Thread.h"

#include <unordered_map>
#include <utility>

namespace BH
{
	// Manager that manager system
	class SystemManager : private Uncopyable
	{
	public:
		typedef std::pair<System *, Thread *> SystemEntry;			//!< System entry
		typedef std::unordered_map<CName, SystemEntry> SystemMap;	//!< Organised system by it's name
		
		DECLARE_SINGLETON(SystemManager);							//!< Declared as singleton

	public:
		// Destructor
		~SystemManager();

		// Add System
		template< typename T >
		T * AddSystem( const Char * name );

		// Get System ("Main" to get main system)
		template< typename T >
		T * GetSystem( const Char * name );

		// Add game component to assigned system base on game component type
		template< typename GameComponentType >
		GameComponentType * AddGameComponentToSystem();

		// Set dependency
		template< typename DependeeType, typename DependentType >
		void GameComponentDependency();

		// Get game component from system
		template< typename GameComponentType >
		GameComponentType * GetGameComponentFromSystem();

		// Add object component to assigned system base on object component type
		template< typename ObjectComponentType >
		void AddObjectComponentToSystem( ObjectComponent * objectcomponent );

		// Get assigned system
		template< typename ObjectComponentType >
		System * GetAssignedSystem();

		// Remove object component from assigned system based on object component type
		template< typename ObjectComponentType >
		void RemoveObjectComponentFromSystem( ObjectComponent * objectcomponent );

		// Initialise all system, threading them
		void Initialise();

		// Shutdown all system, terminate all threads
		void Shutdown();

		// Update all systems
		void Update( f32 dt );

		// Draw on all systems
		void Draw( f32 dt );

		// Is all systems idling
		bool IsAllIdling() const;

		// Pause all object component in all system
		void PauseObjectComponents();

		// Resume all object component in all system
		void ResumeObjectComponents();

		// Hide all object component in all system
		void HideObjectComponents();

		// Show all object component in all system
		void ShowObjectComponents();

	public:
		// Constructor
		SystemManager();

	private:
		// Thread functions for system
		static void SystemLoop( System * system );

	private:
		System * mMainSystem;										//!< System in main thread
		SystemMap mSystems;											//!< All systems
		bool mUpdateFlag;											//!< Update flag

	};

	// Helper functor to add dependency
	template< typename DependeeType, typename DependentType >
	class DependencyFunctor
	{
	public:
		void operator()() const;
	};
}

#include "Core/SystemManager.inl"

// Convenience macro
#define SYSTEM_MANAGER\
	/*::BH::SystemManager::Instance()*/ ::BH::Game::Instance().SystemManager

// Macro to assign game component type to a certain system
#define SYSTEM_ASSIGN_GAMECOMPONENT( GameComponentType, SystemName )\
	DECLARE_TYPE( GameComponentType, GameComponentType );\
	namespace BH\
	{\
		template<>\
		GameComponentType * SystemManager::AddGameComponentToSystem<GameComponentType>()\
		{\
			if( System * system = GetSystem<System>( #SystemName ) )\
				return system->AddGameComponent<GameComponentType>( TypeName<GameComponentType>() );\
			return nullptr;\
		}\
		\
		template<>\
		GameComponentType * SystemManager::GetGameComponentFromSystem<GameComponentType>()\
		{\
			if( System * system = GetSystem<System>( #SystemName ) )\
				return system->GetGameComponent<GameComponentType>( TypeName<GameComponentType>() );\
			return nullptr;\
		}\
		\
		template< typename DependentType >\
		class DependencyFunctor<GameComponentType, DependentType>\
		{\
		public:\
			void operator()() const\
			{\
				if( System * system = SYSTEM_MANAGER.GetSystem<System>( #SystemName ) )\
					system->Dependency( TypeName<GameComponentType>(), TypeName<DependentType>() );\
			}\
		};\
	}

// Macro to assign object component type to a certain system ( Hackery =( )
#define SYSTEM_ASSIGN_OBJCOMPONENT( ObjectComponentType, SystemName )\
	namespace BH\
	{\
		template<>\
		void SystemManager::AddObjectComponentToSystem<ObjectComponentType>( ObjectComponent * objcomponent )\
		{\
			if( System * system = GetSystem<System>( #SystemName ) )\
				system->AddObjectComponent( static_cast<ObjectComponentType *>( objcomponent ) );\
		}\
		\
		template<>\
		void SystemManager::RemoveObjectComponentFromSystem<ObjectComponentType>( ObjectComponent * objcomponent )\
		{\
			if( System * system = GetSystem<System>( #SystemName ) )\
				system->RemoveObjectComponent( static_cast<ObjectComponentType *>( objcomponent ) );\
		}\
		\
		template<>\
		System * SystemManager::GetAssignedSystem<ObjectComponentType>()\
		{\
			return GetSystem<System>( #SystemName );\
		}\
		\
		template<>\
		void * TypeDB::NewEntity<ObjectComponentType>()\
		{\
			BH_STATIC_ASSERT( ( std::is_base_of<ObjectComponent, ObjectComponentType>::value == true ), \
								"SYSTEM_ASSIGN_OBJCOMPONENT - ObjectComponentType is not a derived class of ObjectComponent!" );\
			ObjectComponentType * t = ::BH::ObjectAllocator<ObjectComponentType>::Instance().New();\
			t->mAddToSystem = &ObjectComponent::AddToAssignedSystem<ObjectComponentType>;\
			t->mRemoveFromSystem = &ObjectComponent::RemoveFromAssignedSystem<ObjectComponentType>;\
			return t;\
		}\
	}

#endif