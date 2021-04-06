#pragma once

#include "render_device.hpp"

namespace framework::runtime::render {

	struct render_pipeline final {
		wrapper::directx12::root_signature_info root_signature_info;
		wrapper::directx12::root_signature root_signature;

		wrapper::directx12::input_assembly_info input_assembly;
		wrapper::directx12::rasterization_info rasterization;
		wrapper::directx12::depth_stencil_info depth_stencil;
		wrapper::directx12::blend_info blend;

		wrapper::directx12::shader_code vert_shader;
		wrapper::directx12::shader_code frag_shader;

		wrapper::directx12::graphics_pipeline_info graphics_pipeline_info;
		wrapper::directx12::pipeline_state graphics_pipeline;

		std::vector<DXGI_FORMAT> rtv_format = {};
		
		DXGI_FORMAT dsv_format = DXGI_FORMAT_UNKNOWN;
		
		render_pipeline() = default;
	};

	class render_pipelines final {
	public:
		render_pipelines() = default;

		~render_pipelines() = default;

		uint32 index(const std::string& pipeline, const std::string& name) const noexcept;
		
		bool empty() const noexcept;
		
		const render_pipeline& at(const std::string& name) const;
		
		render_pipeline& operator[](const std::string& name);
	private:
		mapping<std::string, render_pipeline> mPipelines;
	};
	
}
