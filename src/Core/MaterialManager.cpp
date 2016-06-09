// Precompiled Header
#include "Core/Stdafx.h"

#include "Platform/FileSystem.h"

#include "Core/MaterialManager.h"
#include "Core/XMLDeserialiser.h"
#include "Core/XMLSerialiser.h"
#include "Core/Materials.h"

#include <utility>

namespace BH
{
	String MaterialManager::LoadFileName = "Material.xml";
	const Char * MaterialManager::DefaultMaterial = "Plastic";

	MaterialManager::MaterialManager()
	{
	}

	MaterialManager::~MaterialManager()
	{
	}

	void MaterialManager::Initialise()
	{
		LOG("Initialising Material Manager...");
		LoadAllMaterials();

		MaterialList::iterator it = mMaterialList.find( DefaultMaterial );
		if( it == mMaterialList.end() )
		{
			mMaterialList[DefaultMaterial] = ENTITY_MANAGER.New<Phong>();
			mMaterialList[DefaultMaterial]->Name = DefaultMaterial;
		}
		LOG("Material Manager Initialised");
	}

	void MaterialManager::Shutdown()
	{
		//SaveAllMaterials();
	}

	Material * MaterialManager::GetMaterial( const String & name )
	{
		MaterialList::iterator it = mMaterialList.find( name );
		return ( it == mMaterialList.end() ) ? nullptr : it->second;
	}

	void MaterialManager::SetMaterial( const String & name, Material * material )
	{
		if ( !material )
			return;

		MaterialList::iterator it = mMaterialList.find( name );
		if ( it == mMaterialList.end() )
		{
			material->Name = name;
			mMaterialList[name] = material;
		}
	}

	void MaterialManager::RemoveMaterial( const String & name, bool del )
	{
		MaterialList::iterator it = mMaterialList.find( name );
		if ( it != mMaterialList.end() )
		{
			if ( del )
				ENTITY_MANAGER.Delete( static_cast<EntityPtr>( it->second ) );
			mMaterialList.erase( it );
		}
	}

	void MaterialManager::LoadAllMaterials()
	{
		if( !FileSystem::FileCheckExists( LoadFileName.c_str() ) )
			return;

		XMLDeserialiser deserialiser;
		deserialiser.TraverseStream( LoadFileName.c_str() );

		// Materials is just the base class. I need to know
		while ( !deserialiser.IsStreamEnded() )
		{
			Material * m = nullptr;
			deserialiser.DeserialisePtr<Material>( m );
			
			// Null
			if ( !m )
				continue;

			// Material exist
			if ( mMaterialList.find( m->Name ) != mMaterialList.end() )
			{
				ENTITY_MANAGER.Delete( static_cast<EntityPtr>( m ) );
			}
			else
			{
				mMaterialList[m->Name] = m;
			}
		}
	}

	void MaterialManager::SaveAllMaterials()
	{
		if ( FileSystem::FileCheckExists( LoadFileName.c_str() ) )
			FileSystem::FileDelete( "", LoadFileName.c_str() );

		XMLSerialiser serialiser;
		serialiser.TraverseStream( LoadFileName.c_str() );

		for ( const auto & i : mMaterialList )
			serialiser.Serialise<Material>( *i.second );
	}

	void MaterialManager::AssignAvailableMaterialName( String & name ) const
	{
		MaterialList::const_iterator findit = mMaterialList.find( name );
		// If no available name found
		if ( findit == mMaterialList.end() && ( name.length() > 0 ) )
			return;

		String origName = ( name.length() > 0 ) ? name : "NewMaterial";
		s32 postfix = 0;

		// If there are numbers at the back
		if ( origName.back() >= '0' && origName.back() <= '9' )
		{
			// Points at last character
			const Char * endPt = origName.c_str() + origName.length() - 1;

			// Get the point where the object's number starts
			while ( endPt >= origName.c_str() - 1 )	// While end pt does does point to one before the first
			{
				// If endpoint is numeric, move back
				if ( endPt >= origName.c_str() && *endPt >= '0' && *endPt <= '9' )
				{
					--endPt;
				}
				// Finally reach a non numeric character
				else
				{
					std::istringstream ss( endPt + 1 );
					ss >> postfix;
					++postfix;
					origName = String( origName.c_str(), endPt + 1 - origName.c_str() );
					break;
				}
			}
		}

		// Make sure new name does not have duplicates
		while ( findit != mMaterialList.end() )
		{
			std::stringstream newName;
			newName << origName << postfix++;
			name = newName.str().c_str();
			findit = mMaterialList.find( name );
		}
	}
}