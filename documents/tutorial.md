# Tutorial

一部分环境教学以及一部分建议了解的内容。

## 开发环境需求

需要注意的是我们的开发是64位软件，而不是32位(x86)，在VS生成的时候需要注意。

- Windows 10
- VisualStudio 2017/2019
- Vcpkg

## Submodule

[子模块](https://git-scm.com/book/zh/v2/Git-%E5%B7%A5%E5%85%B7-%E5%AD%90%E6%A8%A1%E5%9D%97)可以大致认为引用另外一个库作为当前库的一部分，能够很好的方便项目的部署和构建。

目前项目依赖于一个子模块，因此需要在完成`clone`后将子模块初始化。

```
git clone https://github.com/LinkClinton/pog-se.git

git submodule update --init --recursive
```

第一步克隆项目，第二部初始化子模块。

## Vcpkg

[Vcpkg](https://github.com/microsoft/vcpkg)是微软的开源的包管理器，可以用来管理C++的第三方库。

安装方法在项目主页有很详细的中文说明，可以参考。

安装完成后，可以使用vcpkg这个命令行(CLI)软件安装第三方库。例如安装数学库`glm`，目标架构为64位平台为Windows。

```
./vcpkg install glm:x64-windows
```

目前我们这个项目需要你们自己安装的第三方库:

```
./vcpkg install glm::x64-windows
./vcpkg intsall imgui:x64-windows
./vcpkg install nlohmann-json:x64-windows
```

以上指令你得在vcpkg的安装目录调用。

## ImGui

[ImGui](https://github.com/ocornut/imgui)是一个非常简单的GUI库，我们可以使用它来完成我们的UI界面设计和用户交互。官方有一个很好的说明[Demo](https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp)，里面展示了支持的各种UI组件以及其代码。

为了方便熟悉这个库，这里我额外上传一个项目，你可以在这个项目里面的一个代码块里面写ImGui逻辑进行测试和熟悉。

代码路径为: `systems/update_systems/imgui_update_system`

函数名为: `realtime::playground::systems::imgui_update_system::update`

如果想要编译这个项目，你需要额外使用vcpkg安装如下第三方库

```
./vcpkg install stb:x64-windows
./vcpkg install tinyobjloader:x64-windows
./vcpkg install nlohmann-json:x64-windows
```

## Network

网络这部分我不清楚，请负责网络的同学去调研下。

## Resources

这里提供一些资料。

- [ImGui在扫雷中的使用](https://github.com/LinkClinton/MinesweeperVersus/tree/master/Runtime/Manager/UI/Component)：这里每一个Component在主循环中都会被调用用来生成对应的UI界面。

