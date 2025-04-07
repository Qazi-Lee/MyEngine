#include"pch.h"

#include"ScriptEngine.h"
#include"Engine/Input.h"

#include"Engine/Scene/Entity.h"
#include"Engine/Scene/Component.h"

#include<mono/jit/jit.h>
#include <mono/metadata/mono-config.h>

namespace ENGINE
{
	static Scene* s_scene = nullptr;

	extern "C"
	{
		static bool Input_IsKeyDown(int code)
		{
			return Input::IsKeyPress(code);
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
	}
	ScriptEngine::ScriptEngine(Scene* scene)
	{
		s_scene = scene;
	}
	void ScriptEngine::Register()
	{
		mono_add_internal_call("ScriptEngine.InternalCalls::Input_IsKeyDown", Input_IsKeyDown);

		mono_add_internal_call("ScriptEngine.InternalCalls::TransformComponent_GetTranslation", TransformComponent_GetTranslation);
		mono_add_internal_call("ScriptEngine.InternalCalls::TransformComponent_SetTranslation", TransformComponent_SetTranslation);

		mono_add_internal_call("ScriptEngine.InternalCalls::TagComponent_GetTag", TagComponent_GetTag);
		mono_add_internal_call("ScriptEngine.InternalCalls::TagComponent_SetTag", TagComponent_SetTag);
	}

}