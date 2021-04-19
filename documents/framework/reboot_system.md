# RebootSystem

简单介绍如何编写`RebootSystem`以及如何使用。这里以我们的`ConfigRebootSystem`为例子(它的作用是从我们的磁盘中读取设置文件到内存中)。如果你想在运行过程中动态的读取，请参考`UpdateSystem`或者提前读取到内存中。

首先我们简单介绍下`RebootSystem`的特性。

- 要求继承自`reboot_system`，拥有两个主要的成员函数`resolve`和`release`。其中前者会在程序刚开始运行的时候调用，后者会在程序结束的时候调用。
- `RebootSystem`实现后需要手动加载到我们的`reboot_systems`中去。

## reboot_systems

是所有的`reboot_system`的集合，用来管理所有的`RebootSystem`，例如在程序刚运行的时候遍历每个系统去调用其`resolve`函数。

## Example

首先我们设计一个在程序最开始运行的时候读取设置文件的系统，由于`RebootSystem`的特性，我们知道其会在程序最开始运行的时候调用`resolve`初始化。因此我们可以实现一个`reboot_system`的子类并且重写其虚函数`resolve`。

```C++
namespace framework::systems {

	class config_reboot_system final : public reboot_system {
	public:
		config_reboot_system() = default;

		~config_reboot_system() = default;

		void resolve(const runtime_service& service) override;

		void release(const runtime_service& service) override;
	};
	
}
```

然后我们可以实现这两个虚函数。

```C++
void framework::systems::config_reboot_system::resolve(const runtime_service& service)
{
	// add your config file from resources

	service.resource_system.add("config", read_config_from_file("./resources/configs/default_config.json"));
}

void framework::systems::config_reboot_system::release(const runtime_service& service)
{
	// release your resources
}
```

可以看到我们在`resolve`中将读取的配置文件存放在`resource_system`里面。成功实现了系统后，我们还需要将其加入到`reboot_systems`内(程序初始化的时候只会调用`reboot_systems`内的系统)。

```C++
framework::runtime::reboot::reboot_systems::reboot_systems()
{
	add("ConfigRebootSystem", std::make_shared<systems::config_reboot_system>());
	add("ClearRebootSystem", std::make_shared<systems::clear_reboot_system>());
	add("ImGuiRebootSystem", std::make_shared<systems::imgui_reboot_system>());
}
```

上面是加入三个`RebootSystem`的情况，其运行顺序为加载顺序。