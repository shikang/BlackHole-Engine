// Precompiled Header
#include "Core/CoreEssential.h"

// Assert
#include "Core/Assert.h"

// Event
#include "Core/Delegate.h"
#include "Core/Event.h"
#include "Core/EventManager.h"
#include "Core/Message.h"
#include "Core/JobQueue.h"

// String
#include "Core/CName.h"
#include "Core/Hash.h"

// Memory
#include "Core/ObjectAllocator.h"

// Reflection
#include "Core/EnumConst.h"
#include "Core/Typing.h"
#include "Core/TypeCreator.h"
#include "Core/FieldContainer.h"
#include "Core/Field.h"
#include "Core/Type.h"
#include "Core/TypeDB.h"
#include "Core/TypeRegistrar.h"
#include "Core/Entity.h"
#include "Core/EntityManager.h"

// Serialise
#include "Core/TypeStringifier.h"
#include "Core/Serialiser.h"
#include "Core/Deserialiser.h"
#include "Core/XMLSerialiser.h"
#include "Core/XMLDeserialiser.h"

// Profiler
#include "Core/Profiling.h"
#include "Core/Profiler.h"
#include "Core/ProfileManager.h"

// Service
#include "Core/IUpdatable.h"
#include "Core/IDrawable.h"

// Game Component
#include "Core/GameComponent.h"

// Object
#include "Core/ObjectComponent.h"
#include "Core/Object.h"
#include "Core/ObjectManager.h"

// System
#include "Core/System.h"
#include "Core/SystemManager.h"

// Log
#include "Core/LogManager.h"

// Time
#include "Core/Time.h"
#include "Core/TimePoint.h"
#include "Core/Duration.h"
#include "Core/Stopwatch.h"

// Game Components
#include "Core/InputManager.h"
#include "Core/Renderer.h"
#include "Core/ModelManager.h"
#include "Core/TextureManager.h"
#include "Core/HDRTextureManager.h"
#include "Core/AnimationControllerManager.h"

// Game State
#include "Core/GameState.h"
#include "Core/GameStateManager.h"
#include "Core/LevelState.h"
#include "Core/LevelManager.h"

// Graphics
#include "Core/Materials.h"
#include "Core/Light.h"

// Collision
#include "Core/OBB.h"
#include "Core/Contact.h"
#include "Core/CollisionFunctions.h"
#include "Core/CollisionManager.h"