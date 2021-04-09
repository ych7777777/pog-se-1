#include "clear_render_system.hpp"

#include "../components/framebuffer.hpp"

void framework::systems::clear_render_system::update(const runtime_service& service, const runtime_frame& frame)
{
	if (mCommandAllocator.valid()) return;

	mCommandAllocator = wrapper::directx12::command_allocator::create(service.render_device.device());
	mCommandList = wrapper::directx12::graphics_command_list::create(service.render_device.device(), mCommandAllocator);
}

void framework::systems::clear_render_system::render(const runtime_service& service, const runtime_frame& frame)
{
	mCommandAllocator->Reset();
	mCommandList->Reset(mCommandAllocator.get(), nullptr);
	
	std::vector<components::framebuffer> render_targets;
	std::vector<components::framebuffer> depth_stencils;

	//render_targets.push_back(service.resource_system.resource<components::framebuffer>("Main.RenderTarget"));
	//depth_stencils.push_back(service.resource_system.resource<components::framebuffer>("Main.DepthStencil"));

	if (clear_render_target && !render_targets.empty()) {

		for (const auto& render_target : render_targets) {
			for (size_t index = 0; index < render_target.buffers.size(); index++) {
				const auto& buffer = render_target.buffers[index];
				
				buffer.barrier(mCommandList, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);

				mCommandList.clear_render_target_view(render_target.heap.cpu_handle(index), { 1,1,1,1 });

				buffer.barrier(mCommandList, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
			}
		}
		
	}

	if (clear_depth_stencil && !depth_stencils.empty()) {

		for (const auto& depth_stencil : depth_stencils) {
			for (size_t index = 0; index < depth_stencil.buffers.size(); index++) {
				const auto& buffer = depth_stencil.buffers[index];

				buffer.barrier(mCommandList, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

				mCommandList.clear_depth_stencil_view(depth_stencil.heap.cpu_handle(index), 1.f, 0);

				buffer.barrier(mCommandList, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
			}
		}
		
	}

	mCommandList.close();

	service.render_device.queue().execute({ mCommandList });
}
