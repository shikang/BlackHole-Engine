#ifndef BH_SCRIPTCOMPONENT_CONFIG_H
#define BH_SCRIPTCOMPONENT_CONFIG_H

namespace BH
{
	#if defined( __WIN32__ ) || defined( _WIN32 ) || defined( __WIN64__ ) || defined( _WIN64 ) 
		/*
			If we're not including this from a client build, specify that the stuff
			should get exported. Otherwise, import it.
		*/
		#define BH_API_EXPORT			__declspec( dllexport )
		#define BH_API_IMPORT			__declspec( dllimport )

		#if defined( _LIB )
			// Linux compilers don't have symbol import/export directives.
			#define BH_API
			#define BH_PRIVATE
		#else
			#if defined( BHSCRIPTCOMPONENT_EXPORTS )
				#define BH_API		BH_API_EXPORT
			#else
				#if defined( __MINGW32__ )
					#define BH_API
				#else
					#define BH_API	BH_API_IMPORT
				#endif
			#endif

			#define BH_PRIVATE
		#endif
	#endif
}
#endif