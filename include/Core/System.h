#ifndef BH_SYSTEM_H
#define BH_SYSTEM_H

#include "Core/CoreEssential.h"
#include "Core/CName.h"
#include "Core/Assert.h"
#include "Core/Message.h"
#include "Core/JobQueue.h"
#include "Core/IUpdatable.h"
#include "Core/IDrawable.h"
#include "Core/GameComponent.h"
#include "Core/ObjectComponent.h"
#include "Core/SystemMessaging.h"
#include "Core/EntityManager.h"
#include "Core/Type.h"
#include "Core/Game.h"

#include <type_traits>
#include <unordered_map>
#include <tuple>

namespace BH
{
	// Forward Declarations
	template < typename T, bool UPDATABLE >
	struct AddUpdatableImpl;

	template < typename T, bool UPDATABLE >
	struct RemoveUpdatableImpl;

	template < typename T, bool DRAWABLE >
	struct AddDrawableImpl;

	template < typename T, bool DRAWABLE >
	struct RemoveDrawableImpl;

	template < typename T, bool GAMECOMPONENT >
	struct SortUpdatableImpl;

	template < typename T, bool GAMECOMPONENT >
	struct SortDrawableImpl;

	template < typename T, bool OBJCOMPONENT >
	struct FastInsertUpdatableImpl;

	template < typename T, bool OBJCOMPONENT >
	struct FastInsertDrawableImpl;

	// Interface class for system
	class System
	{
	public:
		GETTER_AUTO(bool, LastFlag);

	public:
		// Constructor
		System( const BH::Char * name );

		// Destructor
		virtual ~System();

		// Stop system
		void StopSystem();

		// Add Game Component to this system
		template< typename T >
		T * AddGameComponent( const Char * name );

		// Get Game Component
		template< typename T >
		T * GetGameComponent( const Char * name );

		// Add component to the system
		template< typename T >
		void AddObjectComponent( T * component );

		// Check for specific component exixt in the object within this system
		template< typename T >
		bool ObjectComponentExist( ObjectID id );

		// Remove Object Component
		template< typename T >
		void RemoveObjectComponent( T * component );

		// Set dependency of game component (dependee depends on dependent)
		void Dependency(const Char * dependee, const Char * dependent);

		// Set enabled status (enable to update)
		void SetEnabled( IUpdatable * updatable, bool enable );

		// Set visible status
		void SetVisible( IDrawable * drawable, bool visible );

		// Check if system is jobless or idling
		bool IsIdling() const;

		// Pause all object component
		void PauseObjectComponents();

		// Resume all object component
		void ResumeObjectComponents();

		// Hide all object component
		void HideObjectComponents();

		// Show all object component
		void ShowObjectComponents();

	public:
		// Send a job to queue for work (Deep copy of arguments will occur)
		template< typename ReturnType, typename... Arguments >
		void AddJob( u32 priority, const Delegate< ReturnType( Arguments... ) > & delegate, Arguments... args );

		// Update all active updatables (current flag is something like bit flipping - to know is component updated in this frame)
		void UpdateUpdatables( f32 dt, bool current_flag );

		// Request something from object component (Blocking function if ComponentType derived from IUpdatable)
		template< typename... Arguments, typename ComponentType, typename ReturnType >
		ReturnType RequestJob( ObjectID id, ReturnType( ComponentType::*MemFun )( Arguments... ) const, Arguments... args );

		// Request something from object component (Blocking function if ComponentType derived from IUpdatable)
		template< typename... Arguments, typename ComponentType, typename ReturnType >
		ReturnType RequestJob( ObjectID id, ReturnType( ComponentType::*MemFun )( Arguments... ), Arguments... args );

		// Draw all visible drawables
		void DrawDrawables( f32 dt );

	protected:
		// Initialise system
		virtual void Initialise();

		// Work on jobs at hand
		virtual void Work();

		// Shutdown system
		virtual void Shutdown();

	private:
		// Process game components (Get the in order initialise and update list)
		void ProcessGameComponent();

		// Update delegate for job queue
		static void UpdateDelegate( IUpdatable * updatable, f32 dt, bool current_flag );

		// Accquire to see if updatable is ready
		template< typename T >
		void UpdatableAcquire( T * component );

		// If type is updatable, added to update list
		template< typename T >
		void AddUpdatable( T * t );

		// If type is updatable, remove from update list
		template< typename T >
		void RemoveUpdatable( T * t );

		// Remove inactive updatables from run list
		void RemoveInActive();

		// Insert active updatables to run list
		void InsertActive( bool flip );

		// If type is drawable, added to draw list
		template< typename T >
		void AddDrawable( T * t );

		// If type is drawable, remove from draw list
		template< typename T >
		void RemoveDrawable( T * t );

		// Remove invisible drawables from draw list
		void RemoveInVisible();

		// Insert visible drawables to draw list
		void InsertVisible();

		// Sort Update List
		inline void SortUpdateList();

		// Sort Drawable List
		inline void SortDrawableList();

		// Fast insert in Update Insert List
		inline void FastInsertUpdateInsertList( IUpdatable * updatable );

		// Fast insert in Drawable Insert List
		inline void FastInsertDrawableInsertList( IDrawable * drawable );

	private:
		// Meta templating
		template < typename T, bool UPDATABLE >
		friend struct AddUpdatableImpl;

		template < typename T, bool UPDATABLE >
		friend struct RemoveUpdatableImpl;

		template < typename T, bool DRAWABLE >
		friend struct AddDrawableImpl;

		template < typename T, bool DRAWABLE >
		friend struct RemoveDrawableImpl;

		template < typename T, bool GAMECOMPONENT >
		friend struct SortUpdatableImpl;

		template < typename T, bool GAMECOMPONENT >
		friend struct SortDrawableImpl;

		template < typename T, bool OBJCOMPONENT >
		friend struct FastInsertUpdatableImpl;

		template < typename T, bool OBJCOMPONENT >
		friend struct FastInsertDrawableImpl;

	protected:
		// Types
		typedef std::unordered_map< CName, GameComponent * > GameComponentMap;
		typedef std::unordered_map< CName, u32 > GameComponentProcessMap;
		typedef std::vector< CName > GameComponentDependeeList;
		typedef std::unordered_map< CName, GameComponentDependeeList > GameComponentDependentMap;
		typedef std::vector< GameComponent * > GameComponentInitialiseList;

		typedef std::unordered_map< CName, std::vector< ObjectComponent * > > ObjectComponentMap;
		typedef std::unordered_map< ObjectID, ObjectComponentMap > ObjectMap;

		typedef std::vector< IUpdatable * > UpdateList;
		typedef std::vector< IDrawable * > DrawList;

	protected:
		GameComponentMap mGameComponentMap;							//!< Game Component Map
		GameComponentProcessMap mGameComponentProcessMap;			//!< Game Components For processing
		GameComponentDependentMap mGameComponentDependentMap;		//!< Game Component Dependent Map
		GameComponentInitialiseList mGameComponentInitialiseList;	//!< Game Component Initialise Map (In order)

		ObjectMap mObjectMap;										//!< Object Map

		JobQueue mJobQueue;											//!< Job queue for system to work on
		bool mLastFlag;												//!< Previous flag for update
		bool mStop;													//!< Flag to stop system

		UpdateList mUpdatableList;									//!< List of all updatables
		UpdateList mUpdatableRunList;								//!< List of all updatables that is updating
		UpdateList mUpdatableInsertList;							//!< List of all updatables that just start updating
		UpdateList mUpdatableRemoveList;							//!< List of all updatables that just stop updating

		DrawList mDrawableList;										//!< List of all drawables
		DrawList mDrawableRunList;									//!< List of all drawables that is drawing
		DrawList mDrawableInsertList;								//!< List of all drawables that just start drawing
		DrawList mDrawableRemoveList;								//!< List of all drawables that just stop drawing

		UpdateList mPausingList;									//!< Currently paused updatable
		DrawList mHidingList;										//!< Currently hiding drawable

		friend class SystemManager;

		const BH::Char * mSystemName;								//!< For debugging
	};
}

#include "Core/System.inl"

#define SYSTEM_FRIENDS\
	friend class Game

#endif