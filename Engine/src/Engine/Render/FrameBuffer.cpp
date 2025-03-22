#include"pch.h"
#include"Render.h"
#include"Engine/Render/FrameBuffer.h"
#include"Platform/OpenGL/OpenGLFrameBuffer.h"
namespace ENGINE
{

	Ref<FrameBuffer> ENGINE::FrameBuffer::Create(const FrameBufferData& data)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: return nullptr;
		case RenderAPI::API::OpenGL: return std::make_shared<OpenGLFrameBuffer>(data);
		}
		return nullptr;
	}
}