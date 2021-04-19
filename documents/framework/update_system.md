# UpdateSystem

简单介绍下如何编写`UpdateSystem`以及如何使用。这里我们以`imgui_update_system`为例子(它的作用是调用ImGui的函数来渲染我们的UI)。

首先我们介绍下`UpdateSystem`的特性。

- 要求继承自`update_system`，拥有一个主要成员函数`update`，在每一帧中都会调用一次。
- `UpdateSystem`一样需要我们手动加载到`update_systems`，如果想实现通过配置文件加载需要我们手动增加一个对应的`reboot_system`(这样可以避免硬编码的系统)。

## update_systems

是所有`update_system`的集合，用来管理所有的`update_system`，例如在程序每一帧的时候会遍历每个系统去调用其`update`函数。

## Example

首先我们设计一个调用ImGui函数渲染UI的系统，由于我们的ImGui需要在`ImGui::NewFrame`后编写代码告诉它我们如何绘制当前一帧的UI。因此我们需要每一帧调用`ImGui::NewFrame`以及后续的绘制代码。那么我们可以通过实现一个`update_system`，通过它的`update`函数来实现我们要的功能。

```C++
namespace framework::systems {

	class imgui_update_system final : public update_system {
	public:
		imgui_update_system() = default;

		~imgui_update_system() = default;

		void update(const runtime_service& service, const runtime_frame& frame) override;
	};
	
}
```

我们可以实`update`函数

```C++
void framework::systems::imgui_update_system::update(const runtime_service& service, const runtime_frame& frame)
{
    ImGui::NewFrame();

    // put imgui code
}

```

我们可以在`ImGui::NewFrame`之后调用我们要渲染的UI组件对应的ImGui代码。这里建议不要将整个项目的代码都直接放在这个函数里面，而是自己通过OOP的设计思路将其分为各种组件然后在函数中调用。

实现我们的`UpdateSystem`后我们还可以实现一个从配置文件中加载我们的`imgui_update_system`到`update_systems`的办法。首先我们分析我们需要什么时候加载，由于目前设计，我们并不允许动态的加载系统(但是可以动态的禁用系统)，因此我们只能在程序初始化的时候加载系统。那么一个简单的思路就有了，通过调用`reboot_system`来检查配置文件中是否要求我们加载`imgui_update_system`，如果要，那么就将其加入到`update_systems`，否则不加入。

```C++
void framework::systems::imgui_reboot_system::resolve(const runtime_service& service)
{
	if (service.resource_system.resource<config>("config").contains("systems")) {
		const auto& systems = service.resource_system.resource<config>("config")["systems"];

		if (systems.find("ImGuiUpdateSystem") != systems.end()) {
			const auto system = systems.at("ImGuiRenderSystem");
			const auto priority = system.at("priority").get<uint32>();
			
			service.update_systems.add("ImGuiUpdateSystem", std::make_shared<imgui_update_system>(), priority);
		}
	}
}
```

这里是我们加载`imgui_update_system`的实现，其他部分和[reboot_system.md](./reboot_system.md)中的没有太大区别，因此就不放了。

可以看到我们在`config_reboot_system`加载的设置文件在这里被访问了，然后我们去询问设置文件里面是否存在`systems`条目，以及`systems`条目里面是否存在`ImGuiUpdateSystem`。如果存在，那么我们读取其系统属性(比如说优先级)，然后将其加载到`update_systems`里面。