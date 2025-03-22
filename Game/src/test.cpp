//#include<Engine.h>
/************程序入口*************/
#include"Engine/EntryPoint.h"
#include"imgui.h"
#include"Platform/OpenGL/OpenGLShader.h"
#include<glm/gtc/type_ptr.hpp>
#include"head/Layer2D.h"

//class ExampleLayer :public ENGINE::Layer
//{
//public:
//	ExampleLayer() : Layer("Example")
//	,m_CameraController(1280/720)
//	{
//		//渲染
//		m_VertexArray.reset(ENGINE::VertexArray::Creat());
//		//m_Shader.reset( ENGINE::Shader::Create(VertexSource, FrageSource));
//		std::string filepath = "assets/Shader/Texture.glsl";
//		m_Shader.reset(ENGINE::Shader::Create(filepath));
//		m_Shader->Bind();
//		float vertexs[4 * 5] =
//		{
//			-0.5f,-0.5f,0.0f,  0.0f,0.0f, 
//			-0.5f,0.5f,0.0f,   0.0f,1.0f,
//			0.5f,-0.5f,0.0f,   1.0f,0.0f,
//			0.5f,0.5f,0.0f,    1.0f,1.0f,
//		};
//
//		ENGINE::Ref<ENGINE::VertexBuffer> m_VertexBuffer;
//		m_VertexBuffer.reset(ENGINE::VertexBuffer::Creat(vertexs, sizeof(vertexs)));
//		ENGINE::BufferLayout layout =
//		{
//			{ENGINE::LayoutElementType::Float3,"a_Pos"},
//			{ENGINE::LayoutElementType::Float2,"a_Texture"}
//		};
//		m_VertexBuffer->SetLayout(layout);
//		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
//		int index[6] = { 0,1,3,0,2,3};
//		ENGINE::Ref<ENGINE::IndexBuffer>m_IndexBuffer;
//		m_IndexBuffer.reset(ENGINE::IndexBuffer::Creat(index, sizeof(index) / sizeof(int)));
//		m_VertexArray->AddIndexBuffer(m_IndexBuffer);
//		m_Texture2D = ENGINE::Texture2D::Create("1.jpg");
//	}
//	void OnUpdate(ENGINE::Time t)override
//	{
//		//LOG_TRACE("ExampleLayer OnUpdate");
//		m_CameraController.OnUpdate(t);
//		ENGINE::RenderCommand::SetClearColor({ 0.0f,0.0f,0.0f,1.0f });
//		ENGINE::RenderCommand::Clear();
//		ENGINE::Render::BeginScene();
//		m_Shader->Bind();
//		m_Texture2D->Bind();
//		std::dynamic_pointer_cast<ENGINE::OpenGLShader>(m_Shader)->UploadUniformMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
//		std::dynamic_pointer_cast<ENGINE::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", color);
//		std::dynamic_pointer_cast<ENGINE::OpenGLShader>(m_Shader)->UploadUniformInt("u_Texture",m_Texture2D->GetSlot());
//		ENGINE::Render::Submit(m_VertexArray);
//		ENGINE::Render::EndScene();
//
//	}
//	void OnImGuiRender()override
//	{
//		ImGui::Begin("test");
//		ImGui::ColorEdit3("Color",glm::value_ptr(color));
//		ImGui::End();
//	}
//	void OnEvent(ENGINE::Event& e)
//	{
//		m_CameraController.OnEvent(e);
//	}
//private:
//	ENGINE::Ref<ENGINE::VertexArray>m_VertexArray;
//	ENGINE::Ref<ENGINE::Shader>m_Shader;
//	ENGINE::GraphicsCameraController m_CameraController;
//	ENGINE::Ref<ENGINE::Texture2D>m_Texture2D;
//	glm::vec3 color=glm::vec3(1.0f);
//};

class Game :public ENGINE:: Application
{
public:
	Game() {
	
		//ExampleLayer* example = new ExampleLayer();
		//PushLayer(example);
		Layer2D* layer2D = new Layer2D();
		PushLayer(layer2D);
	};
	~Game() {};
};

ENGINE::Application* ENGINE::CreatApp()
{
	return new Game();
}
