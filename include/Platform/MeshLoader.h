#ifndef BH_MESH_LOADER_H
#define BH_MESH_LOADER_H

#ifdef _DEBUG
	#pragma comment (lib, "../Lib/libfbxsdk.lib")
#else
	#pragma comment (lib, "../Lib/libfbxsdk.lib")
#endif

#include "Platform/PlatformEssential.h"
#include "Platform/Vertex.h"
#include "Platform/Mesh.h"
#include "Platform/Animation.h"
#include "Platform/AABB.h"

// Dependencies
#include "fbxsdk.h"

#include <vector>

namespace BH
{
	struct BoneData
	{
		String mName;
		s32 mParentIndex;
		s32 mBoneIndex;
		FbxNode * mBoneNode;

		FbxVector4 mBindPos;
		FbxQuaternion mBindRot;

		FbxVector4 mBoneSpacePos;
		FbxQuaternion mBoneSpaceRot;
	};

	struct SkinData
	{
		/*
			Index in PointWeight vector correspond to mesh control 
			points which are mapped by the Position Indices
		*/
		std::vector< WeightVector > mPointWeights;
		bool mSkin;

		SkinData() : mSkin( false ) {}
	};

	typedef std::vector<BoneData> SkeletonData;

	struct MeshData
	{
		std::vector<Vertex> mVertexBuffer;
		std::vector<s32> mIndexBuffer;

		std::vector<s32> mPolygonSizeArray;
		std::vector<FbxVector4> mPoints;
		std::vector<s32> mPointsIndexBuffer;
		std::vector<FbxVector4> mNormals;
		std::vector<s32> mNormalIndexBuffer;
		std::vector<FbxVector2> mUVs;
		std::vector<s32> mUVIndexBuffer;

		SkinData mSkin;
	};

	class MeshLoader
	{
	public:
		// Constructor
		MeshLoader();

		// Destructor
		~MeshLoader();

		// Load mesh
		void LoadMeshFromFile( const FileInfo & fileinfo, Mesh & mesh, AABB & aabb );

		// Create quad
		static void CreateQuad( Mesh & mesh );

		// Create Cube (Line)
		static void CreateLineCube( Mesh & mesh );

		// Create XY Circle (Line)
		static void CreateLineCircle( Mesh & mesh );

		// Create X Line
		static void CreateXLine( Mesh & mesh );

	private:
		// Get Conversion Matrix
		FbxMatrix GetConversionMatrix( FbxScene * scene );

		// Extract Scene Data
		void ExtractSceneData( FbxScene * scene, Mesh & mesh, AABB & aabb );

		// Collect Meshes
		void CollectMeshes( FbxNode * root, std::vector<FbxNode *> & fbxMeshes, bool skin );

		// Extract Geometry
		void ExtractGeometry( FbxNode * node, FbxPose * pose, MeshData & meshdata, const FbxMatrix & conversionMatrix );

		// Extract Skeleton Data
		void ExtractSkeleton( FbxNode * node, SkeletonData & skeleton, std::vector<FbxNode *> & boneNodes );

		// Extract Bind Pose
		void ExtractBindPose( FbxNode * node, FbxPose * pose, BoneData & bone, const FbxMatrix & conversionMatrix );

		// Extract Bind Matrix
		void ExtractBindMatrix( BoneData & bone, FbxAMatrix & mat, const FbxMatrix & conversionMatrix );

		// Extract Skin Data
		bool ExtractSkinWeights( FbxPose * pose, FbxNode * node, SkinData & skinData, SkeletonData & skeleton );

		// Extract Animation Tracks from take
		void ExtractAnimationTracksFromTake( FbxScene * scene, Animation & animation, const FbxMatrix & conversionMatrix, 
											 std::vector<FbxNode*> animationNodes, const Char * takeName );

		// Extract Animation Track
		void ExtractAnimationTrack( FbxNode * node, Animation & animation, FbxAnimLayer * layer, 
									FbxAnimEvaluator * animationEvaluator, const FbxMatrix & conversionMatrix, 
									s32 trackIndex, FbxTime start );

		// Generate Vertices
		void GenerateVertices( MeshData & meshdata );

		// Find matching vertex
		s32 FindMatchingVertex( MeshData & meshdata, std::vector< std::vector<s32> > & controlMap, s32 num,
								std::vector<s32> & sourcePosIndices, std::vector<s32> & sourceNormIndices, 
								std::vector<s32> & sourceUVIndices );

		// Triangulate
		void Triangulate( MeshData & meshdata );

		// Convert Triangle Winding
		void ConvertTriWinding( MeshData & meshdata );

		// Get Bone Index
		s32 GetBoneIndex( FbxNode * node, SkeletonData & skeleton );

		// Get bounding volume
		void GetBoundingVolume( const MeshData & meshdata, AABB & aabb );

	private:
		FbxManager * mFBXManager;

	};
}

#endif