#include "Script/CommonFunctions.h"
#include "Core/Profiling.h"

#include "Platform/Platform.h"
#include "Platform/Vector2f.h"
#include "Platform/Vector3f.h"
#include "Platform/Vector4f.h"
#include "Platform/Matrix4.h"
#include "Platform/AABB.h"
#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/Game.h"
#include "Core/SystemManager.h"
#include "Core/System.h"
#include "Core/Renderer.h"
#include "Core/InputManager.h"
#include "Core/ModelManager.h"
#include "Core/AnimationControllerManager.h"
#include "Core/TextureManager.h"
#include "Core/MaterialManager.h"
#include "Core/GameStateManager.h"
#include "Core/LevelManager.h"
#include "Core/CollisionManager.h"
#include "Core/Light.h"
#include "Core/Math.h"
#include "Core/OBB.h"

static BH::Renderer *					sRenderer					= nullptr;
static BH::ModelManager *				sModelManager				= nullptr;
static BH::AnimationControllerManager * sAnimationControllerManager = nullptr;
static BH::TextureManager *				sTextureManager				= nullptr;
static BH::MaterialManager *			sMaterialManager			= nullptr;
static BH::CollisionManager *			sCollisionManager			= nullptr;

static BH::OBB GetOBBFromAABB( const BH::AABB & aabb, const BH::Vector3f & pos, const BH::Vector3f & size, const BH::Vector3f & rotation )
{
	BH::Matrix4 rot = BH::Matrix4::CreateFromYawPitchRoll( rotation.y, rotation.x, rotation.z );
	BH::Matrix4 trans = BH::Matrix4::CreateTranslation( pos );
	BH::Matrix4 scale = BH::Matrix4::CreateScale( size );

	BH::OBB obb;
	obb.mAxis[0] = BH::Vector3f( rot.m[0][0], rot.m[0][1], rot.m[0][2] );
	obb.mAxis[1] = BH::Vector3f( rot.m[1][0], rot.m[1][1], rot.m[1][2] );
	obb.mAxis[2] = BH::Vector3f( rot.m[2][0], rot.m[2][1], rot.m[2][2] );

	trans = trans * scale;

	BH::Vector3f n_min = trans * aabb.mMin;
	BH::Vector3f n_max = trans * aabb.mMax;
	BH::Vector3f n_scale = n_max - n_min;
	BH::Vector3f n_pos = n_min + 0.5f * n_scale;

	BH::Matrix4 trans_o = BH::Matrix4::CreateTranslation( n_pos - pos );

	trans = BH::Matrix4::CreateTranslation( pos );
	scale = BH::Matrix4::CreateScale( n_scale );

	trans = trans * rot * trans_o * scale;

	obb.mHalfWidth = 0.5f * n_scale;
	obb.mPosition = BH::Vector3f( trans.m[3][0], trans.m[3][1], trans.m[3][2] );

	return obb;
}

#ifdef __cplusplus
extern "C"
{
#endif

	BH_DLL_EXPORT void StartProfiler( const BH::Char * profiler_name )
	{
		START_PROFILE( profiler_name );
	}

	BH_DLL_EXPORT void StopProfiler( const BH::Char * profiler_name )
	{
		END_PROFILE( profiler_name );
	}

	BH_DLL_EXPORT void SetRenderer( BH::Renderer * renderer )
	{
		sRenderer = renderer;
	}

	BH_DLL_EXPORT void SetModelManager( BH::ModelManager * modelManager )
	{
		sModelManager = modelManager;
	}
	
	BH_DLL_EXPORT void SetTextureManager( BH::TextureManager * textureManager )
	{
		sTextureManager = textureManager;
	}
	
	BH_DLL_EXPORT void SetMaterialManager( BH::MaterialManager * materialManager )
	{
		sMaterialManager = materialManager;
	}

	BH_DLL_EXPORT void SetCollisionManager( BH::CollisionManager * collisionManager )
	{
		sCollisionManager = collisionManager;
	}

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
											   const BH::Char * material )
	{
		if ( !sRenderer || !sModelManager || !sTextureManager || !sMaterialManager )
			return;

		BH::Model * pModel = sModelManager->GetModel( model );
		BH::Material * pMaterial = sMaterialManager->GetMaterial( material );

		sRenderer->DrawInstance( BH::Vector3f( posX, posY, posZ ),
								 BH::Vector3f( scaleX, scaleY, scaleZ ), 
								 BH::Vector3f( BH::Math::DegToRad( rotX ), BH::Math::DegToRad( rotY ), BH::Math::DegToRad( rotZ ) ), 
								 pModel, pMaterial );
	}

	BH_DLL_EXPORT void DrawGlobalLight( BH::f32 posX,	
											      BH::f32 posY,
											      BH::f32 posZ,
											      BH::f32 colorR,
											      BH::f32 colorG,
											      BH::f32 colorB,
											      BH::f32 colorA )
	{
		if ( !sRenderer )
			return;

		sRenderer->DrawGlobalLight( BH::Light( BH::Vector3f( posX, posY, posZ ),
											   BH::Vector4f( colorR, colorG, colorB, colorA ) ) );
	}

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
													   BH::f32 depth )
	{
		if ( !sRenderer )
			return;

		sRenderer->DrawDirectionalLight( BH::DirectionalLight( BH::Vector3f( posX, posY, posZ ),
															   BH::Vector4f( colorR, colorG, colorB, colorA ),
															   BH::Vector3f( dirX, dirY, dirZ ),
															   BH::Vector3f( width, height, depth ) ) );
	}

	BH_DLL_EXPORT void DrawLocalLight( BH::f32 posX,	
											     BH::f32 posY,
											     BH::f32 posZ,
											     BH::f32 colorR,
											     BH::f32 colorG,
											     BH::f32 colorB,
											     BH::f32 colorA,
												 BH::f32 radius )
	{
		if ( !sRenderer )
			return;

		sRenderer->DrawLocalLight( BH::LocalLight( BH::Vector3f( posX, posY, posZ ),
												   BH::Vector4f( colorR, colorG, colorB, colorA ),
												   radius ) );
	}

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
											   const BH::Char * tag )
	{
		if ( !sCollisionManager || !sModelManager )
			return;

		BH::AABB aabb = sModelManager->GetAABB( model );
		BH::OBB obb = GetOBBFromAABB( aabb,
									  BH::Vector3f( posX, posY, posZ ),
									  BH::Vector3f( scaleX, scaleY, scaleZ ),
									  BH::Vector3f( BH::Math::DegToRad( rotX ), BH::Math::DegToRad( rotY ), BH::Math::DegToRad( rotZ ) ) );
		sCollisionManager->AddToStaticList( name, obb, passable, tag );
	}

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
												  const BH::Char * tag )
	{
		if ( !sCollisionManager || !sModelManager )
			return;

		BH::AABB aabb = sModelManager->GetAABB( model );
		BH::OBB obb = GetOBBFromAABB( aabb,
									  BH::Vector3f( posX, posY, posZ ),
									  BH::Vector3f( scaleX, scaleY, scaleZ ),
									  BH::Vector3f( BH::Math::DegToRad( rotX ), BH::Math::DegToRad( rotY ), BH::Math::DegToRad( rotZ ) ) );
		sCollisionManager->UpdateStaticObject( name, obb, passable, tag );
	}

	// Remove Static OBB
	BH_DLL_EXPORT void RemoveStaticOBB( const BH::Char * name )
	{
		if ( !sCollisionManager )
			return;

		sCollisionManager->RemoveFromStaticList( name );
	}

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
											   const BH::Char * tag )
	{
		if ( !sCollisionManager || !sModelManager )
			return;

		BH::AABB aabb = sModelManager->GetAABB( model );
		BH::OBB obb = GetOBBFromAABB( aabb,
									  BH::Vector3f( posX, posY, posZ ),
									  BH::Vector3f( scaleX, scaleY, scaleZ ),
									  BH::Vector3f( BH::Math::DegToRad( rotX ), BH::Math::DegToRad( rotY ), BH::Math::DegToRad( rotZ ) ) );
		sCollisionManager->AddToMovingList( name, obb, passable, tag );
	}

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
												  const BH::Char * tag )
	{
		if ( !sCollisionManager || !sModelManager )
			return;

		BH::AABB aabb = sModelManager->GetAABB( model );
		BH::OBB obb = GetOBBFromAABB( aabb,
									  BH::Vector3f( posX, posY, posZ ),
									  BH::Vector3f( scaleX, scaleY, scaleZ ),
									  BH::Vector3f( BH::Math::DegToRad( rotX ), BH::Math::DegToRad( rotY ), BH::Math::DegToRad( rotZ ) ) );
		sCollisionManager->UpdateMovingObject( name, obb, passable, tag );
	}

	// Remove Moving OBB
	BH_DLL_EXPORT void RemoveMovingOBB( const BH::Char * name )
	{
		if ( !sCollisionManager )
			return;

		sCollisionManager->RemoveFromMovingList( name );
	}

#ifdef __cplusplus
}
#endif