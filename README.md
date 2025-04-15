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
