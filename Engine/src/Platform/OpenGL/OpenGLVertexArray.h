#pragma once
#include"Engine/Render/VertexArray.h"
namespace ENGINE
{
	class OpenGLVertexArray:public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
		 void Bind()const override;
		 void UnBind()const override;
		 const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers()const override { return m_VertexBuffers; }
		 const std::shared_ptr<IndexBuffer>& GetIndexBuffer()const override { return m_IndexBuffer; }
		 void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexbuffer)override;
		 void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexbuffer)override;
	private:
		std::vector<std::shared_ptr<VertexBuffer>>m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t VAO;
	};

}