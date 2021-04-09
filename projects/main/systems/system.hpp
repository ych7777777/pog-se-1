#pragma once

#include "../runtime/runtime_service.hpp"
#include "../runtime/runtime_frame.hpp"

namespace framework::systems {

	using runtime::runtime_service;
	using runtime::runtime_frame;
	
	class system : public interfaces::noncopyable {
	public:
		system() = default;

		~system() = default;

		bool enable = true;
	};

	class reboot_system : public system {
	public:
		reboot_system() = default;

		~reboot_system() = default;

		virtual void resolve(const runtime_service& service) = 0;

		virtual void release(const runtime_service& service) = 0;
	};
	
	class update_system : public system {
	public:
		update_system() = default;

		~update_system() = default;

		virtual void update(const runtime_service& service, const runtime_frame& frame) = 0;
	};

	class render_system : public update_system {
	public:
		render_system() = default;

		~render_system() = default;

		virtual void render(const runtime_service& service, const runtime_frame& frame) = 0;
	};
}
