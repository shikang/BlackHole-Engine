#ifndef BH_SCRIPT_OBJECT_H
#define BH_SCRIPT_OBJECT_H

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Script/ScriptConfig.h"
#include "Script/ScriptMethod.h"

namespace BH
{
	// Script object
	class BH_API ScriptObject
	{
	public:
		// Getters
		GETTER_AUTO_CPTR( Char, Name );	//!< Get Name
		GETTER_AUTO_CPTR( Char, Type );	//!< Get Type

		static const Char * InvalidTypeName;
		static const Char * InvalidObjectName;

	public:
		// Call method
		void CallMethod( const Char * method );

		// Call method with params
		template< typename... Args >
		void CallMethod( const Char * method, Args... args );

		// Set value
		template< typename T >
		void SetValue( const Char * name, T val );

		// Get value
		template< typename T >
		T GetValue( const Char * name );

	private:
		// Helper functor to set data of any parameters
		template< typename First, typename... Rest >
		class AddParams
		{
		public:
			void operator()( First f, Rest... r )
			{
				AddParams<First>()( f );
				AddParams<Rest...>()( r... );
			}
		};

		template< typename First >
		class AddParams<First>
		{
		public:
			void operator()( First f )
			{
				detail::ScriptMethod::PushParamsToStack( f );
			}
		};

	public:
		// Not for use!
		inline void InvalidateObject() { mName = InvalidObjectName; mType = InvalidTypeName; }

	public:
		// Constructor
		ScriptObject( const Char * name = "[InvalidName]", const Char * type = "[InvalidType]" );

	private:
		const Char * mName;		//!< Variable name
		const Char * mType;		//!< Type name from scripts

	};
}

#include "Script/ScriptObject.inl"

#endif