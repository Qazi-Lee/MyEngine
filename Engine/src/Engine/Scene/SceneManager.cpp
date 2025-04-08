#include"pch.h"

#include"SceneManager.h"
#include"Engine/Scene/SceneSerialization.h"
namespace ENGINE
{

	 Ref<Scene>& SceneManager::GetScene()
	{
		return Scenes[index];
	}

	 Ref<Scene>& SceneManager::GetScene(int i)
	 {
		 if (i >= 0 && i < size())
		 {
			 return Scenes[i];
		 }
		 else
		 {
			 __debugbreak();
		 }	
	 }

	void SceneManager::AddScene(std::filesystem::path filepath)
	{
		Ref<Scene> newScene = std::make_shared<Scene>();
		SceneSerialization m_SceneSerialization(newScene);
		m_SceneSerialization.Deserialize(filepath.string());
		Scenes.push_back(newScene);
		SceneMap[newScene] = filepath.stem().string();
	}

	void SceneManager::AddScene(std::string name)
	{
		Ref<Scene>scene = std::make_shared<Scene>();
		Scenes.push_back(scene);
		SceneMap[scene] = name;
	}

	void SceneManager::ResetScene(const std::string& name)
	{
		auto& now = GetScene();
		//从原来表中删除
		auto it = SceneMap.find(now);
		if (it != SceneMap.end())
		{
			SceneMap.erase(it);
		}
		now = std::make_shared<Scene>();
		{
			SceneMap[now] = name;
		}

	}

	void SceneManager::DeleteScene(int i)
	{	
		//从表中移除
		auto it = SceneMap.find(Scenes[i]);
		if (it != SceneMap.end())
		{
			SceneMap.erase(it);
		}
		//从数组中移除
		Scenes.erase(Scenes.begin() + i);
	}

	void SceneManager::SetScene(int id)
	{
		index = id;
	}

	void SceneManager::RenameScene(std::string name)
	{
		auto scene = GetScene();
		auto it = SceneMap.find(scene);
		if (it != SceneMap.end())
		{
			it->second = name;
		}
	}


	size_t SceneManager::size()
	{
		return Scenes.size();
	}

}