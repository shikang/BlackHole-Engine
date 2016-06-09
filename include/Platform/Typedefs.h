#ifndef BH_TYPEDEFS_H
#define BH_TYPEDEFS_H

// Disable that annoying deprecation warning from MSVC.
#pragma warning ( disable : 4996 )

// External Dependencies
#include <string>
#include <sstream>
#include <fstream>

namespace BH
{
	// Built in types
	typedef signed char				s8;				//!< 8-bit signed integer
	typedef unsigned char			u8;				//!< 8-bit unsigned integer

	typedef short					s16;			//!< 16-bit signed integer
	typedef unsigned short			u16;			//!< 16-bit unsigned integer

	typedef int						s32;			//!< 32-bit signed integer
	typedef unsigned int			u32;			//!< 32-bit unsigned integer

	typedef __int64					s64;			//!< Windows 64-bit signed integer
	typedef unsigned __int64		u64;			//!< Windows 64-bit signed integer

	typedef float					f32;			//!< 32-bit float
	typedef double					f64;			//!< 64-bit float

	typedef f32						Real;

	// String types
	typedef char					Char;			//!< Char type.
	typedef unsigned char			Uchar;			//!< Unsigned char type.
	typedef wchar_t					Wchar;			//!< Wchar type.
	typedef std::ostream			Ostream;        //!< Output stream.
	typedef std::ostringstream		Ostringstream;  //!< Output string stream.
	typedef std::ofstream			Ofstream;       //!< Output file stream.
	typedef std::istream			Istream;        //!< Input stream.
	typedef std::istringstream		Istringstream;  //!< Input string stream.
	typedef std::ifstream			Ifstream;       //!< Input file stream.
	typedef std::fstream			Fstream;		//!< I/O file stream.
	typedef std::string				String;			//!< String

}

#endif // BH_TYPEDEFS_H
