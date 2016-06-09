#ifndef BH_MATERIAL_MANAGER_H
#define BH_MATERIAL_MANAGER_H

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Core/GameComponent.h"
#include "Core/SystemManager.h"

#include "Core/Materials.h"

#include <unordered_map>

namespace BH
{
	class MaterialManager : public GameComponent
	{
	public:
		typedef std::unordered_map< String, Material * > MaterialList;

		GETTER_AUTO_CREF( MaterialList, MaterialList );

	public:
		// Load directories
		static String LoadFileName;
		static const Char * DefaultMaterial;

	public:
		// Constructor
		MaterialManager();

		// Destructor
		~MaterialManager();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Get material
		Material * GetMaterial( const String & name );

		// Set material
		void SetMaterial( const String & name, Material * material );

		// Remove material
		void RemoveMaterial( const String & name, bool del = true );

		// Get available material name
		void AssignAvailableMaterialName( String & name ) const;

	private:
		// Load all material in the directory
		void LoadAllMaterials();

	public:
		// Save all material in the directory
		void SaveAllMaterials();

	private:
		MaterialList mMaterialList;

	};
}

#include "Core/Typing.h"
DECLARE_TYPE(::BH::MaterialManager, MaterialManager);

#endif