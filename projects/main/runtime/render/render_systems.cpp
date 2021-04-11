#include "render_systems.hpp"

#include "../../systems/system.hpp"
#include "../runtime_service.hpp"

void framework::runtime::render::render_systems::add(const std::string& name,
	const std::shared_ptr<systems::render_system>& system, uint32 priority)
{
	mRenderSystemsPriority.insert({ name, {priority, mCount++} });

	mRenderSystems.insert({ name, system });
}

void framework::runtime::render::render_systems::set(const std::string& name,
	const std::shared_ptr<systems::render_system>& system, uint32 priority)
{
	if (mRenderSystems.find(name) == mRenderSystems.end()) add(name, system, priority);
	else {
		std::get<0>(mRenderSystemsPriority[name]) = priority;

		mRenderSystems[name] = system;
	}
}

void framework::runtime::render::render_systems::remove(const std::string& name)
{
	mRenderSystemsPriority.erase(name);
	
	mRenderSystems.erase(name);
}

void framework::runtime::render::render_systems::resolve(const runtime_service& service)
{
}

void framework::runtime::render::render_systems::release(const runtime_service& service)
{
	service.render_device.wait();
}

void framework::runtime::render::render_systems::update(const runtime_service& service, const runtime_frame& frame)
{
	mRenderSystemsExecuteOrder = std::vector<std::pair<std::string, std::tuple<uint32, uint32>>>(
		mRenderSystemsPriority.begin(), mRenderSystemsPriority.end());

	std::sort(mRenderSystemsExecuteOrder.begin(), mRenderSystemsExecuteOrder.end(), [&](
		const std::pair<std::string, std::tuple<uint32, uint32>>& lhs,
		const std::pair<std::string, std::tuple<uint32, uint32>>& rhs)
		{
			if (std::get<0>(lhs.second) == std::get<0>(rhs.second)) return std::get<1>(lhs.second) < std::get<1>(rhs.second);

			return std::get<0>(lhs.second) < std::get<0>(rhs.second);
		});

	for (const auto& priority : mRenderSystemsExecuteOrder) {
		if (!mRenderSystems[priority.first]->enable) continue;
		
		mRenderSystems[priority.first]->update(service, frame);
	}
}

void framework::runtime::render::render_systems::render(const runtime_service& service, const runtime_frame& frame)
{
	wait(service.render_device);
	
	for (const auto& priority : mRenderSystemsExecuteOrder) {
		if (!mRenderSystems[priority.first]->enable) continue;
		
		mRenderSystems[priority.first]->render(service, frame);
	}
}

void framework::runtime::render::render_systems::wait(const render_device& device)
{
	device.wait();
}

std::shared_ptr<framework::systems::render_system> framework::runtime::render::render_systems::system(
	const std::string& name) const
{
	return mRenderSystems.find(name)->second;
}

bool framework::runtime::render::render_systems::has(const std::string& name) const noexcept
{
	return mRenderSystems.find(name) != mRenderSystems.end();
}

const std::vector<std::pair<std::string, std::tuple<framework::uint32, framework::uint32>>>&
framework::runtime::render::render_systems::systems_execute_order() const noexcept
{
	return mRenderSystemsExecuteOrder;
}

const std::unordered_map<std::basic_string<char>, std::shared_ptr<framework::systems::render_system>>&
framework::runtime::render::render_systems::systems() const noexcept
{
	return mRenderSystems;
}