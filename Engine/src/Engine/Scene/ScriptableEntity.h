#pragma once

#include"Entity.h"

namespace ENGINE {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(ENGINE::Time t) {}
	private:
		ENGINE::Entity m_entity;
		friend class Scene;
	};

}