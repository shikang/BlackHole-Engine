#ifndef XML_SERIALISE_TYPE_H
#define XML_SERIALISE_TYPE_H

#include "Dependencies/tinyxml2.h"

#pragma comment (lib, "../Lib/tinyxml2.lib")

namespace BH
{
	typedef tinyxml2::XMLDocument	SerialiseDoc;
	typedef tinyxml2::XMLElement	SerialiseElement;
}

#endif