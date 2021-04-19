#include "rectangle_reboot_system.hpp"

#include "../update_systems/rectangle_update_systems/rectangle_move_system.hpp"
#include "../update_systems/rectangle_update_systems/rectangle_draw_system.hpp"

void framework::systems::rectangle_reboot_system::resolve(const runtime_service& service)
{
	if (service.resource_system.resource<config>("config").contains("systems")) {
		const auto& systems = service.resource_system.resource<config>("config")["systems"];

		if (systems.find("RectangleMoveSystem") != systems.end()) {
			const auto system = systems.at("RectangleMoveSystem");
			const auto priority = system.at("priority").get<uint32>();

			service.update_systems.add("RectangleMoveSystem", std::make_shared<rectangle_move_system>(), priority);
		}

		if (systems.find("RectangleDrawSystem") != systems.end()) {
			const auto system = systems.at("RectangleDrawSystem");
			const auto priority = system.at("priority").get<uint32>();

			service.update_systems.add("RectangleDrawSystem", std::make_shared<rectangle_draw_system>(), priority);
		}
	}
}

void framework::systems::rectangle_reboot_system::release(const runtime_service& service)
{
}
