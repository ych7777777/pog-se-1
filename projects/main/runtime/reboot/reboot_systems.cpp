#include "reboot_systems.hpp"

#include "../../systems/reboot_systems/rectangle_reboot_system.hpp"
#include "../../systems/reboot_systems/config_reboot_system.hpp"
#include "../../systems/reboot_systems/clear_reboot_system.hpp"
#include "../../systems/reboot_systems/imgui_reboot_system.hpp"

#include "../../systems/system.hpp"
#include "../runtime_service.hpp"

#include <algorithm>

namespace framework::runtime::reboot {

	std::vector<std::pair<std::string, std::tuple<uint32, uint32>>> update_execute_order(
		const mapping<std::string, std::tuple<uint32, uint32>>& priority)
	{
		auto execute_order = std::vector<std::pair<std::string, std::tuple<uint32, uint32>>>(priority.begin(), priority.end());

		std::sort(execute_order.begin(), execute_order.end(), [&](
			const std::pair<std::string, std::tuple<uint32, uint32>>& lhs,
			const std::pair<std::string, std::tuple<uint32, uint32>>& rhs)
			{
				if (std::get<0>(lhs.second) == std::get<0>(rhs.second)) return std::get<1>(lhs.second) < std::get<1>(rhs.second);

				return std::get<0>(lhs.second) < std::get<0>(rhs.second);
			});

		return execute_order;
	}
	
}

framework::runtime::reboot::reboot_systems::reboot_systems()
{
	add("ConfigRebootSystem", std::make_shared<systems::config_reboot_system>());
	add("ClearRebootSystem", std::make_shared<systems::clear_reboot_system>());
	add("ImGuiRebootSystem", std::make_shared<systems::imgui_reboot_system>());
	add("RectangleRebootSystem", std::make_shared<systems::rectangle_reboot_system>());
}

void framework::runtime::reboot::reboot_systems::resolve(const runtime_service& service)
{
	mRebootSystemExecuteOrder = update_execute_order(mRebootSystemsPriority);

	for (const auto& priority : mRebootSystemExecuteOrder) {
		if (!mRebootSystems[priority.first]->enable) continue;

		mRebootSystems[priority.first]->resolve(service);
	}
}

void framework::runtime::reboot::reboot_systems::release(const runtime_service& service)
{
	// unnecessary but beautiful
	mRebootSystemExecuteOrder = update_execute_order(mRebootSystemsPriority);

	for (const auto& priority : mRebootSystemExecuteOrder) {
		if (!mRebootSystems[priority.first]->enable) continue;

		mRebootSystems[priority.first]->release(service);
	}
}

void framework::runtime::reboot::reboot_systems::add(const std::string& name, const std::shared_ptr<systems::reboot_system>& system)
{
	mRebootSystemsPriority.insert({ name, { mCount, mCount } });

	mRebootSystems.insert({ name, system });

	mCount++;
}
