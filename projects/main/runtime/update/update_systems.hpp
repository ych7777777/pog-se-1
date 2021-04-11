#pragma once

#include "../../interfaces/noncopyable.hpp"
#include "../../types.hpp"

#include <string>
#include <memory>

namespace framework::systems {

	class render_system;
	class update_system;
	
}

namespace framework::runtime {

	struct runtime_service;
	struct runtime_frame;
	
}

namespace framework::runtime::update {

	class update_systems final : public interfaces::noncopyable {
	public:
		update_systems() = default;

		~update_systems() = default;

		void add(const std::string& name, const std::shared_ptr<systems::update_system>& system, uint32 priority = 0);

		void set(const std::string& name, const std::shared_ptr<systems::update_system>& system, uint32 priority = 0);

		void remove(const std::string& name);

		void resolve(const runtime_service& service);

		void release(const runtime_service& service);
		
		void update(const runtime_service& service, const runtime_frame& frame);

		std::shared_ptr<systems::update_system> system(const std::string& name) const;

		bool has(const std::string& name) const noexcept;

		const std::vector<std::pair<std::string, std::tuple<uint32, uint32>>>& systems_execute_order() const noexcept;
		
		const mapping<std::string, std::shared_ptr<systems::update_system>>& systems() const noexcept;
	private:
		mapping<std::string, std::shared_ptr<systems::update_system>> mUpdateSystems;
		mapping<std::string, std::tuple<uint32, uint32>> mUpdateSystemsPriority;

		std::vector<std::pair<std::string, std::tuple<uint32, uint32>>> mUpdateSystemExecuteOrder;
		
		uint32 mCount = 0;
	};
	
}
