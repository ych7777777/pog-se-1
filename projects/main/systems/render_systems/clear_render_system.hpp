#pragma once

#include "../system.hpp"

namespace framework::systems {

	class clear_render_system final : public render_system {
	public:
		clear_render_system() = default;

		~clear_render_system() = default;

		void update(const runtime_service& service, const runtime_frame& frame) override;

		void render(const runtime_service& service, const runtime_frame& frame) override;

		bool clear_render_target = true;
		bool clear_depth_stencil = true;
	private:
		wrapper::directx12::command_allocator mCommandAllocator;
		wrapper::directx12::graphics_command_list mCommandList;
	};
	
}
