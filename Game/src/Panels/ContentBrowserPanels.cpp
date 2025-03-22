#include"ContentBrowserPanels.h"

#include"imgui.h"
namespace ENGINE
{
	static const std::filesystem::path  m_AssetsPath = "assets";

	ContentBrowserPanels::ContentBrowserPanels()
		:m_CurrentPath(m_AssetsPath)
	{
		m_DirectoryIcon =Texture2D::Create("Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon =Texture2D::Create("Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanels::OnImGuiRender()
	{
		ImGui::Begin("ContextBrowser");
		if (m_CurrentPath != m_AssetsPath)
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentPath = m_CurrentPath.parent_path();
			}
		}
		//划分布局
		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& it : std::filesystem::directory_iterator(m_CurrentPath))
		{
			const auto &path=it.path();
			//相对路径
			std::string filenameStr = path.filename().string();
			//载入纹理
			Ref<Texture2D>icon = it.is_directory() ? m_DirectoryIcon : m_FileIcon;

			//对每一个拖拽源进行隔离
			ImGui::PushID(filenameStr.c_str());
			//绘制图形
			ImGui::ImageButton((ImTextureID)icon->GetID(), { thumbnailSize,thumbnailSize }, { 0,1 }, { 1,0 });

			//增加拖拽源
			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relative(path);
				const wchar_t* itemPath = relative.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}
			
			//选择
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (it.is_directory())
					m_CurrentPath /= path.filename();
			}
			//添加名称
			ImGui::TextWrapped(filenameStr.c_str());
			//绘制下一列
			ImGui::NextColumn();

			ImGui::PopID();
		}

		//结束列绘制模式
		ImGui::Columns(1);

		//控制大小
		ImGui::SliderFloat("Size", &thumbnailSize, 16, 128);
		ImGui::SliderFloat("Padding", &padding, 0, 16);

		ImGui::End();
	}
}