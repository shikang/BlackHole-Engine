// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/Assert.h"

#include "Platform/MessageBox.h"

namespace BH
{

	void BH::Assert(Char * const expression,
					const Char * const message,
					const Char * const filename,
					const size_t line)
	{
		String file(filename);
		Ostringstream msg;

		msg << "Expression: " << expression << std::endl
			<< "File: " << file.substr(file.find_last_of('\\') + 1) << ", Line: " << line
			<< std::endl << std::endl
			<< "Message: " << std::endl << message << std::endl;

		CreateMessageBox( msg.str( ).c_str( ), "Assertion Error" );
	}

}