#ifndef BH_MESSAGEBOX_H
#define BH_MESSAGEBOX_h

#include "Platform/Typedefs.h"
namespace BH
{
	void CreateMessageBox( const String & message );
	void CreateMessageBox( const String & message, const String & caption );
}

#endif