#pragma once
#include"Engine/Scene/Scene.h"

namespace ENGINE
{
	class SceneManager
	{
	public:
		SceneManager() = default;
		SceneManager(const Ref<Scene>& scene)
		{
			Scenes.push_back(scene);
			SceneMap[scene] = "EditorScene";
		}
		Ref<Scene>& GetScene();
		Ref<Scene>& GetScene(int i);
		void AddScene(std::filesystem::path filepath);
		void AddScene(std::string name="NewScene");
		void ResetScene(const std::string& name="EditorScene");
		void DeleteScene(int i);
		void SetScene(int id);
		void RenameScene(std::string name);
		size_t size();
	public:
		std::unordered_map<Ref<Scene>, std::string>SceneMap;
	private:
		std::vector<Ref<Scene>>Scenes;
		
		uint32_t index=0;
	};

}