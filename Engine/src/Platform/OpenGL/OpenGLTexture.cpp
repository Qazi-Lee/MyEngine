#include"pch.h"
#include"OpenGLTexture.h"

#include"stb_image.h"



namespace ENGINE
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path),m_Slot(0)
	{
		int width, height,channel;
		//读数据时颠倒
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channel,0);
		m_Width = width;
		m_Height = height;
		if (data == nullptr)
		{
			LOG_CORE_ERROR("获取数据失败!");
			stbi_image_free(data);
			return;
		}
		//三通道用RGB,四通道用RGBA
		GLenum m_internalformat, m_format;
		switch (channel)
		{
		case 1: {m_internalformat = GL_R8;    m_format = GL_RED; break; }
		case 3: {m_internalformat = GL_RGB8;  m_format = GL_RGB; break; }
		case 4: {m_internalformat = GL_RGBA8; m_format = GL_RGBA;break; }
		default:{m_internalformat = GL_RGB8;  m_format = GL_RGB; break; }
		}
		Internalformat = m_internalformat; Dataformat = m_format;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, m_internalformat, m_Width, m_Height);

		//缩放过滤
		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//环绕过滤
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);


		//分辨率4的倍数
		if (width % 4 == 0 || height % 4 == 0)
		{
			glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			//GL_UNPACK_ALIGNMENT内存读到opengl
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_format, GL_UNSIGNED_BYTE, data);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(int width, int height)
		:m_Width(width),m_Height(height),m_Slot(0)
	{
		Internalformat = GL_RGBA8; Dataformat = GL_RGBA;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTextureStorage2D(m_ID, 1, Internalformat, m_Width, m_Height);

		//缩放过滤
		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//环绕过滤
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, void* data)
		:m_Width(width), m_Height(height), m_Slot(0)
	{
		Internalformat = GL_RED; Dataformat = GL_RED;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,  // 单通道纹理（只保存灰度值）
			width,
			height,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			data
		);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) const
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, Dataformat, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) 
	{
		m_Slot = slot;
		glBindTextureUnit(slot, m_ID);
	}

	bool OpenGLTexture2D::operator==(const Texture2D& other) const
	{
		return m_ID==((OpenGLTexture2D&)other).m_ID;
	}

}