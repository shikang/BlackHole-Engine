#ifndef BH_VERTEX_H
#define BH_VERTEX_H

#include "Platform/Typedefs.h"
#include "Platform/Vector2f.h"
#include "Platform/Vector3f.h"
#include "Platform/Vector4f.h"

#include <vector>

namespace BH
{
	// @todo: For graphics programmer to expand
	struct Vertex
	{
		Vector3f position;
		Vector4f color;
		Vector2f tex;
		Vector3f normal;
	};

	struct Vertex2D
	{
		Vector2f position;
		Vector4f color;
		Vector2f tex;
	};

	// Skinning Data
	struct JointWeight
	{
		f32 mWeight;
		u32 mIndex;
	};

	typedef std::vector< JointWeight > WeightVector;
}

#endif