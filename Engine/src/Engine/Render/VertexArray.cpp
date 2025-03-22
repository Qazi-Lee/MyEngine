#include"pch.h"
#include"VertexArray.h"
#include"Platform/OpenGL/OpenGLVertexArray.h"
#include"Render.h"
namespace ENGINE
{
	Ref<VertexArray> VertexArray::Creat()
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: return nullptr;
		case RenderAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
		}
		return nullptr;


	}
}