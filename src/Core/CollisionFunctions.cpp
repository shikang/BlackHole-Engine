// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/CollisionFunctions.h"

#include "Platform/Matrix4.h"
#include "Platform/Vector3f.h"

#include <cmath>

namespace BH
{
	/*
	Edge Number
	----------------

	Right:
			1	
		 -------
	8	|		|	9
		|		|
		 -------
			7

	Left:
			3	
		 -------
	11	|		|  10
		|		|
		 -------
			5

	Top:
			2	
		 -------
	3	|		|	1
		|		|
		 -------
			0

	Bottom:
			6	
		 -------
	5	|		|	7
		|		|
		 -------
			4

	Front:
			0	
		 -------
	11	|		|	8
		|		|
		 -------
			4

	Back:
			2	
		 -------
	10	|		|	9
		|		|
		 -------
			6
	*/

	struct ClipVertex
	{
		ClipVertex()
		{
			//f.mKey = -1;
		}

		Vector3f v;
		//FeaturePair f;
	};

	void ComputeIncidentFace( const Matrix4 & iRot, const Vector3f & iPos, const Vector3f & iHalfWidth, 
							  Vector3f n, ClipVertex out[4] )
	{
		// Transform back to euclidean axis
		n = -( iRot.Transpose() * n );
		Vector3f absN( std::abs( n.x ), std::abs( n.y ), std::abs( n.z ) );

		if ( absN.x > absN.y && absN.x > absN.z )
		{
			// Base x axis
			if ( n.x > 0.0f )
			{
				out[0].v = Vector3f( iHalfWidth.x,  iHalfWidth.y, -iHalfWidth.z );
				out[1].v = Vector3f( iHalfWidth.x,  iHalfWidth.y,  iHalfWidth.z );
				out[2].v = Vector3f( iHalfWidth.x, -iHalfWidth.y,  iHalfWidth.z );
				out[3].v = Vector3f( iHalfWidth.x, -iHalfWidth.y, -iHalfWidth.z );

				//out[0].f.mInI = 9;
				//out[0].f.mOutI = 1;
				//out[1].f.mInI = 1;
				//out[1].f.mOutI = 8;
				//out[2].f.mInI = 8;
				//out[2].f.mOutI = 7;
				//out[3].f.mInI = 7;
				//out[3].f.mOutI = 9;
			}
			else
			{
				out[0].v = Vector3f( -iHalfWidth.x, -iHalfWidth.y,  iHalfWidth.z );
				out[1].v = Vector3f( -iHalfWidth.x,  iHalfWidth.y,  iHalfWidth.z );
				out[2].v = Vector3f( -iHalfWidth.x,  iHalfWidth.y, -iHalfWidth.z );
				out[3].v = Vector3f( -iHalfWidth.x, -iHalfWidth.y, -iHalfWidth.z );

				//out[0].f.mInI = 5;
				//out[0].f.mOutI = 11;
				//out[1].f.mInI = 11;
				//out[1].f.mOutI = 3;
				//out[2].f.mInI = 3;
				//out[2].f.mOutI = 10;
				//out[3].f.mInI = 10;
				//out[3].f.mOutI = 5;
			}
		}

		else if ( absN.y > absN.x && absN.y > absN.z )
		{
			if ( n.y > 0.0f )
			{
				out[0].v = Vector3f( -iHalfWidth.x,  iHalfWidth.y,  iHalfWidth.z );
				out[1].v = Vector3f(  iHalfWidth.x,  iHalfWidth.y,  iHalfWidth.z );
				out[2].v = Vector3f(  iHalfWidth.x,  iHalfWidth.y, -iHalfWidth.z );
				out[3].v = Vector3f( -iHalfWidth.x,  iHalfWidth.y, -iHalfWidth.z );

				//out[0].f.mInI = 3;
				//out[0].f.mOutI = 0;
				//out[1].f.mInI = 0;
				//out[1].f.mOutI = 1;
				//out[2].f.mInI = 1;
				//out[2].f.mOutI = 2;
				//out[3].f.mInI = 2;
				//out[3].f.mOutI = 3;
			}
			else
			{
				out[0].v = Vector3f(  iHalfWidth.x, -iHalfWidth.y,  iHalfWidth.z );
				out[1].v = Vector3f( -iHalfWidth.x, -iHalfWidth.y,  iHalfWidth.z );
				out[2].v = Vector3f( -iHalfWidth.x, -iHalfWidth.y, -iHalfWidth.z );
				out[3].v = Vector3f(  iHalfWidth.x, -iHalfWidth.y, -iHalfWidth.z );

				//out[0].f.mInI = 7;
				//out[0].f.mOutI = 4;
				//out[1].f.mInI = 4;
				//out[1].f.mOutI = 5;
				//out[2].f.mInI = 5;
				//out[2].f.mOutI = 6;
				//out[3].f.mInI = 5;
				//out[3].f.mOutI = 6;
			}
		}

		else
		{
			if ( n.z > 0.0f )
			{
				out[0].v = Vector3f( -iHalfWidth.x,  iHalfWidth.y,  iHalfWidth.z );
				out[1].v = Vector3f( -iHalfWidth.x, -iHalfWidth.y,  iHalfWidth.z );
				out[2].v = Vector3f(  iHalfWidth.x, -iHalfWidth.y,  iHalfWidth.z );
				out[3].v = Vector3f(  iHalfWidth.x,  iHalfWidth.y,  iHalfWidth.z );

				//out[0].f.mInI = 0;
				//out[0].f.mOutI = 11;
				//out[1].f.mInI = 11;
				//out[1].f.mOutI = 4;
				//out[2].f.mInI = 4;
				//out[2].f.mOutI = 8;
				//out[3].f.mInI = 8;
				//out[3].f.mOutI = 0;
			}

			else
			{
				out[0].v = Vector3f(  iHalfWidth.x, -iHalfWidth.y, -iHalfWidth.z );
				out[1].v = Vector3f( -iHalfWidth.x, -iHalfWidth.y, -iHalfWidth.z );
				out[2].v = Vector3f( -iHalfWidth.x,  iHalfWidth.y, -iHalfWidth.z );
				out[3].v = Vector3f(  iHalfWidth.x,  iHalfWidth.y, -iHalfWidth.z );

				//out[0].f.mInI = 9;
				//out[0].f.mOutI = 6;
				//out[1].f.mInI = 6;
				//out[1].f.mOutI = 10;
				//out[2].f.mInI = 10;
				//out[2].f.mOutI = 2;
				//out[3].f.mInI = 2;
				//out[3].f.mOutI = 9;
			}
		}

		for ( s32 i = 0; i < 4; ++i )
		{
			out[i].v = ( iRot * out[i].v ) + iPos;
		}
	}

	void ComputeReferenceEdgesAndBasis( const Vector3f & rE, const Matrix4 & rRot, 
										Vector3f n, s32 axis, u8 out[4], Matrix4 & basis, Vector3f & e )
	{
		// Transform back to euclidean axis
		n = rRot.Transpose() * n;

		if ( axis >= 3 )
			axis -= 3;

		switch ( axis )
		{
		case 0:
			if ( n.x > 0.0f )
			{
				out[0] = 1;
				out[1] = 8;
				out[2] = 7;
				out[3] = 9;

				e = Vector3f( rE.y, rE.z, rE.x );

				basis.m[0][0] = rRot.m[1][0]; basis.m[0][1] = rRot.m[1][1]; basis.m[0][2] = rRot.m[1][2];
				basis.m[1][0] = rRot.m[2][0]; basis.m[1][1] = rRot.m[2][1]; basis.m[1][2] = rRot.m[2][2];
				basis.m[2][0] = rRot.m[0][0]; basis.m[2][1] = rRot.m[0][1]; basis.m[2][2] = rRot.m[0][2];
			}
			else
			{
				out[0] = 11;
				out[1] = 3;
				out[2] = 10;
				out[3] = 5;

				e = Vector3f( rE.z, rE.y, rE.x );

				basis.m[0][0] = rRot.m[2][0]; basis.m[0][1] = rRot.m[2][1]; basis.m[0][2] = rRot.m[2][2];
				basis.m[1][0] = rRot.m[1][0]; basis.m[1][1] = rRot.m[1][1]; basis.m[1][2] = rRot.m[1][2];
				basis.m[2][0] = -rRot.m[0][0]; basis.m[2][1] = -rRot.m[0][1]; basis.m[2][2] = -rRot.m[0][2];
			}
			break;

		case 1:
			if ( n.y > 0.0f )
			{
				out[0] = 0;
				out[1] = 1;
				out[2] = 2;
				out[3] = 3;

				e = Vector3f( rE.z, rE.x, rE.y );

				basis.m[0][0] = rRot.m[2][0]; basis.m[0][1] = rRot.m[2][1]; basis.m[0][2] = rRot.m[2][2];
				basis.m[1][0] = rRot.m[0][0]; basis.m[1][1] = rRot.m[0][1]; basis.m[1][2] = rRot.m[0][2];
				basis.m[2][0] = rRot.m[1][0]; basis.m[2][1] = rRot.m[1][1]; basis.m[2][2] = rRot.m[1][2];
			}
			else
			{
				out[0] = 4;
				out[1] = 5;
				out[2] = 6;
				out[3] = 7;

				e = Vector3f( rE.z, rE.x, rE.y );

				basis.m[0][0] = rRot.m[2][0]; basis.m[0][1] = rRot.m[2][1]; basis.m[0][2] = rRot.m[2][2];
				basis.m[1][0] = -rRot.m[0][0]; basis.m[1][1] = -rRot.m[0][1]; basis.m[1][2] = -rRot.m[0][2];
				basis.m[2][0] = -rRot.m[1][0]; basis.m[2][1] = -rRot.m[1][1]; basis.m[2][2] = -rRot.m[1][2];
			}
			break;

		case 2:
			if ( n.z > 0.0f )
			{
				out[0] = 11;
				out[1] = 4;
				out[2] = 8;
				out[3] = 0;

				e = Vector3f( rE.y, rE.x, rE.z );

				basis.m[0][0] = -rRot.m[1][0]; basis.m[0][1] = -rRot.m[1][1]; basis.m[0][2] = -rRot.m[1][2];
				basis.m[1][0] = rRot.m[0][0]; basis.m[1][1] = rRot.m[0][1]; basis.m[1][2] = rRot.m[0][2];
				basis.m[2][0] = rRot.m[2][0]; basis.m[2][1] = rRot.m[2][1]; basis.m[2][2] = rRot.m[2][2];
			}
			else
			{
				out[0] = 6;
				out[1] = 10;
				out[2] = 2;
				out[3] = 9;

				e = Vector3f( rE.y, rE.x, rE.z );

				//basis->SetRows( -rtx.rotation.ey, -rtx.rotation.ex, -rtx.rotation.ez );
				basis.m[0][0] = -rRot.m[1][0]; basis.m[0][1] = -rRot.m[1][1]; basis.m[0][2] = -rRot.m[1][2];
				basis.m[1][0] = -rRot.m[0][0]; basis.m[1][1] = -rRot.m[0][1]; basis.m[1][2] = -rRot.m[0][2];
				basis.m[2][0] = rRot.m[2][0]; basis.m[2][1] = rRot.m[2][1]; basis.m[2][2] = rRot.m[2][2];
			}
			break;
		}
	}

	#define InFront( a ) \
		( ( a ) < 0.0f )

	#define Behind( a ) \
		( ( a ) >= 0.0f )

	#define On( a ) \
		( ( a ) < 0.005f && ( a ) > -0.005f )

	s32 Orthographic( f32 sign, f32 e, s32 axis, s32 /*clipEdge*/, ClipVertex * in, s32 inCount, ClipVertex * out )
	{
		s32 outCount = 0;
		ClipVertex a = in[inCount - 1];

		for ( s32 i = 0; i < inCount; ++i )
		{
			ClipVertex b = in[i];

			f32 da = sign * a.v[axis] - e;
			f32 db = sign * b.v[axis] - e;

			ClipVertex cv;

			// B
			if ( ( ( InFront( da ) && InFront( db ) ) || On( da ) || On( db ) ) )
			{
				BH_ASSERT( outCount < 8, " outCount >= 8" );
				out[outCount++] = b;
			}

			// I
			else if ( InFront( da ) && Behind( db ) )
			{
				//cv.f = b.f;
				cv.v = a.v + ( b.v - a.v ) * ( da / ( da - db ) );
				//cv.f.mOutR = static_cast< u8 >( clipEdge );
				//cv.f.mOutI = 0;
				BH_ASSERT( outCount < 8, " outCount >= 8" );
				out[outCount++] = cv;
			}

			// I, B
			else if ( Behind( da ) && InFront( db ) )
			{
				//cv.f = a.f;
				cv.v = a.v + ( b.v - a.v ) * ( da / ( da - db ) );
				//cv.f.mInR = static_cast< u8 >( clipEdge );
				//cv.f.mInI = 0;
				BH_ASSERT( outCount < 8, " outCount >= 8" );
				out[outCount++] = cv;

				BH_ASSERT( outCount < 8, " outCount >= 8" );
				out[outCount++] = b;
			}

			a = b;
		}

		return outCount;
	}

	// Sutherland Hodgman Clipping, return number of points behind reference face
	s32 Clip( const Vector3f & rPos, const Vector3f & e, u8 clipEdges[4], const Matrix4 & basis, ClipVertex incident[4], ClipVertex outVerts[8], f32 outDepths[8] )
	{
		s32 inCount = 4;
		s32 outCount;
		ClipVertex in[8];
		ClipVertex out[8];

		for ( s32 i = 0; i < 4; ++i )
		{
			in[i].v = basis.Transpose() * ( incident[i].v - rPos );
		}

		outCount = Orthographic( 1.0f, e.x, 0, clipEdges[0], in, inCount, out );

		if ( !outCount )
			return 0;

		inCount = Orthographic( 1.0f, e.y, 1, clipEdges[1], out, outCount, in );

		if ( !inCount )
			return 0;

		outCount = Orthographic( -1.0f, e.x, 0, clipEdges[2], in, inCount, out );

		if ( !outCount )
			return 0;

		inCount = Orthographic( -1.0f, e.y, 1, clipEdges[3], out, outCount, in );

		// Keep incident vertices behind the reference face
		outCount = 0;
		for ( s32 i = 0; i < inCount; ++i )
		{
			f32 d = in[i].v.z - e.z;

			if ( d <= f32( 0.0 ) )
			{
				outVerts[outCount].v = ( basis * in[ i ].v ) + rPos;
				//outVerts[outCount].f = in[i].f;
				outDepths[outCount++] = d;
			}
		}

		BH_ASSERT( outCount <= 8, " outCount > 8" );

		return outCount;
	}

	// Find the furthest point based on n
	void SupportEdge( const Matrix4 & rot, const Vector3f & pos, const Vector3f & e, Vector3f n, Vector3f & lOut, Vector3f & rOut )
	{
		n = rot.Transpose() * n;

		Vector3f absN( std::abs( n.x ), std::abs( n.y ), std::abs( n.z ) );
		Vector3f l, r;

		// x > y
		if ( absN.x > absN.y )
		{
			// x > y > z
			if ( absN.y > absN.z )
			{
				l = Vector3f( e.x, e.y, e.z );
				r = Vector3f( e.x, e.y, -e.z );
			}

			// x > z > y || z > x > y
			else
			{
				l = Vector3f( e.x, e.y, e.z );
				r = Vector3f( e.x, -e.y, e.z );
			}
		}

		// y > x
		else
		{
			// y > x > z
			if ( absN.x > absN.z )
			{
				l = Vector3f( e.x, e.y, e.z );
				r = Vector3f( e.x, e.y, -e.z );
			}

			// z > y > x || y > z > x
			else
			{
				l = Vector3f( e.x, e.y, e.z );
				r = Vector3f( -e.x, e.y, e.z );
			}
		}

		f32 signx = ( n.x >= 0.0f ) ? 1.0f : -1.0f;
		f32 signy = ( n.y >= 0.0f ) ? 1.0f : -1.0f;
		f32 signz = ( n.z >= 0.0f ) ? 1.0f : -1.0f;

		l.x *= signx;
		l.y *= signy;
		l.z *= signz;
		r.x *= signx;
		r.y *= signy;
		r.z *= signz;

		lOut = Vector3f( ( rot * l ) + pos ); //q3Mul( tx, a );
		rOut = Vector3f( ( rot * r ) + pos ); //q3Mul( tx, b );
	}

	inline void EdgesContact( Vector3f & CL, Vector3f & CR, const Vector3f & PL, const Vector3f & QL, const Vector3f & PR, const Vector3f & QR )
	{
		Vector3f DL = QL - PL;
		Vector3f DR = QR - PR;
		Vector3f r = PL - PR;
		f32 a = DL.Dot( DL );
		f32 e = DR.Dot( DR );
		f32 f = DR.Dot( r );
		f32 c = DL.Dot( r );

		f32 b = DL.Dot( DR );
		f32 denom = a * e - b * b;

		f32 TL = ( b * f - c * e ) / denom;
		f32 TR = ( b * TL + f ) / e;

		CL = PL + DL * TL;
		CR = PR + DR * TR;
	}

	inline bool TrackFaceAxis( s32 & axis, s32 n, f32 s, f32 & sMax, const Vector3f & normal, Vector3f & axisNormal )
	{
		// Distance in between. Not collided
		if ( s > 0.0f )
			return true;

		if ( s > sMax )
		{
			sMax = s;
			axis = n;
			axisNormal = normal;
		}

		return false;
	}

	inline bool TrackEdgeAxis( s32 & axis, s32 n, f32 s, f32 & sMax, const Vector3f & normal, Vector3f & axisNormal )
	{
		// Distance in between. Not collided
		if ( s > 0.0f )
			return true;

		f32 l = 1.0f / normal.Length();
		s *= l;

		if ( s > sMax )
		{
			sMax = s;
			axis = n;
			axisNormal = normal * l;
		}

		return false;
	}

	// http://www.randygaul.net/2014/05/22/deriving-obb-to-obb-intersection-sat/
	bool OBBToOBB( Manifold & manifold, const OBB & lhs, const OBB & rhs, bool passable )
	{
		Matrix4 lRot( lhs.mAxis[0].x, lhs.mAxis[0].y, lhs.mAxis[0].z, 0.0f,
					  lhs.mAxis[1].x, lhs.mAxis[1].y, lhs.mAxis[1].z, 0.0f,
					  lhs.mAxis[2].x, lhs.mAxis[2].y, lhs.mAxis[2].z, 0.0f,
					  0.0f,			  0.0f,			  0.0f,			  1.0f );

		Matrix4 rRot( rhs.mAxis[0].x, rhs.mAxis[0].y, rhs.mAxis[0].z, 0.0f,
					  rhs.mAxis[1].x, rhs.mAxis[1].y, rhs.mAxis[1].z, 0.0f,
					  rhs.mAxis[2].x, rhs.mAxis[2].y, rhs.mAxis[2].z, 0.0f,
					  0.0f,			  0.0f,			  0.0f,			  1.0f );

		Matrix4 lRotT = lRot.Transpose();
		Matrix4 C = lRotT * rRot;

		Matrix4 absC = Matrix4::IDENTITY;
		bool parallel = false;
		static const f32 kCosTol = 1.0e-6f;
		for ( s32 i = 0; i < 3; ++i )
		{
			for ( s32 j = 0; j < 3; ++j )
			{
				f32 val = std::abs( C.m[i][j] );
				absC.m[i][j] = val;

				parallel = parallel ? true : ( val + kCosTol >= 1.0f );
			}
		}

		Vector3f t = lRotT * ( rhs.mPosition - lhs.mPosition );

		f32 s;
		f32 lMax = -FLT_MAX;
		f32 rMax = -FLT_MAX;
		f32 eMax = -FLT_MAX;
		s32 lAxis = -1;
		s32 rAxis = -1;
		s32 eAxis = -1;
		Vector3f lN;
		Vector3f rN;
		Vector3f eN;

		// LHS's x axis
		s = std::abs( t.x ) - ( lhs.mHalfWidth.x + Vector3f( absC.m[0][0], absC.m[1][0], absC.m[2][0] ).Dot( rhs.mHalfWidth ) );
		if ( TrackFaceAxis( lAxis, 0, s, lMax, lhs.mAxis[0], lN ) )
			return false;

		// LHS's y axis
		s = std::abs( t.y ) - ( lhs.mHalfWidth.y + Vector3f( absC.m[0][1], absC.m[1][1], absC.m[2][1] ).Dot( rhs.mHalfWidth ) );
		if ( TrackFaceAxis( lAxis, 1, s, lMax, lhs.mAxis[1], lN ) )
			return false;

		// LHS's z axis
		s = std::abs( t.z ) - ( lhs.mHalfWidth.z + Vector3f( absC.m[0][2], absC.m[1][2], absC.m[2][2] ).Dot( rhs.mHalfWidth ) );
		if ( TrackFaceAxis( lAxis, 2, s, lMax, lhs.mAxis[2], lN ) )
			return false;

		// RHS's x axis
		s = std::abs( Vector3f( C.m[0][0], C.m[0][1], C.m[0][2] ).Dot( t ) ) - 
			( rhs.mHalfWidth.x + Vector3f( absC.m[0][0], absC.m[0][1], absC.m[0][2] ).Dot( lhs.mHalfWidth ) );
		if ( TrackFaceAxis( rAxis, 3, s, rMax, rhs.mAxis[0], rN ) )
			return false;

		// RHS's y axis
		s = std::abs( Vector3f( C.m[1][0], C.m[1][1], C.m[1][2] ).Dot( t ) ) - 
			( rhs.mHalfWidth.y + Vector3f( absC.m[1][0], absC.m[1][1], absC.m[1][2] ).Dot( lhs.mHalfWidth ) );
		if ( TrackFaceAxis( rAxis, 4, s, rMax, rhs.mAxis[1], rN ) )
			return false;

		// RHS's z axis
		s = std::abs( Vector3f( C.m[2][0], C.m[2][1], C.m[2][2] ).Dot( t ) ) - 
			( rhs.mHalfWidth.z + Vector3f( absC.m[2][0], absC.m[2][1], absC.m[2][2] ).Dot( lhs.mHalfWidth ) );
		if ( TrackFaceAxis( rAxis, 5, s, rMax, rhs.mAxis[2], rN ) )
			return false;

		// If both box's axes not parallel, additional check
		if ( !parallel )
		{
			f32 rL;
			f32 rR;

			// lhs.x X rhs.x
			rL = lhs.mHalfWidth.y * absC.m[0][2] + lhs.mHalfWidth.z * absC.m[0][1];
			rR = rhs.mHalfWidth.y * absC.m[2][0] + rhs.mHalfWidth.z * absC.m[1][0];
			s = std::abs( t.z * C.m[0][1] - t.y * C.m[0][2] ) - ( rL + rR );
			if ( TrackEdgeAxis( eAxis, 6, s, eMax, Vector3f( 0.0f, -C.m[0][2], C.m[0][1] ), eN ) )
				return false;

			// lhs.x X rhs.y
			rL = lhs.mHalfWidth.y * absC.m[1][2] + lhs.mHalfWidth.z * absC.m[1][1];
			rR = rhs.mHalfWidth.x * absC.m[2][0] + rhs.mHalfWidth.z * absC.m[0][0];
			s = std::abs( t.z * C.m[1][1] - t.y * C.m[1][2] ) - ( rL + rR );
			if ( TrackEdgeAxis( eAxis, 7, s, eMax, Vector3f( 0.0f, -C.m[1][2], C.m[1][1] ), eN ) )
				return false;

			// lhs.x X rhs.z
			rL = lhs.mHalfWidth.y * absC.m[2][2] + lhs.mHalfWidth.z * absC.m[2][1];
			rR = rhs.mHalfWidth.x * absC.m[1][0] + rhs.mHalfWidth.y * absC.m[0][0];
			s = std::abs( t.z * C.m[2][1] - t.y * C.m[2][2] ) - ( rL + rR );
			if ( TrackEdgeAxis( eAxis, 8, s, eMax, Vector3f( 0.0f, -C.m[2][2], C.m[2][1] ), eN ) )
				return false;

			// lhs.y X rhs.x
			rL = lhs.mHalfWidth.x * absC.m[0][2] + lhs.mHalfWidth.z * absC.m[0][0];
			rR = rhs.mHalfWidth.y * absC.m[2][1] + rhs.mHalfWidth.z * absC.m[1][1];
			s = std::abs( t.x * C.m[0][2] - t.z * C.m[0][0] ) - ( rL + rR );
			if ( TrackEdgeAxis( eAxis, 9, s, eMax, Vector3f( C.m[0][2], 0.0f, -C.m[0][0] ), eN ) )
				return false;

			// lhs.y X rhs.y
			rL = lhs.mHalfWidth.x * absC.m[1][2] + lhs.mHalfWidth.z * absC.m[1][0];
			rR = rhs.mHalfWidth.x * absC.m[2][1] + rhs.mHalfWidth.z * absC.m[0][1];
			s = std::abs( t.x * C.m[1][2] - t.z * C.m[1][0] ) - ( rL + rR );
			if ( TrackEdgeAxis( eAxis, 10, s, eMax, Vector3f( C.m[1][2], 0.0f, -C.m[1][0] ), eN ) )
				return false;

			// lhs.y X rhs.z
			rL = lhs.mHalfWidth.x * absC.m[2][2] + lhs.mHalfWidth.z * absC.m[2][0];
			rR = rhs.mHalfWidth.x * absC.m[1][1] + rhs.mHalfWidth.y * absC.m[0][1];
			s = std::abs( t.x * C.m[2][2] - t.z * C.m[2][0] ) - ( rL + rR );
			if ( TrackEdgeAxis( eAxis, 11, s, eMax, Vector3f( C.m[2][2], 0.0f, -C.m[2][0] ), eN ) )
				return false;

			// lhs.z X rhs.x
			rL = lhs.mHalfWidth.x * absC.m[0][1] + lhs.mHalfWidth.y * absC.m[0][0];
			rR = rhs.mHalfWidth.y * absC.m[2][2] + rhs.mHalfWidth.z * absC.m[1][2];
			s = std::abs( t.y * C.m[0][0] - t.x * C.m[0][1] ) - ( rL + rR );
			if ( TrackEdgeAxis( eAxis, 12, s, eMax, Vector3f( -C.m[0][1], C.m[0][0], 0.0f ), eN ) )
				return false;

			// lhs.z X rhs.y
			rL = lhs.mHalfWidth.x * absC.m[1][1] + lhs.mHalfWidth.y * absC.m[1][0];
			rR = rhs.mHalfWidth.x * absC.m[2][2] + rhs.mHalfWidth.z * absC.m[0][2];
			s = std::abs( t.y * C.m[1][0] - t.x * C.m[1][1] ) - ( rL + rR );
			if ( TrackEdgeAxis( eAxis, 13, s, eMax, Vector3f( -C.m[1][1], C.m[1][0], 0.0f ), eN ) )
				return false;

			// lhs.z X rhs.z
			rL = lhs.mHalfWidth.x * absC.m[2][1] + lhs.mHalfWidth.y * absC.m[2][0];
			rR = rhs.mHalfWidth.x * absC.m[1][2] + rhs.mHalfWidth.y * absC.m[0][2];
			s = std::abs( t.y * C.m[2][0] - t.x * C.m[2][1] ) - ( rL + rR );
			if ( TrackEdgeAxis( eAxis, 14, s, eMax, Vector3f( -C.m[2][1], C.m[2][0], 0.0f ), eN ) )
				return false;
		}

		if ( !passable )
		{
			// Artificial axis bias to improve frame coherence
			static const f32 kRelTol = 0.95f;
			static const f32 kAbsTol = 0.01f;
			s32 axis;
			f32 sMax;
			Vector3f n;
			f32 faceMax = std::max( lMax, rMax );
			if ( kRelTol * eMax > faceMax + kAbsTol )
			{
				axis = eAxis;
				sMax = eMax;
				n = eN;
			}
			else
			{
				if ( kRelTol * rMax > lMax + kAbsTol )
				{
					axis = rAxis;
					sMax = rMax;
					n = rN;
				}
				else
				{
					axis = lAxis;
					sMax = lMax;
					n = lN;
				}
			}

			if ( n.Dot( rhs.mPosition - lhs.mPosition ) < 0.0f )
				n = -n;

			BH_ASSERT( axis != -1, "OBBToOBB - invalid axis" );

			if ( axis < 6 )
			{
				Vector3f referencePos;
				Matrix4 referenceRot;
				Vector3f referenceHalfWidth;
				Vector3f incidentPos;
				Matrix4 incidentRot;
				Vector3f incidentHalfWidth;

				bool flip;

				// lhs axis
				if ( axis < 3 )
				{
					referencePos = lhs.mPosition;
					referenceRot = lRot;
					referenceHalfWidth = lhs.mHalfWidth;
					incidentPos = rhs.mPosition;
					incidentRot = rRot;
					incidentHalfWidth = rhs.mHalfWidth;

					flip = false;
				}
				// rhs axis
				else
				{
					referencePos = rhs.mPosition;
					referenceRot = rRot;
					referenceHalfWidth = rhs.mHalfWidth;
					incidentPos = lhs.mPosition;
					incidentRot = lRot;
					incidentHalfWidth = lhs.mHalfWidth;

					flip = true;
					n = -n;
				}

				// Compute reference and incident edge information necessary for clipping
				ClipVertex incident[4];
				ComputeIncidentFace( incidentRot, incidentPos, incidentHalfWidth, n, incident );
				u8 clipEdges[4];
				Matrix4 basis = Matrix4::IDENTITY;
				Vector3f e;
				ComputeReferenceEdgesAndBasis( referenceHalfWidth, referenceRot, n, axis, clipEdges, basis, e );

				// Clip the incident face against the reference face side planes
				ClipVertex out[8];
				f32 depths[8];
				s32 outNum;
				outNum = Clip( referencePos, e, clipEdges, basis, incident, out, depths );

				if ( outNum )
				{
					// Total number of points behind reference face
					manifold.mNumContact = outNum;
					manifold.mNormal = flip ? -n : n;

					for ( s32 i = 0; i < outNum; ++i )
					{
						Contact * c = manifold.mContacts + i;

						//FeaturePair pair = out[i].f;
						//
						//if ( flip )
						//{
						//	std::swap( pair.mInI, pair.mInR );
						//	std::swap( pair.mOutI, pair.mOutR );
						//}
						//
						//c->mFeaturePair = out[i].f;
						c->mPosition = out[i].v;
						c->mPenetration = depths[i];
					}
				}
				else
				{
					manifold.mNumContact = 0;
				}
			}
			else
			{
				n = lRot * n;

				if ( n.Dot( rhs.mPosition - lhs.mPosition ) < 0.0f )
					n = -n;

				Vector3f PL, QL;
				Vector3f PR, QR;
				SupportEdge( lRot, lhs.mPosition, lhs.mHalfWidth, n, PL, QL );
				SupportEdge( rRot, rhs.mPosition, rhs.mHalfWidth, -n, PR, QR );

				Vector3f CL, CR;
				EdgesContact( CL, CR, PL, QL, PR, QR );

				manifold.mNormal = n;
				manifold.mNumContact = 1;

				Contact * c = manifold.mContacts;
				FeaturePair pair;
				pair.mKey = axis;
				//c->mFeaturePair = pair;
				c->mPenetration = sMax;
				c->mPosition = ( CL + CR ) * 0.5f;
			}
		}

		return true;
	}														 
}															 