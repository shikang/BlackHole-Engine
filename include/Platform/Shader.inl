namespace BH
{
	template< typename... Args >
	bool Shader::SetShaderParameters( ShaderType shaderType, u32 bufferNum, const Args&... args )
	{
		// Lock the constant buffer so it can be written to.
		ID3D11DeviceContext * deviceContext = GetContext();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result = deviceContext->Map( mBuffers[shaderType][bufferNum], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
		if ( FAILED( result ) )
		{
			return false;
		}

		SetData<Args...>()( mappedResource.pData, args... );

		// Unlock the constant buffer.
		deviceContext->Unmap( mBuffers[shaderType][bufferNum], 0 );

		// Finally set the constant buffer in the vertex shader with the updated values.
		if ( shaderType == VertexShader )
			deviceContext->VSSetConstantBuffers( bufferNum, 1, mBuffers[shaderType].data() + bufferNum );
		else if ( shaderType == PixelShader )
			deviceContext->PSSetConstantBuffers( bufferNum, 1, mBuffers[shaderType].data() + bufferNum );
		else if ( shaderType == GeometryShader )
			deviceContext->GSSetConstantBuffers( bufferNum, 1, mBuffers[shaderType].data() + bufferNum );
		else if ( shaderType == ComputeShader )
			deviceContext->CSSetConstantBuffers( bufferNum, 1, mBuffers[shaderType].data() + bufferNum );

		return true;
	}

	template< typename T, u32 SIZE >
	bool Shader::SetShaderArrayParameters( ShaderType shaderType, u32 bufferNum, const T ( & a )[ SIZE ] )
	{
		// Lock the constant buffer so it can be written to.
		ID3D11DeviceContext * deviceContext = GetContext();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result = deviceContext->Map( mBuffers[shaderType][bufferNum], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
		if ( FAILED( result ) )
		{
			return false;
		}

		std::memcpy( mappedResource.pData, a, sizeof( T ) * SIZE );

		// Unlock the constant buffer.
		deviceContext->Unmap( mBuffers[shaderType][bufferNum], 0 );

		// Finally set the constant buffer in the vertex shader with the updated values.
		if ( shaderType == VertexShader )
			deviceContext->VSSetConstantBuffers( bufferNum, 1, mBuffers[shaderType].data() + bufferNum );
		else if ( shaderType == PixelShader )
			deviceContext->PSSetConstantBuffers( bufferNum, 1, mBuffers[shaderType].data() + bufferNum );
		else if ( shaderType == GeometryShader )
			deviceContext->GSSetConstantBuffers( bufferNum, 1, mBuffers[shaderType].data() + bufferNum );
		else if ( shaderType == ComputeShader )
			deviceContext->CSSetConstantBuffers( bufferNum, 1, mBuffers[shaderType].data() + bufferNum );

		return true;
	}

	template< typename T, typename Allocator >
	bool Shader::SetShaderVectorParameters( ShaderType shaderType, u32 bufferNum, const std::vector<T, Allocator> & v )
	{
		// Lock the constant buffer so it can be written to.
		ID3D11DeviceContext * deviceContext = GetContext();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result = deviceContext->Map( mBuffers[shaderType][bufferNum], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
		if ( FAILED( result ) )
		{
			return false;
		}

		std::memcpy( mappedResource.pData, v.data(), sizeof( T ) * v.size() );

		// Unlock the constant buffer.
		deviceContext->Unmap( mBuffers[shaderType][bufferNum], 0 );

		// Finally set the constant buffer in the vertex shader with the updated values.
		if ( shaderType == VertexShader )
			deviceContext->VSSetConstantBuffers( bufferNum, 1, mBuffers[shaderType].data() + bufferNum );
		else if ( shaderType == PixelShader )
			deviceContext->PSSetConstantBuffers( bufferNum, 1, mBuffers[shaderType].data() + bufferNum );
		else if ( shaderType == GeometryShader )
			deviceContext->GSSetConstantBuffers( bufferNum, 1, mBuffers[shaderType].data() + bufferNum );
		else if ( shaderType == ComputeShader )
			deviceContext->CSSetConstantBuffers( bufferNum, 1, mBuffers[shaderType].data() + bufferNum );

		return true;
	}
}