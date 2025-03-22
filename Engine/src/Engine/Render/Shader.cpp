#include"pch.h"
#include "Shader.h"
#include"Render.h"
#include"Platform/OpenGL/OpenGLShader.h"
namespace ENGINE
{

	Ref<Shader> Shader::Create(const std::string& VertexSource, const std::string& FragementSource)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: return nullptr;
		case RenderAPI::API::OpenGL: return std::make_shared< OpenGLShader>(VertexSource,FragementSource);
		}
		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string&filepath)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: return nullptr;
		case RenderAPI::API::OpenGL: return  std::make_shared <OpenGLShader>(filepath);
		}
		return nullptr;
	}
}
