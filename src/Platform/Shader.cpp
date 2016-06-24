// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/Shader.h"
#include "Platform/Graphics.h"
#include "Platform/Texture.h"
#include "Platform/RenderTarget.h"
#include "Platform/MultiRenderTarget.h"
#include "Platform/GPUOBuffer.h"

#include <fstream>
#include <string>
#include <codecvt>
#include <locale>

namespace BH
{
	Shader::Shader()
	: mVertexShader( nullptr )
	, mPixelShader( nullptr )
	, mComputeShader( nullptr )
	, mLayout( nullptr )
	{

	}

	Shader::~Shader()
	{
	}

	bool Shader::Initialise( const Char * vs_filename, 
							 const Char * ps_filename,
							 const Char * vs_main_function_name,
							 const Char * ps_main_function_name,
							 const D3D_SHADER_MACRO * vs_macros,
							 const D3D_SHADER_MACRO * ps_macros,
							 ID3DInclude * vs_includes,
							 ID3DInclude * ps_includes )
	{
		std::wstring_convert< std::codecvt_utf8_utf16< wchar_t >> converter;
		std::wstring wVSFileName = converter.from_bytes( vs_filename );
		std::wstring wPSFileName = converter.from_bytes( ps_filename );

		return InitialiseShaders( wVSFileName.c_str(), 
								  wPSFileName.c_str(),
								  vs_main_function_name,
								  ps_main_function_name,
								  vs_macros,
								  ps_macros,
								  vs_includes ? vs_includes : D3D_COMPILE_STANDARD_FILE_INCLUDE,
								  ps_includes ? ps_includes : D3D_COMPILE_STANDARD_FILE_INCLUDE );
	}

	bool Shader::Initialise( const Char * cs_filename,
							const Char * cs_main_function_name,
							const D3D_SHADER_MACRO * cs_macros,
							ID3DInclude * cs_includes )
	{
		std::wstring_convert< std::codecvt_utf8_utf16< wchar_t >> converter;
		std::wstring wFileName = converter.from_bytes( cs_filename );

		return InitialiseShader( wFileName.c_str(), 
								 cs_main_function_name,
								 cs_macros,
								 cs_includes ? cs_includes : D3D_COMPILE_STANDARD_FILE_INCLUDE );
	}

	void Shader::Shutdown()
	{
		ShutdownShader();
	}

	void Shader::Render( u32 index_count )
	{
		ID3D11DeviceContext * deviceContext = GRAPHICS.mPipeline.mDeviceContext;

		// Set the vertex input layout.
		deviceContext->IASetInputLayout( mLayout );

		// Set the vertex and pixel shaders that will be used to render this triangle.
		deviceContext->VSSetShader( mVertexShader, NULL, 0 );
		deviceContext->PSSetShader( mPixelShader, NULL, 0 );

		for ( u32 i = 0; i < TotalShaderType; ++i )
		{
			u32 size = mSamplerStates[i].size();
			for ( u32 j = 0; j < size; ++j )
			{
				if ( i == VertexShader )
					GRAPHICS.mPipeline.GetContext()->VSSetSamplers( j, 1, &mSamplerStates[i][j] );
				else if ( i == PixelShader )
					GRAPHICS.mPipeline.GetContext()->PSSetSamplers( j, 1, &mSamplerStates[i][j] );
				else if ( i == GeometryShader )
					GRAPHICS.mPipeline.GetContext()->GSSetSamplers( j, 1, &mSamplerStates[i][j] );
				//else if ( i == ComputeShader )
				//	GRAPHICS.mPipeline.GetContext()->CSSetSamplers( j, 1, &mSamplerStates[i][j] );
			}
		}

		// Render the triangle.
		deviceContext->DrawIndexed( index_count, 0, 0 );
	}

	void Shader::Dispatch( u32 x, u32 y, u32 z )
	{
		ID3D11DeviceContext * deviceContext = GRAPHICS.mPipeline.mDeviceContext;

		deviceContext->CSSetShader( mComputeShader, NULL, 0 );

		u32 size = mSamplerStates[ComputeShader].size();
		for ( u32 j = 0; j < size; ++j )
		{
			GRAPHICS.mPipeline.GetContext()->CSSetSamplers( j, 1, &mSamplerStates[ComputeShader][j] );
		}

		deviceContext->Dispatch( x, y, z );
	}

	ID3D10Blob * Shader::CompileShader( const WCHAR * filename,
										const Char * main_function_name,
										const Char * profile,						//!< vs_5_0, ps_5_0 (Used to determine which type of shader)
										const D3D_SHADER_MACRO * macros,
										ID3DInclude * includes )
	{
		ID3D10Blob * shaderBuffer = nullptr;
		ID3D10Blob * errorMessage = nullptr;
		DWORD compileOption = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG
		compileOption |= D3D10_SHADER_DEBUG;
#endif

		HRESULT result = D3DCompileFromFile( filename,							//!< Shader File Name
											 macros,							//!< Define Macros to be used in shader
											 includes,							//!< Handle #include within shader
											 main_function_name,				//!< Entry Function Name where shader calls to begin
											 profile,							//!< Shader Features
											 compileOption,						//!< Compile Options 1
											 0,									//!< Compile Options 2
											 &shaderBuffer,						//!< Shader Code
											 &errorMessage );					//!< Error message
		if ( FAILED( result ) )
		{
			// If the shader failed to compile it should have writen something to the error message.
			if ( errorMessage )
			{
				OutputShaderErrorMessage( errorMessage, filename );
			}
			// If there was  nothing in the error message then it simply could not find the shader file itself.
			else
			{
				MessageBoxW( nullptr, filename, L"Missing Shader File", MB_OK );
			}

			return nullptr;
		}

		return shaderBuffer;
	}

	bool Shader::CreateVertexLayout( ID3D10Blob * vertexShaderBuffer )
	{
		// Create the vertex input layout description.
		// Reflect shader info
		ID3D11ShaderReflection * pVertexShaderReflection = nullptr;
		HRESULT result = D3DReflect( vertexShaderBuffer->GetBufferPointer(),
									 vertexShaderBuffer->GetBufferSize(),
									 IID_ID3D11ShaderReflection,
									 reinterpret_cast<void**>( &pVertexShaderReflection ) );

		if ( FAILED( result ) )
		{
			return false;
		}

		// Get shader info
		D3D11_SHADER_DESC shaderDesc;
		pVertexShaderReflection->GetDesc(&shaderDesc);

		// Read input layout description from shader info
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for ( u32 i = 0; i< shaderDesc.InputParameters; ++i )
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pVertexShaderReflection->GetInputParameterDesc( i, &paramDesc );

			// fill out input element desc
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			// determine DXGI format
			if ( paramDesc.Mask == 1 )
			{
				if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if ( paramDesc.Mask <= 3 )
			{
				if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if ( paramDesc.Mask <= 7 )
			{
				if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if ( paramDesc.Mask <= 15 )
			{
				if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			//save element desc
			inputLayoutDesc.push_back( elementDesc );
		}

		ID3D11Device * device = GRAPHICS.mPipeline.mDevice;
		device->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &mLayout);

		//Free allocation shader reflection memory
		pVertexShaderReflection->Release();

		return true;
	}

	bool Shader::CreaterBuffer( ShaderType shaderType, u32 bufferNum, u32 size )
	{
		// Check vector size
		if ( mBuffers[shaderType].size() <= bufferNum )
			mBuffers[shaderType].resize( bufferNum + 1, nullptr );

		// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = size;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		ID3D11Device * device = GRAPHICS.mPipeline.mDevice;
		HRESULT result = device->CreateBuffer( &bufferDesc, NULL, mBuffers[shaderType].data() + bufferNum );

		return !( FAILED( result ) );
	}

	bool Shader::CreateSampler( ShaderType shaderType, u32 samplerNum )
	{
		// Check vector size
		if ( mSamplerStates[shaderType].size() <= samplerNum )
			mSamplerStates[shaderType].resize( samplerNum + 1, nullptr );

		D3D11_SAMPLER_DESC samplerDesc;

		// Create a texture sampler state description.
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		ID3D11Device * device = GRAPHICS.mPipeline.mDevice;
		HRESULT result = device->CreateSamplerState( &samplerDesc, &mSamplerStates[shaderType][samplerNum] );

		return !FAILED( result );
	}

	bool Shader::SetTexture( ShaderType shaderType, u32 texNum, const Texture * texture )
	{
		if ( !texture || !texture->mTextureView )
			return false;

		if ( shaderType == VertexShader )
			GRAPHICS.mPipeline.GetContext()->VSSetShaderResources( texNum, 1, &texture->mTextureView );
		else if ( shaderType == PixelShader )
			GRAPHICS.mPipeline.GetContext()->PSSetShaderResources( texNum, 1, &texture->mTextureView );
		else if ( shaderType == GeometryShader )
			GRAPHICS.mPipeline.GetContext()->GSSetShaderResources( texNum, 1, &texture->mTextureView );
		else if ( shaderType == ComputeShader )
			GRAPHICS.mPipeline.GetContext()->CSSetShaderResources( texNum, 1, &texture->mTextureView );
		
		return true;
	}

	bool Shader::SetTexture( ShaderType shaderType, u32 texNum, const HDRTexture * texture )
	{
		if ( !texture || !texture->mTextureView )
			return false;

		if ( shaderType == VertexShader )
			GRAPHICS.mPipeline.GetContext()->VSSetShaderResources( texNum, 1, &texture->mTextureView );
		else if ( shaderType == PixelShader )
			GRAPHICS.mPipeline.GetContext()->PSSetShaderResources( texNum, 1, &texture->mTextureView );
		else if ( shaderType == GeometryShader )
			GRAPHICS.mPipeline.GetContext()->GSSetShaderResources( texNum, 1, &texture->mTextureView );
		else if ( shaderType == ComputeShader )
			GRAPHICS.mPipeline.GetContext()->CSSetShaderResources( texNum, 1, &texture->mTextureView );
		
		return true;
	}

	bool Shader::SetTexture( ShaderType shaderType, u32 texNum, const RenderTarget * texture )
	{
		if ( !texture || !texture->mShaderResourceView )
			return false;

		if ( shaderType == VertexShader )
			GRAPHICS.mPipeline.GetContext()->VSSetShaderResources( texNum, 1, &texture->mShaderResourceView );
		else if ( shaderType == PixelShader )
			GRAPHICS.mPipeline.GetContext()->PSSetShaderResources( texNum, 1, &texture->mShaderResourceView );
		else if ( shaderType == GeometryShader )
			GRAPHICS.mPipeline.GetContext()->GSSetShaderResources( texNum, 1, &texture->mShaderResourceView );
		else if ( shaderType == ComputeShader )
			GRAPHICS.mPipeline.GetContext()->CSSetShaderResources( texNum, 1, &texture->mShaderResourceView );
		
		return true;
	}

	bool Shader::SetTexture( ShaderType shaderType, u32 texNum, const DepthRenderTarget * texture )
	{
		if ( !texture || !texture->mShaderResourceView )
			return false;

		if ( shaderType == VertexShader )
			GRAPHICS.mPipeline.GetContext()->VSSetShaderResources( texNum, 1, &texture->mShaderResourceView );
		else if ( shaderType == PixelShader )
			GRAPHICS.mPipeline.GetContext()->PSSetShaderResources( texNum, 1, &texture->mShaderResourceView );
		else if ( shaderType == GeometryShader )
			GRAPHICS.mPipeline.GetContext()->GSSetShaderResources( texNum, 1, &texture->mShaderResourceView );
		else if ( shaderType == ComputeShader )
			GRAPHICS.mPipeline.GetContext()->CSSetShaderResources( texNum, 1, &texture->mShaderResourceView );
		
		return true;
	}

	bool Shader::SetTexture( ShaderType shaderType, u32 texNum, const GPUOBuffer * buffer )
	{
		if ( !buffer || !buffer->mShaderResourceView )
			return false;

		if ( shaderType == VertexShader )
			GRAPHICS.mPipeline.GetContext()->VSSetShaderResources( texNum, 1, &buffer->mShaderResourceView );
		else if ( shaderType == PixelShader )
			GRAPHICS.mPipeline.GetContext()->PSSetShaderResources( texNum, 1, &buffer->mShaderResourceView );
		else if ( shaderType == GeometryShader )
			GRAPHICS.mPipeline.GetContext()->GSSetShaderResources( texNum, 1, &buffer->mShaderResourceView );
		else if ( shaderType == ComputeShader )
			GRAPHICS.mPipeline.GetContext()->CSSetShaderResources( texNum, 1, &buffer->mShaderResourceView );
		
		return true;
	}

	bool Shader::SetTexture( ShaderType shaderType, u32 texNum, const MultiRenderTarget * mrt, u32 rtIndex )
	{
		if ( !mrt || mrt->GetRenderTargetsCount() <= rtIndex )
			return false;

		if ( shaderType == VertexShader )
			GRAPHICS.mPipeline.GetContext()->VSSetShaderResources( texNum, 1, &mrt->mShaderResourceViews[rtIndex] );
		else if ( shaderType == PixelShader )
			GRAPHICS.mPipeline.GetContext()->PSSetShaderResources( texNum, 1, &mrt->mShaderResourceViews[rtIndex] );
		else if ( shaderType == GeometryShader )
			GRAPHICS.mPipeline.GetContext()->GSSetShaderResources( texNum, 1, &mrt->mShaderResourceViews[rtIndex] );
		else if ( shaderType == ComputeShader )
			GRAPHICS.mPipeline.GetContext()->CSSetShaderResources( texNum, 1, &mrt->mShaderResourceViews[rtIndex] );
		
		return true;
	}

	bool Shader::SetUnorderedBuffer( u32 num, const GPUOBuffer * buffer )
	{
		if ( !buffer || !buffer->mDataBufferView )
			return false;

		GRAPHICS.mPipeline.GetContext()->CSSetUnorderedAccessViews( num, 1, &buffer->mDataBufferView, NULL );

		return true;
	}

	bool Shader::BindUnorderedTexture( u32 num, const UAVTexture * texture )
	{
		if ( !texture || !texture->mUnorderedAccessView )
			return false;

		GRAPHICS.mPipeline.GetContext()->CSSetUnorderedAccessViews( num, 1, &texture->mUnorderedAccessView, NULL );

		return true;
	}

	bool Shader::BindTexture( ShaderType shaderType, u32 texNum, const UAVTexture * texture )
	{
		if ( !texture || !texture->mShaderResourceView )
			return false;

		if ( shaderType == VertexShader )
			GRAPHICS.mPipeline.GetContext()->VSSetShaderResources( texNum, 1, &texture->mShaderResourceView );
		else if ( shaderType == PixelShader )
			GRAPHICS.mPipeline.GetContext()->PSSetShaderResources( texNum, 1, &texture->mShaderResourceView );
		else if ( shaderType == GeometryShader )
			GRAPHICS.mPipeline.GetContext()->GSSetShaderResources( texNum, 1, &texture->mShaderResourceView );
		else if ( shaderType == ComputeShader )
			GRAPHICS.mPipeline.GetContext()->CSSetShaderResources( texNum, 1, &texture->mShaderResourceView );
		
		return true;
	}

	void Shader::UnBindUnorderedTexture( u32 num )
	{
		static ID3D11UnorderedAccessView * UAV_NULL = nullptr;
		GRAPHICS.mPipeline.GetContext()->CSSetUnorderedAccessViews( num, 1, &UAV_NULL, NULL );
	}

	void Shader::UnBindTexture( ShaderType shaderType, u32 texNum )
	{
		static ID3D11ShaderResourceView * SRV_NULL = nullptr;
		if ( shaderType == VertexShader )
			GRAPHICS.mPipeline.GetContext()->VSSetShaderResources( texNum, 1, &SRV_NULL );
		else if ( shaderType == PixelShader )
			GRAPHICS.mPipeline.GetContext()->PSSetShaderResources( texNum, 1, &SRV_NULL );
		else if ( shaderType == GeometryShader )
			GRAPHICS.mPipeline.GetContext()->GSSetShaderResources( texNum, 1, &SRV_NULL );
		else if ( shaderType == ComputeShader )
			GRAPHICS.mPipeline.GetContext()->CSSetShaderResources( texNum, 1, &SRV_NULL );
	}

	bool Shader::InitialiseShaders( const WCHAR * vs_filename,
									const WCHAR * ps_filename,
									const Char * vs_main_function_name,
									const Char * ps_main_function_name,
									const D3D_SHADER_MACRO * vs_macros,
									const D3D_SHADER_MACRO * ps_macros,
									ID3DInclude * vs_includes,
									ID3DInclude * ps_includes )
	{
		// Compile the vertex shader code.
		ID3D10Blob * vertexShaderBuffer = CompileShader( vs_filename, vs_main_function_name, "vs_5_0", vs_macros, vs_includes );
		if ( !vertexShaderBuffer )
		{
			return false;
		}

		// Compile the pixel shader code.
		ID3D10Blob * pixelShaderBuffer = CompileShader( ps_filename, ps_main_function_name, "ps_5_0", ps_macros, ps_includes );
		if ( !pixelShaderBuffer )
		{
			return false;
		}

		// Create the vertex shader from the buffer.
		ID3D11Device * device = GRAPHICS.mPipeline.mDevice;
		HRESULT result = device->CreateVertexShader( vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Create the pixel shader from the buffer.
		result = device->CreatePixelShader( pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Create the vertex input layout description.
		if( !CreateVertexLayout( vertexShaderBuffer ) )
		{
			return false;
		}

		// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
		vertexShaderBuffer->Release();
		vertexShaderBuffer = 0;

		pixelShaderBuffer->Release();
		pixelShaderBuffer = 0;

		// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
		return CreaterBuffer( VertexShader, 0, sizeof( Matrix4 ) * 4 );
	}

	bool Shader::InitialiseShader( const WCHAR * cs_filename, 
								   const Char * cs_main_function_name,
								   const D3D_SHADER_MACRO * cs_macros,
								   ID3DInclude * cs_includes )
	{
		// Compile the vertex shader code.
		ID3D10Blob * computeShaderBuffer = CompileShader( cs_filename, cs_main_function_name, "cs_5_0", cs_macros, cs_includes );
		if ( !computeShaderBuffer )
		{
			return false;
		}

		// Create the compute shader from the buffer.
		ID3D11Device * device = GRAPHICS.mPipeline.mDevice;
		HRESULT result = device->CreateComputeShader( computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), NULL, &mComputeShader );
		if( FAILED( result ) )
			return false;

		return CreaterBuffer( ComputeShader, 0, sizeof( s32 ) * 4 );
	}

	void Shader::ShutdownShader()
	{
		// Release the constant buffers.
		for ( s32 j = 0; j < TotalShaderType; ++j )
		{
			for ( auto & i : mBuffers[j] )
			{
				if ( i )
				{
					i->Release();
					i = nullptr;
				}
			}
			mBuffers[j].clear();
		}

		// Release the samplers.
		for ( s32 j = 0; j < TotalShaderType; ++j )
		{
			for ( auto & i : mSamplerStates[j] )
			{
				if ( i )
				{
					i->Release();
					i = nullptr;
				}
			}
			mSamplerStates[j].clear();
		}

		// Release the layout.
		if ( mLayout )
		{
			mLayout->Release();
			mLayout = nullptr;
		}

		// Release the pixel shader.
		if ( mPixelShader )
		{
			mPixelShader->Release();
			mPixelShader = nullptr;
		}

		// Release the vertex shader.
		if ( mVertexShader )
		{
			mVertexShader->Release();
			mVertexShader = nullptr;
		}

		// Release the compute shader.
		if ( mComputeShader )
		{
			mComputeShader->Release();
			mComputeShader = nullptr;
		}
	}

	void Shader::OutputShaderErrorMessage( ID3D10Blob * error_msg, const WCHAR * shader_filename )
	{
		// Get a pointer to the error message text buffer.
		const Char * compileErrors = reinterpret_cast<const Char*>( error_msg->GetBufferPointer() );

		// Get the length of the message.
		u32 bufferSize = error_msg->GetBufferSize();

		std::wstring error_filename( std::wstring( shader_filename ) + L"- Error.txt" );
		std::ofstream ofile( error_filename );

		// Write out the error message.
		for ( u32 i = 0; i < bufferSize; ++i )
		{
			ofile << compileErrors[i];
		}

		ofile.close();

		// Release the error message.
		error_msg->Release();
		error_msg = 0;

		// Pop a message up on the screen to notify the user to check the text file for compile errors.
		MessageBoxW( nullptr, L"Error compiling shader.  Check shader-error.txt for message.", error_filename.c_str(), MB_OK );
	}

	ID3D11DeviceContext * Shader::GetContext()
	{
		return GRAPHICS.mPipeline.mDeviceContext;
	}
}