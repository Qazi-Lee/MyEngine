#include"pch.h"
#include "Application.h"
#include"Render/Render.h"
#include"mono/jit/jit.h"
#include"mono/metadata/assembly.h"
#include"mono/metadata/threads.h"
#include"mono/metadata/mono-gc.h"
//定义一个绑定回调函数的宏

namespace ENGINE {
    Application* Application::m_Instance = nullptr;

	Application::Application()
	{
		//初始化mono
	//	mono_set_dirs("C:/Program Files/Mono/lib", "C:/Program Files/Mono/etc");
		std::string rootpath = std::string();
#ifdef MY_FILE_PATH
		rootpath=  MY_FILE_PATH;
#else
		rootpath = "..";
#endif // MY_FILE_PATH
		std::string libpath = rootpath + "/Mono/lib";
		std::string etcpath = rootpath + "/Mono/etc";
		mono_set_dirs(libpath.c_str(),etcpath.c_str());
		m_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Creat());
		m_Window->SetEventCallbackFn(BIND_EVENT_FN(Application::OnEvent));
		Render::Init();
		m_ImGuiLayer = new ImGuiLayer();
		PushLayer(m_ImGuiLayer);
	};
	Application::~Application()
	{
		//存在Bug
		//if (mono_get_root_domain())
		//{
		//	mono_thread_exit();
		//	mono_jit_cleanup(mono_get_root_domain());
		//}
	}
	void ENGINE::Application::Run()
	{
	
		while (run)
		{
			float now = ENGINE::Time::GetTime();
			Time t = Time(now - LastFrameTime);
			LastFrameTime = now;
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(t);
			}
			m_Window->OnUpdate();
		};
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		//使用bind函数来延迟传参数,在调用函数时再传入参数
		//dispatcher.Dispatch<WindowCloseEvent>(std::bind(& Application::OnWindowClosed,this,std::placeholders::_1));用宏函数替换
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResized));
		//LOG_CORE_INFO(e.ToString());
		//在层栈中对事件进行处理
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			//从后往前处理层栈中同类型的层事件
			(*--it)->OnEvent(e);
			//一旦m_Handle为true，则说明事件已经完成，无需再向其他层传递
			if (e.m_Handle)
			{
				break;
			}
		}
	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverLay(Layer* overlay)
	{
		m_LayerStack.PushOverLay(overlay);
	}
	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
	}
	void Application::PopOverLay(Layer* overlay)
	{
		m_LayerStack.PopOverLay(overlay);
	}

	bool Application::OnWindowClosed( WindowCloseEvent& e)
	{
		run = false;
		return true;
	}
	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		Render::OnWindowResize((float)e.GetWidth(),(float)e.GetHeight());
		return false;
	}
}
