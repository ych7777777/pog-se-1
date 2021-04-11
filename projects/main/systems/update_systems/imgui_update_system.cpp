#include "imgui_update_system.hpp"

#include "../render_systems/imgui_render_system.hpp"
#include "../components/framebuffer.hpp"

void framework::systems::imgui_update_system::update(const runtime_service& service, const runtime_frame& frame)
{
	ImGui::NewFrame();
}
