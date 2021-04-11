#include "clear_reboot_system.hpp"

#include "../render_systems/clear_render_system.hpp"

void framework::systems::clear_reboot_system::resolve(const runtime_service& service)
{
	if (service.resource_system.resource<config>("config").contains("systems")) {
		const auto& systems = service.resource_system.resource<config>("config")["systems"];

		if (systems.find("ClearRenderSystem") != systems.end()) {
			const auto system = systems.at("ClearRenderSystem");
			const auto priority = system.at("priority").get<uint32>();

			service.render_systems.add("ClearRenderSystem", std::make_shared<clear_render_system>(), priority);
		}
	}
}

void framework::systems::clear_reboot_system::release(const runtime_service& service)
{
}
