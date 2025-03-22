#include"pch.h"
#include"Render.h"
#include"Platform/OpenGL/OpenGLRenderAPI.h"
#include"Render2D.h"
namespace ENGINE
{
	static RenderAPI* CreatRenderAPI()
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None:   return nullptr;
		case RenderAPI::API::OpenGL: return new OpenGLRenderAPI();
		default:                     return nullptr;
		};
	}
	RenderAPI::API RenderAPI::m_API = RenderAPI::API::OpenGL;
	RenderAPI* RenderCommand::m_RenderAPI = CreatRenderAPI();

	void Render::Init()
	{
		RenderCommand::Init();
		Render2D::Init();
	}

	void Render::BeginScene()
	{

	}

	void Render::EndScene()
	{
	}

	void Render::Submit(const std::shared_ptr<VertexArray>& vertecarray)
	{
		vertecarray->Bind();
		RenderCommand::DrawElement(vertecarray);
	}

	void Render::OnWindowResize(float width, float height)
	{
		RenderCommand::SetViewPort(width,height);
	}

}