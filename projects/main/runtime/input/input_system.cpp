#include "input_system.hpp"

#include <Windows.h>

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

bool framework::runtime::input::input_system::keydown(const input_keycode& code) const noexcept
{
	return KEYDOWN(static_cast<int>(code));
}
