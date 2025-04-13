#include"pch.h"
#include"SceneSerialization.h"
#include"Entity.h"
#include"Component.h"
#include"Engine/Render/Texture.h"
#define YAML_CPP_STATIC_DEFINE
#include<yaml-cpp/yaml.h>
#include<fstream>
#include<tuple>
namespace YAML
{
	//重载运算符
	static Emitter& operator<<(Emitter& out, const glm::vec2& vec)
	{
		out << Flow;
		out << BeginSeq << vec.x << vec.y<< EndSeq;
		return out;
	}
	static Emitter& operator<<( Emitter&out,const glm::vec3&vec)
	{
		out << Flow;
		out << BeginSeq<<vec.x<<vec.y<<vec.z<<EndSeq;
		return out;
	}
	static Emitter& operator<<(Emitter& out, const glm::vec4& vec)
	{
		out << Flow;
		out << BeginSeq << vec.x << vec.y << vec.z<<vec.w << EndSeq;
		return out;
	}
	//增加convert模板
	template<>
	struct convert<glm::vec2>
	{
		//序列化
		static Node encode(const glm::vec2& vec)
		{
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			return node;
		}
		//反序列化
		static bool decode(const Node& node, glm::vec2& vec)
		{
			//将数组转换为vec3
			if (!node.IsSequence() || node.size() != 2)
				return false;
			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec3>
	{
		//序列化
		static Node encode(const glm::vec3& vec)
		{
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			return node;
		}
		//反序列化
		static bool decode(const Node& node, glm::vec3& vec)
		{
			//将数组转换为vec3
			if (!node.IsSequence() || node.size() != 3)
				return false;
			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		//序列化
		static Node encode(const glm::vec4& vec)
		{
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
			return node;
		}
		//反序列化
		static bool decode(const Node& node, glm::vec4& vec)
		{
			//将数组转换为vec3
			if (!node.IsSequence() || node.size() != 4)
				return false;
			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();
			vec.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<std::tuple<float,float>>
	{
		//序列化
		static Node encode(const std::tuple<float,float>& vec)
		{
			Node node;
			auto [x, y] = vec;
			node.push_back(x);
			node.push_back(y);
			return node;
		}
		//反序列化
		static bool decode(const Node& node, std::tuple<float,float>& vec)
		{
			//将数组转换为vec3
			if (!node.IsSequence() || node.size() !=2)
				return false;
			std::get<0>(vec) = node[0].as<float>();
			std::get<1>(vec) = node[1].as<float>();
			return true;
		}
	};
}

namespace ENGINE
{
	static void SerializeEntity( YAML::Emitter& out, Entity& entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::BeginMap;
		//标签组件
		if (entity.HasComponent <TagComponent>())
		{
			out << YAML::Key << "TagComponent" << YAML::BeginMap;
			auto tag = entity.GetComponent<TagComponent>().tag;
			out << YAML::Key << "Tag" << YAML::Value << tag.c_str();
			out << YAML::EndMap;
		}
		//变换组件
		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent" << YAML::BeginMap;
			auto translate = entity.GetComponent<TransformComponent>().Translate;
			auto rotation = entity.GetComponent<TransformComponent>().Rotation;
			auto scale = entity.GetComponent<TransformComponent>().Scale;
			out << YAML::Key << "Translate" << YAML::Value << translate;
			out << YAML::Key << "Rotation" << YAML::Value << rotation;
			out << YAML::Key << "Scale" << YAML::Value << scale;
			out << YAML::EndMap;
		}
		//渲染组件
		if (entity.HasComponent<RenderQuadComponent>())
		{
			out << YAML::Key << "RenderQuadComponent" << YAML::BeginMap;
			auto color = entity.GetComponent<RenderQuadComponent>().color;
			out << YAML::Key << "Color" << YAML::Value << color;
			auto& texture = entity.GetComponent<RenderQuadComponent>().Texture;
			std::string path = std::string();
			if (texture.get())
				path = texture->GetPath();
			out << YAML::Key << "Texture" << YAML::Value << path;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<RenderCircleComponent>())
		{
			out << YAML::Key << "RenderCircleComponent" << YAML::BeginMap;
			auto color = entity.GetComponent<RenderCircleComponent>().color;
			out << YAML::Key << "Color" << YAML::Value << color;
			auto& texture = entity.GetComponent<RenderCircleComponent>().Texture;
			auto& thickness= entity.GetComponent<RenderCircleComponent>().thickness;
			auto& fade = entity.GetComponent<RenderCircleComponent>().fade;
			std::string path = std::string();
			if (texture.get())
				path = texture->GetPath();
			out << YAML::Key << "Texture" << YAML::Value << path;
			out << YAML::Key << "Thickness" << YAML::Value << thickness;
			out << YAML::Key << "Fade" << YAML::Value << fade;
			out << YAML::EndMap;

		}
		//相机组件
		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent" << YAML::BeginMap;
			auto camera = entity.GetComponent<CameraComponent>().camera;
			out << YAML::Key << "Camera" << YAML::BeginMap;
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "AspectRatio" << YAML::Value << camera.GetAspectRatio();
			out << YAML::Key << "OrthographicNearAndFar" << YAML::Flow;
			auto [Near, Far] = camera.GetOrthographicNearAndFar();
			out << YAML::BeginSeq << Near << Far << YAML::EndSeq;
			out << YAML::EndMap;
			auto primary = entity.GetComponent<CameraComponent>().Primary;
			auto fixed = entity.GetComponent< CameraComponent>().FixedAspectRatio;
			out << YAML::Key << "Primary" << YAML::Value << primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << fixed;
			out << YAML::EndMap;
		}
		//2d刚体组件
		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent" << YAML::BeginMap;
			auto& rg2d = entity.GetComponent<Rigidbody2DComponent>();
			out << YAML::Key << "Type" << YAML::Value << (int)rg2d.Type;
			out << YAML::Key << "FixedRotation" << YAML::Value << rg2d.FixedRotation;
			out << YAML::Key << "Size" << YAML::Value << rg2d.size;
			out << YAML::Key << "Density" << YAML::Value << rg2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << rg2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << rg2d.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << rg2d.RestitutionThreshold;
			out << YAML::Key << "Origin" << YAML::Value << rg2d.Origin;
			out << YAML::EndMap;

		}
		//C#脚本组件
		if (entity.HasComponent<CScriptComponent>())
		{
			out << YAML::Key << "CScriptComponent" << YAML::BeginMap;
			auto& csc = entity.GetComponent<CScriptComponent>();
			out << YAML::Key << "ClassName" << YAML::Value << csc.ClassName.c_str();
			out << YAML::Key << "Path" << YAML::Value << csc.Path.c_str();
			out << YAML::EndMap;
		}
		//音频组件
		if (entity.HasComponent<AudioComponent>())
		{
			out << YAML::Key << "AudioComponent" << YAML::BeginMap;
			auto& ac = entity.GetComponent<AudioComponent>();
			out << YAML::Key << "MusicName" << YAML::Value << ac.MusicName.c_str();
			out << YAML::Key << "Path" << YAML::Value << ac.Path.c_str();
			out << YAML::Key << "Volume" << YAML::Value << ac.Volume;
			out << YAML::Key << "stop" << YAML::Value << ac.stop;
			out << YAML::Key << "loop" << YAML::Value << ac.loop;
			out << YAML::EndMap;
		}
		//标签组件
		if (entity.HasComponent<LableComponent>())
		{
			out << YAML::Key << "LableComponent" << YAML::BeginMap;
			auto& lc = entity.GetComponent<LableComponent>();
			out << YAML::Key << "Path" << YAML::Value << lc.Path.c_str();
			out << YAML::Key << "Text" << YAML::Value << lc.Text.c_str();
			out << YAML::Key << "TextColor" << YAML::Value << lc.TextColor;
			out << YAML::EndMap;
		}
		//按钮组件
		if (entity.HasComponent<ButtonComponent>())
		{
			out << YAML::Key << "ButtonComponent" << YAML::BeginMap;
			auto& bc = entity.GetComponent<ButtonComponent>();
			out << YAML::Key << "Path" << YAML::Value << bc.Path.c_str();
			out << YAML::Key << "Text" << YAML::Value << bc.Text.c_str();
			out << YAML::Key << "BackColor" << YAML::Value << bc.BackColor;
			out << YAML::Key << "TextColor" << YAML::Value << bc.TextColor;
			out << YAML::EndMap;
		}
		//TODO:等待其他组件添加
		out << YAML::EndMap;
		out << YAML::EndMap;

	}
	SceneSerialization::SceneSerialization(const Ref<Scene>& scene)
		:m_Scene(scene)
	{

	}

	void SceneSerialization::Serialize(const std::string& filename)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << (uint32_t)typeid(m_Scene).hash_code();
		//所有实体存放在数组中
		out << YAML::Key << "Entities" << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityid) 
			{
				Entity entity ={ entityid, m_Scene.get()};
				if (!entity)
					return;
				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::fstream of;
		of.open(filename, std::ios::out);
		of << out.c_str();
	}

	bool SceneSerialization::Deserialize(const std::string& filename)
	{
		std::fstream f;
		f.open(filename);
		if (!f.is_open())
			return false;
		//转为stringstream（TODO)
		 std::stringstream StrStream;
		 StrStream << f.rdbuf();
		YAML::Node data = YAML::Load(StrStream.str());
		if (!data["Scene"])
			return false;
		std::string Scenename = data["Scene"].as<std::string>();
		LOG_CORE_TRACE("Deserlizing Scene '{0}'", Scenename);
		//得到实体数组
		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entitymap : entities)
			{
				auto entity = entitymap["Entity"];				
				//TagComponent
				std::string name;
				auto tagComponent= entity["TagComponent"];
				if (tagComponent)
				{
					name = tagComponent["Tag"].as<std::string>();
					LOG_CORE_TRACE("Deserializing entity '{0}'", name);
				}
				//创建实体
				Entity DeserializationEntity = m_Scene->CreateEntity(name);
				//TransformComponent
				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& transform = DeserializationEntity.GetComponent<TransformComponent>();
					transform.Translate = transformComponent["Translate"].as<glm::vec3>();
					transform.Rotation= transformComponent["Rotation"].as<glm::vec3>();
					transform.Scale = transformComponent["Scale"].as<glm::vec3>();
				}
				//RenderQuadComponent
				auto renderquadComponennt = entity["RenderQuadComponent"];
				if (renderquadComponennt)
				{
					auto color = renderquadComponennt["Color"].as<glm::vec4>();
					auto &EntityRC=DeserializationEntity.AddComponent<RenderQuadComponent>(color);
					auto path = renderquadComponennt["Texture"].as<std::string>();
					if (!path.empty())
					{
						Ref<Texture2D>texture = Texture2D::Create(path);
						EntityRC.Texture = texture;
					}
				}
				//RenderCircleComponent
				auto rendercircleComponennt = entity["RenderCircleComponent"];
				if (rendercircleComponennt)
				{
					auto color = rendercircleComponennt["Color"].as<glm::vec4>();
					auto& EntityRC = DeserializationEntity.AddComponent<RenderCircleComponent>(color);
					auto path = rendercircleComponennt["Texture"].as<std::string>();
					if (!path.empty())
					{
						Ref<Texture2D>texture = Texture2D::Create(path);
						EntityRC.Texture = texture;
					}
					EntityRC.thickness = rendercircleComponennt["Thickness"].as<float>();
					EntityRC.fade = rendercircleComponennt["Fade"].as<float>();
				}
				//CameraComponent
				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc=DeserializationEntity.AddComponent<CameraComponent>();
					auto camera = cameraComponent["Camera"];
					if (camera)
					{
						auto size = camera["OrthographicSize"].as<float>();
						auto ratio = camera["AspectRatio"].as<float>();
						auto [Near, Far] = camera["OrthographicNearAndFar"].as<std::tuple<float,float>>();
						cc.camera.SetOrthographic(size, Near, Far);
						cc.camera.SetAspectRatio(ratio);
					}
					auto primary = cameraComponent["Primary"].as<bool>();
					auto fixed = cameraComponent["FixedAspectRatio"].as<bool>();
					cc.Primary = primary;
					cc.FixedAspectRatio = fixed;
				}
				//2d刚体组件
				auto bodyComponent = entity["Rigidbody2DComponent"];
				if (bodyComponent)
				{
					auto& bc = DeserializationEntity.AddComponent<Rigidbody2DComponent>();
					bc.Type = (Rigidbody2DComponent::BodyType)bodyComponent["Type"].as<int>();
					bc.FixedRotation = bodyComponent["FixedRotation"].as<bool>();
					bc.size = bodyComponent["Size"].as<glm::vec2>();
					bc.Density = bodyComponent["Density"].as<float>();
					bc.Friction = bodyComponent["Friction"].as<float>();
					bc.Restitution= bodyComponent["Restitution"].as<float>();
					bc.RestitutionThreshold= bodyComponent["RestitutionThreshold"].as<float>();
					bc.Origin = bodyComponent["Origin"].as<glm::vec2>();
				}
				//C#脚本组件
				auto csComponent = entity["CScriptComponent"];
				if (csComponent)
				{
					auto& csc = DeserializationEntity.AddComponent<CScriptComponent>();
					csc.ClassName = csComponent["ClassName"].as<std::string>();
					csc.Path = csComponent["Path"].as<std::string>();
				}
				//音频组件
				auto auComponent = entity["AudioComponent"];
				if (auComponent)
				{
					auto& ac = DeserializationEntity.AddComponent<AudioComponent>();
					ac.MusicName = auComponent["MusicName"].as<std::string>();
					ac.Path = auComponent["Path"].as<std::string>();
					ac.Volume = auComponent["Volume"].as<float>();
					ac.stop = auComponent["stop"].as<bool>();
					ac.loop = auComponent["loop"].as<bool>();
					ac.loadAudio(ac.Path);
				}
				//标签组件
				auto laComponent = entity["LableComponent"];
				if (laComponent)
				{
					auto& lc = DeserializationEntity.AddComponent<LableComponent>();
					lc.Path = laComponent["Path"].as<std::string>();
					lc.Text = laComponent["Text"].as<std::string>();
					lc.TextColor = laComponent["TextColor"].as<glm::vec4>();
				}
				//按钮组件
				auto bComponent = entity["ButtonComponent"];
				if (bComponent)
				{
					auto& bc = DeserializationEntity.AddComponent<ButtonComponent>();
					bc.Path = bComponent["Path"].as<std::string>();
					bc.Text = bComponent["Text"].as<std::string>();
					bc.BackColor = bComponent["BackColor"].as<glm::vec4>();
					bc.TextColor = bComponent["TextColor"].as<glm::vec4>();
				}
			}
		}
		return true;
	}


}