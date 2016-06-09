#ifndef BH_TYPE_REGISTRAR_H
#define BH_TYPE_REGISTRAR_H

#include "Core/CoreEssential.h"

#include "Platform/FileInfo.h"
#include "Platform/Vector2f.h"
#include "Platform/Vector3f.h"
#include "Platform/Vector4f.h"
#include "Platform/Matrix4.h"

// Platform types
DECLARE_TYPE(::BH::FileInfo, FileInfo);
DECLARE_TYPE(::BH::Vector2f, Vector2f);
DECLARE_TYPE(::BH::Vector3f, Vector3f);
DECLARE_TYPE(::BH::Vector4f, Vector4f);

namespace BH
{
	// In charge of auto registering of types for reflection
	class TypeRegistrar
	{
	public:
		// Register types in the Entity Manager.
		static void RegisterTypes();

	private:
		// Constructor.
		TypeRegistrar();

	private:
		static TypeRegistrar sInstance;	//!< Static instance to handle automatic registration.

	};
}

#endif // BH_TYPE_REGISTRAR_H