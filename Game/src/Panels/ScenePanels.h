#pragma once
#include"Engine/Core.h"

//#include"Engine/Scene/Scene.h"
#include"Engine/Log.h"
#include"Engine/Scene/Entity.h"

namespace ENGINE
{
	class ScenePanels
	{
	public:
		ScenePanels() = default;
		ScenePanels(const Ref<Scene>& scene);
		void SetScene(const Ref<Scene>& scene);
		void SetSelectEntity(Entity e) { m_SelectedEntity = e; }
		void OnImGuiRender();
		Entity& GetSelectEntity() { return m_SelectedEntity; }
		void SetSceneState(bool b) { isPlaying = b; }
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponent(Entity entity);
		void AddComponent(Entity entity);
	private:
		Ref<Scene>m_Scene;
		Entity m_SelectedEntity;
		bool isPlaying = false;
	};
	
}