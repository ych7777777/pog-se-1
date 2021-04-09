#include "runtime_process.hpp"
#include "runtime_frame.hpp"

#include "../extensions/json/json.hpp"

#include <chrono>

framework::runtime::runtime_process::runtime_process() :
	mRenderDevice(D3D_FEATURE_LEVEL_11_0), mRuntimeService(*this)
{
	mResourceSystem.add("property", extensions::json::read_config_from_file("./resources/property.json"));
	mResourceSystem.add("config", extensions::json::read_config_from_file("./resources/configs/default_config.json"));
}

void framework::runtime::runtime_process::run_loop()
{
	using time_point = std::chrono::high_resolution_clock;

	mWindowSystems.resolve(mRuntimeService);
	mRebootSystems.resolve(mRuntimeService);
	mUpdateSystems.resolve(mRuntimeService);
	mRenderSystems.resolve(mRuntimeService);
	
	auto current = time_point::now();
	
	runtime_frame frame;

	while (mWindowSystems.living() && mWindowSystems.active()) {
		auto duration = std::chrono::duration_cast<std::chrono::duration<float>>(time_point::now() - current);

		current = time_point::now();

		// if the duration time more than 1s, we think we use pause to debug program
		// so the delta time is 0
		frame.delta_time = duration.count() > 1 ? 0 : duration.count();
		frame.total_time = frame.total_time + duration.count();

		mWindowSystems.update(mRuntimeService, frame);
		mUpdateSystems.update(mRuntimeService, frame);
		mRenderSystems.update(mRuntimeService, frame);

		mRenderSystems.render(mRuntimeService, frame);
		mWindowSystems.render(mRuntimeService, frame);
		
		frame.frame_index++;
	}

	mWindowSystems.release(mRuntimeService);
	mUpdateSystems.release(mRuntimeService);
	mRenderSystems.release(mRuntimeService);
	mRebootSystems.release(mRuntimeService);
}