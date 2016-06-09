#ifndef BH_MATERIALS_H
#define BH_MATERIALS_H

#include "Platform/Vector3f.h"
#include "Platform/Shader.h"

#include "Core/CoreEssential.h"
#include "Core/Entity.h"
#include "Core/TypeCreator.h"

namespace BH
{
	// Base Material class
	class Material : public Entity
	{
	public:
		// Constructor
		Material() {}

		// Destructor
		virtual ~Material() {}

		// Called just before the internal representation of the entity changes. (e.g. Before deserialisation)
		virtual OPTIONS OnPreChanged() { return Entity::O_NORMAL; }

		// Called when the internal representation of the entity changes. (e.g. After deserialisation)
		virtual void OnChanged() {}

		// Called when the entity is about to be saved. (e.g. Before serialisation)
		virtual OPTIONS OnPreSaved() const { return Entity::O_NORMAL; }

		// Called after the entity is saved. (e.g. After serialisation)
		virtual void OnSaved() const {}

		// Send material info to GPU
		virtual void SendMaterial( Shader &, ShaderType, u32  ) const {}

	public:
		String Name;	//!< Material name

	};

	// Phong Material
	class Phong : public Material
	{
	public:
		// Constructor
		Phong() : mSpecular( 0.7f, 0.7f, 0.7f ), mRoughness( 128.0f ), mTexture( "white" ) {}
		Phong( const Vector3f & specular, f32 roughness, const String & texture ) : mSpecular( specular ), mRoughness( roughness ), mTexture( texture ) {}

		// Destructor
		~Phong() {}

		// Sending material info to GPU
		void SendMaterial( Shader & shader, ShaderType type, u32 bufferNum ) const override;

	private:
		Vector3f mSpecular;		//!< Specular factor of material
		f32 mRoughness;			//!< Roughness of material
		String mTexture;		//!< Texture for material

		friend class TypeRegistrar;
	};
}

// Declare Material Type
DECLARE_TYPE( ::BH::Material, Material );

// Declare Phong Type
DECLARE_TYPE( ::BH::Phong, Phong );

#endif