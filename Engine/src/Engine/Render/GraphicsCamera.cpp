#include"pch.h"
#include"GraphicsCamera.h"
#include"Engine/Input.h"
#include"Engine/KeyCode.h"

namespace ENGINE
{

	GraphicsCamera2D::GraphicsCamera2D(float left, float right, float bottom, float top)
		:m_ProjectionMatrix(glm::ortho(left,right,bottom,top,-1.0f,1.0f)),m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void GraphicsCamera2D::Recalculate()
	{
		glm::mat4 tranform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = glm::inverse(tranform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		//m_ViewProjectionMatrix = m_ViewMatrix* m_ProjectionMatrix;
	}
	
	GraphicsCameraController::GraphicsCameraController(float aspectratio)
		:m_Camera(-aspectratio *Scale, aspectratio*Scale,-Scale,Scale),AspectRatio(aspectratio)
	{

	}

	void GraphicsCameraController::OnUpdate(Time t)
	{
		if (Input::IsKeyPress(ME_KEY_LEFT))
		{
			CameraPosition.x += CameraSpeed*t.GetSecond();
		}
		if (Input::IsKeyPress(ME_KEY_RIGHT))
		{
			CameraPosition.x -= CameraSpeed* t.GetSecond();
		}
		if (Input::IsKeyPress(ME_KEY_UP))
		{
			CameraPosition.y -= CameraSpeed * t.GetSecond();
		}
		if (Input::IsKeyPress(ME_KEY_DOWN))
		{
			CameraPosition.y += CameraSpeed * t.GetSecond();
		}
		if (Input::IsKeyPress(ME_KEY_A))
		{
			CameraRotation -= CameraRotationSpeed * t.GetSecond();
		}
		if (Input::IsKeyPress(ME_KEY_D))
		{
			CameraRotation += CameraRotationSpeed * t.GetSecond();
		}
		if (Input::IsKeyPress(ME_KEY_W))
		{
			Scale -= CameraScaleSpeed * t.GetSecond();
		}
		if (Input::IsKeyPress(ME_KEY_S))
		{
			Scale += CameraScaleSpeed * t.GetSecond();
		}
		m_Camera.SetPositon(CameraPosition);
		m_Camera.SetRotation(CameraRotation);
		m_Camera.SetProjection(-AspectRatio * Scale, AspectRatio * Scale, -Scale, Scale);
	}

	void GraphicsCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(GraphicsCameraController::OnWindowResized));
	}

	void GraphicsCameraController::Resize(float width, float height)
	{
		AspectRatio = width/ height;
		m_Camera.SetProjection(-AspectRatio * Scale, AspectRatio * Scale, -Scale, Scale);

	}

	bool GraphicsCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-AspectRatio * Scale, AspectRatio * Scale, -Scale, Scale);
		return false;
	}
}