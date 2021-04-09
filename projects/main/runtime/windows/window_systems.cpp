#include "window_systems.hpp"

#include "../../extensions/json/json.hpp"

#include "../runtime_service.hpp"
#include "../runtime_frame.hpp"

void framework::runtime::windows::window_systems::resolve(const runtime_service& service)
{
	const auto& window_property = service.resource_system.resource<config>("property")["window"];
	
	mMainWindow = main_window(service.render_device.queue(), service.render_device.device(),
		window_property["name"], window_property["size_x"], window_property["size_y"]);
}

void framework::runtime::windows::window_systems::release(const runtime_service& service)
{
}

void framework::runtime::windows::window_systems::update(const runtime_service& service, const runtime_frame& frame)
{
	if (mMainWindow.living() == false) return;
	
	mMainWindow.update(service, frame);
}

void framework::runtime::windows::window_systems::render(const runtime_service& service, const runtime_frame& frame)
{
	if (mMainWindow.living() == false) return;
	
	mMainWindow.present(true);
}

const framework::runtime::windows::main_window& framework::runtime::windows::window_systems::window() const noexcept
{
	return mMainWindow;
}

bool framework::runtime::windows::window_systems::living() const noexcept
{
	return mMainWindow.living();
}

bool framework::runtime::windows::window_systems::active() const noexcept
{
	return mMainWindow.mHandle != nullptr;
}