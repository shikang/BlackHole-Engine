#include "Platform/Stdafx.h"

#include "Platform/Vector4f.h"

namespace BH
{
	const Vector4f Vector4f::ZERO = Vector4f(0.0f);
	const Vector4f Vector4f::ONE = Vector4f(1.0f);
	const Vector4f Vector4f::UNIT_X = Vector4f(1.0f, 0.0f, 0.0f, 0.0f);
	const Vector4f Vector4f::UNIT_Y = Vector4f(0.0f, 1.0f, 0.0f, 0.0f);
	const Vector4f Vector4f::UNIT_Z = Vector4f(0.0f, 0.0f, 1.0f, 0.0f);
	const Vector4f Vector4f::UNIT_W = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
}