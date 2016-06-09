#ifndef BH_SCRIPT_LOADER_H
#define BH_SCRIPT_LOADER_H

#include "Platform/PlatformWindow.h"

#include "Core/CoreEssential.h"
#include "Core/IDrawable.h"
#include "Core/GameComponent.h"
#include "Core/TypeDB.h"

#include "Script/ScriptComponent.h"

namespace BH
{
	class ScriptComponent;
	class ScriptEventManager;

	class ScriptLoader : public GameComponent
	{
	public:
		typedef std::list< ScriptComponent * > ScriptList;						//!< List of scripts
		typedef std::unordered_map< String, ScriptComponent * > Components;		//!< Scripts sorted by component type
		typedef std::unordered_map< String, Components > Objects;				//!< Objects with script type sorted by name

	public:
		GETTER_AUTO_REF( ScriptList, ScriptList );

	public:
		// Constructor
		ScriptLoader();

		// Destructor
		~ScriptLoader();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Reinitialise (Recompile C# code)
		void ReInitialise();

	public:
		// Get script component based on object and type
		ScriptComponent * GetScriptByObjectAndType( const String & object, const String & type );

	private:
		// Load all scripts
		void LoadAllScripts();

		// Prepare script for compilation
		void PrepareScript();

		// Initialise Callback
		void ScriptInitialiseCallback( ScriptComponent * component );

		// Shutdown Callback
		void ScriptShutdownCallback( ScriptComponent * component );

	public:
		static String LoadDirectory;

	private:
		ScriptList mScriptList;
		Objects mScriptObjects;						//!< Objects containing scripts (For fast lookup)
		ScriptEventManager * mScriptEventManager;

	};

	template<>
	void * TypeDB::NewEntity<ScriptComponent>();
}

#include "Core/Typing.h"
DECLARE_TYPE( ::BH::ScriptLoader, ScriptLoader );

#endif