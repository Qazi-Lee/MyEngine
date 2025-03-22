#pragma once

#include"Engine/Render/Camera.h"
#include<tuple>

namespace ENGINE
{
	class SceneCamera :public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera()=default;
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);
		void SetAspectRatio(float aspectratio);
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicSize() const { return m_OrthographicSize; }
		std::tuple<float, float> GetOrthographicNearAndFar()const { return  { m_OrthographicNear, m_OrthographicFar }; }
		float GetAspectRatio()const { return m_AspectRatio; }
	private:
		void RecalculateProjection();
	private:
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};

}