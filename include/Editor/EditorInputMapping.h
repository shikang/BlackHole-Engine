#include <map>

#include "Platform/Keybind.h"

#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE
#include "CEGUI/InputEvent.h"

namespace BH
{
	class EditorInputMapping
	{
	public:
		// Blackhole to CEGUI key mapping
		static std::map< Key::KeyCode, CEGUI::Key::Scan > KeyMappingTable;
		static std::map< Mouse::Button, CEGUI::MouseButton > MouseMappingTable;
	};
}
#endif