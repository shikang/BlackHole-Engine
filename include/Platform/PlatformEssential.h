#ifndef BH_PLATFORM_ESSENTIAL_H
#define BH_PLATFORM_ESSENTIAL_H

// Forward Declaration found in platform layer
namespace BH
{
	// File System
	class FileInfo;
	class FileSystem;

	// Math
	class Vector2f;
	class Vector3f;
	class Vector4f;
	class Matrix4;
	class Plane;
	class Quaternion;

	// Graphics
	class Graphics;
	class Mesh;
	class Skeleton;
	struct Animation;
	class Camera;
	class Shader;
	class Texture;
	class UAVTexture;
	class HDRTexture;
	class RenderTarget;
	class DepthRenderTarget;
	class MultiRenderTarget;
	class GPUOBuffer;
	class GPUIBuffer;
	struct AABB;
}

// STL
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

#include <array>
#include <vector>
#include <stack>
#include <deque>
#include <queue>
#include <list>

#include <string>
#include <bitset>

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

#include <functional>
#include <algorithm>
#include <iterator>
#include <limits>

#include <utility>
#include <tuple>
#include <type_traits>

#include <memory>
//#include <thread>
//#include <mutex>
//#include <atomic>

//#include <hash_set>
//#include <hash_map>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

#endif