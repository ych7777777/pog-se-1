#pragma once

#include "resources/resource_system.hpp"
#include "windows/window_systems.hpp"
#include "reboot/reboot_systems.hpp"
#include "update/update_systems.hpp"
#include "render/render_systems.hpp"
#include "render/render_device.hpp"
#include "input/input_system.hpp"

namespace framework::runtime {

	class runtime_process;
	
	struct runtime_service final {
		resources::resource_system& resource_system;
		
		windows::window_systems& window_systems;

		reboot::reboot_systems& reboot_systems;
		
		update::update_systems& update_systems;
		
		render::render_systems& render_systems;
		render::render_device& render_device;

		input::input_system& input_system;

		runtime_service(runtime_process& process);
	};
	
}
