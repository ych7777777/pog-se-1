#pragma once

#include "runtime_service.hpp"

namespace framework::runtime {

	class runtime_process final : public interfaces::noncopyable {
	public:
		runtime_process();

		~runtime_process() = default;

		void run_loop();

		friend struct runtime_service;
	private:
		resources::resource_system mResourceSystem;
		
		windows::window_systems mWindowSystems;

		reboot::reboot_systems mRebootSystems;
		
		update::update_systems mUpdateSystems;
		
		render::render_systems mRenderSystems;
		render::render_device mRenderDevice;

		input::input_system mInputSystem;
		
		runtime_service mRuntimeService;
	};
	
}
