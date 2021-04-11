#pragma once

#include "../../extensions/imgui/imgui.hpp"
#include "../system.hpp"

namespace framework::systems {

	class imgui_update_system final : public update_system {
	public:
		imgui_update_system() = default;

		~imgui_update_system() = default;

		void update(const runtime_service& service, const runtime_frame& frame) override;
	private:
		uint32 mRenderTargetSizeX = 0;
		uint32 mRenderTargetSizeY = 0;

		ImTextureID mRenderTargetID = nullptr;
	};
	
}
