#pragma once

#include <directx12-wrapper/swap_chain.hpp>

#include "../../interfaces/noncopyable.hpp"
#include "../../types.hpp"

#include <string>

namespace framework::runtime {

	struct runtime_service;
	struct runtime_frame;
	
}

namespace framework::runtime::windows {

	using handle = void*;

	class main_window final : public interfaces::noncopyable {
	public:
		main_window() = default;

		main_window(
			const wrapper::directx12::command_queue& queue,
			const wrapper::directx12::device& device,
			const std::string& name, uint32 size_x, uint32 size_y);

		main_window(main_window&& window) noexcept;

		~main_window();

		void update(const runtime_service& service, const runtime_frame& frame);

		void present(bool sync = true) const;

		bool living() const noexcept;

		uint32 size_x() const noexcept;

		uint32 size_y() const noexcept;

		wrapper::directx12::swap_chain swap_chain() const noexcept;
		
		main_window& operator=(main_window&& window) noexcept;

		friend class window_systems;
	private:
		wrapper::directx12::swap_chain mSwapChain;

		uint32 mSizeX = 0;
		uint32 mSizeY = 0;
		
		handle mHandle = nullptr;
		bool mLiving = false;
	};

}
