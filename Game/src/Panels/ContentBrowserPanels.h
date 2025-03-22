#pragma once
#include<filesystem>

#include"Engine/Render/Texture.h"
namespace ENGINE
{
	class ContentBrowserPanels
	{
	public:
		ContentBrowserPanels();
		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentPath;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D>m_FileIcon;
	};
}