#pragma once
#include"Engine/Render/GraphicsContext.h"

struct GLFWwindow;
namespace ENGINE
{
	class OpenglContext :public GraphicsContext
	{
	public:
		OpenglContext(GLFWwindow*windowHandle);
		void Init()override;
		void SwapBuffer()override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}