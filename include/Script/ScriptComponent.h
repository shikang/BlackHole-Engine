#ifndef BH_SCRIPT_COMPONENT_H
#define BH_SCRIPT_COMPONENT_H

#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/TypeCreator.h"
#include "Core/ObjectComponent.h"
#include "Core/FunctionsFramework.h"
#include "Core/IUpdatable.h"
#include "Core/System.h"
#include "Core/SystemManager.h"

#include "Script/ScriptObject.h"

#undef BH_API
#include "Script/ScriptComponentConfig.h"

namespace BH
{
	class ScriptComponent;

	class BH_API ScriptType : public Entity
	{
	public:
		String mType;
		ScriptComponent * mParent;

		// Define type to be reflected
		MARK_DEFINE_TYPE( ScriptType );

		ScriptType() : mType( "[InvalidType]" ), mParent( nullptr ) {}
		ScriptType( const String & type ) : mType( type ), mParent( nullptr ) {}

		// Called just before the internal representation of the entity changes. (e.g. Before deserialisation)
		virtual OPTIONS OnPreChanged();

		// Called when the internal representation of the entity changes. (e.g. After deserialisation)
		virtual void OnChanged();

		// Called when the entity is about to be saved. (e.g. Before serialisation)
		virtual OPTIONS OnPreSaved() const;

		// Called after the entity is saved. (e.g. After serialisation)
		virtual void OnSaved() const;

		// Set pointer to changing object
		static void SetChangingObject( Object ** changingObj );

	private:
		static Object ** ChangingObject;

	};

	class BH_API ScriptComponent : public ObjectComponent, public IUpdatable, public IDrawable
	{
	public:
		// Getter and Setter (If component is already initialise, this will affect nothing)
		GETSET_AUTO( ScriptType, Script );

		// For tools use
		inline void SetScriptReflectionType( const Type * type ) { mDummyType = type; }

		// Get script variable name
		inline const Char * GetScriptVariableName() const { return mScriptObject.GetName(); }

	public:
		// Constructor
		ScriptComponent();

		// Destructor
		virtual ~ScriptComponent();

		// Set type db for script component
		static void SetTypeDB( TypeDB * types );

		// Set initialise callback for script component
		static void SetInitialiseCallback( const std::function< void( ScriptComponent * ) > & callback );

		// Set shutdown callback for script component
		static void SetShutdownCallback( const std::function< void( ScriptComponent * ) > & callback );

	public:
		// Initialise
		virtual void Initialise();

		// Update
		virtual void Update( f32 dt ) override;

		// Draw
		virtual void Draw( f32 dt ) override;

		// Shutdown
		virtual void Shutdown();

		// Called just before the internal representation of the entity changes. (e.g. Before deserialisation)
		virtual Entity::OPTIONS OnPreChanged();

		// Called when the internal representation of the entity changes. (e.g. After deserialisation)
		virtual void OnChanged();

		// Called when the entity is about to be saved. (e.g. Before serialisation)
		virtual Entity::OPTIONS OnPreSaved() const;

		// Called after the entity is saved. (e.g. After serialisation)
		virtual void OnSaved() const;

		// Get Parent object name
		String GetObjectName() const;

		// Call script function
		template< typename... Args >
		void CallScriptFunction( const String & functionName, Args... args );

	public:
		// Get default constructed script data
		static void * GetDefaultConstructedScriptData( const String & scriptType, const String & uniqueName );

		// Does this script type exist
		static bool IsScriptType( const String & scriptType );

		// Get all existing script types
		static std::vector<String> GetAllScriptTypes();

		// Set Script type
		void SetScriptType( const String & type );

		// Get the correct Object pointer
		void ResetObject();

		// Refresh Script value (If value changed)
		void RefreshScriptValue( f32 dt );
		void RefreshScriptValueByField( const Field * field );

		// Get field value from script (call FreeScriptValue if done)
		void * GetScriptValue();

		// Get value from script value (E.g. { Position, x } Get Position's x component from Transform Component)
		void * GetValueFromScriptValue( std::list< const Char * > & fieldNames, void * data );

		// Free Script Value
		void FreeScriptValue( void * data );

	private:
		// Helper function to get value from script
		void GetValuesFromScript() const;

		// Helper funcion to set value to scipt
		void SetValuesToScript() const;

		// Initialise Script
		void InitialiseScript();

		// Help get script value
		static void GetScriptValueByField( const Field * field, void * data, ScriptObject * scriptObject );

	private:
		// Befriend object component friend
		BH_OBJECT_COMPONENT_FRIENDS;

		// Define type to be reflected
		MARK_DEFINE_TYPE( ScriptComponent );

	private:
		//String mScriptType;								//!< Script Type name
		ScriptType mScript;									//!< Store Script type info
		String mScriptObjectName;							//!< Name of script object
		mutable ScriptObject mScriptObject;					//!< Script object
		mutable Char * mData;								//!< Dummy object used for reflection of script type
		const Type * mDummyType;							//!< Script type info
		bool mInitialised;									//!< Is Script initialised
		float mRefreshTimer;								//!< Time since last refresh

		friend class ScriptType;

	private:
		static TypeDB * Types;								//!< Because if ENTITY_MANAGER.Types is called from another dll, it will not work
		static std::function< void( ScriptComponent * ) > 
			InitialiseCallback;								//!< Callback function called after initialisation
		static std::function< void( ScriptComponent * ) > 
			ShutdownCallback;								//!< Callback function called after shutdown
	};
}

#include "Script/ScriptComponent.inl"

DECLARE_TYPE( ::BH::ScriptType, ScriptType );
DECLARE_TYPE( ::BH::ScriptComponent, ScriptComponent );

#endif