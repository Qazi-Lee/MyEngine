#pragma once
#include<entt.hpp>
#include"Engine/Core.h"
#include"Engine/Render/EditorCamera.h"
#include<string>

class b2World;

namespace ENGINE
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		Scene(const Scene&) = default;
		Entity CreateEntity(const std::string& name=std::string());
		void OnUpdate(Time t);
		void OnUpdateEditor(Time s, const EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);
		void DestoryEntity(const Entity& entity);

		static Ref<Scene> Copy(Ref<Scene> other);

		void OnRuntimeStart();
		void OnRuntimeEnd();

		void ShowEntitySize();//Test
	private:
		template<typename T>
		void OnComponentAdded(T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		b2World* m_b2World=nullptr;
		friend class Entity;
		friend class SceneSerialization;
		friend class ScenePanels;
	};

}