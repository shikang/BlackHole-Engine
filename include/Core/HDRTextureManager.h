#ifndef BH_HDR_TEXTURE_MANAGER_H
#define BH_HDR_TEXTURE_MANAGER_H

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Core/GameComponent.h"
#include "Core/SystemManager.h"

#include <unordered_map>

namespace BH
{
	class HDRTextureManager : public GameComponent
	{
	public:
		// First - HDR Texture, Second - Irradiance Map
		typedef std::pair< HDRTexture *, HDRTexture * > HDRPair;
		typedef std::unordered_map< String, HDRPair > HDRTextureList;

	public:
		// Load directories
		static String LoadDirectory;

	public:
		// Constructor
		HDRTextureManager();

		// Destructor
		~HDRTextureManager();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Get mesh
		HDRPair GetTexture(const String & name);

	private:
		// Load all meshes in the directory
		void LoadAllTextures();

	private:
		HDRTextureList mTextureList;

	};
}

#include "Core/Typing.h"
DECLARE_TYPE( ::BH::HDRTextureManager, HDRTextureManager );

#endif