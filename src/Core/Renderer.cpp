// Precompiled
#include "Core/Stdafx.h"

#include "Platform/Graphics.h"
#include "Platform/Mesh.h"
#include "Platform/Shader.h"

#include "Core/Renderer.h"
#include "Core/TextureManager.h"
#include "Core/HDRTextureManager.h"
#include "Core/Game.h"
#include "Core/Math.h"
#include "Core/MaterialManager.h"

namespace BH
{
	f32 Renderer::Near = 1.0f;
	f32 Renderer::Far = 1000.0f;
	f32 Renderer::ExponentConstant = 100.0f;
	f32 Renderer::TraceSampleRate = 5.0f;
	bool Renderer::TraceDownSample = false;
	bool Renderer::TraceJitter = false;
	bool Renderer::TraceEdgeBlur = false;
	bool Renderer::ShowSkyDome = true;
	bool Renderer::ShowDebugLight = true;
	f32 Renderer::AmbientRangeOfInfluence = 2.0f;
	f32 Renderer::AmbientScaleFactor = 2.0f;
	f32 Renderer::AmbientContrast = 2.0f;
	bool Renderer::EnableAmbientOcculsion = true;
	String Renderer::SkyName = "Newport";

	static const u32 TRACE_RADIUS = 30;

	Renderer::Renderer()
	: IDrawable( UINT_MAX )
	, mGraphics( Graphics::sInstance )
	, mCamera( nullptr )
	, mBlurKernelRadius( 50 )
	, mSceneOffset( Vector2f::ZERO )
	, mSceneDimension( Vector2f::ONE )
	, mHideSkyDome( false )
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::ShutdownShaders()
	{
		for ( auto & i : mShaders )
			i.second.Shutdown();
	}

	void Renderer::InitialiseShaders()
	{
		// ----------------------- Geometry pass -----------------------
		mShaders["Geo"].Initialise( "../shaders/geometry_vs.hlsl",
									"../shaders/geometry_ps.hlsl",
									"GeometryVertexShader",
									"GeometryPixelShader" );

		// Geometry pass - Material buffer
		mShaders["Geo"].CreaterBuffer( PixelShader, 0, sizeof( Vector3f ) + sizeof( f32 ) );

		// Geometry pass - Texture Sampler
		mShaders["Geo"].CreateSampler( PixelShader, 0 );

		// ----------------------- Drawing quad -----------------------
		mShaders["Quad"].Initialise( "../shaders/quad_vs.hlsl",
									 "../shaders/quad_ps.hlsl",
									 "QuadVertexShader",
									 "QuadPixelShader" );

		// ----------------------- Ambient pass -----------------------
		D3D_SHADER_MACRO ambientShaderMacros[2];
		String iblNDef = std::to_string( IBL_N );
		ambientShaderMacros[0].Name = "IBL_N";
		ambientShaderMacros[0].Definition = iblNDef.c_str();
		ambientShaderMacros[1].Name = NULL;
		ambientShaderMacros[1].Definition = NULL;

		bool initialise = mShaders["Ambient"].Initialise( "../shaders/ambient_vs.hlsl",
														  "../shaders/ambient_ps.hlsl",
														  "AmbientVertexShader",
														  "AmbientPixelShader",
														  nullptr,
														  ambientShaderMacros );

		// Ambient pass - Ambient buffer
		mShaders["Ambient"].CreaterBuffer( PixelShader, 0, sizeof( Vector4f ) + sizeof( Vector4f ) );
		mShaders["Ambient"].CreaterBuffer( PixelShader, 1, sizeof( Vector4f ) * IBL_N );

		// Ambient pass - Texture Sampler
		mShaders["Ambient"].CreateSampler( PixelShader, 0 );

		// ----------------------- Ambient Occlusion Factor pass -----------------------
		mShaders["AmbientOcclusion"].Initialise( "../shaders/ambientOcclusion_vs.hlsl",
												 "../shaders/ambientOcclusion_ps.hlsl",
												 "AOVertexShader",
												 "AOPixelShader" );

		// Lighting pass - Light buffer
		mShaders["AmbientOcclusion"].CreaterBuffer( PixelShader, 0, sizeof( f32 ) + sizeof( f32 ) + sizeof( f32 ) + sizeof( f32 ) );

		// Lighting pass - Texture Sampler
		mShaders["AmbientOcclusion"].CreateSampler( PixelShader, 0 );

		// ----------------------- Lighting pass -----------------------
		mShaders["Lighting"].Initialise( "../shaders/light_vs.hlsl",
										 "../shaders/light_ps.hlsl",
										 "LightVertexShader",
										 "LightPixelShader" );

		// Lighting pass - Light buffer
		mShaders["Lighting"].CreaterBuffer( PixelShader, 0, sizeof( Vector4f ) + 
															sizeof( Vector3f ) + sizeof( f32 ) + 
															sizeof( Vector3f ) + sizeof( f32 ) + 
															sizeof( Vector3f ) + sizeof( f32 ) );

		mShaders["Lighting"].CreaterBuffer( PixelShader, 1, sizeof( Matrix4 ) + sizeof( s32 ) * 4 );

		// Lighting pass - Texture Sampler
		mShaders["Lighting"].CreateSampler( PixelShader, 0 );

		// ----------------------- Directional Lighting pass -----------------------
		mShaders["DirLighting"].Initialise( "../shaders/light_vs.hlsl",
											"../shaders/directionalLight_ps.hlsl",
											"LightVertexShader",
											"LightPixelShader" );

		// Lighting pass - Light buffer
		mShaders["DirLighting"].CreaterBuffer( PixelShader, 0, sizeof( Vector4f ) + 
											   sizeof( Vector3f ) + sizeof( f32 ) + 
											   sizeof( Vector3f ) + sizeof( f32 ) + 
											   sizeof( Vector3f ) + sizeof( f32 ) + 
											   sizeof( Vector3f ) + sizeof( f32 ) + 
											   sizeof( Vector3f ) + sizeof( f32 ) );

		mShaders["DirLighting"].CreaterBuffer( PixelShader, 1, sizeof( Matrix4 ) );

		// Lighting pass - Texture Sampler
		mShaders["DirLighting"].CreateSampler( PixelShader, 0 );

		// ----------------------- Trace sample pass -----------------------
		mShaders["Trace"].Initialise( "../shaders/trace_vs.hlsl",
									  "../shaders/trace_ps.hlsl",
									  "TraceVertexShader",
									  "TracePixelShader" );

		// Tracing pass - Light buffer
		mShaders["Trace"].CreaterBuffer( PixelShader, 0, sizeof( Vector4f ) + 
										 sizeof( Vector3f ) + sizeof( f32 ) + 
										 sizeof( Vector3f ) + sizeof( f32 ) + 
										 sizeof( Vector3f ) + sizeof( f32 ) +
										 sizeof( Vector3f ) + sizeof( f32 ) +
										 sizeof( Vector2f ) + sizeof( Vector2f ) +
										 sizeof( Vector3f ) + sizeof( f32 ) );

		mShaders["Trace"].CreaterBuffer( PixelShader, 1, sizeof( Matrix4 ) + sizeof( Matrix4 ) );

		// Tracing pass - Texture Sampler
		mShaders["Trace"].CreateSampler( PixelShader, 0 );

		// Exit Trace pass
		mShaders["ExitTrace"].Initialise( "../shaders/trace_vs.hlsl",
										  "../shaders/exitTrace_ps.hlsl",
										  "TraceVertexShader",
										  "TracePixelShader" );

		// ----------------------- Edge blur pass -----------------------
		// Edge Detection
		mShaders["EdgeDetection"].Initialise( "../shaders/quad_vs.hlsl",
											  "../shaders/edgeDetection_ps.hlsl",
											  "QuadVertexShader",
											  "EdgePixelShader" );

		mShaders["EdgeDetection"].CreaterBuffer( PixelShader, 0, 
												 sizeof( Vector2f ) + sizeof( Vector2f ) );

		mShaders["EdgeDetection"].CreateSampler( PixelShader, 0 );

		// Edge Blur
		mShaders["EdgeBlur"].Initialise( "../shaders/quad_vs.hlsl",
										 "../shaders/edgeBlur_ps.hlsl",
										 "QuadVertexShader",
										 "EdgePixelShader" );

		mShaders["EdgeBlur"].CreaterBuffer( PixelShader, 0, 
											sizeof( Vector2f ) + sizeof( Vector2f ) );

		mShaders["EdgeBlur"].CreateSampler( PixelShader, 0 );

		// ----------------------- Blend light pass -----------------------
		mShaders["BlendLight"].Initialise( "../shaders/blendlight_vs.hlsl",
										   "../shaders/blendlight_ps.hlsl",
										   "BlendLightVertexShader",
										   "BlendLightPixelShader" );

		mShaders["BlendLight"].CreaterBuffer( PixelShader, 0, 
											  sizeof( Vector2f ) + sizeof( Vector2f ) );

		// Lighting pass - Texture Sampler
		mShaders["BlendLight"].CreateSampler( PixelShader, 0 );

		// ----------------------- Flat shader -----------------------
		mShaders["Flat"].Initialise( "../shaders/flat_vs.hlsl",
									 "../shaders/flat_ps.hlsl",
									 "FlatVertexShader",
									 "FlatPixelShader" );

		mShaders["Flat"].CreaterBuffer( PixelShader, 0, sizeof( Vector4f ) );

		// Flat drawing - Texture Sampler
		mShaders["Flat"].CreateSampler( PixelShader, 0 );

		// ----------------------- Depth shader for shadow map -----------------------
		mShaders["Depth"].Initialise( "../shaders/depth_vs.hlsl",
									  "../shaders/depth_ps.hlsl",
									  "DepthVertexShader",
									  "DepthPixelShader" );

		mShaders["ExponentialDepth"].Initialise( "../shaders/depth_vs.hlsl",
												 "../shaders/exponentialDepth_ps.hlsl",
												 "DepthVertexShader",
												 "DepthPixelShader" );

		mShaders["ExponentialDepth"].CreaterBuffer( PixelShader, 0, sizeof( Vector3f ) + sizeof( f32 ) );

		// ----------------------- Horizontal Blur compute shader -----------------------
		String kernalRadiusDef = std::to_string( MAX_KERNEL_RADIUS );
		String batchXDef = std::to_string( BATCH_X_NUM );
		String batchYDef = std::to_string( BATCH_Y_NUM );

		D3D_SHADER_MACRO blurShaderMacros[4];
		blurShaderMacros[0].Name = "MAX_KERNEL_RADIUS";
		blurShaderMacros[0].Definition = kernalRadiusDef.c_str();
		blurShaderMacros[1].Name = "BATCH_X_NUM";
		blurShaderMacros[1].Definition = batchXDef.c_str();
		blurShaderMacros[2].Name = "BATCH_Y_NUM";
		blurShaderMacros[2].Definition = batchYDef.c_str();
		blurShaderMacros[3].Name = NULL;
		blurShaderMacros[3].Definition = NULL;

		mShaders["HBlur"].Initialise( "../shaders/horizontalBlur_cs.hlsl",
									  "HorizontalBlurMain",
									  blurShaderMacros );
		
		mShaders["HBlur"].CreaterBuffer( ComputeShader, 1, sizeof( Vector4f ) * ( 2 * MAX_KERNEL_RADIUS + 1 ) );

		// ----------------------- Vertical Blur compute shader -----------------------
		mShaders["VBlur"].Initialise( "../shaders/verticalBlur_cs.hlsl",
								 "VerticalBlurMain",
								 blurShaderMacros );
		
		mShaders["VBlur"].CreaterBuffer( ComputeShader, 1, sizeof( Vector4f ) * ( 2 * MAX_KERNEL_RADIUS + 1 ) );

		// ----------------------- Horizontal Bilateral Blur compute shader -----------------------
		D3D_SHADER_MACRO bblurShaderMacros[4];
		bblurShaderMacros[0].Name = "MAX_KERNEL_RADIUS";
		bblurShaderMacros[0].Definition = kernalRadiusDef.c_str();
		bblurShaderMacros[1].Name = "BATCH_X_NUM";
		bblurShaderMacros[1].Definition = batchXDef.c_str();
		bblurShaderMacros[2].Name = "BATCH_Y_NUM";
		bblurShaderMacros[2].Definition = batchYDef.c_str();
		bblurShaderMacros[3].Name = NULL;
		bblurShaderMacros[3].Definition = NULL;

		mShaders["HBilateralBlur"].Initialise( "../shaders/horizontalBilateralBlur_cs.hlsl",
											   "HorizontalBlurMain",
											   bblurShaderMacros );
		
		mShaders["HBilateralBlur"].CreaterBuffer( ComputeShader, 1, sizeof( Vector4f ) * ( 2 * MAX_KERNEL_RADIUS + 1 ) );

		// ----------------------- Vertical Bilateral Blur compute shader -----------------------
		mShaders["VBilateralBlur"].Initialise( "../shaders/verticalBilateralBlur_cs.hlsl",
											   "VerticalBlurMain",
											   bblurShaderMacros );
		
		mShaders["VBilateralBlur"].CreaterBuffer( ComputeShader, 1, sizeof( Vector4f ) * ( 2 * MAX_KERNEL_RADIUS + 1 ) );

		// ----------------------- Sky dome shader -----------------------
		mShaders["Sky"].Initialise( "../shaders/skydome_vs.hlsl",
									"../shaders/skydome_ps.hlsl",
									"SkyVertexShader",
									"SkyPixelShader" );

		mShaders["Sky"].CreaterBuffer( PixelShader, 0, sizeof( Vector4f ) );

		// Sky drawing - Texture Sampler
		mShaders["Sky"].CreateSampler( PixelShader, 0 );
	}

	void Renderer::Initialise()
	{
		LOG("Initialising Renderer...");
		mGraphics.Initialise( GAME.Window, Near, Far );

		mCamera = mCamera ? mCamera : &mDefaultCamera;
		mGraphics.GetPipeline()->SetWorld( Matrix4::IDENTITY );

		mGBuffers.Initialise( 4, GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mDepthBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mHBlurDepthBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mBlurDepthBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mSceneBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mTraceBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mDTraceBuffer.Initialise( GAME.Window.GetWidth() / 5,  GAME.Window.GetHeight() / 5 );
		mExitTraceBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mGradientTraceBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mGradientBlurTraceBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mHBlurTraceBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mBlurTraceBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mAmbientBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mHBlurAmbientBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );
		mBlurAmbientBuffer.Initialise( GAME.Window.GetWidth(), GAME.Window.GetHeight() );

		//mAmbient = Vector4f( 0.1f, 0.1f, 0.1f, 1.0f );
		mAmbient = Vector4f::ONE;

		// For scanning frame buffer or drawing ui
		MeshLoader::CreateQuad( mQuad );

		InitialiseShaders();		

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::F9, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( Renderer, RecompileShader )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::F6, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( Renderer, ToggleDrawGBuffers )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::F7, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( Renderer, ToggleDrawShadowMap )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::F8, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( Renderer, ToggleDrawBlur )>( this ) );
		mGraphics.GetPipeline()->SetBackFaceCull( true );
		mGraphics.GetPipeline()->SetZBuffer( true );

		BuildWeights();
		BuildTraceWeights();
		BuildPseudoRandomSet();

		LOG("Renderer Initialised");
	}

	void Renderer::RecompileShader()
	{
		ShutdownShaders();
		InitialiseShaders();
	}

	void Renderer::Shutdown()
	{
		mGBuffers.Shutdown();
		mDepthBuffer.Shutdown();
		mHBlurDepthBuffer.Shutdown();
		mBlurDepthBuffer.Shutdown();
		mSceneBuffer.Shutdown();
		mTraceBuffer.Shutdown();
		mDTraceBuffer.Shutdown();
		mExitTraceBuffer.Shutdown();
		mGradientTraceBuffer.Shutdown();
		mGradientBlurTraceBuffer.Shutdown();
		mHBlurTraceBuffer.Shutdown();
		mBlurTraceBuffer.Shutdown();
		mAmbientBuffer.Shutdown();
		mHBlurAmbientBuffer.Shutdown();
		mBlurAmbientBuffer.Shutdown();
		
		mQuad.Shutdown();

		ShutdownShaders();

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::F9, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( Renderer, RecompileShader )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::F6, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( Renderer, ToggleDrawGBuffers )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::F7, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( Renderer, ToggleDrawShadowMap )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::F8, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( Renderer, ToggleDrawBlur )>( this ) );

		mGraphics.Shutdown();
	}

	void Renderer::Draw( f32 dt  )
	{
		mGraphics.GetPipeline()->BeginScene( Vector4f::UNIT_W );
		mSceneBuffer.ClearRenderTarget( Vector4f::ZERO );
		mTraceBuffer.ClearRenderTarget( Vector4f::UNIT_W );
		mDTraceBuffer.ClearRenderTarget(Vector4f::UNIT_W);
		mAmbientBuffer.ClearRenderTarget( Vector4f::ONE );

		Render();

		mDrawEvent.Raise( dt );

		mGraphics.GetPipeline()->SetBackFaceCull( true );
		mGraphics.GetPipeline()->SetZBuffer( true );

		mGraphics.GetPipeline()->EndScene();
	}

	void Renderer::DrawInstance( const Matrix4 & transform,
							     const Mesh * mesh,				
							     const Material * material )
	{
		mInstanceList.push_back( InstanceInfo{ transform,
											   mesh,
											   material } );
	}

	void Renderer::DrawInstance( const Vector3f & position,			
								 const Vector3f & scale,
								 const Vector3f & rotation,
								 const Mesh * mesh,
								 const Material * material )
	{
		Matrix4 transform = Matrix4::CreateTranslation( position ) * Matrix4::CreateFromYawPitchRoll( rotation.y, rotation.x, rotation.z ) * Matrix4::CreateScale( scale );
		DrawInstance( transform, mesh, material );
	}

	void Renderer::DrawGlobalLight( const Light & light )
	{
		mGlobalLights.push_back( light );
	}

	void Renderer::DrawDirectionalLight( const DirectionalLight & light )
	{
		mDirectionalLights.push_back( light );
	}

	void Renderer::DrawLocalLight( const LocalLight & light )
	{
		mLocalLights.push_back( light );
	}

	void Renderer::DrawLines( const Matrix4 & transform,
							  const Vector4f & color,
							  const Mesh * mesh,
							  bool onHUD )
	{
		mLineList.push_back( LineInfo{ transform,
									   color,
									   mesh,
									   onHUD } );
	}

	void Renderer::Render()
	{
		// Generate the view matrix based on the camera's position.
		mCamera->UpdateView();

		// Geometry pass
		GeometryPass();

		// Draw scene in buffer
		mSceneBuffer.SetRenderTarget();

		// Lighting pass
		AmbientOcculsionPass();
		AmbientPass();
		LightPass();

		// Sky Dome pass
		if ( !mHideSkyDome )
			SkyDomePass();

		// Blend Light pass
		BlendLightPass();

		// @cleanup: Debug code
		if ( ShowDebugLight )
		{
			DrawLight( mGlobalLights );
			DrawLight( mLocalLights );
			DrawDirectionalLight( mDirectionalLights );
		}

		// Draw lines last
		LinesPass();
		
		// End scene drawing
		mGraphics.GetPipeline()->SetBackBufferRenderTarget();
		DrawFinalScene();
		
		// @cleanup: Debug code
		if ( mDrawG )
			DrawGBuffers();

		// @cleanup: Debug code
		if ( mDrawShadowMap )
			DrawDepthBuffer();

		if ( mDrawBlur )
			DrawBlur();

		mInstanceList.clear();
		mGlobalLights.clear();
		mLocalLights.clear();
		mDirectionalLights.clear();
		mLineList.clear();
	}

	void Renderer::GeometryPass()
	{
		mGBuffers.ClearRenderTargets( Vector4f::ZERO );
		mGBuffers.SetRenderTargets();

		for ( auto & i : mInstanceList )
		{
			// Texture for this instance
			//mShaders["Geo"].SetTexture( PixelShader, 0, i.mTexture );

			Matrix4 world = i.mTransform * mGraphics.GetPipeline()->GetWorld();
			// Set Transform for this instance
			mShaders["Geo"].SetShaderParameters( VertexShader,
												 0, 
												 world.Transpose(),
												 mCamera->GetView().Transpose(),
												 mGraphics.GetPipeline()->GetProjection().Transpose(),
												 world.Inverse().Transpose() );
			
			i.mMaterial->SendMaterial( mShaders["Geo"], PixelShader, 0 );

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			i.mMesh->BindBuffers();

			mShaders["Geo"].Render( i.mMesh->GetIndexCount() );
		}

		//mGraphics.GetPipeline()->SetBackBufferRenderTarget();
		mGraphics.GetPipeline()->ResetViewport();
	}

	void Renderer::AmbientOcculsionPass()
	{
		mAmbientBuffer.SetRenderTarget();
		mGraphics.GetPipeline()->SetZBuffer( false );

		// Full screen quad
		mShaders["AmbientOcclusion"].SetShaderParameters( VertexShader,
														  0,
														  Matrix4::CreateTranslation( -0.5f, -0.5f, 0.0f ).Transpose(),
														  Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
														  Matrix4::CreateOrthographic( 1.0f, 1.0f, Near, Far ).Transpose() );

		mShaders["AmbientOcclusion"].SetShaderParameters( PixelShader,
														  0,
														  AmbientRangeOfInfluence,
														  AmbientScaleFactor,
														  AmbientContrast,
														  EnableAmbientOcculsion ? 1 : 0 );

		mShaders["AmbientOcclusion"].SetTexture( PixelShader, 0, &mGBuffers, 0 );
		mShaders["AmbientOcclusion"].SetTexture( PixelShader, 1, &mGBuffers, 1 );

		mQuad.BindBuffers();

		mShaders["AmbientOcclusion"].Render( mQuad.GetIndexCount() );

		mSceneBuffer.SetRenderTarget();
		mGraphics.GetPipeline()->SetZBuffer( true );

		BlurAmbientMap();
	}

	void Renderer::AmbientPass()
	{
		mGraphics.GetPipeline()->SetZBuffer( false );
		//mGraphics.GetPipeline()->SetAdditiveBlend();

		HDRTexture * skyMap = SYSTEM_MANAGER.GetGameComponentFromSystem<HDRTextureManager>()->GetTexture( SkyName ).first;
		HDRTexture * irradianceMap = SYSTEM_MANAGER.GetGameComponentFromSystem<HDRTextureManager>()->GetTexture( SkyName ).second;

		// Full screen quad
		mShaders["Ambient"].SetShaderParameters( VertexShader,
												 0,
												 Matrix4::CreateTranslation( -0.5f, -0.5f, 0.0f ).Transpose(),
												 Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
												 Matrix4::CreateOrthographic( 1.0f, 1.0f, Near, Far ).Transpose() );

		mShaders["Ambient"].SetShaderParameters( PixelShader,
												 0,
												 mAmbient,
												 mCamera->GetPosition(),
												 skyMap->GetTextureDimension() );

		mShaders["Ambient"].SetShaderArrayParameters( PixelShader, 1, mHammersley );

		// Diffuse Texture
		mShaders["Ambient"].SetTexture( PixelShader, 0, &mGBuffers, 0 );
		mShaders["Ambient"].SetTexture( PixelShader, 1, &mGBuffers, 1 );
		mShaders["Ambient"].SetTexture( PixelShader, 2, &mGBuffers, 2 );
		mShaders["Ambient"].SetTexture( PixelShader, 3, &mGBuffers, 3 );
		mShaders["Ambient"].SetTexture( PixelShader, 4, skyMap );
		mShaders["Ambient"].SetTexture( PixelShader, 5, irradianceMap );
		mShaders["Ambient"].BindTexture( PixelShader, 6, &mBlurAmbientBuffer );

		mQuad.BindBuffers();

		mShaders["Ambient"].Render( mQuad.GetIndexCount() );

		mShaders["Ambient"].UnBindTexture( PixelShader, 6 );

		mGraphics.GetPipeline()->SetNoBlend();
		mGraphics.GetPipeline()->SetZBuffer( true );
	}

	void Renderer::DepthPass( const Vector3f & lightPos, Matrix4 & viewLight, Matrix4 & perspLight, f32 & lightNear, f32 & lightFar )
	{
		static const f32 PI_QUARTER = Math::Pi * 0.25f;
		
		// @todo: scene's boundng box (hardcoded)
		Vector3f minPoint = Vector3f( -7.5f, -7.5f, -7.5f );
		Vector3f maxPoint = Vector3f( 7.5f, 3.0f, 7.5f );
		Vector3f center = minPoint + 0.5f * ( maxPoint - minPoint );
		Vector3f boundingPoints[8] = {
									     minPoint,											//!< 000
										 Vector3f( minPoint.x, minPoint.y, maxPoint.z ),	//!< 001 
										 Vector3f( minPoint.x, maxPoint.y, minPoint.z ),	//!< 010 
										 Vector3f( minPoint.x, maxPoint.y, maxPoint.z ),	//!< 011
										 Vector3f( maxPoint.x, minPoint.y, minPoint.z ),	//!< 100
										 Vector3f( maxPoint.x, minPoint.y, maxPoint.z ),	//!< 101
										 Vector3f( maxPoint.x, maxPoint.y, minPoint.z ),	//!< 110
										 Vector3f( maxPoint.x, maxPoint.y, maxPoint.z ),	//!< 111
									 };

		// Calculating near and far distance (To minimize depth range)
		lightNear = 1.0f;
		lightFar = -FLT_MAX;

		// ---------------------------------------------- Depth ---------------------------------------------------
		mDepthBuffer.ClearRenderTarget( Vector4f::ONE );
		mDepthBuffer.SetRenderTarget();
		mGraphics.GetPipeline()->SetZBuffer( true );
		mGraphics.GetPipeline()->SetFrontFaceCull( true );
		mGraphics.GetPipeline()->SetNoBlend();
		
		// Light looking at scene (Assuming global light is always outside the scene bounding box)
		Vector3f v = center - lightPos;
		v.Normalize();

		if ( fabs( v.Dot( Vector3f::UNIT_Y ) ) >= 1.0f - FLT_EPSILON )
			viewLight = Matrix4::CreateLookAt( lightPos, center, Vector3f::UNIT_X );
		else
			viewLight = Matrix4::CreateLookAt( lightPos, center, Vector3f::UNIT_Y );
		
		// Find width and height based on distance of light and scene's bounding box position
		f32 minCos = 1.0f;
		u32 minIndex = 9;
		Vector3f lightVec = center - lightPos;
		lightVec.Normalize();
		for ( u32 index = 0; index < 8; ++index )
		{
			Vector3f pointVec = boundingPoints[index] - lightPos;
			
			f32 distance = pointVec.Dot( lightVec );
			if ( distance > 0.0f )
				lightFar = std::max( lightFar, distance );

			pointVec.Normalize();
			f32 cosA = lightVec.Dot( pointVec );
		
			if ( cosA >= 0.0f && minCos > cosA )
			{
				minCos = cosA;
				minIndex = index;
				//lightNear = distance;
			}
		}

		if ( lightFar < lightNear )
		{
			//lightNear = Near;
			lightFar = Far;
		}
		else
		{
			//lightNear *= 0.9f;
			lightFar *= 1.1f;
		}
		
		// Light inside scene bounding box, use regular glitchy perspective transform
		if ( minIndex == 9 )
		{
			perspLight = Matrix4::CreatePerspectiveFieldOfView( PI_QUARTER, 
																static_cast<f32>( mDepthBuffer.GetWidth() ) / 
																static_cast<f32>( mDepthBuffer.GetHeight() ),
																lightNear,
																lightFar);
		}
		else
		{
			f32 minSin = std::sqrtf( 1.0f - minCos * minCos );
			f32 width = 2.0f * lightNear * minSin / minCos;
			f32 aspectRatio = static_cast<f32>( mDepthBuffer.GetWidth() ) / static_cast<f32>( mDepthBuffer.GetHeight() );
			perspLight = Matrix4::CreatePerspective( aspectRatio * width, width, lightNear, lightFar );
		}
		
		mShaders["ExponentialDepth"].SetShaderParameters( PixelShader,
														  0,
														  Vector3f( lightNear, lightFar, ExponentConstant ) );		//!< @hardcode: hardcoded c value

		for ( auto & i : mInstanceList )
		{
			Matrix4 world = i.mTransform * mGraphics.GetPipeline()->GetWorld();

			// Set Transform for this instance
			mShaders["ExponentialDepth"].SetShaderParameters( VertexShader,
															  0, 
															  world.Transpose(),
															  viewLight.Transpose(),
															  perspLight.Transpose(),
															  world.Inverse().Transpose() );

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			i.mMesh->BindBuffers();

			mShaders["ExponentialDepth"].Render( i.mMesh->GetIndexCount() );
		}		

		// ----------------------------------------- Blur ------------------------------------------
		//mGraphics.GetPipeline()->SetBackBufferRenderTarget();
		mSceneBuffer.SetRenderTarget();
		BlurDepthMap();
	}

	void Renderer::DirectionalDepthPass( const Vector3f & lightPos, const Vector3f & direction,
										 const Vector3f & dimension, Matrix4 & viewLight, 
										 Matrix4 & perspLight, f32 & lightNear, f32 & lightFar )
	{
		// @todo: Calculating near and far distance (To minimize depth range)
		lightNear = 0.1f;
		lightFar = 50.0f;

		// ---------------------------------------------- Depth ---------------------------------------------------
		mDepthBuffer.ClearRenderTarget( Vector4f::ONE );
		mDepthBuffer.SetRenderTarget();
		mGraphics.GetPipeline()->SetZBuffer( true );
		//mGraphics.GetPipeline()->SetFrontFaceCull( true );
		mGraphics.GetPipeline()->SetBackFaceCull( true );
		mGraphics.GetPipeline()->SetNoBlend();
		
		// Light looking at scene
		if ( fabs( direction.Dot( Vector3f::UNIT_Y ) ) >= 1.0f - FLT_EPSILON )
			viewLight = Matrix4::CreateLookAt( lightPos, lightPos + 30.0f * direction, Vector3f::UNIT_X );
		else
			viewLight = Matrix4::CreateLookAt( lightPos, lightPos + 30.0f * direction, Vector3f::UNIT_Y );
		
		perspLight = Matrix4::CreateOrthographic( dimension.x, 
												  dimension.y,
												  lightNear,
												  lightFar);

		for ( auto & i : mInstanceList )
		{
			Matrix4 world = i.mTransform * mGraphics.GetPipeline()->GetWorld();

			// Set Transform for this instance
			mShaders["Depth"].SetShaderParameters( VertexShader,
												   0, 
												   world.Transpose(),
												   viewLight.Transpose(),
												   perspLight.Transpose(),
												   world.Inverse().Transpose() );

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			i.mMesh->BindBuffers();

			mShaders["Depth"].Render( i.mMesh->GetIndexCount() );
		}		

		//mGraphics.GetPipeline()->SetBackBufferRenderTarget();
		mSceneBuffer.SetRenderTarget();
	}

	void Renderer::BlurDepthMap()
	{
		mShaders["HBlur"].SetShaderParameters( ComputeShader, 0, mDepthBuffer.GetWidth(), mDepthBuffer.GetHeight(), mBlurKernelRadius );
		mShaders["HBlur"].SetShaderArrayParameters( ComputeShader, 1, mWeights );
		mShaders["HBlur"].SetTexture( ComputeShader, 0, &mDepthBuffer );
		//mShaders["HBlur"].SetTexture( ComputeShader, 0, &mGBuffers, 2 );
		mShaders["HBlur"].BindUnorderedTexture( 0, &mHBlurDepthBuffer );
		mShaders["HBlur"].Dispatch( mDepthBuffer.GetWidth() / BATCH_X_NUM, mDepthBuffer.GetHeight(), 1 );
		mShaders["HBlur"].UnBindUnorderedTexture( 0 );

		mShaders["VBlur"].SetShaderParameters( ComputeShader, 0, mDepthBuffer.GetWidth(), mDepthBuffer.GetHeight(), mBlurKernelRadius );
		mShaders["VBlur"].SetShaderArrayParameters( ComputeShader, 1, mWeights );
		mShaders["VBlur"].BindTexture( ComputeShader, 0, &mHBlurDepthBuffer );
		mShaders["VBlur"].BindUnorderedTexture( 0, &mBlurDepthBuffer );
		mShaders["VBlur"].Dispatch( mHBlurDepthBuffer.GetWidth(), mHBlurDepthBuffer.GetHeight() / BATCH_Y_NUM, 1 );
		mShaders["VBlur"].UnBindTexture( ComputeShader, 0 );
		mShaders["VBlur"].UnBindUnorderedTexture( 0 );
	}

	void Renderer::BlurAmbientMap()
	{
		mShaders["HBilateralBlur"].SetShaderParameters( ComputeShader, 0, mAmbientBuffer.GetWidth(), mAmbientBuffer.GetHeight(), mBlurKernelRadius );
		mShaders["HBilateralBlur"].SetShaderArrayParameters( ComputeShader, 1, mWeights );
		mShaders["HBilateralBlur"].SetTexture( ComputeShader, 0, &mAmbientBuffer );
		mShaders["HBilateralBlur"].SetTexture( ComputeShader, 1, &mGBuffers, 1 );
		mShaders["HBilateralBlur"].BindUnorderedTexture( 0, &mHBlurAmbientBuffer );
		mShaders["HBilateralBlur"].Dispatch( mAmbientBuffer.GetWidth() / BATCH_X_NUM, mAmbientBuffer.GetHeight(), 1 );
		mShaders["HBilateralBlur"].UnBindUnorderedTexture( 0 );

		mShaders["VBilateralBlur"].SetShaderParameters( ComputeShader, 0, mHBlurAmbientBuffer.GetWidth(), mHBlurAmbientBuffer.GetHeight(), mBlurKernelRadius );
		mShaders["VBilateralBlur"].SetShaderArrayParameters( ComputeShader, 1, mWeights );
		mShaders["VBilateralBlur"].BindTexture( ComputeShader, 0, &mHBlurAmbientBuffer );
		mShaders["VBilateralBlur"].SetTexture( ComputeShader, 1, &mGBuffers, 1 );
		mShaders["VBilateralBlur"].BindUnorderedTexture( 0, &mBlurAmbientBuffer );
		mShaders["VBilateralBlur"].Dispatch( mHBlurAmbientBuffer.GetWidth(), mHBlurAmbientBuffer.GetHeight() / BATCH_Y_NUM, 1 );
		mShaders["VBilateralBlur"].UnBindTexture( ComputeShader, 0 );
		mShaders["VBilateralBlur"].UnBindUnorderedTexture( 0 );
	}

	void Renderer::TraceSamplePass( const DirectionalLight & light, const Matrix4 & shadow,
									const Matrix4 & invPV, f32 lightNear, f32 lightFar )
	{
		// Cube
		Mesh * beam = SYSTEM_MANAGER.GetGameComponentFromSystem<MeshManager>()->GetMesh( "Cube" );

		Vector3f lightDimension = light.GetDimension();

		Matrix4 rotation;
		if ( fabs( light.GetDirection().Dot( Vector3f::UNIT_Y ) ) >= 1.0f - FLT_EPSILON )
			rotation = Matrix4::CreateLookAt( Vector3f::ZERO, light.GetDirection(), Vector3f::UNIT_X );
		else
			rotation = Matrix4::CreateLookAt( Vector3f::ZERO, light.GetDirection(), Vector3f::UNIT_Y );
		rotation = rotation.Inverse();

		Matrix4 transform = ( Matrix4::CreateTranslation( light.GetPosition() + 0.5f * lightDimension.z * light.GetDirection() ) * 
							 rotation *
							 Matrix4::CreateScale( lightDimension ) *
							mGraphics.GetPipeline()->GetWorld() ).Transpose();

		// ----------------------------------------------- Exit Trace ----------------------------------------------------
		mExitTraceBuffer.ClearRenderTarget( Vector4f::ZERO );
		mExitTraceBuffer.SetRenderTarget();
		mGraphics.GetPipeline()->SetZBuffer( false );
		mGraphics.GetPipeline()->SetFrontFaceCull( true );
		mGraphics.GetPipeline()->SetNoBlend();

		mShaders["ExitTrace"].SetShaderParameters( VertexShader,
											   0,
											   transform,
											   mCamera->GetView().Transpose(),
											   mGraphics.GetPipeline()->GetProjection().Transpose() );

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		beam->BindBuffers();

		mShaders["ExitTrace"].Render( beam->GetIndexCount() );

		// ---------------------------------------------- Trace Sample ---------------------------------------------------
		if ( !TraceDownSample )
		{
			mTraceBuffer.SetRenderTarget();
			mGraphics.GetPipeline()->SetZBuffer( false );
			mGraphics.GetPipeline()->SetFrontFaceCull( true );
			//mGraphics.GetPipeline()->SetBackFaceCull( true );

			//mGraphics.GetPipeline()->SetNoBlend();
			mGraphics.GetPipeline()->SetAdditiveBlend();

			// Texture for this instance
			mShaders["Trace"].SetTexture( PixelShader, 0, &mGBuffers, 0 );
			mShaders["Trace"].SetTexture( PixelShader, 1, &mGBuffers, 1 );
			mShaders["Trace"].SetTexture( PixelShader, 2, &mGBuffers, 2 );
			mShaders["Trace"].SetTexture( PixelShader, 3, &mGBuffers, 3 );
			mShaders["Trace"].SetTexture( PixelShader, 4, &mDepthBuffer );
			Texture * jitter = TraceJitter ? SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTexture( "noise" )
										   : SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTexture( "black" );
			mShaders["Trace"].SetTexture( PixelShader, 5, jitter );

			mShaders["Trace"].SetShaderParameters( VertexShader,
												   0,
												   transform,
												   mCamera->GetView().Transpose(),
												   mGraphics.GetPipeline()->GetProjection().Transpose(),
												   transform.Inverse() );

			mShaders["Trace"].SetShaderParameters( PixelShader,
												   0,
												   light.GetColor(),
												   mCamera->GetPosition(),
												   static_cast< f32 >( mTraceBuffer.GetWidth() ),
												   light.GetPosition(),
												   static_cast< f32 >( mTraceBuffer.GetHeight() ),
												   light.GetDirection(),
												   TraceSampleRate,
												   Vector2f( lightNear, lightFar ),
												   Vector2f::ZERO,
												   light.GetDimension() );

			mShaders["Trace"].SetShaderParameters( PixelShader,
												   1,
												   shadow.Transpose(),
												   invPV.Transpose() );

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			beam->BindBuffers();

			mShaders["Trace"].Render( beam->GetIndexCount() );

			mShaders["Trace"].UnBindTexture( PixelShader, 4 );
		}
		else
		{
			mDTraceBuffer.SetRenderTarget();
			mGraphics.GetPipeline()->SetZBuffer( false );
			mGraphics.GetPipeline()->SetFrontFaceCull( true );
			//mGraphics.GetPipeline()->SetBackFaceCull( true );

			//mGraphics.GetPipeline()->SetNoBlend();
			mGraphics.GetPipeline()->SetAdditiveBlend();

			// Texture for this instance
			mShaders["Trace"].SetTexture( PixelShader, 0, &mGBuffers, 0 );
			mShaders["Trace"].SetTexture( PixelShader, 1, &mGBuffers, 1 );
			mShaders["Trace"].SetTexture( PixelShader, 2, &mGBuffers, 2 );
			mShaders["Trace"].SetTexture( PixelShader, 3, &mGBuffers, 3 );
			mShaders["Trace"].SetTexture( PixelShader, 4, &mDepthBuffer );
			Texture * jitter = TraceJitter ? SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTexture( "noise" )
										   : SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTexture( "black" );
			mShaders["Trace"].SetTexture( PixelShader, 5, jitter );

			mShaders["Trace"].SetShaderParameters( VertexShader,
												   0,
												   transform,
												   mCamera->GetView().Transpose(),
												   mGraphics.GetPipeline()->GetProjection().Transpose(),
												   transform.Inverse() );

			mShaders["Trace"].SetShaderParameters( PixelShader,
												   0,
												   light.GetColor(),
												   mCamera->GetPosition(),
												   static_cast< f32 >( mDTraceBuffer.GetWidth() ),
												   light.GetPosition(),
												   static_cast< f32 >( mDTraceBuffer.GetHeight() ),
												   light.GetDirection(),
												   TraceSampleRate,
												   Vector2f( lightNear, lightFar ),
												   Vector2f::ZERO,
												   light.GetDimension() );

			mShaders["Trace"].SetShaderParameters( PixelShader,
												   1,
												   shadow.Transpose(),
												   invPV.Transpose() );

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			beam->BindBuffers();

			mShaders["Trace"].Render( beam->GetIndexCount() );

			mShaders["Trace"].UnBindTexture( PixelShader, 4 );

			// ---------------------------------------------- Blur ---------------------------------------------------
			GRAPHICS.GetPipeline()->ResetViewport();
			mGraphics.GetPipeline()->SetBackFaceCull( true );
			mGraphics.GetPipeline()->SetNoBlend();
			if ( TraceEdgeBlur )
				EdgeBlurPass();
		}
		
		// ---------------------------------------------- Lighting ---------------------------------------------------
		mSceneBuffer.SetRenderTarget();
		mGraphics.GetPipeline()->SetAdditiveBlend();
		//mGraphics.GetPipeline()->SetNoBlend();
		mGraphics.GetPipeline()->SetZBuffer( false );
		//mGraphics.GetPipeline()->SetBackFaceCull( true );
		mGraphics.GetPipeline()->SetFrontFaceCull( true );

		// Texture for this instance
		mShaders["DirLighting"].SetTexture( PixelShader, 0, &mGBuffers, 0 );
		mShaders["DirLighting"].SetTexture( PixelShader, 1, &mGBuffers, 1 );
		mShaders["DirLighting"].SetTexture( PixelShader, 2, &mGBuffers, 2 );
		mShaders["DirLighting"].SetTexture( PixelShader, 3, &mGBuffers, 3 );
		mShaders["DirLighting"].SetTexture( PixelShader, 4, &mDepthBuffer );

		// Full screen quad
		mShaders["DirLighting"].SetShaderParameters( VertexShader,
													 0,
													 transform,
													 mCamera->GetView().Transpose(),
													 mGraphics.GetPipeline()->GetProjection().Transpose() );

		Vector3f lightSide( rotation.m[0][0], rotation.m[0][1], rotation.m[0][2] );
		Vector3f lightUp( rotation.m[1][0], rotation.m[1][1], rotation.m[1][2] );

		mShaders["DirLighting"].SetShaderParameters( PixelShader,
													 0,
													 light.GetColor(),
													 mCamera->GetPosition(),
													 static_cast< f32 >( mSceneBuffer.GetWidth() ),
													 light.GetPosition(),
													 static_cast< f32 >( mSceneBuffer.GetHeight() ),
													 light.GetDirection(),
													 light.GetDimension().x * 0.5f,
													 lightSide,
													 light.GetDimension().y * 0.5f,
													 lightUp,
													 light.GetDimension().z );

		//shadow = dbias * shadow;
		mShaders["DirLighting"].SetShaderParameters( PixelShader,
													 1,
													 shadow.Transpose() );

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		beam->BindBuffers();

		mShaders["DirLighting"].Render( beam->GetIndexCount() );

		mShaders["DirLighting"].UnBindTexture( PixelShader, 4 );
	}

	void Renderer::EdgeBlurPass()
	{
		// ----------------------------- Edge Detection ---------------------------------
		mGradientTraceBuffer.ClearRenderTarget( Vector4f::ZERO );
		mGradientTraceBuffer.SetRenderTarget();
		mGraphics.GetPipeline()->SetZBuffer( false );

		// Set Transform for this instance
		mShaders["EdgeDetection"].SetShaderParameters( VertexShader,
													  0,
													  Matrix4::CreateTranslation( 0.0f, 0.0f, 0.0f ).Transpose(),
													  Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
													  Matrix4::CreateOrthographicOffCenter( 0.0f, 1.0f, 0.0f, 1.0f, Near, Far ).Transpose() );

		// Texture for this instance
		mShaders["EdgeDetection"].SetTexture( PixelShader, 0, &mDTraceBuffer );

		Vector2f invDimension( 1.0f / static_cast<f32>( mDTraceBuffer.GetWidth() ),
							   1.0f / static_cast<f32>( mDTraceBuffer.GetHeight() ) );
		mShaders["EdgeDetection"].SetShaderParameters( PixelShader, 0, invDimension );

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		mQuad.BindBuffers();

		mShaders["EdgeDetection"].Render( mQuad.GetIndexCount() );

		// ---------------------------------- Edge Blur -------------------------------------
		mGradientBlurTraceBuffer.ClearRenderTarget( Vector4f::ZERO );
		mGradientBlurTraceBuffer.SetRenderTarget();

		// Set Transform for this instance
		mShaders["EdgeBlur"].SetShaderParameters( VertexShader,
												  0,
												  Matrix4::CreateTranslation( 0.0f, 0.0f, 0.0f ).Transpose(),
												  Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
												  Matrix4::CreateOrthographicOffCenter( 0.0f, 1.0f, 0.0f, 1.0f, Near, Far ).Transpose() );

		// Texture for this instance
		mShaders["EdgeBlur"].SetTexture( PixelShader, 0, &mDTraceBuffer );
		mShaders["EdgeBlur"].SetTexture( PixelShader, 1, &mGradientTraceBuffer );

		invDimension.x = 1.0f / static_cast<f32>( mGradientTraceBuffer.GetWidth() );
		invDimension.y = 1.0f / static_cast<f32>( mGradientTraceBuffer.GetHeight() );
		mShaders["EdgeBlur"].SetShaderParameters( PixelShader, 0, invDimension );

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		mQuad.BindBuffers();

		mShaders["EdgeBlur"].Render( mQuad.GetIndexCount() );

		// ------------------------------ Blur Pass --------------------------------------------
		mSceneBuffer.SetRenderTarget();

		mShaders["HBlur"].SetShaderParameters( ComputeShader, 0, mGradientBlurTraceBuffer.GetWidth(), mGradientBlurTraceBuffer.GetHeight(), TRACE_RADIUS );
		mShaders["HBlur"].SetShaderArrayParameters( ComputeShader, 1, mTWeights );
		mShaders["HBlur"].SetTexture( ComputeShader, 0, &mGradientBlurTraceBuffer );
		//mShaders["HBlur"].SetTexture( ComputeShader, 0, &mGBuffers, 2 );
		mShaders["HBlur"].BindUnorderedTexture( 0, &mHBlurTraceBuffer );
		mShaders["HBlur"].Dispatch( mGradientBlurTraceBuffer.GetWidth() / BATCH_X_NUM, mGradientBlurTraceBuffer.GetHeight(), 1 );
		mShaders["HBlur"].UnBindUnorderedTexture( 0 );

		mShaders["VBlur"].SetShaderParameters( ComputeShader, 0, mGradientBlurTraceBuffer.GetWidth(), mGradientBlurTraceBuffer.GetHeight(), TRACE_RADIUS );
		mShaders["VBlur"].SetShaderArrayParameters( ComputeShader, 1, mTWeights );
		mShaders["VBlur"].BindTexture( ComputeShader, 0, &mHBlurTraceBuffer );
		mShaders["VBlur"].BindUnorderedTexture( 0, &mBlurTraceBuffer );
		mShaders["VBlur"].Dispatch( mHBlurTraceBuffer.GetWidth(), mHBlurTraceBuffer.GetHeight() / BATCH_Y_NUM, 1 );
		mShaders["VBlur"].UnBindTexture( ComputeShader, 0 );
		mShaders["VBlur"].UnBindUnorderedTexture( 0 );

		// -------------------------------------------------------------------------------------
		mGraphics.GetPipeline()->SetZBuffer( true );
	}

	void Renderer::BlendLightPass()
	{
		// ---------------------------------------------- Blend Light ---------------------------------------------------
		mGraphics.GetPipeline()->SetZBuffer( false );
		mGraphics.GetPipeline()->SetBackFaceCull( true );
		//mGraphics.GetPipeline()->SetAlphaBlend();
		mGraphics.GetPipeline()->SetAdditiveBlend();

		// Texture for this instance
		if ( !TraceDownSample )
		{
			mShaders["BlendLight"].SetTexture( PixelShader, 0, &mTraceBuffer );
		}
		else
		{
			if ( !TraceEdgeBlur )
				mShaders["BlendLight"].SetTexture( PixelShader, 0, &mDTraceBuffer );
			else
				mShaders["BlendLight"].BindTexture( PixelShader, 0, &mBlurTraceBuffer );
		}

		// Full screen quad
		mShaders["BlendLight"].SetShaderParameters( VertexShader,
													0,
													Matrix4::CreateTranslation( -0.5f, -0.5f, 0.0f ).Transpose(),
													Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
													Matrix4::CreateOrthographic( 1.0f, 1.0f, Near, Far ).Transpose() );

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		mQuad.BindBuffers();

		mShaders["BlendLight"].Render( mQuad.GetIndexCount() );

		mShaders["BlendLight"].UnBindTexture( PixelShader, 0 );

		mGraphics.GetPipeline()->SetZBuffer( true );
		mGraphics.GetPipeline()->SetNoBlend();
	}

	void Renderer::LightPass()
	{
		// DirectX left handed coordinates system screw me up
		static const Matrix4 bias = Matrix4::CreateTranslation( 0.5f, 0.5f, 0.5f ) * Matrix4::CreateScale( 0.5f, -0.5f, 0.5f );

		for ( auto & i : mGlobalLights )
		{
			// ---------------------------------------------- Depth ---------------------------------------------------
			Matrix4 viewLight, perspLight;
			f32 lightNear, lightFar;
			DepthPass( i.GetPosition(), viewLight, perspLight, lightNear, lightFar );

			// --------------------------------------------- Lighting -------------------------------------------------
			//mGraphics.GetPipeline()->SetBackBufferRenderTarget();
			mSceneBuffer.SetRenderTarget();
			mGraphics.GetPipeline()->SetAdditiveBlend();
			mGraphics.GetPipeline()->SetZBuffer( false );
			mGraphics.GetPipeline()->SetBackFaceCull( true );

			// Texture for this instance
			mShaders["Lighting"].SetTexture( PixelShader, 0, &mGBuffers, 0 );
			mShaders["Lighting"].SetTexture( PixelShader, 1, &mGBuffers, 1 );
			mShaders["Lighting"].SetTexture( PixelShader, 2, &mGBuffers, 2 );
			mShaders["Lighting"].SetTexture( PixelShader, 3, &mGBuffers, 3 );
			//mShaders["Lighting"].SetTexture( PixelShader, 4, &mDepthBuffer );
			mShaders["Lighting"].BindTexture( PixelShader, 4, &mBlurDepthBuffer );

			// Full screen quad
			mShaders["Lighting"].SetShaderParameters( VertexShader,
													  0,
													  Matrix4::CreateTranslation( -0.5f, -0.5f, 0.0f ).Transpose(),
													  Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
													  Matrix4::CreateOrthographic( 1.0f, 1.0f, Near, Far ).Transpose() );

			mShaders["Lighting"].SetShaderParameters( PixelShader,
													  0,
													  i.GetColor(),
													  mCamera->GetPosition(),
													  //mGBuffers.GetResolution().x,
													  static_cast< f32 >( mSceneBuffer.GetWidth() ),
													  i.GetPosition(),
													  //mGBuffers.GetResolution().y,
													  static_cast< f32 >( mSceneBuffer.GetHeight() ),
													  Vector3f( lightNear, lightFar, ExponentConstant ),
													  -1.0f );

			Matrix4 shadow = bias * perspLight * viewLight;
			mShaders["Lighting"].SetShaderParameters( PixelShader,
													  1,
													  shadow.Transpose(),
													  1 );

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			mQuad.BindBuffers();

			mShaders["Lighting"].Render( mQuad.GetIndexCount() );

			mShaders["Lighting"].UnBindTexture( PixelShader, 4 );
		}

		static const Matrix4 dbias = Matrix4::CreateTranslation( 0.5f, 0.5f, 0.0f ) * Matrix4::CreateScale( 0.5f, -0.5f, 1.0f );
		for ( auto & i : mDirectionalLights )
		{
			// ---------------------------------------------- Depth ---------------------------------------------------
			Matrix4 viewLight, perspLight;
			f32 lightNear, lightFar;
			DirectionalDepthPass( i.GetPosition(), i.GetDirection(), i.GetDimension(), viewLight, perspLight, lightNear, lightFar );

			// ------------------------------------------- Trace Sample -----------------------------------------------
			Matrix4 shadow = perspLight * viewLight;
			Matrix4 invPV = mGraphics.GetPipeline()->GetProjection() * mCamera->GetView();
			TraceSamplePass( i, shadow, invPV.Inverse(), lightNear, lightFar );
		}

		//mGraphics.GetPipeline()->SetBackBufferRenderTarget();
		mSceneBuffer.SetRenderTarget();
		mGraphics.GetPipeline()->SetAdditiveBlend();
		mGraphics.GetPipeline()->SetZBuffer( false );
		mGraphics.GetPipeline()->SetBackFaceCull( true );

		Mesh * sphere = SYSTEM_MANAGER.GetGameComponentFromSystem<MeshManager>()->GetMesh( "Sphere" );

		for ( auto & i : mLocalLights )
		{
			// Texture for this instance
			mShaders["Lighting"].SetTexture( PixelShader, 0, &mGBuffers, 0 );
			mShaders["Lighting"].SetTexture( PixelShader, 1, &mGBuffers, 1 );
			mShaders["Lighting"].SetTexture( PixelShader, 2, &mGBuffers, 2 );
			mShaders["Lighting"].SetTexture( PixelShader, 3, &mGBuffers, 3 );

			// Sphere
			Matrix4 world = Matrix4::CreateTranslation( i.GetPosition() ) * Matrix4::CreateScale( i.GetRadius() ) * mGraphics.GetPipeline()->GetWorld();
			mShaders["Lighting"].SetShaderParameters( VertexShader,
													  0, 
													  world.Transpose(),
													  mCamera->GetView().Transpose(),
													  mGraphics.GetPipeline()->GetProjection().Transpose(),
													  world.Inverse().Transpose() );

			mShaders["Lighting"].SetShaderParameters( PixelShader,
													  0,
													  i.GetColor(),
													  mCamera->GetPosition(),
													  //mGBuffers.GetResolution().x,
													  static_cast< f32 >( mSceneBuffer.GetWidth() ),
													  i.GetPosition(),
													  //mGBuffers.GetResolution().y,
													  static_cast< f32 >( mSceneBuffer.GetHeight() ),
													  Vector3f( Near, Far, ExponentConstant ),
													  i.GetRadius() );

			mShaders["Lighting"].SetShaderParameters( PixelShader,
													  1,
													  Matrix4::IDENTITY,
													  0 );

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			sphere->BindBuffers();

			mShaders["Lighting"].Render( sphere->GetIndexCount() );
		}

		//mGraphics.GetPipeline()->SetBackBufferRenderTarget();
		mGraphics.GetPipeline()->SetZBuffer( true );
		mGraphics.GetPipeline()->SetNoBlend();
	}

	void Renderer::DrawFinalScene()
	{
		mGraphics.GetPipeline()->SetZBuffer( false );

		// Set Transform for this instance
		Matrix4 transform = Matrix4::CreateTranslation( mSceneOffset.x, mSceneOffset.y, 0.0f ) * Matrix4::CreateScale( mSceneDimension.x, mSceneDimension.y, 1.0f );
		mShaders["Quad"].SetShaderParameters( VertexShader,
											  0,
											  transform.Transpose(),
											  Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
											  Matrix4::CreateOrthographicOffCenter( 0.0f, 1.0f, 0.0f, 1.0f, Near, Far ).Transpose() );

		// Texture for this instance
		mShaders["Quad"].SetTexture( PixelShader, 0, &mSceneBuffer );

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		mQuad.BindBuffers();

		mShaders["Quad"].Render( mQuad.GetIndexCount() );

		mGraphics.GetPipeline()->SetZBuffer( true );
	}

	void Renderer::ToggleDrawGBuffers()
	{
		mDrawG = !mDrawG;
	}

	void Renderer::ToggleDrawShadowMap()
	{
		mDrawShadowMap = !mDrawShadowMap;
	}

	void Renderer::ToggleDrawBlur()
	{
		mDrawBlur = !mDrawBlur;
	}

	void Renderer::DrawGBuffers()
	{
#if 0
		mGraphics.GetPipeline()->SetZBuffer( false );

		for ( u32 i = 0; i < 4; ++i )
		{
			// Set Transform for this instance
			mShaders["Quad"].SetShaderParameters( VertexShader,
												  0,
												  Matrix4::CreateTranslation( 1.0f * ( i % 2 ), 1.0f * ( i > 1 ? 1.0f : 0.0f ), 0.0f ).Transpose(),
												  Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
												  Matrix4::CreateOrthographicOffCenter( 0.0f, 2.0f, 0.0f, 2.0f, Near, Far ).Transpose() );

			// Texture for this instance
			mShaders["Quad"].SetTexture( PixelShader, 0, &mGBuffers, i );

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			mQuad.BindBuffers();

			mShaders["Quad"].Render( mQuad.GetIndexCount() );
		}

		mGraphics.GetPipeline()->SetZBuffer( true );
#else
		mGraphics.GetPipeline()->SetZBuffer( false );

		// Set Transform for this instance
		mShaders["Quad"].SetShaderParameters( VertexShader,
											  0,
											  Matrix4::CreateTranslation( 0.0f, 0.0f, 0.0f ).Transpose(),
											  Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
											  Matrix4::CreateOrthographicOffCenter( 0.0f, 1.0f, 0.0f, 1.0f, Near, Far ).Transpose() );

		// Texture for this instance
		mShaders["Quad"].SetTexture( PixelShader, 0, &mGradientBlurTraceBuffer );

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		mQuad.BindBuffers();

		mShaders["Quad"].Render( mQuad.GetIndexCount() );

		// Unbind texture for compute shader uses later on
		mShaders["Quad"].UnBindTexture( PixelShader, 0 );

		mGraphics.GetPipeline()->SetZBuffer( true );
#endif
	}

	void Renderer::DrawDepthBuffer()
	{
#if 0
		mGraphics.GetPipeline()->SetZBuffer( false );

		// Set Transform for this instance
		mShaders["Quad"].SetShaderParameters( VertexShader,
											  0,
											  Matrix4::CreateTranslation( 0.0f, 0.0f, 0.0f ).Transpose(),
											  Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
											  Matrix4::CreateOrthographicOffCenter( 0.0f, 4.0f, 0.0f, 4.0f, Near, Far ).Transpose() );

		// Texture for this instance
		mShaders["Quad"].SetTexture( PixelShader, 0, &mDepthBuffer );

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		mQuad.BindBuffers();

		mShaders["Quad"].Render( mQuad.GetIndexCount() );

		// Set Transform for this instance
		mShaders["Quad"].SetShaderParameters( VertexShader,
											  0,
											  Matrix4::CreateTranslation( 1.0f, 0.0f, 0.0f ).Transpose(),
											  Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
											  Matrix4::CreateOrthographicOffCenter( 0.0f, 4.0f, 0.0f, 4.0f, Near, Far ).Transpose() );

		// Texture for this instance
		mShaders["Quad"].BindTexture( PixelShader, 0, &mHBlurDepthBuffer );

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		mQuad.BindBuffers();

		mShaders["Quad"].Render( mQuad.GetIndexCount() );

		// Unbind texture for compute shader uses later on
		mShaders["Quad"].UnBindTexture( PixelShader, 0 );

		// Set Transform for this instance
		mShaders["Quad"].SetShaderParameters( VertexShader,
											  0,
											  Matrix4::CreateTranslation( 2.0f, 0.0f, 0.0f ).Transpose(),
											  Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
											  Matrix4::CreateOrthographicOffCenter( 0.0f, 4.0f, 0.0f, 4.0f, Near, Far ).Transpose() );

		// Texture for this instance
		mShaders["Quad"].BindTexture( PixelShader, 0, &mBlurDepthBuffer );

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		mQuad.BindBuffers();

		mShaders["Quad"].Render( mQuad.GetIndexCount() );

		// Unbind texture for compute shader uses later on
		mShaders["Quad"].UnBindTexture( PixelShader, 0 );

		mGraphics.GetPipeline()->SetZBuffer( true );
#else
		mGraphics.GetPipeline()->SetZBuffer( false );

		// Set Transform for this instance
		mShaders["Quad"].SetShaderParameters( VertexShader,
											  0,
											  Matrix4::CreateTranslation( 0.0f, 0.0f, 0.0f ).Transpose(),
											  Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
											  Matrix4::CreateOrthographicOffCenter( 0.0f, 1.0f, 0.0f, 1.0f, Near, Far ).Transpose() );

		// Texture for this instance
		//mShaders["Quad"].BindTexture( PixelShader, 0, &mHBlurAmbientBuffer );
		//mShaders["Quad"].SetTexture( PixelShader, 0, &mGradientBlurTraceBuffer );
		mShaders["Quad"].BindTexture( PixelShader, 0, &mHBlurTraceBuffer );
		
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		mQuad.BindBuffers();

		mShaders["Quad"].Render( mQuad.GetIndexCount() );

		// Unbind texture for compute shader uses later on
		mShaders["Quad"].UnBindTexture( PixelShader, 0 );

		mGraphics.GetPipeline()->SetZBuffer( true );
#endif
	}

	void Renderer::DrawBlur()
	{
		mGraphics.GetPipeline()->SetZBuffer( false );

		// Set Transform for this instance
		mShaders["Quad"].SetShaderParameters( VertexShader,
											  0,
											  Matrix4::CreateTranslation( 0.0f, 0.0f, 0.0f ).Transpose(),
											  Matrix4::CreateLookAt( -Vector3f::UNIT_Z, Vector3f::ZERO, Vector3f::UNIT_Y ).Transpose(),
											  Matrix4::CreateOrthographicOffCenter( 0.0f, 1.0f, 0.0f, 1.0f, Near, Far ).Transpose() );

		// Texture for this instance
		//mShaders["Quad"].SetTexture( PixelShader, 0, &mDepthBuffer );
		//mShaders["Quad"].SetTexture( PixelShader, 0, &mTraceBuffer );
		mShaders["Quad"].SetTexture(PixelShader, 0, &mDTraceBuffer);
		//mShaders["Quad"].SetTexture( PixelShader, 0, &mGradientTraceBuffer );
		//mShaders["Quad"].SetTexture(PixelShader, 0, &mExitTraceBuffer);
		//mShaders["Quad"].SetTexture( PixelShader, 0, &mAmbientBuffer );
		//mShaders["Quad"].BindTexture( PixelShader, 0, &mBlurDepthBuffer );
		//mShaders["Quad"].BindTexture( PixelShader, 0, &mBlurAmbientBuffer );

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		mQuad.BindBuffers();

		mShaders["Quad"].Render( mQuad.GetIndexCount() );

		// Unbind texture for compute shader uses later on
		mShaders["Quad"].UnBindTexture( PixelShader, 0 );

		mGraphics.GetPipeline()->SetZBuffer( true );
	}

	void Renderer::DrawDirectionalLight( const DirectionalLightList & lights )
	{
		// Texture for this instance
		mShaders["Flat"].SetTexture( PixelShader, 0, 
									 SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTexture( "white" ) );

		Mesh * light = SYSTEM_MANAGER.GetGameComponentFromSystem<MeshManager>()->GetMesh( "Cube" );

		for ( auto & i : lights )
		{
			Matrix4 rotation;
			if ( fabs( i.GetDirection().Dot( Vector3f::UNIT_Y ) ) >= 1.0f - FLT_EPSILON )
				rotation = Matrix4::CreateLookAt( Vector3f::ZERO, i.GetDirection(), Vector3f::UNIT_X );
			else
				rotation = Matrix4::CreateLookAt( Vector3f::ZERO, i.GetDirection(), Vector3f::UNIT_Y );
			rotation = rotation.Inverse();

			// Set Transform for this instance
			mShaders["Flat"].SetShaderParameters( VertexShader,
												  0,
												  ( Matrix4::CreateTranslation( i.GetPosition() ) * 
													rotation *
												    Matrix4::CreateScale( i.GetDimension().x, i.GetDimension().y, 1.0f ) *
												  mGraphics.GetPipeline()->GetWorld() ).Transpose(),
												  mCamera->GetView().Transpose(),
												  mGraphics.GetPipeline()->GetProjection().Transpose() );

			mShaders["Flat"].SetShaderParameters( PixelShader,
												  0,
												  i.GetColor() );

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			light->BindBuffers();

			mShaders["Flat"].Render( light->GetIndexCount() );
		}
	}

	void Renderer::LinesPass()
	{
		// Texture for this instance
		mShaders["Flat"].SetTexture( PixelShader, 0, SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTexture( "white" ) );

		for ( auto & i : mLineList )
		{
			Matrix4 world = i.mTransform * mGraphics.GetPipeline()->GetWorld();

			mGraphics.GetPipeline()->SetZBuffer( !i.mHUD );

			// Set Transform for this instance
			mShaders["Flat"].SetShaderParameters( VertexShader,
												  0,
												  world.Transpose(),
												  mCamera->GetView().Transpose(),
												  mGraphics.GetPipeline()->GetProjection().Transpose() );

			mShaders["Flat"].SetShaderParameters( PixelShader,
												  0,
												  i.mColor );

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			i.mMesh->BindBuffers();

			mShaders["Flat"].Render( i.mMesh->GetIndexCount() );
		}

		mGraphics.GetPipeline()->SetZBuffer( true );
	}

	void Renderer::SkyDomePass()
	{
		mGraphics.GetPipeline()->SetZBuffer( false );
		mGraphics.GetPipeline()->SetFrontFaceCull( true );
		mGraphics.GetPipeline()->SetAlphaBlend();

		Mesh * sphere = SYSTEM_MANAGER.GetGameComponentFromSystem<MeshManager>()->GetMesh( "Sphere" );
		// Set Transform for this instance
		mShaders["Sky"].SetShaderParameters( VertexShader,
											 0,
											 ( Matrix4::CreateTranslation( mCamera->GetPosition() ) * Matrix4::CreateRotationZ( Math::Pi ) * Matrix4::CreateScale( 50.0f ) ).Transpose(),
											 mCamera->GetView().Transpose(),
											 mGraphics.GetPipeline()->GetProjection().Transpose() );

		// Texture for this instance
		//mShaders["Sky"].SetTexture( PixelShader, 0, SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTexture( SkyName ) );
		if ( ShowSkyDome )
			mShaders["Sky"].SetTexture( PixelShader, 0, SYSTEM_MANAGER.GetGameComponentFromSystem<HDRTextureManager>()->GetTexture( SkyName ).first );
		else
			mShaders["Sky"].SetTexture( PixelShader, 0, SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTexture( "black" ) );

		mShaders["Sky"].SetTexture( PixelShader, 1, &mGBuffers, 0 );

		mShaders["Sky"].SetShaderParameters( PixelShader,
											 0,
											 Vector2f( f32( mSceneBuffer.GetWidth() ), f32( mSceneBuffer.GetHeight() ) ) );

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		sphere->BindBuffers();

		mShaders["Sky"].Render( sphere->GetIndexCount() );

		mGraphics.GetPipeline()->SetNoBlend();
		mGraphics.GetPipeline()->SetBackFaceCull( true );
		mGraphics.GetPipeline()->SetZBuffer( true );
	}

	void Renderer::SetCamera( Camera * cam )
	{
		mCamera = cam ? cam : &mDefaultCamera;
	}

	GraphicsPipeline * Renderer::GetPipeline()
	{
		return mGraphics.GetPipeline();
	}

	void Renderer::RegisterToDrawEvent( const DrawDelegate & ddelegate )
	{
		mDrawEvent.Register( ddelegate );
	}
	
	void Renderer::UnregisterFromDrawEvent( const DrawDelegate & ddelegate )
	{
		mDrawEvent.Unregister( ddelegate );
	}

	void Renderer::BuildWeights()
	{
		u32 kernalWidth = 2 * mBlurKernelRadius + 1;

		f32 variance = 4.0f;
		f32 covariance = variance * variance;
		f32 sum = 0.0f;
		for ( u32 i = 0; i < kernalWidth; ++i )
		{
			f32 x = static_cast<f32>( i ) - static_cast<f32>( mBlurKernelRadius );
			mWeights[i].x = std::exp( -( ( x * x ) / ( 2.0f * covariance ) ) );
			sum += mWeights[i].x;
		}

		for ( u32 i = 0; i < kernalWidth; ++i )
		{
			mWeights[i].x /= sum;
		}
	}

	void Renderer::BuildTraceWeights()
	{
		u32 kernalWidth = 2 * TRACE_RADIUS + 1;

		f32 variance = 4.0f;
		f32 covariance = variance * variance;
		f32 sum = 0.0f;
		for ( u32 i = 0; i < kernalWidth; ++i )
		{
			f32 x = static_cast<f32>( i ) - static_cast<f32>( TRACE_RADIUS );
			mTWeights[i].x = std::exp( -( ( x * x ) / ( 2.0f * covariance ) ) );
			sum += mTWeights[i].x;
		}

		for ( u32 i = 0; i < kernalWidth; ++i )
		{
			mTWeights[i].x /= sum;
		}
	}

	void Renderer::SetBlurKernelRadius( s32 radius )
	{
		mBlurKernelRadius = Math::Clamp( mBlurKernelRadius + radius, 0, MAX_KERNEL_RADIUS );
		BuildWeights();
	}

	void Renderer::SetSceneViewport( f32 x, f32 y, f32 width, f32 height )
	{
		BH_ASSERT( x >= 0.0f && x <= 1.0f, "Scene viewport offset not within range" );
		BH_ASSERT( y >= 0.0f && y <= 1.0f, "Scene viewport offset not within range" );
		BH_ASSERT( width >= 0.0f && width <= 1.0f, "Scene viewport offset not within range" );
		BH_ASSERT( height >= 0.0f && height <= 1.0f, "Scene viewport offset not within range" );

		mSceneOffset.x = x;
		mSceneOffset.y = y;
		mSceneDimension.x = width;
		mSceneDimension.y = height;
	}

	void Renderer::BuildPseudoRandomSet()
	{
		for ( s32 k = 0; k < IBL_N; ++k ) 
		{
			f32 u = 0.0f;
			f32 p = 0.5f;
			for ( s32 kk = k; kk != 0 ; p *= 0.5f, kk >>= 1 )
			{
				if ( kk & 1 )
					u += p;
			}

			f32 v = ( k + 0.5f ) / static_cast< f32 >( IBL_N );
			mHammersley[k].x = u;
			mHammersley[k].y = v;
		}

	}
}