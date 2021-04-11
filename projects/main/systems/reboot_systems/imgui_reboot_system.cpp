#include "imgui_reboot_system.hpp"

#include "../render_systems/imgui_render_system.hpp"
#include "../update_systems/imgui_update_system.hpp"

void framework::systems::imgui_reboot_system::resolve(const runtime_service& service)
{
	if (service.resource_system.resource<config>("config").contains("systems")) {
		const auto& systems = service.resource_system.resource<config>("config")["systems"];

		if (systems.find("ImGuiRenderSystem") != systems.end()) {
			const auto system = systems.at("ImGuiRenderSystem");
			const auto priority = system.at("priority").get<uint32>();
			const auto font = system.at("font_size").get<uint32>();
			
			service.render_systems.add("ImGuiRenderSystem", std::make_shared<imgui_render_system>(
				service.window_systems.window().swap_chain(), service.render_device, font), priority);
		}

		if (systems.find("ImGuiUpdateSystem") != systems.end()) {
			const auto system = systems.at("ImGuiRenderSystem");
			const auto priority = system.at("priority").get<uint32>();
			
			service.update_systems.add("ImGuiUpdateSystem", std::make_shared<imgui_update_system>(), priority);
		}
	}
}

void framework::systems::imgui_reboot_system::release(const runtime_service& service)
{
}
