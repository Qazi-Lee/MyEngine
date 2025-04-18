#include"head/RunGame.h"

using namespace ENGINE;

static const std::filesystem::path  m_ScenePath = "assets/Scene/Test";
RunGame::RunGame()
{
	m_SceneManager = std::make_shared<SceneManager>();
	for (auto& it : std::filesystem::directory_iterator(m_ScenePath))
	{
		const auto& path = it.path();
		m_SceneManager->AddScene(path);
	}
	ScriptEngine::SetSceneManager(m_SceneManager);
}

void RunGame::OnAttach()
{
	BeginGame();
	m_ActiveScene = m_SceneManager->GetScene();
}

void RunGame::OnDetach()
{
	EndGame();
}

void RunGame::OnUpdate(ENGINE::Time t)
{
	Render2D::ResetStats();
	RenderCommand::SetClearColor({ 0.0f,0.0f,0.0f,1.0f });
	RenderCommand::Clear();
	m_ActiveScene = m_SceneManager->GetScene();
	m_ActiveScene->OnUpdate(t);
}

void RunGame::OnEvent(ENGINE::Event& e)
{
}

void RunGame::OnImGuiRender()
{

}

void RunGame::BeginGame()
{
	for (auto it = m_SceneManager->SceneMap.begin(); it != m_SceneManager->SceneMap.end(); it++)
	{
		it->first->OnRuntimeStart();
	}
}

void RunGame::EndGame()
{
	for (auto it = m_SceneManager->SceneMap.begin(); it != m_SceneManager->SceneMap.end(); it++)
	{
		it->first->OnRuntimeEnd();
	}
}
