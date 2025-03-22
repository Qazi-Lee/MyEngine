#pragma once
namespace ENGINE
{
	enum class LayoutElementType
	{
     None=0,
	 Float, Float2, Float3, Float4,
	 Int, Int2, Int3, Int4,
	 Mat3, Mat4,Bool
	};
	static uint32_t GetSize(LayoutElementType type)
	{
		switch (type)
		{
		case ENGINE::LayoutElementType::None:	return 0;
			
		case ENGINE::LayoutElementType::Float:  return 4;
			
		case ENGINE::LayoutElementType::Float2: return 4*2;
			
		case ENGINE::LayoutElementType::Float3: return 4*3;
			
		case ENGINE::LayoutElementType::Float4: return 4*4;
			
		case ENGINE::LayoutElementType::Int:	return 4;
			
		case ENGINE::LayoutElementType::Int2:   return 4*2;
											    
		case ENGINE::LayoutElementType::Int3:   return 4*3;
											    
		case ENGINE::LayoutElementType::Int4:   return 4*4;
											    
		case ENGINE::LayoutElementType::Mat3:   return 4*3*3;
											    
		case ENGINE::LayoutElementType::Mat4:   return 4*4*4;
											    
		case ENGINE::LayoutElementType::Bool:   return 1;
											    
		default:                                return 0;
	}
	}
	class LayoutElement
	{
	public:
		LayoutElement(LayoutElementType type, std::string name,const bool &normal=false)
			:Type(type), Name(name),Size(GetSize(type)),Offset(0),Normalizer(normal)
		{
		}
		uint32_t GetCount()const
		{
			switch (Type)
			{
			case ENGINE::LayoutElementType::None	:return 0;
			
			case ENGINE::LayoutElementType::Float:	return 1;
			
			case ENGINE::LayoutElementType::Float2:	return 2;
			
			case ENGINE::LayoutElementType::Float3:	return 3;
			
			case ENGINE::LayoutElementType::Float4:	return 4;
			
			case ENGINE::LayoutElementType::Int:	return 1;
			
			case ENGINE::LayoutElementType::Int2:	return 2;
			
			case ENGINE::LayoutElementType::Int3:	return 3;
			
			case ENGINE::LayoutElementType::Int4:	return 4;
			
			case ENGINE::LayoutElementType::Mat3:	return 3*3;
			
			case ENGINE::LayoutElementType::Mat4:	return 4*4;
			
			case ENGINE::LayoutElementType::Bool:	return 1;
			
			default:								return 0;
			
			}
		}
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		bool Normalizer;
		LayoutElementType Type;
	};
	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(std::initializer_list<LayoutElement>layout) :
			m_Layout(layout) ,m_Stride(0)
		{
			uint32_t offset=0;
			for (auto& element : m_Layout)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
		inline	const std::vector<LayoutElement>& GetLayout()const { return m_Layout; }
		inline  uint32_t GetStride()const { return m_Stride; }
		inline std::vector<LayoutElement>::iterator begin() { return m_Layout.begin(); }
		inline std::vector<LayoutElement>::iterator end() { return m_Layout.end(); }
		inline std::vector<LayoutElement>::const_iterator begin()const { return m_Layout.begin(); }
		inline std::vector<LayoutElement>::const_iterator end() const{ return m_Layout.end(); }
		inline size_t size() const { return m_Layout.size(); }
		inline LayoutElement& operator[](int i) { return m_Layout[i]; }

	private:
		std::vector<LayoutElement>m_Layout;
		uint32_t m_Stride=0;
	};
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};
		virtual void Bind() const =0;
		virtual void UnBind() const =0;
		virtual const BufferLayout& GetLayout()const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual void SetData(void* data, int size)=0;
		static Ref<VertexBuffer> Creat(float *vertexs,int size);
		static Ref<VertexBuffer> Creat(int size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {};
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual int GetCount()const = 0;
		static IndexBuffer* Creat(int *index,int count);

	};
}
