#pragma once

#include "../../extensions/imgui/imgui.hpp"
#include "../system.hpp"

namespace framework::systems {

	class imgui_update_system final : public update_system {
	public:
		imgui_update_system() = default;

		~imgui_update_system() = default;

		void update(const runtime_service& service, const runtime_frame& frame) override;
	};
	
}
