#ifndef BH_CORE_ESSENTIAL_H
#define BH_CORE_ESSENTIAL_H

#pragma warning(disable:4201)

#include "Platform/PlatformEssential.h"
#include "Platform/Typedefs.h"

// Forward Declaration found in Core layer
namespace BH
{
	// String
	class Hash;
	struct CName;

	// Memory
	template< typename T > class ObjectAllocator;

	// Reflection
	class EnumConst;
	class FieldContainer;
	class FieldAttributes;
	class Field;
	class Type;
	class TypeDB;
	class Entity;
	class EntityManager;

	// Handles
	class HandleManager;

	// Services
	class IUpdatable;
	class IDrawable;
	class System;
	class SystemManager;
	class GameComponent;
	class ObjectComponent;
	class Object;
	class ObjectManager;
	class Game;
	class GameState;
	class LevelState;

	// Graphics
	class Material;
	class Phong;
	class Light;
	class DirectionalLight;
	class LocalLight;
	class Model;

	// Game Components
	class InputManager;
	class GameStateManager;
	class Renderer;
	class TextureManager;
	class ModelManager;
	class MaterialManager;

	// Collision
	struct Contact;
	struct Manifold;
	struct OBB;
	class CollisionManager;
}

#endif