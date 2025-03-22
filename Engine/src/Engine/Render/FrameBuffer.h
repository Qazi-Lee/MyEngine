#pragma once

#include"Engine/Core.h"
namespace ENGINE
{
	struct FrameBufferData
	{
		//缓冲区大小
		uint32_t  width;
		uint32_t  height;
		//样本数
		uint32_t  sample = 1;
	};
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
		virtual const FrameBufferData& GetData()const = 0;
		virtual uint32_t GetColorAttachmentID()const = 0;
		virtual int ReadPixel(uint32_t attachmentindex, int x, int y)const =0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void ClearAttachment(uint32_t attachmentindex, int value)const =0;
		static Ref<FrameBuffer> Create(const FrameBufferData& data);
	};
}