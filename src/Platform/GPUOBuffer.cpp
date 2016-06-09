// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/GPUOBuffer.h"
#include "Platform/Graphics.h"
#include <memory>

namespace BH
{
	GPUOBuffer::GPUOBuffer()
	: mShaderResourceView( nullptr )
	, mDataBuffer( nullptr )
	, mDataBufferView( nullptr )
	, mDataTexture( nullptr )
	, mWidth( 0 )
	, mHeight( 0 )
	{
	}

	GPUOBuffer::~GPUOBuffer()
	{
	}

	void GPUOBuffer::Initialise( u32 width, u32 height )
	{
		// The compute shader will need to output to some buffer, therefore create a GPU buffer for that.
		D3D11_BUFFER_DESC descGPUBuffer;
		ZeroMemory( &descGPUBuffer, sizeof( descGPUBuffer ) );
		descGPUBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		descGPUBuffer.ByteWidth = sizeof( Vector4f ) * width * height;
		descGPUBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		descGPUBuffer.StructureByteStride = sizeof( Vector4f );				// Assuming the output data is in the RGBA format, 4 floats

		ID3D11Device * device = GRAPHICS.GetPipeline()->GetDevice();
		if ( FAILED( device->CreateBuffer( &descGPUBuffer, NULL, &mDataBuffer ) ) )
			return;

		// The view we need for the output is an unordered access view. This is to allow the compute shader to write anywhere in the buffer.
		D3D11_BUFFER_DESC descBuf;
		ZeroMemory( &descBuf, sizeof( descBuf ) );
		mDataBuffer->GetDesc( &descBuf );

		D3D11_UNORDERED_ACCESS_VIEW_DESC descView;
		ZeroMemory( &descView, sizeof( descView ) );
		descView.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		descView.Buffer.FirstElement = 0;

		descView.Format = DXGI_FORMAT_UNKNOWN;      // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
		descView.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

		if ( FAILED( device->CreateUnorderedAccessView( mDataBuffer, &descView, &mDataBufferView ) ) )
			return;

		mWidth = width;
		mHeight = height;
	}

	void GPUOBuffer::Shutdown()
	{
		if ( mShaderResourceView )
		{
			mShaderResourceView->Release();
			mShaderResourceView = nullptr;
		}

		if ( mDataBuffer )
		{
			mDataBuffer->Release();
			mDataBuffer = nullptr;
		}

		if ( mDataBufferView )
		{
			mDataBufferView->Release();
			mDataBufferView = nullptr;
		}

		if ( mDataTexture )
		{
			mDataTexture->Release();
			mDataTexture = nullptr;
		}
	}

	void GPUOBuffer::CopyBufferToTexture()
	{
		// I'm pretty sure there is a better way to do things
		ID3D11Device * device = GRAPHICS.GetPipeline()->GetDevice();
		ID3D11DeviceContext * context = GRAPHICS.GetPipeline()->GetContext();
		std::unique_ptr < Vector4f[] > buffer( nullptr );
		UINT byteSize = 0;

		// Copy to a buffer
		{
			ID3D11Buffer* debugbuf = NULL;

			D3D11_BUFFER_DESC desc;
			ZeroMemory( &desc, sizeof( desc ) );
			mDataBuffer->GetDesc( &desc );

			byteSize = desc.ByteWidth;

			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.Usage = D3D11_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.MiscFlags = 0;

			if ( SUCCEEDED( device->CreateBuffer( &desc, NULL, &debugbuf ) ) )
			{
				context->CopyResource( debugbuf, mDataBuffer );

				D3D11_MAPPED_SUBRESOURCE mappedResource;
				if ( context->Map( debugbuf, 0, D3D11_MAP_READ, 0, &mappedResource ) != S_OK )
					return;

				// Copying data
				Vector4f * outBuff = new Vector4f[ byteSize / sizeof( Vector4f ) ];
				memcpy( outBuff, mappedResource.pData, byteSize) ;
				buffer.reset( outBuff );

				context->Unmap( debugbuf, 0 );

				debugbuf->Release();
			}
		}

		// Copying to texture
		{
			if ( mShaderResourceView )
				mShaderResourceView->Release();

			if ( mDataTexture )
				mDataTexture->Release();

			D3D11_TEXTURE2D_DESC textureDesc;

			// Initialize the render target texture description.
			ZeroMemory( &textureDesc, sizeof( textureDesc ) );

			// Setup the render target texture description.
			textureDesc.Width = mWidth;
			textureDesc.Height = mHeight;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.Usage = D3D11_USAGE_DYNAMIC;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			textureDesc.MiscFlags = 0;

			if ( device->CreateTexture2D( &textureDesc, NULL, &mDataTexture ) != S_OK )
				return;

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			if ( context->Map( mDataTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource ) != S_OK )
				return;

			memcpy( mappedResource.pData, buffer.get(), byteSize );

			context->Unmap( mDataTexture, 0 );

			// Create a view of the output texture
			D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
			ZeroMemory( &viewDesc, sizeof( viewDesc ) );
			viewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Texture2D.MipLevels = 1;
			viewDesc.Texture2D.MostDetailedMip = 0;
			if ( FAILED( device->CreateShaderResourceView( mDataTexture, &viewDesc, &mShaderResourceView ) ) )
				return;
		}
	}

	ID3D11ShaderResourceView * GPUOBuffer::GetShaderResourceView()
	{
		return mShaderResourceView;
	}
}