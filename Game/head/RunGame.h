#pragma once
#include"Engine.h"

class RunGame :public ENGINE::Layer
{
public:
	RunGame();
	void OnAttach()override;
	void OnDetach()override;
	void OnUpdate(ENGINE::Time t)override;
	void OnEvent(ENGINE::Event& e)override;
	void OnImGuiRender()override;
private:
	ENGINE::Ref<ENGINE::Scene>m_ActiveScene;
	ENGINE::Ref<ENGINE::SceneManager> m_SceneManager;
};