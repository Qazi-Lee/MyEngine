#pragma once
#include"Engine/Render/Shader.h"
#include<string>
namespace ENGINE
{
	typedef uint32_t GLenum;
	class OpenGLShader :public Shader
	{
	public:
		OpenGLShader(const std::string& VertexSource, const std::string& FragementSource);
		OpenGLShader(const std::string& filepath);
		~OpenGLShader();
		void Bind()const override;
		void UnBind()const override;
		void SetFloat(const std::string& name, float value)override;
		void SetInt(const std::string& name, int value)override;
		void SetMat4(const std::string& name, glm::mat4 mat) override;
		void SetFloat3(const std::string& name, glm::vec3 vec) override;
		virtual void SetIntArray(const std::string& name, int* value, int32_t count)override;
	public:
		void UploadUniformFloat(const std::string& name, const float& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& vec3);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& vec4);
		void UploadUniformInt(const std::string& name, const int&value);
		void UploadUniformInt3(const std::string& name, const glm::vec3& vec3);
		void UploadUniformInt4(const std::string& name, const glm::vec4& vec4);
		void UploadUniformMat3(const std::string& name, const glm::mat3& martix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& martix);
		void UploadUniformIntArray(const std::string& name, int* value, int32_t count);
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string>GetSource(const std::string& source);
		void Complie(const std::unordered_map<GLenum, std::string>&ShaderSsource);
	private:
		uint32_t m_ID;
};
}