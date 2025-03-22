#pragma once
#include"Engine/Render/Texture.h"
#include<glad/glad.h>

namespace ENGINE
{
	class OpenGLTexture2D :public Texture2D 
	{
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(int width, int height);
		~OpenGLTexture2D();
		void SetData(void* data, uint32_t size)const override;
		uint32_t GetWidth()const override { return m_Width; }
		uint32_t GetHeight()const override { return m_Height; }
		uint32_t GetSlot()const override { return m_Slot; }
		uint32_t GetID()const override { return m_ID; }
		std::string GetPath()const override { return m_Path; }
		void Bind(uint32_t slot =1) override;
		bool operator==(const Texture2D& other)const override;
	private:
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_ID;
		uint32_t m_Slot;
		GLenum Internalformat, Dataformat;
	};

}