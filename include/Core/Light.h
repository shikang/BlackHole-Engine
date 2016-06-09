#ifndef BH_LIGHT_H
#define BH_LIGHT_H

#include "Platform/Vector2f.h"
#include "Platform/Vector3f.h"
#include "Platform/Vector4f.h"

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"

namespace BH
{
	class Light
	{
	public:
		GETSET_AUTO( Vector3f, Position );
		GETSET_AUTO( Vector4f, Color );

	public:
		// Constructor
		Light( const Vector3f & position, const Vector4f & color ) 
			: mPosition( position ), mColor( color ) {}

		// Destructor
		~Light() {};

	private:
		Vector3f mPosition;
		Vector4f mColor;

	};

	class DirectionalLight
	{
	public:
		GETSET_AUTO( Vector3f, Position );
		GETSET_AUTO( Vector4f, Color );
		GETSET_AUTO( Vector3f, Direction );
		GETSET_AUTO( Vector3f, Dimension );

	public:
		// Constructor
		DirectionalLight( const Vector3f & position, const Vector4f & color, 
						  const Vector3f & direction, const Vector3f & dimension ) 
			: mPosition( position ), mColor( color ), mDirection( direction )
			, mDimension( dimension ) { mDirection.Normalize(); }

		// Destructor
		~DirectionalLight() {};

	private:
		Vector3f mPosition;
		Vector3f mDirection;
		Vector3f mDimension;
		Vector4f mColor;
	};

	class LocalLight
	{
	public:
		GETSET_AUTO( Vector3f, Position );
		GETSET_AUTO( Vector4f, Color );
		GETSET_AUTO( f32, Radius );

	public:
		// Constructor
		LocalLight( const Vector3f & position, const Vector4f & color, f32 radius ) 
			: mPosition( position ), mColor( color ), mRadius( radius ) {}

		// Destructor
		~LocalLight() {};

	private:
		Vector3f mPosition;
		Vector4f mColor;
		f32 mRadius;

	};
}

#endif