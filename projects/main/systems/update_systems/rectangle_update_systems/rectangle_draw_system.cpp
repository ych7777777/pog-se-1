#include "rectangle_draw_system.hpp"

#include "../../../extensions/imgui/imgui.hpp"

void framework::systems::rectangle_draw_system::update(const runtime_service& service, const runtime_frame& frame)
{
	const auto center_x = service.resource_system.resource<float>("rectangle_center_x");
	const auto center_y = service.resource_system.resource<float>("rectangle_center_y");

	const auto half_size = mSize * 0.5f;

	// draw rectangle 
	ImGui::GetBackgroundDrawList()->AddRectFilled(
		ImVec2(center_x - half_size, center_y - half_size),
		ImVec2(center_x + half_size, center_y + half_size),
		ImColor(mRed, mGreen, mBlue)
	);
}
