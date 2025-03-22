#pragma once
#include<string>
#include<glm/glm.hpp>
namespace ENGINE
{
	class Shader
	{
	public:
		
		virtual ~Shader()=default;
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetMat4(const std::string& name, glm::mat4 mat) =0;
		virtual void SetFloat3(const std::string& name, glm::vec3 vec) = 0;
		virtual void SetIntArray(const std::string& name, int* value,int32_t count) = 0;
		static Ref<Shader> Create(const std::string& VertexSource, const std::string& FragementSource);
		static Ref<Shader> Create(const std::string& filepath);
	};

}