#pragma once
#include<string>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include "SceneCamera.h"
#include"ScriptableEntity.h"
#include"Engine/Render/Texture.h"
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
namespace ENGINE
{
	struct TagComponent
	{
		std::string tag;
		TagComponent(const std::string& name = std::string()):tag(name){}
		TagComponent() = default;
		TagComponent(const TagComponent& other) = default;

	};
	struct TransformComponent
	{
		glm::vec3 Translate = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);
		TransformComponent(const glm::vec3&translate):Translate(translate){}
		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		 glm::mat4 GetTransform() 
		{ 
			 glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			 return glm::translate(glm::mat4(1.0f), Translate)
				 * rotation
				 * glm::scale(glm::mat4(1.0f), Scale);
		}
	};
	struct RenderQuadComponent
	{
		glm::vec4 color=glm::vec4(1.0f);
		Ref<Texture2D> Texture;
		RenderQuadComponent(const glm::vec4&col):color(col){}
		RenderQuadComponent() = default;
		RenderQuadComponent(const RenderQuadComponent& other) = default;
		operator const glm::vec4& () { return color; }
	};

	struct RenderCircleComponent
	{
		glm::vec4 color = glm::vec4(1.0f);
		Ref<Texture2D> Texture;
		float thickness = 1.0f;
		float fade = 0.005f;
		RenderCircleComponent() = default;
		RenderCircleComponent(const RenderCircleComponent&) = default;
	};
	struct CameraComponent
	{
		SceneCamera camera;
		//主相机标志
		bool Primary = false;
		bool FixedAspectRatio = false;
		CameraComponent(const SceneCamera& c) :camera(c) {}
		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;

	};

	struct ScriptComponent
	{
		std::string ClassName;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};

	struct NativeScriptComponent
	{
		//绑定一个脚本实体
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InitInstanceFunc)();//初始化指针
		void(*DestroyInstanceFunc)(NativeScriptComponent*);//销毁指针

		template<typename T>
		void Bind()
		{
			InitInstanceFunc = []() {return static_cast<ScriptableEntity*>(new T()); };
			DestroyInstanceFunc = [](NativeScriptComponent*nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	//2d刚体组件
	struct Rigidbody2DComponent
	{
		enum  class BodyType{Static=0,Dynamic,Kinematic};
		BodyType Type = BodyType::Static;
		//是否禁止旋转
		bool FixedRotation = false;

		//获取2d实体
		void* RuntimeBody = nullptr;

		//boxsize
		glm::vec2 size = { 0.5f,0.5f };
		//密度
		float Density = 1.0f;
		//摩擦
		float Friction = 0.5f;
		//弹性系数
		float Restitution = 0.0f;
		//弹性系数阈值
		float RestitutionThreshold = 1.0f;

		//夹具
		void* RuntimeFixture = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};


}