#include"pch.h"
#include"OpenGLVertexArray.h"
#include<glad/glad.h>
namespace ENGINE
{
	static GLenum BufferToOpenGL(LayoutElementType type)
	{
		switch (type)
		{
		case LayoutElementType::None:  return GL_NONE;
		case LayoutElementType::Float: return GL_FLOAT;
		case LayoutElementType::Float2:return GL_FLOAT;
		case LayoutElementType::Float3:return GL_FLOAT;
		case LayoutElementType::Float4:return GL_FLOAT;
		case LayoutElementType::Int:   return GL_INT;
		case LayoutElementType::Int2:  return GL_INT;
		case LayoutElementType::Int3:  return GL_INT;
		case LayoutElementType::Int4:  return GL_INT;
		case LayoutElementType::Mat3:  return GL_MATRIX_STRIDE;
		case LayoutElementType::Mat4:  return GL_MATRIX_STRIDE;
		case LayoutElementType::Bool:  return GL_BOOL;
		default:					   return GL_NONE;
		}

	}
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &VAO);

	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1,&VAO);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(VAO);
	}
	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexbuffer)
	{
		this->Bind();
		if (vertexbuffer->GetLayout().size())
		{
			int index = 0;
			for (auto& element : vertexbuffer->GetLayout())
			{
				switch (element.Type)
				{
				case LayoutElementType::None:
				case LayoutElementType::Float:
				case LayoutElementType::Float2:
				case LayoutElementType::Float3:
				case LayoutElementType::Float4:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index, element.GetCount(),
						BufferToOpenGL(element.Type),
						element.Normalizer ? GL_TRUE : GL_FALSE,
						vertexbuffer->GetLayout().GetStride(),
						(void*)element.Offset);
					index++;
					break;
				}
				case LayoutElementType::Int:
				case LayoutElementType::Int2:
				case LayoutElementType::Int3:
				case LayoutElementType::Int4:
				case LayoutElementType::Bool:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribIPointer(index, element.GetCount(),
						BufferToOpenGL(element.Type),
						vertexbuffer->GetLayout().GetStride(),
						(void*)element.Offset);
					index++;
					break;
				}
				case LayoutElementType::Mat3:
				case LayoutElementType::Mat4:
				{
					uint8_t count = element.GetCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(index);
						glVertexAttribIPointer(index,count,
							BufferToOpenGL(element.Type),
							vertexbuffer->GetLayout().GetStride(),
							(const void*)(element.Offset+sizeof(float)*count*i));
						glVertexAttribDivisor(index, 1);
						index++;
					}
					break;
				}
				}
			}
			m_VertexBuffers.push_back(vertexbuffer);
		}
		else
		{
			std::cout << "未设置布局，添加失败！" << std::endl;
		}
		this->UnBind();
	}
	void OpenGLVertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexbuffer)
	{
		this->Bind();
		indexbuffer->Bind();
		m_IndexBuffer = indexbuffer;
		indexbuffer->UnBind();
		this->UnBind();
	}
}