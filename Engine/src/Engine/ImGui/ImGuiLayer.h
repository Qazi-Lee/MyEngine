#pragma once

#include"Engine/Layer.h"

#include"Engine/Event/MouseEvent.h"
#include"Engine/Event/KeyEvent.h"


namespace ENGINE {
	class  ImGuiLayer :public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach()override;
		void OnDetach()override;
		void OnImGuiRender()override;
		void Begin();
		void End();
	};


}