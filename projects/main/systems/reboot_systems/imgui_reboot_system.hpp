#pragma once

#include "../system.hpp"

namespace framework::systems {

	class imgui_reboot_system final : public reboot_system {
	public:
		imgui_reboot_system() = default;

		~imgui_reboot_system() = default;

		void resolve(const runtime_service& service) override;

		void release(const runtime_service& service) override;
	};
	
}
