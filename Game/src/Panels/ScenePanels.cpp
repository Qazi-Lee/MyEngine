#include"ScenePanels.h"

#include"imgui.h"
#include"Engine/Scene/Component.h"
#include"glm/gtc/type_ptr.hpp"
#include<algorithm>
#include<filesystem>
namespace ENGINE
{

	ScenePanels::ScenePanels(const Ref<Scene>& scene)
	{
		SetScene(scene);
	}

	void ScenePanels::SetScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
		m_SelectedEntity =Entity();
	}

	void ScenePanels::OnImGuiRender()
	{
		//ImGui::ShowMetricsWindow();
		ImGui::Begin("Scene Entity");
		if (!m_Scene->m_Registry.empty())
		{
			m_Scene->m_Registry.each([&](auto entityid)
				{
					Entity entity(entityid, m_Scene.get());
					DrawEntityNode(entity);
				}
			);
		}
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectedEntity=Entity();
		}
		//右键添加实体功能

		if (ImGui::BeginPopupContextWindow(0,1,false))
		{
			if (ImGui::MenuItem("Add Entity"))
			{
				m_Scene->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		//选中后绘制组件
		ImGui::Begin("Component");
		if (m_SelectedEntity)
		{
			DrawComponent(m_SelectedEntity);
			AddComponent(m_SelectedEntity);
		}
		ImGui::End();
	}

	void ScenePanels::DrawEntityNode(Entity entity)
	{
		std::string empty = "Empty Entity";
		auto& tag = entity.HasComponent<TagComponent>()?entity.GetComponent<TagComponent>().tag:empty;
		ImGuiTreeNodeFlags flag = (entity==m_SelectedEntity)? ImGuiTreeNodeFlags_Selected:0|ImGuiTreeNodeFlags_OpenOnArrow;
		bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flag, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				m_Scene->DestoryEntity(entity);
				if (m_SelectedEntity == entity)
				{
					m_SelectedEntity = Entity();
				}
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			ImGui::TreePop();
		}

	}
	void ScenePanels::DrawComponent(Entity entity)
	{
		ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer,tag.c_str());
			ImGui::InputText("Tag", buffer, sizeof(buffer));
			tag = std::string(buffer);
			ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f);
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("Tag");
			}
			if (ImGui::BeginPopup("Tag",ImGuiPopupFlags_NoOpenOverExistingPopup))
			{
				if (ImGui::MenuItem("Delete Component"))
				{
					entity.RemoveComponent<TagComponent>();
				}
				ImGui::EndPopup();
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flag, "Transform");
			ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f);
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("Transform");
			}
			if (open)
			{
				auto& translate = entity.GetComponent<TransformComponent>().Translate;
				ImGui::DragFloat3("Position", glm::value_ptr(translate), 0.1f);
				auto& rotation = entity.GetComponent<TransformComponent>().Rotation;
				ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1f);
				auto& scale = entity.GetComponent<TransformComponent>().Scale;
				ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f);
				if (ImGui::BeginPopup("Transform", ImGuiPopupFlags_NoOpenOverExistingPopup))
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity.RemoveComponent<TransformComponent>();
					}
					ImGui::EndPopup();
				}
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera");
			ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f);
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("Camera");
			}
			if (open)
			{
				auto& camera = entity.GetComponent<CameraComponent>();
				bool primary = camera.Primary;
				if (ImGui::Checkbox("Primary", &primary))
				{
					camera.Primary = camera.Primary == true ? false : true;
				}
				bool fixed = camera.FixedAspectRatio;
				if (ImGui::Checkbox("FixedScale", &fixed))
				{
					camera.FixedAspectRatio = camera.FixedAspectRatio == true ? false : true;
				}
				if (ImGui::BeginPopup("Camera", ImGuiPopupFlags_NoOpenOverExistingPopup))
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity.RemoveComponent<CameraComponent>();
					}
					ImGui::EndPopup();
				}
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<RenderQuadComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(RenderQuadComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Quad");
			ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f);
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("RenderQuad");
			}
			if (open)
			{
				auto& color = entity.GetComponent<RenderQuadComponent>().color;
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
				float width = ImGui::GetWindowSize().x / 3;
				float height = ImGui::GetWindowSize().y / 4;
				auto& texture = entity.GetComponent<RenderQuadComponent>().Texture;
				Ref<Texture2D>blackTex = Texture2D::Create(1,1);
				uint32_t black = 0x00000000;
				blackTex->SetData(&black, sizeof(uint32_t));
				uint32_t textureID = texture.get() ? texture->GetID() : blackTex->GetID();
				ImGui::Image((void*)textureID, ImVec2(width, height), { 0,1 }, { 1,0 });
				//添加拖拽
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturepath = std::filesystem::path(path);
						if (texturepath.extension().string() == ".jpg" || texturepath.extension().string() == ".png"
							|| texturepath.extension().string() == ".tiff" || texturepath.extension().string() == ".jpeg"
							|| texturepath.extension().string() == ".gif" || texturepath.extension().string() == ".bmp")
						{
							texture = Texture2D::Create(texturepath.string());
						}
						else
						{
							LOG_INFO("纹理格式错误！");
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::TextWrapped("Texture");
				if (ImGui::BeginPopup("RenderQuad", ImGuiPopupFlags_NoOpenOverExistingPopup))
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity.RemoveComponent<RenderQuadComponent>();
					}
					ImGui::EndPopup();
				}
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<RenderCircleComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(RenderCircleComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Circle");
			ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f);
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("RenderCircle");
			}
			if (open)
			{
				auto& component = entity.GetComponent<RenderCircleComponent>();
				auto& color =component.color;
				auto& thickness = component.thickness;
				auto& fade = component.fade;
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
				ImGui::SliderFloat("Thickness", &thickness, 0.0f, 1.0f);
				ImGui::SliderFloat("Fade", &fade, 0.0f, 1.0f);
				float width = ImGui::GetWindowSize().x / 3;
				float height = ImGui::GetWindowSize().y / 4;
				auto& texture = entity.GetComponent<RenderCircleComponent>().Texture;
				Ref<Texture2D>blackTex = Texture2D::Create(1, 1);
				uint32_t black = 0x00000000;
				blackTex->SetData(&black, sizeof(uint32_t));
				uint32_t textureID = texture.get() ? texture->GetID() : blackTex->GetID();
				ImGui::Image((void*)textureID, ImVec2(width, height), { 0,1 }, { 1,0 });
				//添加拖拽
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturepath = std::filesystem::path(path);
						if (texturepath.extension().string() == ".jpg" || texturepath.extension().string() == ".png"
							|| texturepath.extension().string() == ".tiff" || texturepath.extension().string() == ".jpeg"
							|| texturepath.extension().string() == ".gif" || texturepath.extension().string() == ".bmp")
						{
							texture = Texture2D::Create(texturepath.string());
						}
						else
						{
							LOG_INFO("纹理格式错误！");
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::TextWrapped("Texture");
				if (ImGui::BeginPopup("RenderCircle", ImGuiPopupFlags_NoOpenOverExistingPopup))
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity.RemoveComponent<RenderCircleComponent>();
					}
					ImGui::EndPopup();
				}
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(Rigidbody2DComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Rigidbody2D");
			ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f);
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("Rigidbody2D");
			}
			if (open)
			{
				auto& rgd2d = entity.GetComponent<Rigidbody2DComponent>();
				//控制刚体类型
				const char* bodyTypeStr[] = { "Static","Dynamic","Kinematic" };
				const char* currentBodyType = bodyTypeStr[(int)rgd2d.Type];
				if (ImGui::BeginCombo("Body Type", currentBodyType))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentBodyType == bodyTypeStr[i];
						if (ImGui::Selectable(bodyTypeStr[i], isSelected))
						{
							currentBodyType = bodyTypeStr[i];
							rgd2d.Type = (Rigidbody2DComponent::BodyType)i;
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				//控制尺寸
				float sizex = rgd2d.size.x;
				float sizey = rgd2d.size.y;
				ImGui::DragFloat("SizeX", &sizex, 0.01f, 0.0001f, 0.5f);
				ImGui::DragFloat("SizeY", &sizey, 0.01f, 0.0001f, 0.5f);
				rgd2d.size.x = std::max(0.001f, sizex);
				rgd2d.size.y = std::max(0.001f, sizey);
				//旋转禁用
				ImGui::Checkbox("Fixed Rotation", &rgd2d.FixedRotation);

				//夹具参数控制
				ImGui::DragFloat("Density", &rgd2d.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &rgd2d.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &rgd2d.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &rgd2d.RestitutionThreshold, 0.01f, 0.0f);


				if (ImGui::BeginPopup("Rigidbody2D", ImGuiPopupFlags_NoOpenOverExistingPopup))
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity.RemoveComponent<Rigidbody2DComponent>();
					}
					ImGui::EndPopup();
				}
				ImGui::TreePop();
			}
			
		}

		if (entity.HasComponent<CScriptComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(CScriptComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "CScript");
			ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f);
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("CScript");
			}
			if (open)
			{
				auto& asc = entity.GetComponent<CScriptComponent>();
				ImGui::Text("C#:");
				std::string name = asc.ClassName;
				ImGui::SameLine();
				ImVec2 buttonsize = { ImGui::GetWindowWidth() - 100,20 };
				if (ImGui::Button(name.c_str(),buttonsize))
				{
					if (!asc.Path.empty())
					{
						std::string command = "start " + asc.Path;
						system(command.c_str());
					}
				}
				//拖拽源
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path aspath = std::filesystem::path(path);
						if (aspath.extension().string() == ".cs" )
						{
							asc.ClassName = aspath.filename().string();
							asc.Path = aspath.string();
						}
						else
						{
							LOG_INFO("脚本格式错误");
						}
					}
					ImGui::EndDragDropTarget();
				}
				if (ImGui::BeginPopup("CScript", ImGuiPopupFlags_NoOpenOverExistingPopup))
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity.RemoveComponent<CScriptComponent>();
					}
					ImGui::EndPopup();
				}
				ImGui::TreePop();
			}
		}
	}
	void ScenePanels::AddComponent(Entity entity)
	{
		if (ImGui::Button("AddComponent"))
			ImGui::OpenPopup("AddComponent");
		if (ImGui::BeginPopup("AddComponent"))
		{
			if (!entity.HasComponent<TagComponent>())
			{
				if (ImGui::MenuItem("TagComponent"))
				{
					entity.AddComponent<TagComponent>("Empty Entity");
				}
			}
			if (!entity.HasComponent<TransformComponent>())
			{
				if (ImGui::MenuItem("TransformComponent"))
				{
					entity.AddComponent<TransformComponent>();
				}
			}
			if (!entity.HasComponent<RenderQuadComponent>()&& !entity.HasComponent<RenderCircleComponent>())
			{

				if (ImGui::MenuItem("RenderQuadComponent"))
				{
					entity.AddComponent<RenderQuadComponent>();
				}
				if (ImGui::MenuItem("RenderCircleComponent"))
				{
					entity.AddComponent<RenderCircleComponent>();
				}

			}
			if (!entity.HasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("CameraComponent"))
				{
					entity.AddComponent<CameraComponent>();
				}
			}
			if (!entity.HasComponent<Rigidbody2DComponent>())
			{
				if (!isPlaying)
				{
					if (ImGui::MenuItem("Rigidbody2DComponent"))
					{

						entity.AddComponent<Rigidbody2DComponent>();
					}
				}
			}
			if (!entity.HasComponent<CScriptComponent>())
			{
				if (ImGui::MenuItem("CScriptComponent"))
				{
					entity.AddComponent<CScriptComponent>();
				}
			}
			ImGui::EndPopup();
		}
	}
}