#pragma once
#include"Engine/Scene/Scene.h"

namespace ENGINE
{
	class SceneManager
	{
	public:
		SceneManager()=default;
		SceneManager(const Ref<Scene>& scene);
		~SceneManager()
		{
			for (Ref<Scene> s : Scenes)
			{
				s->OnRuntimeEnd();
			}
			Scenes.clear();
		}
		Ref<Scene>& GetScene();
		Ref<Scene>& GetScene(int i);
		void AddScene(std::filesystem::path filepath);
		void AddScene(std::string name="NewScene");
		void ResetScene(const std::string& name="EditorScene");
		void DeleteScene(int i);
		void SetScene(int id);
		void RenameScene(std::string name);
		uint32_t GetIndex()const { return index; }
		size_t size();
	public:
		std::unordered_map<Ref<Scene>, std::string>SceneMap;
	private:
		std::vector<Ref<Scene>>Scenes;
		uint32_t index=0;
	};

}