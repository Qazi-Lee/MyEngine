#pragma once
#include"Engine/Render/Buffer.h"

namespace ENGINE
{

	class OpenGLVertexBuffer:public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(int size);
		OpenGLVertexBuffer(float*vertexs,int size);
		~OpenGLVertexBuffer();
		void Bind()const override;
		void UnBind()const override;
		void SetData(void* data, int size)override;
		const BufferLayout& GetLayout()const override { return m_Layout; }
		void SetLayout(const BufferLayout& layout)override { m_Layout = layout; }
	private:
		uint32_t VBO;
		BufferLayout m_Layout;
	};
	
	class OpenGLIndexBuffer :public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(int *index,int count);
		~OpenGLIndexBuffer();
		void Bind()const override;
		void UnBind()const override;
		int GetCount()const override { return m_count; }
	private:
		uint32_t EBO;
		int m_count;
	};
}