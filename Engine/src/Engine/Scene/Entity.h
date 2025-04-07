#pragma once
#include<entt.hpp>
#include"Scene.h"
namespace ENGINE
{
	class  Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);
		Entity(const Entity& other)=default;

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_Entity);
		}

		template<typename T>
		T& GetComponent()
		{
			if (!HasComponent<T>())
			{
				LOG_CORE_ERROR("组件不存在！");
				__debugbreak();
			}
			return m_Scene->m_Registry.get<T>(m_Entity);
		}

		template<typename T,typename... Args>
		T& AddComponent(Args&&... args)
		{
			if (HasComponent<T>())
			{
				LOG_CORE_ERROR("组件已存在！");
				__debugbreak();
			}
			T&component= m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(component);
			return component;

		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registry.remove<T>(m_Entity);
			//增加移除组件函数回调

		}


		operator bool()const { return m_Scene!=nullptr&&m_Entity!=entt::null&&m_Scene->m_Registry.valid(m_Entity); }
		operator entt::entity()const { return m_Entity; }
		operator uint32_t()const { return (uint32_t)m_Entity; }

		bool operator==(const Entity& other) { return this->m_Entity == other.m_Entity&&this->m_Scene==other.m_Scene; }
		bool operator!=(const Entity& other) { return !(*this == other); }
	private:
		entt::entity m_Entity{entt::null};
		Scene* m_Scene = nullptr;
	};

	
}