#ifndef BH_COMMONFUNCTIONS_H
#define BH_COMMONFUNCTIONS_H

#include "Core/CoreEssential.h"

// Genric functions expose for scripts uses
#ifdef __cplusplus
extern "C"
{
#endif

	// Start profiler
	__declspec( dllexport ) void StartProfiler( const BH::Char * profiler_name );

	// Stop profiler
	__declspec( dllexport ) void StopProfiler( const BH::Char * profiler_name );

	// Set renderer
	__declspec( dllexport ) void SetRenderer( BH::Renderer * renderer );

	// Set mesh manager
	__declspec( dllexport ) void SetModelManager( BH::ModelManager * modelManager );

	// Set texture manager
	__declspec( dllexport ) void SetTextureManager( BH::TextureManager * textureManager );

	// Set material manager
	__declspec( dllexport ) void SetMaterialManager( BH::MaterialManager * materialManager );

	// Set collision manager
	__declspec( dllexport ) void SetCollisionManager( BH::CollisionManager * collisionManager );

	// Draw instance
	__declspec( dllexport ) void DrawInstance( BH::f32 posX,	
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
	__declspec( dllexport ) void DrawGlobalLight( BH::f32 posX,	
											      BH::f32 posY,
											      BH::f32 posZ,
											      BH::f32 colorR,
											      BH::f32 colorG,
											      BH::f32 colorB,
											      BH::f32 colorA );

	// Draw local light
	__declspec( dllexport ) void DrawDirectionalLight( BH::f32 posX,	
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
	__declspec( dllexport ) void DrawLocalLight( BH::f32 posX,	
											     BH::f32 posY,
											     BH::f32 posZ,
											     BH::f32 colorR,
											     BH::f32 colorG,
											     BH::f32 colorB,
											     BH::f32 colorA,
												 BH::f32 radius );

	// Add Static OBB
	__declspec( dllexport ) void AddStaticOBB( const BH::Char * name,
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
	__declspec( dllexport ) void UpdateStaticOBB( const BH::Char * name,
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
	__declspec( dllexport ) void RemoveStaticOBB( const BH::Char * name );

	// Add Moving OBB
	__declspec( dllexport ) void AddMovingOBB( const BH::Char * name,
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
	__declspec( dllexport ) void UpdateMovingOBB( const BH::Char * name,
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
	__declspec( dllexport ) void RemoveMovingOBB( const BH::Char * name );

#ifdef __cplusplus
}
#endif

#endif