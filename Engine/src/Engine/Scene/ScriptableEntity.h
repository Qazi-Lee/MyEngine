#pragma once

#include"Entity.h"
#include"mono/jit/jit.h"
#include"mono/metadata/assembly.h"
#include <mono/metadata/debug-helpers.h>
#include"mono/metadata/class.h"

#include"ScriptEngine.h"

namespace ENGINE {

	struct TransformComponent;
	//加载c#
    class CLRLoader {
	public:
		CLRLoader(const char* assemblyPath, const char* assemblyName);
		~CLRLoader() {
			mono_domain_set(mono_get_root_domain(), 0); // 先切换回主域
			mono_domain_free(domain, 0);
			//std::cout << "~CLRLoader" << std::endl;
		}
		MonoImage* GetDepImage()const;
		// 发现所有派生类
		std::vector<MonoClass*> FindDerivedClasses();
        MonoDomain* domain;
	private:
        MonoImage* image;
		MonoImage* depimage;
        const char* baseClassName;

        // 递归检查类继承关系
        bool IsSubclassOf(MonoClass* klass, const char* baseName);

    };
	//桥接脚本，用来和c#间共享
	class ScriptableEntity
	{
	public:
		ScriptableEntity(const char* assemblyPath, const char* baseClass,int id=0,Scene* scene=nullptr);
		virtual ~ScriptableEntity() {
			for (auto obj : objects)
			{
				obj = nullptr;
			}
		//	std::cout << "~ScriptableEntity" << std::endl;
		}
		virtual void OnCreate();
		virtual void OnDestroy();
		virtual void OnUpdate(float t);

	public:
		CLRLoader loader;
		ScriptEngine engine;
		std::vector<MonoObject*>objects;
	private:
		ENGINE::Entity m_entity;
		template<typename T>
		T& GetComponent()
		{
			return m_entity.GetComponent<T>();
		}
		friend class Scene;
	};

}