#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"Engine/Event/WindowEvent.h"
namespace ENGINE
{
	class GraphicsCamera2D
	{
	public:
		GraphicsCamera2D(float left,float right,float bottom,float top);

		void SetPositon(const glm::vec3& position) { m_Position = position; Recalculate(); }
		void SetRotation(float rotation) { m_Rotation = rotation; Recalculate();}
		void SetProjection(float left, float right, float bottom, float top) { m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f); Recalculate(); }
		const glm::mat4& GetViewProjectionMatrix()const { return m_ViewProjectionMatrix; }
	private:
		void Recalculate();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position=glm::vec3(0.0f);
		float m_Rotation = 0.0f;
	};
	class GraphicsCameraController
	{
	public:
		GraphicsCameraController(float aspectratio);
		void OnUpdate(Time t);
		void OnEvent(Event&e);
		void Resize(float width, float height);
		GraphicsCamera2D& GetCamera(){ return m_Camera; }
	private:
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		GraphicsCamera2D m_Camera;
		float AspectRatio;
		float Scale=1.0f;
		glm::vec3 CameraPosition = glm::vec3(0.0f);
		float CameraRotation = 0.0f;
		float CameraSpeed = 0.2f;
		float CameraRotationSpeed = 0.8f;
		float CameraScaleSpeed = 0.2f;
	};

}