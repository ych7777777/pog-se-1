#pragma once

#include "../system.hpp"

namespace framework::systems {

	class rectangle_reboot_system final : public reboot_system {
	public:
		rectangle_reboot_system() = default;

		~rectangle_reboot_system() = default;

		void resolve(const runtime_service& service) override;

		void release(const runtime_service& service) override;
	};
	
}
