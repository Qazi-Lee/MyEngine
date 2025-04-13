#include"pch.h"
#include"Texture.h"
#include"Render.h"
#include"Platform/OpenGL/OpenGLTexture.h"
namespace ENGINE
{
	Ref<Texture2D> Texture2D::Create(int width, int height)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: return nullptr;
		case RenderAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(width,height);
		}
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, void* data)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: return nullptr;
		case RenderAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height,data);
		}
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: return nullptr;
		case RenderAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
		}
		return nullptr;
	}

}