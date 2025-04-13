#include"pch.h"
#include"OpenGLRenderAPI.h"
#include<glad/glad.h>
namespace ENGINE
{
	void OpenGLRenderAPI::Init() const
	{
		//启用混合
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);

	}
	void OpenGLRenderAPI::SetClearColor(const glm::vec4& color) const
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderAPI::SetViewPort(float width, float height) const
	{
		glViewport(0, 0, width, height);
	}

	void OpenGLRenderAPI::Clear() const
	{
		//深度缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderAPI::DrawElement(const Ref<VertexArray>& vertexarray,const int&count) const
	{
		vertexarray->Bind();
		vertexarray->GetIndexBuffer()->Bind();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		vertexarray->UnBind();
	}
	void OpenGLRenderAPI::DrawLine(const Ref<VertexArray>& vertexarray, const int& count) const
	{
		vertexarray->Bind();
		glDrawArrays(GL_LINES, 0, count);
		vertexarray->UnBind();
	}
	void OpenGLRenderAPI::SetLineWidth(float width) const
	{
		glLineWidth(width);
	}
	void OpenGLRenderAPI::EnableDepth() const
	{
		glEnable(GL_DEPTH_TEST);
	}
	void OpenGLRenderAPI::DisableDepth() const
	{
		glDisable(GL_DEPTH_TEST);
	}
}