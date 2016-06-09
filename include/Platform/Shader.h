#ifndef BH_SHADER_H
#define BH_SHADER_H

#include "Platform/PlatformEssential.h"
#include "Platform/Typedefs.h"
#include "Platform/Matrix4.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#include <tuple>

namespace BH
{
	enum ShaderType
	{
		VertexShader = 0,
		PixelShader,
		GeometryShader,
		ComputeShader,

		TotalShaderType
	};

	class Shader
	{
	public:
		// Constructor
		Shader();

		// Destructor
		~Shader();

		// Initialise vertex and pixel shaders
		bool Initialise( const Char * vs_filename, 
						 const Char * ps_filename,
						 const Char * vs_main_function_name,
						 const Char * ps_main_function_name,
						 const D3D_SHADER_MACRO * vs_macros = nullptr,
						 const D3D_SHADER_MACRO * ps_macros = nullptr,
						 ID3DInclude * vs_includes = nullptr,
						 ID3DInclude * ps_includes = nullptr );

		// Initialise compute shader
		bool Initialise( const Char * cs_filename, 
						 const Char * cs_main_function_name,
						 const D3D_SHADER_MACRO * cs_macros = nullptr,
						 ID3DInclude * cs_includes = nullptr );

		// Shutdown
		void Shutdown();

	public:
		/*
			Create Buffer (Please note that buffer is aligned by 4 byte)
			https://msdn.microsoft.com/en-us/library/windows/desktop/bb509632%28v=vs.85%29.aspx
		*/
		bool CreaterBuffer( ShaderType shaderType, u32 bufferNum, u32 size );

		// Set buffer for vertex shader
		template< typename... Args >
		bool SetShaderParameters( ShaderType shaderType,
								  u32 bufferNum,				//!< Position in shader
								  const Args&... args );

		template< typename T, u32 SIZE >
		bool SetShaderArrayParameters( ShaderType shaderType,
									   u32 bufferNum,			//!< Position in shader
									   const T ( & a )[ SIZE ] );

		// Create Sampler (For texture)
		bool CreateSampler( ShaderType shaderType, u32 samplerNum );

		// Set texture
		bool SetTexture( ShaderType shaderType, u32 texNum, const Texture * texture );

		// Set texture
		bool SetTexture( ShaderType shaderType, u32 texNum, const HDRTexture * texture );

		// Set texture
		bool SetTexture( ShaderType shaderType, u32 texNum, const RenderTarget * texture );

		// Set texture
		bool SetTexture( ShaderType shaderType, u32 texNum, const DepthRenderTarget * texture );

		// Set texture
		bool SetTexture( ShaderType shaderType, u32 texNum, const GPUOBuffer * buffer );

		// Set texture
		bool SetTexture( ShaderType shaderType, u32 texNum, const MultiRenderTarget * mrt, u32 rtIndex );

		// Set unordered buffer
		bool SetUnorderedBuffer( u32 num, const GPUOBuffer * buffer );

		// Set unordered Texture
		bool BindUnorderedTexture( u32 num, const UAVTexture * texture );

		// UnBind data from GPU
		void UnBindUnorderedTexture( u32 num );

		// Set texture
		bool BindTexture( ShaderType shaderType, u32 texNum, const UAVTexture * texture );

		// Unbind texture
		void UnBindTexture( ShaderType shaderType, u32 texNum );

		// Render Vertex, (Geometry), Pixel Shader
		void Render( u32 index_count );

		// Dispatch Compute Shader
		void Dispatch( u32 x, u32 y, u32 z );

	private:
		// Compile Shader
		ID3D10Blob * CompileShader( const WCHAR * filename,
									const Char * main_function_name,
									const Char * profile,						//!< vs_5_0, ps_5_0 (Used to determine which type of shader)
									const D3D_SHADER_MACRO * macros = nullptr,
									ID3DInclude * includes = nullptr );

		// Create Vertex layout using reflection.
		bool CreateVertexLayout( ID3D10Blob * vertexShaderBuffer );

		// Initialise Vertex and Pixel Shaders
		bool InitialiseShaders( const WCHAR * vs_filename,
								const WCHAR * ps_filename,
								const Char * vs_main_function_name,
								const Char * ps_main_function_name,
								const D3D_SHADER_MACRO * vs_macros = nullptr,
								const D3D_SHADER_MACRO * ps_macros = nullptr,
								ID3DInclude * vs_includes = nullptr,
								ID3DInclude * ps_includes = nullptr );

		// Initialise Compute Shader
		bool InitialiseShader( const WCHAR * cs_filename, 
							   const Char * cs_main_function_name,
							   const D3D_SHADER_MACRO * cs_macros = nullptr,
							   ID3DInclude * cs_includes = nullptr );

		// Shutdown Shader
		void ShutdownShader();

		// Get Device Context
		ID3D11DeviceContext * GetContext();

		// Output Shader Error Message
		static void OutputShaderErrorMessage( ID3D10Blob * error_msg, const WCHAR * shader_filename );

	private:
		// Helper functor to set data of any parameters
		template< typename First, typename... Rest >
		class SetData
		{
		public:
			void operator()( void * dataPtr, const First & f, const Rest&... r )
			{
				*reinterpret_cast<First *>( dataPtr ) = f;
				SetData<Rest...>()( reinterpret_cast<Char *>( dataPtr ) + sizeof( First ), r... );
			}
		};

		template< typename First >
		class SetData<First>
		{
		public:
			void operator()( void * dataPtr, const First & f )
			{
				*reinterpret_cast<First *>( dataPtr ) = f;
			}
		};

	private:
		ID3D11VertexShader * mVertexShader;
		ID3D11PixelShader * mPixelShader;
		ID3D11ComputeShader * mComputeShader;
		ID3D11InputLayout * mLayout;
		std::vector<ID3D11Buffer *> mBuffers[TotalShaderType];
		std::vector<ID3D11SamplerState *> mSamplerStates[TotalShaderType];

		friend class Graphics;
	};
}

#include "Platform/Shader.inl"

#endif