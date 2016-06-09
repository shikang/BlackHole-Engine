#ifndef BH_CONSOLECOLOR_H
#define BH_CONSOLECOLOR_H

// ConsoleColor.h
// http://www.codeproject.com/Articles/16431/Add-color-to-your-std-cout

#include "Platform/PlatformEssential.h"
#include "Platform/Typedefs.h"

// Change console colour to blue
BH::Ostream & ConsoleBlue(BH::Ostream &s);
BH::Ostream & ConsoleRed(BH::Ostream &s);
BH::Ostream & ConsoleGreen(BH::Ostream &s);
BH::Ostream & ConsoleYellow(BH::Ostream &s);
BH::Ostream & ConsoleWhite(BH::Ostream &s);

#endif