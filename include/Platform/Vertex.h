#ifndef BH_VERTEX_H
#define BH_VERTEX_H

#include "Platform/Platform.h"
#include "Platform/Typedefs.h"
#include "Platform/Vector2f.h"
#include "Platform/Vector3f.h"
#include "Platform/Vector4f.h"

#include <vector>

namespace BH
{
	BH_CONSTEXPR int BH_VERTEX_WEIGHTS_NUM = 4;

	// @todo: For graphics programmer to expand
	struct Vertex
	{
		Vector3f position;
		Vector4f color;
		Vector2f tex;
		Vector3f normal;
	};

	struct SkinVertex
	{
		Vector3f position;
		Vector4f color;
		Vector2f tex;
		Vector3f normal;
		f32 weights[BH_VERTEX_WEIGHTS_NUM];
		s32 weightIndices[BH_VERTEX_WEIGHTS_NUM];
	};

	struct Vertex2D
	{
		Vector2f position;
		Vector4f color;
		Vector2f tex;
	};
}

#endif