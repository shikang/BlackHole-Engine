#ifndef BH_PICKING_H
#define BH_PICKING_H

#include "Core/CoreEssential.h"

#include "Platform/Vector3f.h"
#include "Platform/AABB.h"
#include "Platform/Matrix4.h"
#include "Platform/Vector2f.h"

namespace BH
{
	struct PickableInstance
	{
		Vector3f mPosition;		//!< Position
		Vector3f mScale;		//!< Scale
		Vector3f mRotation;		//!< Rotation (in radian)
		AABB mAABB;				//!< Untransformed AABB
	};

	// Choose picked
	Hash Pick( const std::unordered_map< Hash, PickableInstance > & pickables,	//!< Pickables
			   const Matrix4 & world,											//!< From model to world
			   const Matrix4 & view,											//!< From world to view
			   const Matrix4 & perspective,										//!< From view to perspective
			   const Vector2f & mousepos,										//!< Mouse click position
			   const Vector2f & screenDimension );								//!< Window dimension

	// Get click point on near and far plane
	void GetPickingInfo( const Matrix4 & world,									//!< From model to world
						 const Matrix4 & view,									//!< From world to view
						 const Matrix4 & perspective,							//!< From view to perspective
						 const Vector2f & mousepos,								//!< Mouse click position
						 const Vector2f & screenDimension,						//!< Window dimension
						 Vector3f & clickNear, Vector3f & clickFar );

	// Get sphere radius in view frame (For picking in view)
	f32 GetPickingRadius( const Vector3f & scale, 
						  const Matrix4 & view );

	// Time of intersection
	f32 RayOBBIntersection( const Vector3f& ray, const Vector3f& clickpoint, 
							const Vector3f& pos, const Vector3f& scale, 
							const Vector3f axis[3] );

	f32 RayPlaneIntersection( const Vector3f & ray, const Vector3f & clickpoint, 
							  const Vector3f & pos, 
							  const Vector3f & normal );

	f32 RayPlaneEdgeIntersection( const Vector3f & ray, const Vector3f & clickpoint, 
							      const Vector3f & pos, f32 radius, 
							      const Vector3f & normal );

	f32 RaySphereIntersection( const Vector3f & viewpoint, const Vector3f & clickpoint, 
							   const Vector3f & center, f32 radius );
}

#endif