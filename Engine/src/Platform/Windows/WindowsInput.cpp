#include "pch.h"
#include "WindowsInput.h"
#include"Engine/Application.h"
#include"GLFW/glfw3.h"
namespace ENGINE {
    Input*Input::m_instance = new WindowsInput();
    bool ENGINE::WindowsInput::IsKeyPressImpl(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());
        bool state = glfwGetKey(window, keycode);
        #pragma warning (suppress : 4806)
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressImpl(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());
        bool state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> WindowsInput::GetMousePositionImpl()
    {
        auto window = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return { (float)x,(float)y };
    }

    float WindowsInput::GetMouseXImpl()
    {
        return GetMousePositionImpl().first;
    }

    float WindowsInput::GetMouseYImpl()
    {
        return GetMousePositionImpl().second;
    }

}
