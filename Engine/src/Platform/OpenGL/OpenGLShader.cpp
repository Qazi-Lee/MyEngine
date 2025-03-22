#include"pch.h"
#include"OpenGLShader.h"
#include<glad/glad.h>
#include<glm/gtc/type_ptr.hpp>
namespace ENGINE
{
	static GLenum StringToShaderType(const std::string& type)
	{
		std::string tmp;
		for (int i = 0; i < type.size(); i++)
		{
			if (type[i] != ' ')
			{
				tmp.push_back(type[i]);
			}
		}
		if (tmp == "Vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (tmp == "Frage")
		{
			return GL_FRAGMENT_SHADER;
		}
		else
		{
			LOG_CORE_ERROR("未知着色器类型！");
			return -1;
		}
	}

	static std::string ShderTypeToString(const GLenum& type)
	{
		if (type == GL_VERTEX_SHADER)
			return "VERTEX";
		else if (type == GL_FRAGMENT_SHADER)
			return "FRAGE";
		else
			return "ERRORTYPE";
	}

	void OpenGLShader::Complie(const std::unordered_map<GLenum, std::string>& ShaderSource)
	{
		m_ID = glCreateProgram();
		std::vector<GLenum>shaders;
		int  success;
		char infoLog[512];
		for (auto object : ShaderSource)
		{
			GLenum type = object.first;
			std::string s = object.second;
			uint32_t shader = glCreateShader(type);
			const GLchar* source = (const GLchar*)s.c_str();
			glShaderSource(shader, 1, &source, 0);
			glCompileShader(shader);
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::"<< ShderTypeToString(type)<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
			};
			glAttachShader(m_ID, shader);
			shaders.push_back(shader);
		}
		glLinkProgram(m_ID);
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
		}
		for (auto shader : shaders)
		{
			glDetachShader(m_ID, shader);
			glDeleteShader(shader);
		}
	}

	OpenGLShader::OpenGLShader(const std::string& VertexSource, const std::string& FragementSource)
	{
		std::unordered_map<GLenum, std::string>source;
		source[GL_VERTEX_SHADER] = VertexSource;
		source[GL_FRAGMENT_SHADER] = FragementSource;
		Complie(source);
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		Complie(GetSource(source));
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}

	void OpenGLShader::Bind()const
	{
		glUseProgram(m_ID);
	}

	void OpenGLShader::UnBind()const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		UploadUniformFloat(name,value);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, glm::mat4 mat) 
	{
		UploadUniformMat4(name, mat);
	}

	void OpenGLShader::SetFloat3(const std::string& name, glm::vec3 vec) 
	{
		UploadUniformFloat3(name, vec);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* value, int32_t count)
	{
		UploadUniformIntArray(name, value, count);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float& value)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec3)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform3f(location,vec3.x,vec3.y,vec3.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vec4)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform4f(location,vec4.x,vec4.y,vec4.z,vec4.w);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int& value)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformInt3(const std::string& name, const glm::vec3& vec3)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform3i(location, vec3.x, vec3.y, vec3.z);
	}

	void OpenGLShader::UploadUniformInt4(const std::string& name, const glm::vec4& vec4)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform4i(location, vec4.x, vec4.y, vec4.z, vec4.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& martix)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(martix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& martix)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(martix));
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* value, int32_t count)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform1iv(location,count,value);
	}
	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::fstream f;
		std::string result;
		f.open(filepath, std::ios::in | std::ios::binary);
		if (f.is_open())
		{
			f.seekg(0, std::ios::end);
			result.resize(f.tellg());
			f.seekp(0, std::ios::beg);
			f.read(&result[0], result.size());
		}
		else
		{
			LOG_CORE_TRACE("File Open Failed");
			__debugbreak();
		}
		f.close();
		return result;
	}
	std::unordered_map<GLenum, std::string> OpenGLShader::GetSource(const std::string& source)
	{
		std::unordered_map<GLenum, std::string>result;
		std::string Flag = "#type";
		size_t FlagLength = Flag.size();
		size_t pos=source.find_first_of(Flag, 0);
		while (pos != std::string::npos)
		{
			//找到下一个回车换行符
			size_t end= source.find_first_of("\r\n", pos);
			if (end == std::string::npos)
			{
				LOG_CORE_ERROR("着色器文件格式错误");
				break;
			}
			std::string type = source.substr(pos + FlagLength, end - (pos + FlagLength));
			std::cout << type << std::endl;
			//跳过中途的换行符
			size_t next = source.find_first_not_of("\r\n", end);
			pos = source.find(Flag, next);
			result[StringToShaderType(type)] = source.substr(next,pos-(next==std::string::npos?source.size()-1:next));
		}
		return result;
	}

}