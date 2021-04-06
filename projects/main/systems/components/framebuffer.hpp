#pragma once

#include <directx12-wrapper/descriptors/descriptor_heap.hpp>
#include <directx12-wrapper/resources/texture2d.hpp>

namespace framework::systems::components {

	struct framebuffer final {
		std::vector<wrapper::directx12::texture2d> buffers;

		wrapper::directx12::descriptor_heap heap;
		
		framebuffer() = default;
	};
	
}
