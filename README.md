# MyEngine

MyEngine 是一个开源的 2D 游戏引擎，设计用于帮助开发者构建高效、可扩展的游戏。该引擎结合了现代的ECS、OpenGL 渲染、音频管理系统、Box2D 物理引擎和 C# 脚本系统，并且有基于ImGui的可交互界面UI。

## 项目结构

│ ├── /Engine # 引擎核心代码（C++）

│ ├── /Game # 引擎UI层和渲染层

│ ├── /ScriptEngine # c#部分的核心代码

│ ├── /box2d # box2d库，支持物理引擎功能

│ ├── /GLFW # GLFW库,支持opengl渲染功能

│ ├── /glad # glad库，支持opengl渲染功能

│ ├── /ImGui # ImGui库，支持UI系统功能

│ ├── /miniaudio # miniaudio库，支持音频系统功能

│ ├── /stb_image # stb_iamge库，支持图像加载功能

│ ├── /yaml # yaml ,用于场景的序列化和反序列化

│ ├── /glm # 数学库

│ ├── /Mono # Mono库，用于支持内嵌c#功能

│ ├── /VulkanSDK /Third # 其他第三方库和头文件


## 特性

- **2D 渲染引擎**：基于 OpenGL 提供2D，包括纹理、文字和帧缓冲，采用批处理的方式渲染。
- **物理引擎**：集成 Box2D 用于高效的 2D 物理模拟，包括重力模拟，碰撞模拟等。
- **ECS系统**：采用 EnTT 组件架构，在场景中管理实体和组件，并提供了序列化和反序列化的功能。
- **C#脚本系统**：引擎通过 Mono 提供对 C# 脚本的支持，使得游戏逻辑可以灵活编写。
- **UI界面**：引擎使用ImGui来提供简洁的UI界面
- **抽象化设计**：渲染接口抽象化，为图形AIP切换提供基础
