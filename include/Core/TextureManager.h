#ifndef BH_TEXTURE_MANAGER_H
#define BH_TEXTURE_MANAGER_H

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Core/GameComponent.h"
#include "Core/SystemManager.h"

#include <unordered_map>

namespace BH
{
	class TextureManager : public GameComponent
	{
	public:
		typedef std::unordered_map< String, Texture * > TextureList;

		GETTER_AUTO_CREF( TextureList, TextureList );

	public:
		// Load directories
		static String LoadDirectory;

	public:
		// Constructor
		TextureManager();

		// Destructor
		~TextureManager();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Get texture
		Texture * GetTexture( const String & name );

	private:
		// Load all textures in the directory
		void LoadAllTextures();

	private:
		TextureList mTextureList;

	};
}

#include "Core/Typing.h"
DECLARE_TYPE(::BH::TextureManager, TextureManager);

#endif