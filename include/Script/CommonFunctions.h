#ifndef BH_COMMONFUNCTIONS_H
#define BH_COMMONFUNCTIONS_H

#include "Platform/Platform.h"
#include "Core/CoreEssential.h"

// Genric functions expose for scripts uses
#ifdef __cplusplus
extern "C"
{
#endif

	// Start profiler
	BH_DLL_EXPORT void StartProfiler( const BH::Char * profiler_name );

	// Stop profiler
	BH_DLL_EXPORT void StopProfiler( const BH::Char * profiler_name );

	// Set renderer
	BH_DLL_EXPORT void SetRenderer( BH::Renderer * renderer );

	// Set mesh manager
	BH_DLL_EXPORT void SetModelManager( BH::ModelManager * modelManager );

	// Set texture manager
	BH_DLL_EXPORT void SetTextureManager( BH::TextureManager * textureManager );

	// Set material manager
	BH_DLL_EXPORT void SetMaterialManager( BH::MaterialManager * materialManager );

	// Set collision manager
	BH_DLL_EXPORT void SetCollisionManager( BH::CollisionManager * collisionManager );

	// Draw instance
	BH_DLL_EXPORT void DrawInstance( BH::f32 posX,	
									 BH::f32 posY,
									 BH::f32 posZ,
									 BH::f32 scaleX,
									 BH::f32 scaleY,
									 BH::f32 scaleZ,
									 BH::f32 rotX,
									 BH::f32 rotY,
									 BH::f32 rotZ,
									 const BH::Char * model,		
									 const BH::Char * material );

	// Draw global light
	BH_DLL_EXPORT void DrawGlobalLight( BH::f32 posX,	
										BH::f32 posY,
										BH::f32 posZ,
										BH::f32 colorR,
										BH::f32 colorG,
										BH::f32 colorB,
										BH::f32 colorA );

	// Draw local light
	BH_DLL_EXPORT void DrawDirectionalLight( BH::f32 posX,	
											 BH::f32 posY,
											 BH::f32 posZ,
											 BH::f32 colorR,
											 BH::f32 colorG,
											 BH::f32 colorB,
											 BH::f32 colorA,
											 BH::f32 dirX,
											 BH::f32 dirY,
											 BH::f32 dirZ,
											 BH::f32 width,
											 BH::f32 height,
											 BH::f32 depth );

	// Draw local light
	BH_DLL_EXPORT void DrawLocalLight( BH::f32 posX,	
									   BH::f32 posY,
									   BH::f32 posZ,
									   BH::f32 colorR,
									   BH::f32 colorG,
									   BH::f32 colorB,
									   BH::f32 colorA,
									   BH::f32 radius );

	// Add Static OBB
	BH_DLL_EXPORT void AddStaticOBB( const BH::Char * name,
									 BH::f32 posX,	
									 BH::f32 posY,
									 BH::f32 posZ,
									 BH::f32 scaleX,
									 BH::f32 scaleY,
									 BH::f32 scaleZ,
									 BH::f32 rotX,
									 BH::f32 rotY,
									 BH::f32 rotZ,
									 const BH::Char * model,
									 bool passable,
									 const BH::Char * tag );

	// Update Static OBB
	BH_DLL_EXPORT void UpdateStaticOBB( const BH::Char * name,
										BH::f32 posX,	
										BH::f32 posY,
										BH::f32 posZ,
										BH::f32 scaleX,
										BH::f32 scaleY,
										BH::f32 scaleZ,
										BH::f32 rotX,
										BH::f32 rotY,
										BH::f32 rotZ,
										const BH::Char * model,
										bool passable,
										const BH::Char * tag );

	// Remove Static OBB
	BH_DLL_EXPORT void RemoveStaticOBB( const BH::Char * name );

	// Add Moving OBB
	BH_DLL_EXPORT void AddMovingOBB( const BH::Char * name,
									 BH::f32 posX,	
									 BH::f32 posY,
									 BH::f32 posZ,
									 BH::f32 scaleX,
									 BH::f32 scaleY,
									 BH::f32 scaleZ,
									 BH::f32 rotX,
									 BH::f32 rotY,
									 BH::f32 rotZ,
									 const BH::Char * model,
									 bool passable,
									 const BH::Char * tag );

	// Update Moving OBB
	BH_DLL_EXPORT void UpdateMovingOBB( const BH::Char * name,
										BH::f32 posX,	
										BH::f32 posY,
										BH::f32 posZ,
										BH::f32 scaleX,
										BH::f32 scaleY,
										BH::f32 scaleZ,
										BH::f32 rotX,
										BH::f32 rotY,
										BH::f32 rotZ,
										const BH::Char * model,
										bool passable,
										const BH::Char * tag );

	// Remove Moving OBB
	BH_DLL_EXPORT void RemoveMovingOBB( const BH::Char * name );

#ifdef __cplusplus
}
#endif

#endif