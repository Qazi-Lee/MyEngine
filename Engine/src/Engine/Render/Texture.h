#pragma once
#include"Engine/Core.h"
#include<string>
namespace ENGINE
{
	class Texture
	{
	public:
		virtual uint32_t GetWidth()const =0;
		virtual uint32_t GetHeight()const = 0; 
		virtual uint32_t GetSlot()const = 0;
		virtual void SetData(void* data, uint32_t size)const = 0;
		virtual void Bind(uint32_t slot = 1) = 0;
		virtual ~Texture() = default;
	};
	class Texture2D :public Texture
	{
	public:
		virtual ~Texture2D() = default;
		static Ref<Texture2D> Create(int width, int height);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height,void* data);
		static Ref<Texture2D> Create(const std::string&path);
		virtual std::string GetPath()const=0;
		virtual uint32_t GetID()const = 0;
		virtual bool operator==(const Texture2D& other)const = 0;
	};
}