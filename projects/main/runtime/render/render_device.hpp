#pragma once

#include <directx12-wrapper/descriptors/root_signature.hpp>
#include <directx12-wrapper/pipelines/pipeline_state.hpp>
#include <directx12-wrapper/commands/command_queue.hpp>
#include <directx12-wrapper/shaders/shader_code.hpp>
#include <directx12-wrapper/resources/texture2d.hpp>
#include <directx12-wrapper/resources/buffer.hpp>

#include "../../interfaces/noncopyable.hpp"
#include "../../types.hpp"

namespace framework::runtime::render {
	
	class render_device final : public interfaces::noncopyable {
	public:
		render_device() = default;

		render_device(const D3D_FEATURE_LEVEL& level);

		~render_device() = default;

		void wait() const;

		wrapper::directx12::command_queue queue() const noexcept;

		wrapper::directx12::device device() const noexcept;

		wrapper::directx12::fence fence() const noexcept;
	private:
		wrapper::directx12::command_queue mMainQueue;

		wrapper::directx12::device mDevice;
		wrapper::directx12::fence mFence;
	};

	template <typename T>
	void update_buffer_if_smaller_or_empty(const wrapper::directx12::device& device,
		const std::vector<T>& data, wrapper::directx12::buffer& buffer)
	{
		if (!buffer.valid() || buffer.size_in_bytes() < data.size() * sizeof(T)) {
			buffer = wrapper::directx12::buffer::create(device, wrapper::directx12::resource_info::upload(),
				std::max(static_cast<size_t>(256), sizeof(T) * data.size()));
		}

		buffer.copy_from_cpu(data.data(), sizeof(T) * data.size());
	}
	
}
