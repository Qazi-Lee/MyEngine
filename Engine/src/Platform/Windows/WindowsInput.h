#pragma once
#include"Engine/Input.h"
namespace ENGINE {
	class  WindowsInput :public Input
	{
	protected:
		bool IsKeyPressImpl(int keycode)override;
		bool IsMouseButtonPressImpl(int button)override;
	    std::pair<float, float> GetMousePositionImpl()override;
		float GetMouseXImpl()override;
		float GetMouseYImpl()override;
};

}
