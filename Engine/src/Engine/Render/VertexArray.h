#pragma once
#include<memory>
#include"Buffer.h"
namespace ENGINE
{
	class VertexArray
	{
	public:
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers()const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer()const = 0;
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer)=0;
		virtual void AddIndexBuffer(const Ref<IndexBuffer>& indexbuffer) = 0;
		static Ref<VertexArray> Creat();
	};
}