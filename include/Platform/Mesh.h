#ifndef BH_MESH_H
#define BH_MESH_H

#include "Platform/Vertex.h"
#include "Platform/Graphics.h"

#include <d3d11.h>

namespace BH
{
	// @todo: For graphics programmer to expand
	class Mesh
	{
	public:
		// Constructor
		Mesh();

		// Destructor
		~Mesh();

		// Shutdown
		void Shutdown();

	public:
		// Initialise Buffers
		template< typename VertexType >
		bool InitialiseBuffers( const std::vector<VertexType> & vertexBuffer,
								const std::vector<s32> & indexBuffer,
								bool line = false );

		// Bind Buffers
		void BindBuffers() const;

		// Shutdown Buffers
		void ShutdownBuffers();

		// Get Index count
		u32 GetIndexCount() const;

	private:
		ID3D11Buffer * mVertexBuffer;
		ID3D11Buffer * mIndexBuffer;
		//D3D_PRIMITIVE_TOPOLOGY mType;
		bool mLine;

		s32 mVertexCount;
		s32 mIndexCount;
		u32 mStride;		//!< Vertex Buffer stride

		friend class Graphics;
	};

}

#include "Platform/Mesh.inl"

#endif