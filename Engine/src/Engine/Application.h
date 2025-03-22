#pragma once
#include"Core.h"
#include"Window.h"
#include"Event/WindowEvent.h"
#include"LayerStack.h"
#include"ImGui/ImGuiLayer.h"
namespace ENGINE {

	 class  Application
	{
	public:
		Application() ;
		virtual ~Application() ;
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverLay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverLay(Layer* overlay);
		static Application& GetApp(){ return *m_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		inline void Close() { run = false; }
	private:
		//关闭窗口函数，返回值为bool，将返回值传递给事件的m_Handle，如果返回值为true，则说明该事件已经处理完毕，无需传递给其他层
		bool OnWindowClosed( WindowCloseEvent&e);
		//改变窗口尺寸函数，通过渲染器改变视口
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window>m_Window;
		ImGuiLayer*m_ImGuiLayer;
		ENGINE::LayerStack m_LayerStack;
		bool run=true;
		static Application* m_Instance;
		float LastFrameTime = 0;
	};
	 Application* CreatApp();
}

