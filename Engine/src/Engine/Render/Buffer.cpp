#include"pch.h"
#include"Buffer.h"
#include"Render.h"
#include"Platform/OpenGL/OpenGLBuffer.h"
namespace ENGINE
{
	Ref<VertexBuffer> VertexBuffer::Creat(float* vertexs, int size) {

		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: return nullptr;
		case RenderAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertexs,size);
		}
		return nullptr;
	}

	Ref<VertexBuffer>VertexBuffer::Creat(int size)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: return nullptr;
		case RenderAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(size);
		}
		return nullptr;
	}


	IndexBuffer* IndexBuffer::Creat(int* index, int count)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: return nullptr;
		case RenderAPI::API::OpenGL: return new OpenGLIndexBuffer(index,count);
		}
		return nullptr;
	}

}