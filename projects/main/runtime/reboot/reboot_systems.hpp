#pragma once

#include "../../interfaces/noncopyable.hpp"
#include "../../types.hpp"

#include <memory>
#include <string>

namespace framework::systems {

	class reboot_system;
	
}

namespace framework::runtime {

	struct runtime_service;
	
}

namespace framework::runtime::reboot {

	class reboot_systems final : public interfaces::noncopyable {
	public:
		reboot_systems();

		~reboot_systems() = default;
		
		void resolve(const runtime_service& service);

		void release(const runtime_service& service);
	private:
		void add(const std::string& name, const std::shared_ptr<systems::reboot_system>& system);

		mapping<std::string, std::shared_ptr<systems::reboot_system>> mRebootSystems;
		mapping<std::string, std::tuple<uint32, uint32>> mRebootSystemsPriority;

		std::vector<std::pair<std::string, std::tuple<uint32, uint32>>> mRebootSystemExecuteOrder;

		uint32 mCount = 0;
	};
	
}
