#pragma once

#include "../system.hpp"

namespace framework::systems {

	class config_reboot_system final : public reboot_system {
	public:
		config_reboot_system() = default;

		~config_reboot_system() = default;

		void resolve(const runtime_service& service) override;

		void release(const runtime_service& service) override;
	};
	
}
