#pragma once
#include"Scene.h"
#include"Engine/Core.h"

#include<string>


namespace ENGINE
{
	class SceneSerialization
	{
	public:
		SceneSerialization(const Ref<Scene>&scene);

		void Serialize(const std::string& filename);
		bool Deserialize(const std::string& filename);
	private:
		Ref<Scene>m_Scene;
	};
}
