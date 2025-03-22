#pragma once
#include"Engine/Render/FrameBuffer.h"

namespace ENGINE
{
	class OpenGLFrameBuffer :public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferData& data);
		~OpenGLFrameBuffer();

		void Bind()const override;
		void UnBind()const override;
		const FrameBufferData& GetData()const override { return m_Data; }
		uint32_t GetColorAttachmentID()const override { return m_ColorAttachment; }
		int ReadPixel(uint32_t attachindex, int x, int y)const override;
		void ClearAttachment(uint32_t attachindex, int value)const override;
		void Resize(uint32_t width, uint32_t height)override;
	private:
		void CreateBuffer();
	private:
		FrameBufferData m_Data;
		uint32_t m_ID=0;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		uint32_t m_EntityIdAttachment;
	};

}