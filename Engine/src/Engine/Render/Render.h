#pragma once
#include<glm/glm.hpp>
#include"VertexArray.h"
namespace ENGINE
{
	class RenderAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
		virtual void Init()const = 0;
		virtual void SetClearColor(const glm::vec4& color)const = 0;
		virtual void SetViewPort(float width, float height)const = 0;
		virtual void Clear()const = 0;
		virtual void DrawElement(const std::shared_ptr<VertexArray>& vertexarray,const int&count=0)const=0;
		virtual void DrawLine(const std::shared_ptr<VertexArray>& vertexarray, const int& count = 0)const = 0;
		virtual void SetLineWidth(float width)const =0;
		static API GetAPI() { return m_API; }
	private:
		static API m_API;
	};

	class RenderCommand
	{
	public:
		inline static void Init() { m_RenderAPI->Init(); }
		inline static void SetClearColor(const glm::vec4& color) { m_RenderAPI->SetClearColor(color); }
		inline static void SetViewPort(float width, float height) { m_RenderAPI->SetViewPort(width,height); }
		inline static void Clear() { m_RenderAPI->Clear(); }
		inline static void DrawElement(const std::shared_ptr<VertexArray>& vertexarray,const int&count=0) { m_RenderAPI->DrawElement(vertexarray,count); }
		inline static void DrawLine(const std::shared_ptr<VertexArray>& vertexarray, const int& count = 0) { m_RenderAPI->DrawLine(vertexarray, count); }
		inline static void SetLineWidth(float width) { m_RenderAPI->SetLineWidth(width); }
	private:
		static RenderAPI* m_RenderAPI;
	};
	class Render
	{
	public:
		static void Init();
		static  void BeginScene();
		static  void EndScene();
		static  void Submit(const std::shared_ptr<VertexArray>&vertecarray);
		static  void OnWindowResize(float width, float height);
	};
}