#include"pch.h"
#include"Render2D.h"
#include"Engine/Render/Render.h"
#include"Engine/Render/VertexArray.h"
#include"Engine/Render/Shader.h"
namespace ENGINE
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec2 texturecoord;
		float textureindex;
		glm::vec4 color;

		int EntityID;
	};

	struct CircleVertex
	{
		glm::vec3 worldposition;
		glm::vec3 localposition;
		glm::vec4 color;
		glm::vec2 texturecoord;
		float textureindex;
		//绘制圆环
		float thickness;
		//虚化
		float fade;

		int EntityID;
	};

	struct LineVertex
	{
		glm::vec3 position;
		glm::vec4 color;

		int EntityID;
	};
	struct RenderData
	{
		static const uint32_t MAXSIZE = 1000;
		static const uint32_t MAXVERTEX = MAXSIZE * 4;
		static const uint32_t MAXINDEX = MAXSIZE * 6;
		static const uint32_t MAXTEXTURE = 32;

		Ref<VertexArray>QuadVertexArray;
		Ref<VertexBuffer>QuadVertexBuffer;
		Ref<Shader>QuadShader;
		Ref<Texture2D>WhiteTexture;

		Ref<VertexArray>CircleVertexArray;
		Ref<VertexBuffer>CircleVertexBuffer;
		Ref<Shader>CircleShader;

		Ref<VertexArray>LineVertexArray;
		Ref<VertexBuffer>LineVertexBuffer;
		Ref<Shader>LineShader;

		int QuadIndexCount = 0; 
		QuadVertex* QuadVertexDataBase = nullptr;
		QuadVertex* QuadVertexDataPtr = nullptr;

		int CircleIndexCount = 0;
		CircleVertex* CircleVertexDataBase = nullptr;
		CircleVertex* CircleVertexDataPtr = nullptr;

		int LineIndexCount = 0;
		LineVertex* LineVertexDataBase = nullptr;
		LineVertex* LineVertexDataPtr = nullptr;

		std::array<Ref<Texture2D>, MAXTEXTURE>m_TextureSlots;
		int TextureCount = 0;

		glm::vec4 QuadVertexPosition[4];

		float m_LineWidth=1.0f;
		Render2D::Stats m_stats;
	};
	static RenderData m_Data;

	void Render2D::Init()
	{
		//初始化矩形顶点和着色器
		m_Data.QuadVertexArray = VertexArray::Creat();
		m_Data.CircleVertexArray = VertexArray::Creat();
		m_Data.LineVertexArray = VertexArray::Creat();;
		//纹理
		std::string filepath = "assets/Shader/Render2D_Quad.glsl";
		m_Data.QuadShader = Shader::Create(filepath);
		filepath = "assets/Shader/Render2D_Circle.glsl";
		m_Data.CircleShader = Shader::Create(filepath);
		filepath = "assets/Shader/Render2D_Line.glsl";
		m_Data.LineShader = Shader::Create(filepath);
		//白纹理
		m_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whitedata = 0xffffffff;
		m_Data.WhiteTexture->SetData(&whitedata, sizeof(uint32_t));

		m_Data.m_TextureSlots[0] = m_Data.WhiteTexture;

		//顶点数据
		m_Data.QuadVertexBuffer = VertexBuffer::Creat(m_Data.MAXVERTEX * sizeof(QuadVertex));
		BufferLayout layout =
		{
			{LayoutElementType::Float3,"a_Pos"},
			{LayoutElementType::Float2,"a_Texture"},
			{LayoutElementType::Float,"a_TextureIndex"},
			{LayoutElementType::Float4,"a_Color"},
			{LayoutElementType::Int,"a_EntityID"}
		};
		m_Data.QuadVertexBuffer->SetLayout(layout);
		m_Data.QuadVertexArray->AddVertexBuffer(m_Data.QuadVertexBuffer);


		m_Data.QuadVertexDataBase = new QuadVertex[m_Data.MAXVERTEX];

		//初始化顶点位置(顺时针)
		m_Data.QuadVertexPosition[0] = { -0.5f,-0.5f,0.0f,1.0f };
		m_Data.QuadVertexPosition[1] = { 0.5f,-0.5f,0.0f,1.0f };
		m_Data.QuadVertexPosition[2] = { 0.5f,0.5f,0.0f,1.0f };
		m_Data.QuadVertexPosition[3] = { -0.5f,0.5f,0.0f,1.0f };

		//索引数据
		int * indices = new int[m_Data.MAXINDEX];

		int offset = 0;
		for (int i = 0; i < m_Data.MAXINDEX; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 0;
			indices[i + 4] = offset + 2;
			indices[i + 5] = offset + 3;

			offset += 4;
		}
		Ref<IndexBuffer>quadIB;
		quadIB.reset(IndexBuffer::Creat(indices, m_Data.MAXINDEX));
		m_Data.QuadVertexArray->AddIndexBuffer(quadIB);
		delete[]indices;

		//圆形，用矩形通过渐变变为圆形
		m_Data.CircleVertexBuffer = VertexBuffer::Creat(m_Data.MAXVERTEX * sizeof(CircleVertex));
		m_Data.CircleVertexBuffer->SetLayout({
			{ LayoutElementType::Float3, "a_WorldPosition" },
			{ LayoutElementType::Float3, "a_LocalPosition" },
			{ LayoutElementType::Float4, "a_Color"         },
			{LayoutElementType::Float2,  "a_Texture"       },
			{LayoutElementType::Float,   "a_TextureIndex"   },
			{ LayoutElementType::Float,  "a_Thickness"     },
			{ LayoutElementType::Float,  "a_Fade"          },
			{ LayoutElementType::Int,    "a_EntityID"      }
			});
		m_Data.CircleVertexArray->AddVertexBuffer(m_Data.CircleVertexBuffer);
		m_Data.CircleVertexArray->AddIndexBuffer(quadIB); // Use quad IB
		m_Data.CircleVertexDataBase = new CircleVertex[m_Data.MAXVERTEX];
		
		//线条
		m_Data.LineVertexBuffer = VertexBuffer::Creat(m_Data.MAXVERTEX * sizeof(LineVertex));
		m_Data.LineVertexBuffer->SetLayout({
			{ LayoutElementType::Float3, "a_Position" },
			{ LayoutElementType::Float4, "a_Color"    },
			{ LayoutElementType::Int,    "a_EntityID" }
			});
		m_Data.LineVertexArray->AddVertexBuffer(m_Data.LineVertexBuffer);
		m_Data.LineVertexDataBase = new LineVertex[m_Data.MAXVERTEX];

		int32_t sample[RenderData::MAXTEXTURE];
		for (int i = 0; i < RenderData::MAXTEXTURE; i++)
		{
			sample[i] = i;
		}
		m_Data.QuadShader->Bind();
		m_Data.QuadShader->SetIntArray("u_Texture", sample, RenderData::MAXTEXTURE);
		m_Data.CircleShader->Bind();
		m_Data.CircleShader->SetIntArray("u_Texture", sample, RenderData::MAXTEXTURE);
	}

	void Render2D::Shutdown()
	{
		delete[]m_Data.QuadVertexDataBase;
		delete[]m_Data.CircleVertexDataBase;
	}

	void Render2D::BeginScene(const GraphicsCamera2D& camera)
	{
		m_Data.QuadShader->Bind();
		m_Data.QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		StartBatch();
	}

	void Render2D::BeginScene(const Camera& camera,const glm::mat4& transform)
	{
		glm::mat4 project = camera.m_Projection * glm::inverse(transform);
		m_Data.QuadShader->Bind();
		m_Data.QuadShader->SetMat4("u_ViewProjection", project);
		m_Data.CircleShader->Bind();
		m_Data.CircleShader->SetMat4("u_ViewProjection", project);
		m_Data.LineShader->Bind();
		m_Data.LineShader->SetMat4("u_ViewProjection", project);
		StartBatch();
	}

	void Render2D::BeginScene(const EditorCamera& camera)
	{
		glm::mat4 project = camera.GetViewProjection();
		m_Data.QuadShader->Bind();
		m_Data.QuadShader->SetMat4("u_ViewProjection", project);
		m_Data.CircleShader->Bind();
		m_Data.CircleShader->SetMat4("u_ViewProjection", project);
		m_Data.LineShader->Bind();
		m_Data.LineShader->SetMat4("u_ViewProjection", project);
		StartBatch();
	}

	void Render2D::EndScene()
	{
		Flush();
	}

	void Render2D::Flush()
	{
		//纹理绑定对应槽
		for (int i = 0; i < m_Data.TextureCount; i++)
		{
			m_Data.m_TextureSlots[i]->Bind(i);
		}
		//矩形
		if (m_Data.QuadIndexCount)
		{
			uint32_t datasize = (uint32_t)((uint8_t*)m_Data.QuadVertexDataPtr - (uint8_t*)m_Data.QuadVertexDataBase);
			m_Data.QuadVertexBuffer->SetData((void*)m_Data.QuadVertexDataBase, datasize);
			m_Data.QuadShader->Bind();
			RenderCommand::DrawElement(m_Data.QuadVertexArray, m_Data.QuadIndexCount);
			m_Data.m_stats.DrawCalls++;
		}
		//圆形
		if (m_Data.CircleIndexCount)
		{
			uint32_t datasize = (uint32_t)((uint8_t*)m_Data.CircleVertexDataPtr - (uint8_t*)m_Data.CircleVertexDataBase);
			m_Data.CircleVertexBuffer->SetData((void*)m_Data.CircleVertexDataBase, datasize);
			m_Data.CircleShader->Bind();
			RenderCommand::DrawElement(m_Data.CircleVertexArray, m_Data.CircleIndexCount);
			m_Data.m_stats.DrawCalls++;
		}
		if (m_Data.LineIndexCount)
		{
			uint32_t datasize = (uint32_t)((uint8_t*)m_Data.LineVertexDataPtr - (uint8_t*)m_Data.LineVertexDataBase);
			m_Data.LineVertexBuffer->SetData((void*)m_Data.LineVertexDataBase, datasize);
			m_Data.LineShader->Bind();
			RenderCommand::SetLineWidth(m_Data.m_LineWidth);
			RenderCommand::DrawLine(m_Data.LineVertexArray, m_Data.LineIndexCount);
			m_Data.m_stats.DrawCalls++;

		}
	}


	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const float& rotation)
	{
		//变换
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		DrawQuad(transform, color);
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const float& rotation)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color, rotation);
	}

	void Render2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color,const int& entityID)
	{
		//不传入纹理则使用白纹理
		float textureindex = 0.0f;

		if (m_Data.QuadIndexCount >= RenderData::MAXINDEX)
		{
			NextBatch();
		}
		constexpr glm::vec2 TextureCoord[4] = {
			{ 0.0f,0.0f },
			{ 1.0f,0.0f },
			{ 1.0f,1.0f },
			{ 0.0f,1.0f }
		};
		for (int i = 0; i < 4; i++)
		{
			m_Data.QuadVertexDataPtr->position = transform * m_Data.QuadVertexPosition[i];
			m_Data.QuadVertexDataPtr->texturecoord = TextureCoord[i];
			m_Data.QuadVertexDataPtr->textureindex = textureindex;
			m_Data.QuadVertexDataPtr->color = color;
			m_Data.QuadVertexDataPtr->EntityID = entityID;
			m_Data.QuadVertexDataPtr++;
		}


		m_Data.QuadIndexCount += 6;
		m_Data.m_stats.QuadCount++;

	}

	void Render2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, const int& entityID)
	{
		float textureindex = 0.0f;

		if (m_Data.QuadIndexCount >= RenderData::MAXINDEX)
		{
			NextBatch();
		}
		//先判断是否存储对应纹理
		for (int i = 0; i < m_Data.TextureCount; i++)
		{
			if (*m_Data.m_TextureSlots[i].get() == *texture.get())
			{
				textureindex = (float)i;
				break;
			}
		}
		//不存在则添加到纹理数组(可能超出范围)
		if (textureindex == 0.0f)
		{
			//超限绘制下一个
			if (m_Data.TextureCount >= RenderData::MAXTEXTURE)
			{
				NextBatch();
			}
			m_Data.m_TextureSlots[m_Data.TextureCount] = texture;
			textureindex = (float)m_Data.TextureCount;
			m_Data.TextureCount++;
		}


		constexpr glm::vec2 TextureCoord[4] = {
			{ 0.0f,0.0f },
			{ 1.0f,0.0f },
			{ 1.0f,1.0f },
			{ 0.0f,1.0f }
		};

		for (int i = 0; i < 4; i++)
		{
			m_Data.QuadVertexDataPtr->position = transform * m_Data.QuadVertexPosition[i];
			m_Data.QuadVertexDataPtr->texturecoord = TextureCoord[i];
			m_Data.QuadVertexDataPtr->textureindex = textureindex;
			m_Data.QuadVertexDataPtr->color = color;
			m_Data.QuadVertexDataPtr->EntityID = entityID;
			m_Data.QuadVertexDataPtr++;
		}
		m_Data.QuadIndexCount += 6;
		m_Data.m_stats.QuadCount++;

	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, const float& rotation)
	{
		//变换
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		DrawQuad(transform, color, texture);
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, const float& rotation)
	{																				
		DrawQuad({ position.x,position.y,0.0f }, size, texture,color, rotation);
	}																				
							
	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& texture, const glm::vec4& color, const float& rotation)
	{
		float textureindex = 0.0f;

		if (m_Data.QuadIndexCount >= RenderData::MAXINDEX)
		{
			NextBatch();
		}
		//先判断是否存储对应纹理
		for (int i = 0; i < m_Data.TextureCount; i++)
		{

			if (*m_Data.m_TextureSlots[i].get() == *(texture->GetTexture().get()))
			{
				textureindex = (float)i;
				break;
			}
		}
		//不存在则添加到纹理数组(可能超出范围)
		if (textureindex == 0.0f)
		{
			//超限绘制下一个
			if (m_Data.TextureCount >= RenderData::MAXTEXTURE)
			{
				NextBatch();
			}
			m_Data.m_TextureSlots[m_Data.TextureCount] = texture->GetTexture();
			textureindex = (float)m_Data.TextureCount;
			m_Data.TextureCount++;
		}

		//变换
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });


		const glm::vec2* TextureCoord=texture->GetTextureCoord();

		for (int i = 0; i < 4; i++)
		{
			m_Data.QuadVertexDataPtr->position = transform * m_Data.QuadVertexPosition[i];
			m_Data.QuadVertexDataPtr->texturecoord = TextureCoord[i];
			m_Data.QuadVertexDataPtr->textureindex = textureindex;
			m_Data.QuadVertexDataPtr->color = color;
			m_Data.QuadVertexDataPtr++;
		}

		m_Data.QuadIndexCount += 6;
		m_Data.m_stats.QuadCount++;
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& texture, const glm::vec4& color, const float& rotation)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, texture, color, rotation);
	}

	void Render2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		float textureindex = 0.0f;

		if (m_Data.QuadIndexCount >= RenderData::MAXINDEX)
		{
			NextBatch();
		}
		constexpr glm::vec2 TextureCoord[4] = {
			{ 0.0f,0.0f },
			{ 1.0f,0.0f },
			{ 1.0f,1.0f },
			{ 0.0f,1.0f }
		};
		for (int i = 0; i < 4; i++)
		{
			m_Data.CircleVertexDataPtr->worldposition = transform *m_Data.QuadVertexPosition[i];
			m_Data.CircleVertexDataPtr->localposition = m_Data.QuadVertexPosition[i] * 2.0f;
			m_Data.CircleVertexDataPtr->color = color;
			m_Data.CircleVertexDataPtr->texturecoord = TextureCoord[i];
			m_Data.CircleVertexDataPtr->textureindex = textureindex;
			m_Data.CircleVertexDataPtr->thickness = thickness;
			m_Data.CircleVertexDataPtr->fade = fade;
			m_Data.CircleVertexDataPtr->EntityID = entityID;
			m_Data.CircleVertexDataPtr++;
		}

		m_Data.CircleIndexCount += 6;
		m_Data.m_stats.QuadCount++;
	}

	void Render2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, float thickness, float fade, int entityID)
	{
		float textureindex = 0.0f;

		if (m_Data.QuadIndexCount >= RenderData::MAXINDEX)
		{
			NextBatch();
		}
		//先判断是否存储对应纹理
		for (int i = 0; i < m_Data.TextureCount; i++)
		{
			if (*m_Data.m_TextureSlots[i].get() == *texture.get())
			{
				textureindex = (float)i;
				break;
			}
		}
		//不存在则添加到纹理数组(可能超出范围)
		if (textureindex == 0.0f)
		{
			//超限绘制下一个
			if (m_Data.TextureCount >= RenderData::MAXTEXTURE)
			{
				NextBatch();
			}
			m_Data.m_TextureSlots[m_Data.TextureCount] = texture;
			textureindex = (float)m_Data.TextureCount;
			m_Data.TextureCount++;
		}
		constexpr glm::vec2 TextureCoord[4] = {
			{ 0.0f,0.0f },
			{ 1.0f,0.0f },
			{ 1.0f,1.0f },
			{ 0.0f,1.0f }
		};
		for (int i = 0; i < 4; i++)
		{
			m_Data.CircleVertexDataPtr->worldposition = transform * m_Data.QuadVertexPosition[i];
			m_Data.CircleVertexDataPtr->localposition = m_Data.QuadVertexPosition[i] * 2.0f;
			m_Data.CircleVertexDataPtr->color = color;
			m_Data.CircleVertexDataPtr->texturecoord = TextureCoord[i];
			m_Data.CircleVertexDataPtr->textureindex = textureindex;
			m_Data.CircleVertexDataPtr->thickness = thickness;
			m_Data.CircleVertexDataPtr->fade = fade;
			m_Data.CircleVertexDataPtr->EntityID = entityID;
			m_Data.CircleVertexDataPtr++;
		}

		m_Data.CircleIndexCount += 6;
		m_Data.m_stats.QuadCount++;
	}

	void Render2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		m_Data.LineVertexDataPtr->position = p0;
		m_Data.LineVertexDataPtr->color = color;
		m_Data.LineVertexDataPtr->EntityID = entityID;
		m_Data.LineVertexDataPtr++;

		m_Data.LineVertexDataPtr->position = p1;
		m_Data.LineVertexDataPtr->color = color;
		m_Data.LineVertexDataPtr->EntityID = entityID;
		m_Data.LineVertexDataPtr++;

		m_Data.LineIndexCount += 2;
	}
	void Render2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void Render2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * m_Data.QuadVertexPosition[i];

		DrawLine(lineVertices[0], lineVertices[1], color, entityID);
		DrawLine(lineVertices[1], lineVertices[2], color, entityID);
		DrawLine(lineVertices[2], lineVertices[3], color, entityID);
		DrawLine(lineVertices[3], lineVertices[0], color, entityID);
	}

	void Render2D::SetLineWidth(float width)
	{
		m_Data.m_LineWidth = width;
	}

	float Render2D::GetLineWidth()
	{
		return m_Data.m_LineWidth;
	}

	void Render2D::ResetStats()
	{
		memset(&m_Data.m_stats, 0, sizeof(Stats));
	}
	
	Render2D::Stats Render2D::GetStats()
	{
		return m_Data.m_stats;
	}

	void Render2D::StartBatch()
	{
		m_Data.QuadVertexDataPtr  = m_Data.QuadVertexDataBase;
		m_Data.QuadIndexCount = 0;

		m_Data.CircleVertexDataPtr = m_Data.CircleVertexDataBase;
		m_Data.CircleIndexCount = 0;

		m_Data.LineVertexDataPtr = m_Data.LineVertexDataBase;
		m_Data.LineIndexCount = 0;

		m_Data.TextureCount   = 1;
	}

	void Render2D::NextBatch()
	{
		Flush();
		StartBatch();
	}
}

