#include"pch.h"
#include"OpenGLFrameBuffer.h"

#include"glad/glad.h"
namespace ENGINE
{

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferData& data)
	{
		m_Data = data;
		CreateBuffer();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glViewport(0, 0, m_Data.width, m_Data.height);
	}

	void OpenGLFrameBuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int OpenGLFrameBuffer::ReadPixel(uint32_t attachindex, int x, int y) const
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0+ attachindex);
		GLint data;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &data);
		return data;
	}

	void OpenGLFrameBuffer::ClearAttachment(uint32_t attachindex, int value) const
	{

		GLenum format=attachindex==0?GL_RGBA8:GL_RED_INTEGER;
		GLenum type = attachindex == 0 ? GL_UNSIGNED_BYTE : GL_INT;
		glClearTexImage(GL_COLOR_ATTACHMENT0 + attachindex, 0, format, type, &value);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Data.width = width;
		m_Data.height = height;
		CreateBuffer();
	}

	void OpenGLFrameBuffer::CreateBuffer()
	{
		//判断是否存在
		if (m_ID)
		{
			glDeleteFramebuffers(1, &m_ID);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
		}
		glCreateFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glCreateTextures(GL_TEXTURE_2D,1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_Data.width, m_Data.height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_ColorAttachment, 0);

		//获得实体ID
		glCreateTextures(GL_TEXTURE_2D, 1, &m_EntityIdAttachment);
		glBindTexture(GL_TEXTURE_2D, m_EntityIdAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32I, m_Data.width, m_Data.height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_EntityIdAttachment, 0);

		//指定绘制数量
		GLenum buffer[2] = { GL_COLOR_ATTACHMENT0 ,GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2,buffer);


		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Data.width, m_Data.height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		if (!glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			LOG_CORE_TRACE("帧缓冲加载失败！");
			__debugbreak();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}