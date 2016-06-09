namespace BH
{
	template< typename VertexType >
	bool Mesh::InitialiseBuffers( const std::vector<VertexType> & vertexBuffer,
								  const std::vector<s32> & indexBuffer,
								  bool line )
	{
		//mType = line ? D3D11_PRIMITIVE_TOPOLOGY_LINELIST : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		mLine = line;

		// Set the number of vertices in the vertex array.
		mVertexCount = vertexBuffer.size();

		// Set the number of indices in the index array.
		mIndexCount = indexBuffer.size();

		// Stride
		mStride = sizeof( VertexType );

		// Set up the description of the static vertex buffer.
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = mStride * mVertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = vertexBuffer.data();
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		ID3D11Device * device = GRAPHICS.mPipeline.mDevice;
		HRESULT result = device->CreateBuffer( &vertexBufferDesc, &vertexData, &mVertexBuffer );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Set up the description of the static index buffer.
		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof( s32 ) * mIndexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = indexBuffer.data();
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer( &indexBufferDesc, &indexData, &mIndexBuffer );
		if ( FAILED( result ) )
		{
			return false;
		}

		return true;
	}
}