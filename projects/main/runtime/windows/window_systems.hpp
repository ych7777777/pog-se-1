#pragma once

#include "main_window.hpp"

namespace framework::runtime {

	struct runtime_service;
	struct runtime_frame;
	
}

namespace framework::runtime::windows {

	class window_systems final : public interfaces::noncopyable {
	public:
		window_systems() = default;

		~window_systems() = default;

		void resolve(const runtime_service& service);

		void release(const runtime_service& service);
		
		void update(const runtime_service& service, const runtime_frame& frame);

		void render(const runtime_service& service, const runtime_frame& frame);

		const main_window& window() const noexcept;
		
		bool living() const noexcept;

		bool active() const noexcept;
	private:
		main_window mMainWindow;
	};
	
}
