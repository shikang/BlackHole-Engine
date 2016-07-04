#ifndef BH_RENDERER_H
#define BH_RENDERER_H

#include "Core/Event.h"
#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Core/IDrawable.h"
#include "Core/GameComponent.h"
#include "Core/SystemManager.h"
#include "Core/Materials.h"

#include "Platform/Camera.h"
#include "Platform/Shader.h"
#include "Platform/GraphicsPipeline.h"
#include "Platform/MeshLoader.h"
#include "Platform/UAVTexture.h"
#include "Platform/RenderTarget.h"
#include "Platform/DepthRenderTarget.h"
#include "Platform/MultiRenderTarget.h"
#include "Platform/GPUOBuffer.h"
#include "Platform/HDRTexture.h"

#define MAX_KERNEL_RADIUS 50
#define BATCH_X_NUM 128
#define BATCH_Y_NUM 72
#define IBL_N 40
#define MAX_BONE_MATRICES 80

namespace BH
{
	class Renderer : public GameComponent, public IDrawable
	{
	public:
		static f32 Near;					//!< Near distance
		static f32 Far;						//!< Far distance
		static f32 ExponentConstant;		//!< Exponential Shadow Map constant
		static f32 TraceSampleRate;			//!< Tracing Sample rate
		static bool TraceDownSample;		//!< Tracing jittering
		static bool TraceJitter;			//!< Tracing jittering
		static bool TraceEdgeBlur;			//!< Tracing blur edge
		static bool ShowSkyDome;			//!< Show Sky Dome
		static bool ShowDebugLight;			//!< Show Debug Light
		static f32 AmbientRangeOfInfluence;	//!< Ambient Occlusion Range
		static f32 AmbientScaleFactor;		//!< Ambient Scale Factor
		static f32 AmbientContrast;			//!< Ambient Constrast
		static bool EnableAmbientOcculsion;	//!< Enable Ambient Occulsion
		static String SkyName;				//!< Texture name for sky dome

		GETSET_AUTO( Vector4f, Ambient );
		GETTER_AUTO_CREF( Vector2f, SceneOffset );
		GETTER_AUTO_CREF( Vector2f, SceneDimension );

	public:
		struct InstanceInfo
		{
			Matrix4 mTransform;
			const Model * mModel;
			const Material * mMaterial;
			const AnimationController * mAnimationController;
		};

		struct LineInfo
		{
			Matrix4 mTransform;
			Vector4f mColor;
			const Mesh * mMesh;
			bool mHUD;
		};

		typedef Event< void( f32 ) > DrawEvent;
		typedef DrawEvent::Delegate DrawDelegate;

	public:
		// Getters
		GETTER_AUTO_PTR( Camera, Camera );

		// Set cam as current Camera
		void SetCamera( Camera * cam );

		// Get graphics pipeline
		GraphicsPipeline * GetPipeline();

		// Register to draw event
		void RegisterToDrawEvent( const DrawDelegate & ddelegate );

		// Unregister from draw event
		void UnregisterFromDrawEvent( const DrawDelegate & ddelegate );

	public:
		// Constructor
		Renderer();

		// Destructor
		~Renderer();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Draw
		void Draw( f32 dt ) override;

		// Draw Instance
		void DrawInstance( const Matrix4 & transform,						//!< Instance Transformation
						   const Model * model,								//!< Instance Model
						   const Material * material,						//!< Instance Material
						   const AnimationController * anim = nullptr );	//!< Instance Animation

		// Draw Instance
		void DrawInstance( const Vector3f & position,						//!< Instance Transformation
						   const Vector3f & scale,
						   const Vector3f & rotation,
						   const Model * model,								//!< Instance Model
						   const Material * material );						//!< Instance Material

		// Draw Light
		void DrawGlobalLight( const Light & light );
		void DrawDirectionalLight( const DirectionalLight & light );
		void DrawLocalLight( const LocalLight & light );

		// Draw Lines
		void DrawLines( const Matrix4 & transform,							//!< Lines Transformation
						const Vector4f & color,								//!< Color
						const Mesh * mesh,									//!< Line Mesh
						bool onHUD = false );								//!< Line ignore depth (always on top)

		// Set Blur Kernel Radius (max 50)
		void SetBlurKernelRadius( s32 radius );

		// Set scene viewport (All range from 0 to 1)
		void SetSceneViewport( f32 x, f32 y, f32 width, f32 height );

		// Hide SkyDome (for tools)
		inline void HideSkyDome( bool hide ) { mHideSkyDome = hide; }

	private:
		// Shutdown shaders
		void ShutdownShaders();

		// Initialise shaders
		void InitialiseShaders();

		// Render (Called in draw function)
		void Render();

		// Geometry Pass for deferred shading
		void GeometryPass();

		// Ambient Occulsion Pass
		void AmbientOcculsionPass();

		// Ambient Pass for deferred shading
		void AmbientPass();

		// Light Pass for deferred shading
		void LightPass();

		// Depth Pass
		void DepthPass( const Vector3f & lightPos,
						Matrix4 & viewLight, Matrix4 & perspLight, 
						f32 & lightNear, f32 & lightFar );

		// Depth Pass
		void DirectionalDepthPass( const Vector3f & lightPos, const Vector3f & direction,
								   const Vector3f & dimension, Matrix4 & viewLight, 
								   Matrix4 & perspLight, f32 & lightNear, f32 & lightFar );

		// Blur depth map
		void BlurDepthMap();

		// Blur Ambient map
		void BlurAmbientMap();

		// Trace Sample pass
		void TraceSamplePass( const DirectionalLight & light, const Matrix4 & shadow,
							  const Matrix4 & invPV, f32 lightNear, f32 lightFar );

		// Edge blur pass
		void EdgeBlurPass();

		// Blend Light pass
		void BlendLightPass();

		// Lines Pass
		void LinesPass();

		// Draw Final Scene (After post processing)
		void DrawFinalScene();

		// Sky Dome pass
		void SkyDomePass();

	private:
		// Builds weights for blur
		void BuildWeights();

		// Builds trace weights for blur
		void BuildTraceWeights();

		// Build Pseudo random set
		void BuildPseudoRandomSet();

	private:
		typedef std::vector< InstanceInfo >				InstanceList;
		typedef std::vector< LineInfo >					LineList;
		typedef std::vector< Light >					GlobalLightList;
		typedef std::vector< DirectionalLight >			DirectionalLightList;
		typedef std::vector< LocalLight >				LocalLightList;

		typedef std::unordered_map< String, Shader >	ShaderList;

	private:
		Graphics & mGraphics;							//!< Platform wrapped
		Camera * mCamera;								//!< Current camera
		Camera mDefaultCamera;							//!< Default camera
		InstanceList mInstanceList;						//!< Stuffs to draw
		LineList mLineList;								//!< Lines to draw
		GlobalLightList mGlobalLights;					//!< Global lights to draw
		DirectionalLightList mDirectionalLights;		//!< DIrectional lights to draw
		LocalLightList mLocalLights;					//!< Local lights to draw
		DrawEvent mDrawEvent;							//!< Draw event

		Vector4f mAmbient;								//!< Ambient
		Vector4f mWeights[2 * MAX_KERNEL_RADIUS + 1];	//!< Blur weights
		s32 mBlurKernelRadius;							//!< Blur Kernel radius
		Vector4f mHammersley[IBL_N];					//!< Pseudo random set
		Vector2f mSceneOffset;							//!< Scene offset
		Vector2f mSceneDimension;						//!< Scene dimension
		Vector4f mTWeights[2 * MAX_KERNEL_RADIUS + 1];	//!< Trace Blur weights

		bool mHideSkyDome;								//!< Hide Skydome

	private:
		MultiRenderTarget mGBuffers;					//!< Geometry Buffers
		DepthRenderTarget mDepthBuffer;					//!< Depth Buffer
		RenderTarget mSceneBuffer;						//!< Final scene before post processing
		UAVTexture mHBlurDepthBuffer;					//!< Horizontal Blurred Depth Buffer
		UAVTexture mBlurDepthBuffer;					//!< Blurred Depth Buffer
		DepthRenderTarget mTraceBuffer;					//!< Sample Traces for volume light
		DepthRenderTarget mDTraceBuffer;				//!< Downsample Sample Traces for volume light
		DepthRenderTarget mExitTraceBuffer;				//!< End Position Sample Buffer
		DepthRenderTarget mGradientTraceBuffer;			//!< Gradient Sample Traces for volume light
		DepthRenderTarget mGradientBlurTraceBuffer;		//!< Edge Blurred Sample Traces for volume light
		UAVTexture mHBlurTraceBuffer;					//!< Horizontal Blurred Depth Buffer
		UAVTexture mBlurTraceBuffer;					//!< Blurred Depth Buffer
		RenderTarget mAmbientBuffer;					//!< Ambient factor buffer
		UAVTexture mHBlurAmbientBuffer;					//!< Horizontal Blurred Depth Buffer
		UAVTexture mBlurAmbientBuffer;					//!< Blurred Depth Buffer

		ShaderList mShaders;							//!< All Shaders

		Mesh mQuad;										//!< Quad

	private:
		// If draw buffer
		bool mDrawG = false;
		bool mDrawBlur = false;
		bool mDrawShadowMap = false;

		// Toggle draw g buffers
		void ToggleDrawGBuffers();

		// Toggle draw blur
		void ToggleDrawShadowMap();

		// Toggle draw blur
		void ToggleDrawBlur();

		// Recompile Shader
		void RecompileShader();

		// Show G-Buffers (Debug)
		void DrawGBuffers();

		// Show Depth Buffer (Debug)
		void DrawDepthBuffer();

		// Show blur
		void DrawBlur();

		// Draw Light (Debug)
		template< typename LIGHTS >
		void DrawLight( const LIGHTS & lights )
		{
			// Texture for this instance
			mShaders["Flat"].SetTexture( PixelShader, 0, 
									SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTexture( "white" ) );

			const Mesh & light = SYSTEM_MANAGER.GetGameComponentFromSystem<ModelManager>()->GetModel( "Sphere" )->mMesh;

			for ( auto & i : lights )
			{
				// Set Transform for this instance
				mShaders["Flat"].SetShaderParameters( VertexShader,
													  0,
													  ( Matrix4::CreateTranslation( i.GetPosition() ) * 
													    Matrix4::CreateScale( 0.2f * Vector3f::ONE ) *
													    mGraphics.GetPipeline()->GetWorld() ).Transpose(),
													  mCamera->GetView().Transpose(),
													  mGraphics.GetPipeline()->GetProjection().Transpose() );

				mShaders["Flat"].SetShaderParameters( PixelShader,
													  0,
													  i.GetColor() );

				// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
				light.BindBuffers();

				mShaders["Flat"].Render( light.GetIndexCount() );
			}
		}

		// Draw Directional Light (Debug)
		void DrawDirectionalLight( const DirectionalLightList & lights );

	};
}

#include "Core/Typing.h"
DECLARE_TYPE(::BH::Renderer, Renderer);

#endif