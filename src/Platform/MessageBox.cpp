// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/MessageBox.h"

#include <windows.h>

namespace BH
{
	void CreateMessageBox( const String & message )
	{
		CreateMessageBox( message, String( ) );
	}

	void CreateMessageBox( const String & message, const String & caption )
	{
		int result = ::MessageBox(nullptr, message.c_str(),
								  caption.c_str(), MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_DEFBUTTON2);

		switch (result)
		{
		case IDABORT:
			::exit(EXIT_FAILURE);
			break;

		case IDRETRY:
			::DebugBreak();
			break;

		case IDIGNORE:
			break;
		}
	}
}