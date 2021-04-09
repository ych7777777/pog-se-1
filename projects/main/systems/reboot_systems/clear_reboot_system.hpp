#pragma once

#include "../system.hpp"

namespace framework::systems {

	class clear_reboot_system final : public reboot_system {
	public:
		clear_reboot_system() = default;

		~clear_reboot_system() = default;

		void resolve(const runtime_service& service) override;

		void release(const runtime_service& service) override;
	};
	
}
