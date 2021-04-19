#include "rectangle_move_system.hpp"

void framework::systems::rectangle_move_system::update(const runtime_service& service, const runtime_frame& frame)
{
	// if we do not have rectangle center x and y, we need add it into our resources
	if (!service.resource_system.has<float>("rectangle_center_x") &&
		!service.resource_system.has<float>("rectangle_center_y")) {

		const auto center_x = service.window_systems.window().size_x() * 0.5f;
		const auto center_y = service.window_systems.window().size_y() * 0.5f;

		service.resource_system.add("rectangle_center_x", center_x);
		service.resource_system.add("rectangle_center_y", center_y);
	}

	auto& center_x = service.resource_system.resource<float>("rectangle_center_x");
	auto& center_y = service.resource_system.resource<float>("rectangle_center_y");

	float delta_vector_x = 0;
	float delta_vector_y = 0;

	// record which keycode down
	if (service.input_system.keydown(runtime::input::input_keycode::W)) delta_vector_y -= 1;
	if (service.input_system.keydown(runtime::input::input_keycode::S)) delta_vector_y += 1;
	if (service.input_system.keydown(runtime::input::input_keycode::A)) delta_vector_x -= 1;
	if (service.input_system.keydown(runtime::input::input_keycode::D)) delta_vector_x += 1;

	constexpr float speed = 200;

	// update center position
	center_x += delta_vector_x * frame.delta_time * speed;
	center_y += delta_vector_y * frame.delta_time * speed;
}
