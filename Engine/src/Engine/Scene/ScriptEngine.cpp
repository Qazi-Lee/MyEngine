#include"pch.h"

#include"ScriptEngine.h"
#include"Engine/Input.h"

#include"Engine/Scene/Entity.h"
#include"Engine/Scene/Component.h"

#include"box2d/box2d.h"

#include <mono/metadata/mono-config.h>
#include<mono/metadata/reflection.h>


namespace ENGINE
{
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> EntityHasComponentFunc;

	static Ref<SceneManager> sceneManager;

	static Scene* s_scene = nullptr;
	extern "C"
	{
		static void SceneManager_GetIndex(uint32_t * index)
		{
			if (sceneManager.get())
			{
				uint32_t i = sceneManager->GetIndex();
				*index = sceneManager->GetIndex();
			}
		}
		static void SceneManager_GetSize(uint32_t* size)
		{
			if (sceneManager.get())
			{
				uint32_t s = sceneManager->size();
				*size = (uint32_t)sceneManager->size();
			}
		}
		static void SceneManager_SetIndex(uint32_t* index)
		{
			if (sceneManager.get())
			{
				sceneManager->SetScene(*index);
			}
		}
		static bool Input_IsKeyDown(int code)
		{
			return Input::IsKeyPress(code);
		}

		static bool Entity_HasComponent(int id, MonoReflectionType* componentType)
		{
			MonoType* type = mono_reflection_type_get_type(componentType);
			Entity e = { (entt::entity)id,s_scene };
			auto it = EntityHasComponentFunc.find(type);
			if (it != EntityHasComponentFunc.end())
			{
				return EntityHasComponentFunc.at(type)(e);
			}
			else
			{
				std::cout << "c++中未注册的组件类型！"<<std::endl;
				return false;
			}
		}
		static void TagComponent_GetTag(int id, MonoString** str)
		{
			Entity e = { (entt::entity)id,s_scene };
			auto& tag = e.GetComponent<TagComponent>();
			*str = mono_string_new(mono_domain_get(), tag.tag.c_str());
		}
		static void TagComponent_SetTag(int id, MonoString** str)
		{
			Entity e = { (entt::entity)id,s_scene };
			auto& tag = e.GetComponent<TagComponent>();
			const char* cstr = mono_string_to_utf8(*str);
			tag.tag = cstr;
			mono_free((void*)cstr);
		}
		static void TransformComponent_GetTranslation(int id,glm::vec3* translation)
		{
			Entity e= { (entt::entity)id,s_scene };
			auto& transform=e.GetComponent<TransformComponent>();
			*translation = transform.Translate;
		}
		static void TransformComponent_SetTranslation(int id, glm::vec3* translation)
		{
			Entity e = { (entt::entity)id,s_scene };
			auto& transform = e.GetComponent<TransformComponent>();
			transform.Translate= *translation;
		}
		static void Rigidbody2DComponent_ApplyLinearImpulse(int id, glm::vec2* impulse, glm::vec2* point, bool wake)
		{
			Entity e = { (entt::entity)id,s_scene };
			auto& rb2d = e.GetComponent<Rigidbody2DComponent>();
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
		}

		static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(int id, glm::vec2* impulse, bool wake)
		{
			Entity e = { (entt::entity)id,s_scene };
			auto& rb2d = e.GetComponent<Rigidbody2DComponent>();
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
		}

		static void Rigidbody2DComponent_GetLinearVelocity(int id, glm::vec2* outLinearVelocity)
		{
			Entity e = { (entt::entity)id,s_scene };
			auto& rb2d = e.GetComponent<Rigidbody2DComponent>();
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			const b2Vec2& linearVelocity = body->GetLinearVelocity();
			*outLinearVelocity = glm::vec2(linearVelocity.x, linearVelocity.y);
		}

		static void Rigidbody2DComponent_SetLinearVelocity(int id, glm::vec2* outLinearVelocity)
		{
			Entity e = { (entt::entity)id,s_scene };
			auto& rb2d = e.GetComponent<Rigidbody2DComponent>();
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			body->SetLinearVelocity(b2Vec2(outLinearVelocity->x, outLinearVelocity->y));
		}

		static Rigidbody2DComponent::BodyType Rigidbody2DComponent_GetType(int id)
		{
			Entity e = { (entt::entity)id,s_scene };
			auto& rb2d = e.GetComponent<Rigidbody2DComponent>();
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			switch (body->GetType())
			{
				case b2_staticBody:
				{
					return Rigidbody2DComponent::BodyType::Static;
				}
				case b2_dynamicBody:
				{
					return Rigidbody2DComponent::BodyType::Dynamic;
				}
				case b2_kinematicBody:
				{
					return Rigidbody2DComponent::BodyType::Kinematic;
				}
			}
		}

		static void Rigidbody2DComponent_SetType(int id, Rigidbody2DComponent::BodyType bodyType)
		{
			Entity e = { (entt::entity)id,s_scene };
			auto& rb2d = e.GetComponent<Rigidbody2DComponent>();
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			b2BodyType type;
			switch (bodyType)
			{
				case  Rigidbody2DComponent::BodyType::Static:
				{
					type = b2_staticBody; break;
				}
				case  Rigidbody2DComponent::BodyType::Dynamic:
				{
					type = b2_dynamicBody; break;
				}
				case  Rigidbody2DComponent::BodyType::Kinematic:
				{
					type = b2_kinematicBody; break;
				}
			}
			body->SetType(type);
		}
	}
	ScriptEngine::ScriptEngine(Scene*scene)
	{
		s_scene = scene;
	}
	void ScriptEngine::SetSceneManager(const Ref<SceneManager>& sm)
	{
		sceneManager = sm;
	}
	void ScriptEngine::Register()
	{

		mono_add_internal_call("ScriptEngine.InternalCalls::SceneManager_GetIndex", SceneManager_GetIndex);
		mono_add_internal_call("ScriptEngine.InternalCalls::SceneManager_GetSize", SceneManager_GetSize);
		mono_add_internal_call("ScriptEngine.InternalCalls::SceneManager_SetIndex", SceneManager_SetIndex);

		mono_add_internal_call("ScriptEngine.InternalCalls::Input_IsKeyDown", Input_IsKeyDown);

		mono_add_internal_call("ScriptEngine.InternalCalls::Entity_HasComponent", Entity_HasComponent);

		mono_add_internal_call("ScriptEngine.InternalCalls::TransformComponent_GetTranslation", TransformComponent_GetTranslation);
		mono_add_internal_call("ScriptEngine.InternalCalls::TransformComponent_SetTranslation", TransformComponent_SetTranslation);

		mono_add_internal_call("ScriptEngine.InternalCalls::TagComponent_GetTag", TagComponent_GetTag);
		mono_add_internal_call("ScriptEngine.InternalCalls::TagComponent_SetTag", TagComponent_SetTag);

		mono_add_internal_call("ScriptEngine.InternalCalls::Rigidbody2DComponent_ApplyLinearImpulse", Rigidbody2DComponent_ApplyLinearImpulse);
		mono_add_internal_call("ScriptEngine.InternalCalls::Rigidbody2DComponent_ApplyLinearImpulseToCenter", Rigidbody2DComponent_ApplyLinearImpulseToCenter);
		mono_add_internal_call("ScriptEngine.InternalCalls::Rigidbody2DComponent_GetLinearVelocity", Rigidbody2DComponent_GetLinearVelocity);
		mono_add_internal_call("ScriptEngine.InternalCalls::Rigidbody2DComponent_SetLinearVelocity", Rigidbody2DComponent_SetLinearVelocity);
		mono_add_internal_call("ScriptEngine.InternalCalls::Rigidbody2DComponent_GetType", Rigidbody2DComponent_GetType);
		mono_add_internal_call("ScriptEngine.InternalCalls::Rigidbody2DComponent_SetType", Rigidbody2DComponent_SetType);

	}
	template<typename Component>
	static void RegisterComponent(MonoImage*image)
	{
		std::string_view typeName = typeid(Component).name();
		size_t pos = typeName.find_last_of(':');
		std::string_view structName = typeName.substr(pos + 1);
		std::string managedTypename = fmt::format("ScriptEngine.{}", structName);

		MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(),image);
		if (!managedType)
		{
			LOG_CORE_ERROR("Could not find component type {}", managedTypename);
			return;
		}
		EntityHasComponentFunc[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
	}

	void ScriptEngine::RegisterComponents(MonoImage*image)
	{
		RegisterComponent<TagComponent>(image);
		RegisterComponent<TransformComponent>(image);
		RegisterComponent<Rigidbody2DComponent>(image);
		//TODO:注册其他组件
		//RegisterComponent<RenderQuadComponent>(image);
		//RegisterComponent<RenderCircleComponent>(image);
		//RegisterComponent<CameraComponent>(image);
		//RegisterComponent<CScriptComponent>(image);	
		//RegisterComponent<AudioComponent>(image);

	}

}