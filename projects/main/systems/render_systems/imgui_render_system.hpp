#pragma once

#include <directx12-wrapper/extensions/imgui.hpp>
#include <directx12-wrapper/swap_chain.hpp>

#include "../system.hpp"

namespace framework::systems {

	using runtime::render::render_device;
	
	class imgui_render_system final : public render_system {
	public:
		explicit imgui_render_system(
			const wrapper::directx12::swap_chain& swap_chain,
			const render_device& device, uint32 font_size);

		void update(const runtime_service& service, const runtime_frame& frame) override;

		void render(const runtime_service& service, const runtime_frame& frame) override;

		ImTextureID allocate(const wrapper::directx12::texture2d& resource, size_t index) const;
		
		~imgui_render_system();
	private:
		wrapper::directx12::command_allocator mCommandAllocator;
		wrapper::directx12::graphics_command_list mCommandList;

		wrapper::directx12::descriptor_heap mRenderTargetViewHeap;
		wrapper::directx12::descriptor_heap mImGuiDescriptorHeap;
		
		wrapper::directx12::swap_chain mSwapChain;

		wrapper::directx12::device mDevice;
	};
	
}
