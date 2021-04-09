#include "update_systems.hpp"

#include "../../systems/system.hpp"
#include "../runtime_service.hpp"

void framework::runtime::update::update_systems::add(const std::string& name,
	const std::shared_ptr<systems::update_system>& system, uint32 priority)
{
	mUpdateSystemsPriority.insert({ name, { priority, mCount++ } });

	mUpdateSystems.insert({ name, system });
}

void framework::runtime::update::update_systems::set(const std::string& name,
	const std::shared_ptr<systems::update_system>& system, uint32 priority)
{
	if (mUpdateSystems.find(name) == mUpdateSystems.end()) add(name, system, priority);
	else {
		std::get<0>(mUpdateSystemsPriority[name]) = priority;

		mUpdateSystems[name] = system;
	}
}

void framework::runtime::update::update_systems::remove(const std::string& name)
{
	mUpdateSystemsPriority.erase(name);

	mUpdateSystems.erase(name);
}

void framework::runtime::update::update_systems::resolve(const runtime_service& service)
{
}

void framework::runtime::update::update_systems::release(const runtime_service& service)
{
}

void framework::runtime::update::update_systems::update(const runtime_service& service, const runtime_frame& frame)
{
	mUpdateSystemExecuteOrder = std::vector<std::pair<std::string, std::tuple<uint32, uint32>>>(
		mUpdateSystemsPriority.begin(), mUpdateSystemsPriority.end());

	std::sort(mUpdateSystemExecuteOrder.begin(), mUpdateSystemExecuteOrder.end(), [&](
		const std::pair<std::string, std::tuple<uint32, uint32>>& lhs,
		const std::pair<std::string, std::tuple<uint32, uint32>>& rhs)
		{
			if (std::get<0>(lhs.second) == std::get<0>(rhs.second)) return std::get<1>(lhs.second) < std::get<1>(rhs.second);

			return std::get<0>(lhs.second) < std::get<0>(rhs.second);
		});

	for (const auto& priority : mUpdateSystemExecuteOrder) {
		if (!mUpdateSystems[priority.first]->enable) continue;
		
		mUpdateSystems[priority.first]->update(service, frame);
	}
}

std::shared_ptr<framework::systems::update_system> framework::runtime::update::update_systems::system(
	const std::string& name) const
{
	return mUpdateSystems.find(name)->second;
}

bool framework::runtime::update::update_systems::has(const std::string& name) const noexcept
{
	return mUpdateSystems.find(name) != mUpdateSystems.end();
}

const std::vector<std::pair<std::string, std::tuple<framework::uint32, framework::uint32>>>&
	framework::runtime::update::update_systems::systems_execute_order() const noexcept
{
	return mUpdateSystemExecuteOrder;
}

const std::unordered_map<std::basic_string<char>, std::shared_ptr<framework::systems::update_system>>&
	framework::runtime::update::update_systems::systems() const noexcept
{
	return mUpdateSystems;
}
