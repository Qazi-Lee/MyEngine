#pragma once
#include"Engine.h"
#include"Engine/Render/Texture.h"
#include"Panels/ScenePanels.h"
#include"Panels/ContentBrowserPanels.h"
#include"Engine/Render/EditorCamera.h"
class Layer2D :public ENGINE::Layer
{
public:
	Layer2D();
	void OnAttach()override;
	void OnDetach()override;
	void OnUpdate(ENGINE::Time t)override;
	void OnEvent(ENGINE::Event&e)override;
	void OnImGuiRender()override;
private:
	bool OnMousePressed(ENGINE::MousePressedEvent& e);
	bool OnKeyPreesed(ENGINE::KeyPressedEvent& e);
private:
	void NewScene();
	void OpenScene();
	void OpenScene(std::filesystem::path filepath);
	void SaveScene();
	void OnScenePlay();
	void OnSceneStop();

	void OnOverlayRender();
private:
	ENGINE::GraphicsCameraController m_CameraController;
	ENGINE::Ref<ENGINE::Texture2D>playIcon;
	ENGINE::Ref<ENGINE::Texture2D>stopIcon;
	ENGINE::Ref<ENGINE::SubTexture2D> subtexture;
	ENGINE::Ref<ENGINE::FrameBuffer>framebuffer;
	ENGINE::Ref<ENGINE::Scene>m_ActiveScene;
	ENGINE::Ref<ENGINE::Scene>m_EditorScene;
	ENGINE::Ref<ENGINE::ScenePanels>m_ScenePanels;
	ENGINE::Ref<ENGINE::ContentBrowserPanels>m_ContentBrowserPanels;
	ENGINE::EditorCamera m_EditorCamera;


	ENGINE::Entity camera;
	ENGINE::Entity m_HoveredEntity=ENGINE::Entity();
	bool PrimaryCamera = false;
	bool viewportfocused = false;
	bool viewporthovered = false;
	bool ShowPhysicsColliders = true;
	int ImGuizmoType = -1;
	
	glm::vec2 ViewportBounds[2];
	glm::vec4 color = glm::vec4(1.0f);
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	glm::vec2 size = glm::vec2(1.0f, 1.0f);
	glm::vec2 viewportsize=glm::vec2(0.0f);

	enum class SceneState
	{
		Edit = 0,
		Play
	};
	SceneState m_SceneState=SceneState::Edit;
};