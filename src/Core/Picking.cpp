// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/Picking.h"

namespace BH
{
	void GetPickingInfo( const Matrix4 & world,
						 const Matrix4 & view,
						 const Matrix4 & perspective,
						 const Vector2f & mousepos,
						 const Vector2f & screenDimension,
						 Vector3f & clickNear, Vector3f & clickFar )
	{
		Vector2f mousePos = Vector2f::Clamp( mousepos, Vector2f::ZERO, screenDimension );

		Vector3f winCoord( mousePos.x, mousePos.y, 0.0f );

		clickNear = Vector3f::Unproject( winCoord, 0, 0, screenDimension.x, screenDimension.y, 0.0f, 1.0f, perspective, view, world );

		winCoord.z = 1.0f;
		clickFar = Vector3f::Unproject( winCoord, 0, 0, screenDimension.x, screenDimension.y, 0.0f, 1.0f, perspective, view, world );
	}

	f32 GetPickingRadius( const Vector3f & scale, 
						  const Matrix4 & view )
	{
		f32 radius = 0.5f * std::max( scale.x, std::max( scale.y, scale.z ) );

		f32 x_length = Vector3f( view.m[0][0], view.m[1][0], view.m[2][0] ).Length();
		f32 y_length = Vector3f( view.m[0][1], view.m[1][1], view.m[2][1] ).Length();
		f32 z_length = Vector3f( view.m[0][2], view.m[1][2], view.m[2][2] ).Length();

		f32 l = std::max( x_length, std::max( y_length, z_length ) );
		radius *= l;

		return radius;
	}


	f32 RayOBBIntersection( const Vector3f& ray, const Vector3f& clickpoint, 
							const Vector3f& pos, const Vector3f& scale, 
							const Vector3f axis[3] )
	{
		f32 tMin = 0.0f;
		f32 tMax = FLT_MAX;

		Vector3f delta = pos - clickpoint;

		Vector3f aabb_min = -0.5f * scale;
		Vector3f aabb_max = 0.5f * scale;

		// Test intersection with the 2 planes perpendicular to the OBB's axis
		for( s32 i = 0; i < 3; ++i )
		{
			f32 e = axis[i].Dot( delta );
			f32 f = ray.Dot( axis[i] );

			if ( fabs( f ) > FLT_EPSILON )
			{ 
				// Standard case
				f32 t1 = ( e + aabb_min[i] ) / f; // Intersection with the "near" plane
				f32 t2 = ( e + aabb_max[i] ) / f; // Intersection with the "far" plane
				// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

				// We want t1 to represent the nearest intersection, 
				// so if it's not the case, invert t1 and t2
				if ( t1 > t2 )
					std::swap( t1, t2 );

				// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
				tMax = std::min( tMax, t2 );

				// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
				tMin = std::max( tMin, t1 );

				// And here's the trick :
				// If "far" is closer than "near", then there is NO intersection.
				// See the images in the tutorials for the visual explanation.
				if ( tMax < tMin )
					return -1.0f;

			}
			else
			{ 
				// Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
				if( -e + aabb_min[i] > 0.0f || -e + aabb_max[i] < 0.0f )
					return -1.0f;
			}
		}

		return tMin;
	}

	f32 RayPlaneIntersection( const Vector3f & ray, const Vector3f & clickpoint, 
							  const Vector3f & pos, 
							  const Vector3f & normal )
	{
		Vector3f d = pos - clickpoint;

		// trival rejection, parallel
		f32 norm_dot_ray = normal.Dot( ray );

		if( fabs( norm_dot_ray ) <= FLT_EPSILON )
			return 0.0f;

		f32 t = ( normal.Dot( pos ) - normal.Dot( clickpoint ) ) / norm_dot_ray;

		return t;
	}

	f32 RayPlaneEdgeIntersection( const Vector3f & ray, const Vector3f & clickpoint, 
								  const Vector3f & pos, f32 radius, 
								  const Vector3f & normal )
	{
		const f32 PICKING_THRESHOLD = 0.5f;

		f32 t = RayPlaneIntersection( ray, clickpoint, pos, normal );

		if( t <= 0.0f )
			return -1.0f;

		// points of intersection
		Vector3f i = clickpoint + t * ray;

		f32 radius_square = radius * radius;

		// If click near edge
		if( fabs( Vector3f::DistanceSquared( i, pos ) - radius_square ) <= PICKING_THRESHOLD * radius_square )
			return t;
		else
			return -1.0f;
	}

	f32 RaySphereIntersection( const Vector3f & viewpoint, const Vector3f & clickpoint, 
							   const Vector3f & center, f32 radius )
	{
		Vector3f q = clickpoint - viewpoint;

		Vector3f c_e = center - viewpoint;
		f32 c_e_sq_length = c_e.Dot( c_e );

		Vector3f c_p = center - clickpoint;

		f32 sq_radius = radius * radius;

		// trivial accpetance (Inside sphere)
		if( c_p.Dot( c_p ) < sq_radius && c_e_sq_length < sq_radius )
			return 0.0f;

		f32 m = c_e.Dot( q );

		// trivial rejection (Going opposite direction)
		if( m < 0.0f ) 
			return -1.0f;

		f32 q_length = q.Length();
		m /= q_length;

		f32 sq_n = c_e_sq_length - m * m;

		// trivial rejection (Going toward but miss the sphere)
		if( sq_n > sq_radius )
			return -1.0f;

		f32 s = sqrt( sq_radius - sq_n );

		return m - s;
	}

	Hash Pick( const std::unordered_map< Hash, PickableInstance > & pickables,
			   const Matrix4 & world,
			   const Matrix4 & view,
			   const Matrix4 & perspective,
			   const Vector2f & mousepos,
			   const Vector2f & screenDimension )
	{
		Vector3f clickNear, clickFar;
		GetPickingInfo( world, view, perspective, mousepos, screenDimension, clickNear, clickFar );
		Vector3f ray = clickFar - clickNear;
		ray.Normalize();

		f32 minTime = FLT_MAX;
		Hash minChoice = "";

		for ( const auto & i : pickables )
		{
			Matrix4 rot = Matrix4::CreateFromYawPitchRoll( i.second.mRotation.y, i.second.mRotation.x, i.second.mRotation.z );
			Matrix4 trans = Matrix4::CreateTranslation( i.second.mPosition );
			Matrix4 scale = Matrix4::CreateScale( i.second.mScale );

			Vector3f axis[3] =	{
									Vector3f( rot.m[0][0], rot.m[0][1], rot.m[0][2] ),
									Vector3f( rot.m[1][0], rot.m[1][1], rot.m[1][2] ),
									Vector3f( rot.m[2][0], rot.m[2][1], rot.m[2][2] )
								};

			trans = trans * scale;

			Vector3f n_min = trans * i.second.mAABB.mMin;
			Vector3f n_max = trans * i.second.mAABB.mMax;
			Vector3f n_scale = n_max - n_min;
			Vector3f n_pos = n_min + 0.5f * n_scale;

			Matrix4 trans_o = Matrix4::CreateTranslation( n_pos - i.second.mPosition );

			trans = Matrix4::CreateTranslation( i.second.mPosition );
			scale = Matrix4::CreateScale( n_scale );

			trans = trans * rot * trans_o * scale;

			//n_pos = Vector3f( trans.m[0][3], trans.m[1][3], trans.m[2][3] );
			n_pos = Vector3f( trans.m[3][0], trans.m[3][1], trans.m[3][2] );

			f32 t = RayOBBIntersection( ray, clickNear, n_pos, n_scale, axis );
			if ( t >= 0.0f )
			{
				if ( minChoice == "" || t < minTime )
				{
					minTime = t;
					minChoice = i.first;
				}
			}
		}

		return minChoice;
	}
}