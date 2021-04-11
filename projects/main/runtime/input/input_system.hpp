#pragma once

#include "../../interfaces/noncopyable.hpp"
#include "../../types.hpp"

#include "input_keycode.hpp"

namespace framework::runtime::input {

	class input_system final : public interfaces::noncopyable {
	public:
		input_system() = default;

		~input_system() = default;

		bool keydown(const input_keycode& code) const noexcept;
	};
	
}
