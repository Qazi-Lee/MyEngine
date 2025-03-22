#pragma once
#include"Engine/Core.h"
namespace ENGINE {
	//轮询
	class  Input
	{
	public:
		inline static bool IsKeyPress(int keycode) { return m_instance->IsKeyPressImpl(keycode); }
		inline static bool IsMouseButtonPress(int button) { return m_instance->IsMouseButtonPressImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return m_instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return m_instance->GetMouseXImpl(); }
		inline static float  GetMouseY() { return m_instance->GetMouseYImpl(); }
	protected:
		virtual bool IsKeyPressImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		

	private:
		static Input* m_instance;
	};

}
