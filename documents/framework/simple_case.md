# SimpleCase

这里实现一个使用`WSAD`控制小方块进行移动的例子。

## UI

由于我们只有一个小方块，因此我们只需要渲染一个小的方块就行。问题是我们得需要知道方块的位置在哪。而UI不管逻辑，因此它需要从逻辑那里获取到。

我们额外添加一个`update_system`并且保证其在`imgui_update_system`之后运行。这样做的目的是避免修改`imgui_update_system`的同时我们也可以在我们新添加的系统里面调用绘制函数。

## Logic

对于逻辑来说，他需要读取按键的输入来对我们的小方块的位置进行移动。

## Framework

因此我们总共需要两个`update_system`一个用来渲染小方块一个用来移动小方块。

- `rectangle_draw_system` : 用来渲染小方块
- `rectangle_move_system` ：用来移动小方块

以及一个`reboot_system`用来从配置加载我们的系统。同时还增加了一个配置文件`simple_config.json`用来加载我们的这个例子。当你在`config_reboot_system`中切换加载的`config`文件的时候就可以运行不同的例子。比如说你加载`simple_config`就会运行这个例子，加载`default_config`就是空白内容。

## Code

这里我就放核心代码即可。

```C++
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
```

这里是移动系统的代码，首先我们判断是否放入了小方块位置数据到`resource_system`里面，如果没有那么就放入。然后我们从`resource_system`中获取上一帧我们的小方块的位置，判断输入对其位置进行更新。

```C++
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
```

这里是绘制系统的代码，首先我们获取小方块的位置，然后计算出其左上角和右下角位置，调用ImGui渲染。注意绘制系统要在移动系统之后运行。

```C++
void framework::systems::rectangle_reboot_system::resolve(const runtime_service& service)
{
	if (service.resource_system.resource<config>("config").contains("systems")) {
		const auto& systems = service.resource_system.resource<config>("config")["systems"];

		if (systems.find("RectangleMoveSystem") != systems.end()) {
			const auto system = systems.at("RectangleMoveSystem");
			const auto priority = system.at("priority").get<uint32>();

			service.update_systems.add("RectangleMoveSystem", std::make_shared<rectangle_move_system>(), priority);
		}

		if (systems.find("RectangleDrawSystem") != systems.end()) {
			const auto system = systems.at("RectangleDrawSystem");
			const auto priority = system.at("priority").get<uint32>();

			service.update_systems.add("RectangleDrawSystem", std::make_shared<rectangle_draw_system>(), priority);
		}
	}
}
```

这里是从配置文件加载系统的代码，我们通过`resource_system`获取配置文件判断是否要加载我们的系统。同样我们也可以在这里将`rectangle_center_xxx`放入到`resource_system`里面，避免了在移动系统里面进行判断。