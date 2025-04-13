#include"head/Layer2D.h"
#include<glm/gtc/type_ptr.hpp>
#include"Engine/Render/Render2D.h"
#include"imgui.h"
#include"Engine/Utils/PlatformUtils.h"
#include"ImGuizmo.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

using namespace ENGINE;

static ImVec2 WorldToScreen(const glm::vec3& worldPos, const glm::mat4& view, const glm::mat4& proj) {
    glm::vec4 clipPos = proj * view * glm::vec4(worldPos, 1.0f);
    clipPos /= clipPos.w; // 透视除法
    return ImVec2(
        (clipPos.x + 1.0f) * 0.5f * ImGui::GetIO().DisplaySize.x,
        (1.0f - clipPos.y) * 0.5f * ImGui::GetIO().DisplaySize.y
    );
}

Layer2D::Layer2D()
	:Layer("2D"), m_CameraController(1280 / 720)
{

}

static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
{
    // From glm::decompose in matrix_decompose.inl

    using namespace glm;
    using T = float;

    mat4 LocalMatrix(transform);

    // Normalize the matrix.
    if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
        return false;

    // First, isolate perspective.  This is the messiest.
    if (
        epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
    {
        // Clear the perspective partition
        LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
        LocalMatrix[3][3] = static_cast<T>(1);
    }

    // Next take care of translation (easy).
    translation = vec3(LocalMatrix[3]);
    LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

    vec3 Row[3], Pdum3;

    // Now get scale and shear.
    for (length_t i = 0; i < 3; ++i)
        for (length_t j = 0; j < 3; ++j)
            Row[i][j] = LocalMatrix[i][j];

    // Compute X scale factor and normalize first row.
    scale.x = length(Row[0]);
    Row[0] = detail::scale(Row[0], static_cast<T>(1));
    scale.y = length(Row[1]);
    Row[1] = detail::scale(Row[1], static_cast<T>(1));
    scale.z = length(Row[2]);
    Row[2] = detail::scale(Row[2], static_cast<T>(1));

    // At this point, the matrix (in rows[]) is orthonormal.
    // Check for a coordinate system flip.  If the determinant
    // is -1, then negate the matrix and the scaling factors.
#if 0
    Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
    if (dot(Row[0], Pdum3) < 0)
    {
        for (length_t i = 0; i < 3; i++)
        {
            scale[i] *= static_cast<T>(-1);
            Row[i] *= static_cast<T>(-1);
        }
    }
#endif

    rotation.y = asin(-Row[0][2]);
    if (cos(rotation.y) != 0) {
        rotation.x = atan2(Row[1][2], Row[2][2]);
        rotation.z = atan2(Row[0][1], Row[0][0]);
    }
    else {
        rotation.x = atan2(-Row[2][0], Row[1][1]);
        rotation.z = 0;
    }


    return true;
}

void Layer2D::OnAttach()
{
	playIcon = Texture2D::Create("Icons/PlayButton.png");
    stopIcon= Texture2D::Create("Icons/StopButton.png");
    //subtexture = SubTexture2D::Create(sprite, { 0,1 }, { 101.5f,93.f });

    FrameBufferData fbd;
    fbd.width = 1280; fbd.height = 720;
    framebuffer = FrameBuffer::Create(fbd);

    m_SceneManager = SceneManager(std::make_shared<Scene>());
    m_ActiveScene = m_SceneManager.GetScene();
    m_ScenePanels = std::make_shared<ScenePanels>(m_ActiveScene);
    m_ContentBrowserPanels = std::make_shared<ContentBrowserPanels>();

}


void Layer2D::OnDetach()
{

}

void Layer2D::OnUpdate(Time t)
{
    //  m_Scene->ShowEntitySize();

    if (viewportfocused && m_SceneState == SceneState::Edit)
    {
        m_CameraController.OnUpdate(t);
        m_EditorCamera.OnUpdate(t);
    }
    Render2D::ResetStats();
    framebuffer->Bind();
    RenderCommand::SetClearColor({ 0.0f,0.0f,0.0f,1.0f });
    RenderCommand::Clear();

    switch (m_SceneState)
    {
        case SceneState::Edit:
        {
            m_ActiveScene->OnUpdateEditor(t, m_EditorCamera); break;
        }
        
        case SceneState::Play:
        {
            m_ActiveScene->OnUpdate(t); break;
        }
    }
     //鼠标拾取
                                                                                                                                                                                                                          auto [mx, my] = ImGui::GetMousePos();
     mx -= ViewportBounds[0].x;
     my -= ViewportBounds[0].y;
     glm::vec2 viewportsize = ViewportBounds[1] - ViewportBounds[0];
     //反转y坐标w
     my = viewportsize.y - my;
     if(mx>=0&&my>=0&&mx<=viewportsize.x&&my<=viewportsize.y)
     {
         int data = -1;
         data = framebuffer->ReadPixel(1, (int)mx,(int)my)-1;
         m_HoveredEntity = data < 0 ? Entity() : Entity((entt::entity)data, m_ActiveScene.get());
        // std::cout <<data<<std::endl;
     }
     //可视化物理碰撞器
     OnOverlayRender();
    framebuffer->UnBind();
}

void Layer2D::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
    if (viewporthovered&&m_SceneState==SceneState::Edit)
    {
        m_EditorCamera.OnEvent(e);
    }
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MousePressedEvent>(BIND_EVENT_FN(Layer2D::OnMousePressed));
    dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Layer2D::OnKeyPreesed));
}

void Layer2D::OnImGuiRender()
{
    static bool p_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    //场景控制渲染
    m_ScenePanels->OnImGuiRender();
    //资产浏览器渲染
    m_ContentBrowserPanels->OnImGuiRender();

    ImGui::Begin("Setting");
    ImGui::Checkbox("ShowPhysicsColliders", &ShowPhysicsColliders);
    auto stats = Render2D::GetStats();
    ImGui::Text("DrawCall:%d", stats.DrawCalls);
    ImGui::Text("Vertex:%d", stats.GetQuadVertexCount());
    ImGui::Text("Index:%d", stats.GetQuadIndexCount());
    std::string se = std::string();
    if (m_HoveredEntity)
    {
        se = m_HoveredEntity.GetComponent<TagComponent>().tag;
    }
    ImGui::Text("SelectedEntity:%s", se.c_str());
    ImGui::End();


    ImGui::Begin("SceneController");
    for (int i = 0; i < m_SceneManager.size(); i++)
    {
        ImGui::PushID(i);

        std::string name = m_SceneManager.SceneMap[m_SceneManager.GetScene(i)];
        char* buffer =(char*)name.c_str();
        if (ImGui::InputText("", buffer, 128))
        {
            if (ImGui::IsItemEdited()) {
                std::string rename(buffer);
                m_SceneManager.RenameScene(rename);
            }
        }

        ImGui::SameLine(0, ImGui::GetWindowSize().x / 7);
        bool isSelected = m_SceneManager.GetScene(i) == m_SceneManager.GetScene();
        if (isSelected)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.9f, 0.2f, 0.2f, 1.0f)); // 高亮
        }
        if (ImGui::Button("!", ImVec2(ImGui::GetWindowSize().x / 7, 20)))
        {
            if (m_SceneState != SceneState::Edit)
                OnSceneStop();
            m_SceneManager.SetScene(i);
            m_ActiveScene = m_SceneManager.GetScene();
            m_ScenePanels->SetScene(m_ActiveScene);
        }
        if (isSelected)
        {
            ImGui::PopStyleColor(1);
        }

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Scene"))
            {
                if (m_SceneManager.size() > 1)
                {
                    m_SceneManager.DeleteScene(i);
                    m_SceneManager.SetScene(0);
                    m_ActiveScene = m_SceneManager.GetScene();
                }
            }
            ImGui::EndPopup();
        }
        ImGui::PopID();
    }
    if (ImGui::Button("Add New Scene",ImVec2(ImGui::GetWindowSize().x,20)))
    {
        m_SceneManager.AddScene();
    }
    if (ImGui::Button("Load now Scene", ImVec2(ImGui::GetWindowSize().x, 20)))
    {
        if (m_SceneState != SceneState::Edit)
            OnSceneStop();
        //传入参数说明：前面字符表示传入参数类型名字以及类型定义 \0之后的参数进行筛选
        std::filesystem::path filepath = FileDialogs::OpenFile("Engine Scene (*.scene) \0*.scene\0");
        if (!filepath.empty())
        {
            if (filepath.extension().string() != ".scene")
            {
                LOG_CORE_ERROR("Could not load {0} - not a scene file", filepath.filename().string());
            }
            else
            {
                m_SceneManager.AddScene(filepath);
            }
        }
    }
    ImGui::End();


    ImGui::Begin("Viewport");

    //得到视口位置
    auto viewportoffset = ImGui::GetCursorPos();//标题栏偏移量
    auto viewportpos = ImGui::GetWindowPos();
    viewportpos.x += viewportoffset.x;
    viewportpos.y += viewportoffset.y;
    //std::cout << viewportoffset.x <<" " << viewportoffset.y << std::endl;
    ViewportBounds[0] = { viewportpos.x,viewportpos.y };
    ViewportBounds[1] = { viewportpos.x + ImGui::GetContentRegionAvail().x,viewportpos.y + ImGui::GetContentRegionAvail().y};
    viewportfocused = ImGui::IsWindowFocused();
    viewporthovered=ImGui::IsWindowHovered();
    ImVec2 fbd = { (float)framebuffer->GetData().width,(float)framebuffer->GetData().height };
    viewportsize = { ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y };
    if (fbd.x != viewportsize.x || fbd.y != viewportsize.y)
    {
        framebuffer->Resize((uint32_t)viewportsize.x,(uint32_t)viewportsize.y);
        //改变所有相机的投影矩阵
        m_ActiveScene->OnViewportResize((uint32_t)viewportsize.x, (uint32_t)viewportsize.y);
        m_EditorCamera.SetViewportSize(viewportsize.x, viewportsize.y);
       //m_CameraController.Resize(viewportsize.x, viewportsize.y);
    }
    uint32_t fb = framebuffer->GetColorAttachmentID();
    ImGui::Image((void*)fb, ImVec2(viewportsize.x, viewportsize.y),ImVec2(0, 1), ImVec2(1, 0));

    //得到拖拽对象
    if (ImGui::BeginDragDropTarget()&&m_SceneState==SceneState::Edit) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
        {
            const wchar_t* path = (const wchar_t*)payload->Data;
            
            OpenScene(path);
       }
        ImGui::EndDragDropTarget();
    }
    //ImGuizmo
    Entity select = m_ScenePanels->GetSelectEntity();
    if (select&&ImGuizmoType!=-1&&m_SceneState==SceneState::Edit)
    {
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
            ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
        //得到相机
        glm::mat4 view = m_EditorCamera.GetViewMatrix();
        glm::mat4 projection = m_EditorCamera.GetProjection();

        //得到实体矩阵
        auto& tc = select.GetComponent<TransformComponent>();
        auto transform = tc.GetTransform();

        ImGuizmo::Manipulate(
            glm::value_ptr(view),      // 视图矩阵
            glm::value_ptr(projection),// 投影矩阵
            (ImGuizmo::OPERATION)ImGuizmoType,               // 操作类型
            ImGuizmo::LOCAL,                    // 变换空间
            glm::value_ptr(transform),     // 被操作的矩阵
            nullptr,                         // 快照矩阵（未使用）
            nullptr                          // 增量快照（未使用）
        );

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 translate, rotation, scale;

            DecomposeTransform(transform, translate, rotation, scale);

            tc.Translate = translate;
            tc.Rotation = rotation;
            tc.Scale = scale;
        }
    }

    //TODO:网格(先完成2D和3D切换)

    ImGui::End();

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            //TODO:加入快捷键
            if (ImGui::MenuItem("New"))
            {
                NewScene();
            }
            if (ImGui::MenuItem("Load"))
            {
                OpenScene();
            }
            if (ImGui::MenuItem("Save"))
            {
                SaveScene();
            }

            if (ImGui::MenuItem("Close"))
                Application::GetApp().Close();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    //开始按钮
    {
        ImGui::Begin("##toolbar",nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
       // ImGui::Begin("##toolbar");
        Ref<Texture2D>icon = m_SceneState == SceneState::Edit ? playIcon : stopIcon;
        float size = ImGui::GetContentRegionAvail().y;
        //居中
        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.5 - size * 0.5);
        if (ImGui::ImageButton((ImTextureID)icon->GetID(), { size,size }, { 0,1 }, { 1,0 }))
        {
            if (m_SceneState == SceneState::Edit)
                OnScenePlay();
            else if (m_SceneState == SceneState::Play)
                OnSceneStop();
        }
        ImGui::End();
    }
    ImGui::End();
}


bool Layer2D::OnMousePressed(MousePressedEvent& e)
{
    if (e.GetButton() == ME_MOUSE_BUTTON_1)
    {
        if (viewporthovered && !ImGuizmo::IsOver())
        {
            m_ScenePanels->SetSelectEntity(m_HoveredEntity);
        }
    }
    return false;
}

bool Layer2D::OnKeyPreesed(ENGINE::KeyPressedEvent& e)
{
    bool control = Input::IsKeyPress(ME_KEY_LEFT_CONTROL) || Input::IsKeyPress(ME_KEY_RIGHT_CONTROL);
    if (control&&m_SceneState==SceneState::Edit)
    {
        switch (e.GetKeyCode())
        {
        case ME_KEY_Q:
            ImGuizmoType = ImGuizmoType == ImGuizmo::TRANSLATE ? -1 : ImGuizmo::TRANSLATE;
            break;
        case ME_KEY_W:
            ImGuizmoType = ImGuizmoType == ImGuizmo::ROTATE ? -1 : ImGuizmo::ROTATE;
            break;
        case ME_KEY_E:
            ImGuizmoType = ImGuizmoType == ImGuizmo::SCALE ? -1 : ImGuizmo::SCALE;
            break;
        }
    }
    return false;
}

void Layer2D::NewScene()
{
    if (m_SceneState != SceneState::Edit)
        OnSceneStop();
    m_SceneManager.ResetScene();
    m_ActiveScene = m_SceneManager.GetScene();
    m_ActiveScene->OnViewportResize(viewportsize.x, viewportsize.y);
    m_ScenePanels->SetScene(m_ActiveScene);
}

void Layer2D::OpenScene()
{
    if (m_SceneState != SceneState::Edit)
        OnSceneStop();
    //传入参数说明：前面字符表示传入参数类型名字以及类型定义 \0之后的参数进行筛选
    std::string filepath = FileDialogs::OpenFile("Engine Scene (*.scene) \0*.scene\0");
    if (!filepath.empty())
    {
        OpenScene(filepath);
    }
}

void Layer2D::OpenScene(std::filesystem::path filepath)
{
    if (filepath.extension().string() != ".scene")
    {
        LOG_CORE_ERROR("Could not load {0} - not a scene file", filepath.filename().string());
        return;
    }
    m_SceneManager.ResetScene(filepath.stem().string());
    SceneSerialization m_SceneSerialization(m_SceneManager.GetScene());
    m_SceneSerialization.Deserialize(filepath.string());

    m_ActiveScene = m_SceneManager.GetScene();
    m_ScenePanels->SetScene(m_ActiveScene);

    m_ActiveScene->OnViewportResize(viewportsize.x, viewportsize.y);
}

void Layer2D::SaveScene()
{
    std::string filepath = FileDialogs::SaveFile("Engine Scene (*.scene) \0*.scene\0");
    if (!filepath.empty())
    {
        SceneSerialization m_SceneSerialization(m_SceneManager.GetScene());
        m_SceneSerialization.Serialize(filepath);
    }
}

void Layer2D::OnScenePlay()
{
    m_SceneState = SceneState::Play;
    m_ActiveScene = Scene::Copy(m_SceneManager.GetScene());
    m_ActiveScene->OnRuntimeStart();
    m_ScenePanels->SetSceneState(1);
    m_ScenePanels->SetScene(m_ActiveScene);
}

void Layer2D::OnSceneStop()
{
    m_SceneState = SceneState::Edit;
    m_ActiveScene->OnRuntimeEnd();
    m_ActiveScene = m_SceneManager.GetScene();
    m_ScenePanels->SetSceneState(0);
    m_ScenePanels->SetScene(m_ActiveScene);
}

void Layer2D::OnOverlayRender()
{
    if (ShowPhysicsColliders)
    {
        if (m_SceneState == SceneState::Edit)
        {
            Render2D::BeginScene(m_EditorCamera);
        }
        else if (m_SceneState == SceneState::Play)
        {
            auto view = m_ActiveScene->GetEntityWithComponent<TransformComponent, CameraComponent>();
            bool has = false;
            for (auto e : view)
            {
                auto& cc = view.get<CameraComponent>(e);
                auto& trans = view.get<TransformComponent>(e);
                if (cc.Primary)
                {
                    Render2D::BeginScene(cc.camera, trans.GetTransform());
                    has = true;
                    break;
                }
            }
            if (!has)
                return;
        }
        auto view = m_ActiveScene->GetEntityWithComponent<TransformComponent, Rigidbody2DComponent>();
        for (auto entityid : view)
        {
            //判断是方形还是圆形
            Entity entity = { entityid,m_ActiveScene.get() };
            auto trans = entity.GetComponent<TransformComponent>();
            auto rg2d = entity.GetComponent<Rigidbody2DComponent>();
            //控制尺寸
            trans.Scale.x *= 2.0 * rg2d.size.x;
            trans.Scale.y *= 2.0 * rg2d.size.y;
            //增加悬浮
            trans.Translate = glm::vec3(trans.Translate.x, trans.Translate.y, trans.Translate.z + 0.01f);
            glm::mat4 rotation = glm::toMat4(glm::quat(trans.Rotation));
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), trans.Translate)
                * rotation
                * glm::scale(glm::mat4(1.0f), trans.Scale);

            glm::vec4 color = { 0.0,1.0,0.0,1.0 };
            if (entity.HasComponent<RenderQuadComponent>())
            {
                Render2D::DrawRect(transform, color);

            }
            else if (entity.HasComponent<RenderCircleComponent>())
            {
                Render2D::DrawCircle(transform, color, 0.01f);
            }
        }
        Render2D::EndScene();
    }
}
