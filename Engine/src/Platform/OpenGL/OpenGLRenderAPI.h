#pragma once
#include"Engine/Render/Render.h"

namespace ENGINE
{
	class OpenGLRenderAPI :public RenderAPI
	{
	public:
		 void Init()const override;
		 void SetClearColor(const glm::vec4& color)const override;
		 void SetViewPort(float width, float height)const override;
		 void Clear()const override;
		 void DrawElement(const Ref<VertexArray>& vertexarray,const int& count=0)const override;
		 void DrawLine(const Ref<VertexArray>& vertexarray, const int& count = 0)const override;
		 void SetLineWidth(float width)const override;
	};
}