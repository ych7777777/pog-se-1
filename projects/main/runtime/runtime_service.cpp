#include "runtime_service.hpp"

#include "runtime_process.hpp"

framework::runtime::runtime_service::runtime_service(runtime_process& process) :
	resource_system(process.mResourceSystem), window_systems(process.mWindowSystems),
	reboot_systems(process.mRebootSystems), update_systems(process.mUpdateSystems),
	render_systems(process.mRenderSystems), render_device(process.mRenderDevice),
	input_system(process.mInputSystem)
{
}
