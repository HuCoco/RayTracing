#include "RenderUI.h"
#include <Core/Color.h>

RenderUI::RenderUI()
{

}

RenderUI::~RenderUI()
{

}

RenderUI* RenderUI::GetInstance()
{
    static RenderUI sInstance;
    return &sInstance;
}

void RenderUI::Initialize()
{

}

void RenderUI::Finalize()
{

}

void RenderUI::Update()
{

    ViewWindowManager::GetInstance()->Update();

    ImGui::SetWindowPos("Ray Tracing Option", ImVec2(0, 0));
    ImGui::SetWindowSize("Ray Tracing Option", ImVec2(ImGui::GetIO().DisplaySize.x / 4, ImGui::GetIO().DisplaySize.y));

    ImGui::Begin("Ray Tracing Option", nullptr, ImGuiWindowFlags_NoResize);
    {
        static int sCurrentWindow = 0;
        static char sViewWindowName[255];
        {
            ImGui::Text("View Window");
            ImGui::Separator();
           
            ImGui::InputText("", sViewWindowName, 255);
            ImGui::SameLine();
            if (ImGui::Button("Create"))
            {
                if (strlen(sViewWindowName))
                {
                    ViewWindowManager::GetInstance()->CreateViewWindow(sViewWindowName);
                    sCurrentWindow = ViewWindowManager::GetInstance()->GetViewWindowCount();
                    memset(sViewWindowName, 0, sizeof(sViewWindowName));
                }
            }

            ImGui::NewLine();
            ImGui::Text("Defualt View Window");
            ImGui::Separator();
            if (ViewWindowManager::GetInstance()->GetViewWindowCount())
            {
                ImGui::Combo("", &sCurrentWindow, ViewWindowManager::ItemGetter, ViewWindowManager::GetInstance()->GetViewWindowNameList(), ViewWindowManager::GetInstance()->GetViewWindowCount());
                if (ImGui::Button("Active"))
                {
                    ViewWindowManager::GetInstance()->ActiveWindow(sCurrentWindow);
                }
                ImGui::SameLine();
                if (ImGui::Button("Remove"))
                {
                    ViewWindowManager::GetInstance()->RemoveViewWindow(sCurrentWindow);
                    sCurrentWindow = 0;
                }
                ImGui::SameLine();
                if (ImGui::Button("Set Defualt"))
                {
                    ViewWindowManager::GetInstance()->SetDefualtViewWindow(sCurrentWindow);
                }
                ImGui::Text("Current Defualt View Window : [ %s ]", ViewWindowManager::GetInstance()->GetDefualtViewWindowName());
            }
            else
            {
                ImGui::TextColored(ImColor(1.0f, 0.0f, 0.0f, 1.0f), "Create a view window please.");
            }

            
        }


        {
            ImGui::NewLine();
            ImGui::Text("Function Select:");
            ImGui::Separator();

            ImGui::RadioButton(m_SettingPageSubTitle[RENDERING_PARAMETER_PAGE], reinterpret_cast<int*>(&m_SelectedPage), RENDERING_PARAMETER_PAGE);
            ImGui::RadioButton(m_SettingPageSubTitle[PBR_VIEWER], reinterpret_cast<int*>(&m_SelectedPage), PBR_VIEWER);
            ImGui::RadioButton(m_SettingPageSubTitle[RENDERING_RESULT_VIEWER], reinterpret_cast<int*>(&m_SelectedPage), RENDERING_RESULT_VIEWER);
        }


        ImGui::NewLine();

        ImGui::Text("Output View Window");
        ImGui::Separator();
        static int sOutputViewWindow = 0;
        if (ViewWindowManager::GetInstance()->GetViewWindowCount())
        {
            ImGui::Combo("", &sOutputViewWindow, ViewWindowManager::ItemGetter, ViewWindowManager::GetInstance()->GetViewWindowNameList(), ViewWindowManager::GetInstance()->GetViewWindowCount());
        }
        else
        {
            ImGui::TextColored(ImColor(1.0f, 0.0f, 0.0f, 1.0f), "Create a view window please.");
        }

        switch (m_SelectedPage)
        {
        case RenderUI::PBR_VIEWER:
            ImGui::Text(m_RenderModeTitle);
            ImGui::Separator();
            ImGui::RadioButton(m_RenderModeSubTitle[BLINN_PHONG], reinterpret_cast<int*>(&m_RenderMode), BLINN_PHONG);
            ImGui::RadioButton(m_RenderModeSubTitle[PHYSICALLY_BASED], reinterpret_cast<int*>(&m_RenderMode), PHYSICALLY_BASED);

            ImGui::Text("Material Settings");
            ImGui::Separator();
            if (m_RenderMode == BLINN_PHONG)
            {
                static Color sAmblent;
                static Color sReflection;
                static Color sDiffuse;
                static Color sSpecular;
                static float sShininess;

                ImGui::ColorEdit3("Abient", (float*)&sAmblent);
                ImGui::ColorEdit3("Reflection", (float*)&sReflection);
                ImGui::ColorEdit3("Diffuse", (float*)&sDiffuse);
                ImGui::ColorEdit3("Specular", (float*)&sSpecular);
                ImGui::SliderFloat("Shininess", &sShininess, 0.001f, 128.0f);
            }
            else if(m_RenderMode == PHYSICALLY_BASED)
            {
                static Color sAlbedo;
                static float sMetallic;
                static float sRoughness;

                ImGui::ColorEdit3("Albedo", (float*)&sAlbedo);
                ImGui::SliderFloat("Roughness", &sMetallic, 0.001f, 1.0f);
                ImGui::SliderFloat("Metallic", &sRoughness, 0.001f, 1.0f);
            }

            break;
        case RenderUI::RENDERING_PARAMETER_PAGE:
        {
            ImGui::Text(m_RenderModeTitle);
            ImGui::Separator();
            ImGui::RadioButton(m_RenderModeSubTitle[BLINN_PHONG], reinterpret_cast<int*>(&m_RenderMode), BLINN_PHONG);
            ImGui::RadioButton(m_RenderModeSubTitle[PHYSICALLY_BASED], reinterpret_cast<int*>(&m_RenderMode), PHYSICALLY_BASED);


            ImGui::Text(m_AccelerationModeTitle);
            ImGui::Separator();
            ImGui::RadioButton(m_AccelerationModeSubTitle[NONE_ACCELERATION], reinterpret_cast<int*>(&m_AccelerationMode), NONE_ACCELERATION);
            ImGui::RadioButton(m_AccelerationModeSubTitle[CPU_MULTITHREAD], reinterpret_cast<int*>(&m_AccelerationMode), CPU_MULTITHREAD);
            ImGui::RadioButton(m_AccelerationModeSubTitle[GPU_COMPUTE_SHADER], reinterpret_cast<int*>(&m_AccelerationMode), GPU_COMPUTE_SHADER);

            ImGui::Text(m_SceneTypeTitle);
            ImGui::Separator();
            ImGui::RadioButton(m_SceneTypeSubTitle[SCENE_1], reinterpret_cast<int*>(&m_SceneType), SCENE_1);
            ImGui::RadioButton(m_SceneTypeSubTitle[SCENE_2], reinterpret_cast<int*>(&m_SceneType), SCENE_2);
            ImGui::RadioButton(m_SceneTypeSubTitle[SCENE_3], reinterpret_cast<int*>(&m_SceneType), SCENE_3);

            if (ViewWindowManager::GetInstance()->GetViewWindowCount())
            {
                if (ImGui::Button("Go!"))
                {

                }
            }



            break;
        }

        case RenderUI::RENDERING_RESULT_VIEWER:
            break;
        default:
            break;
        }


    }
    ImGui::End();
}

ViewWindowManager::ViewWindowManager()
    : m_DefualtViewWindow(-1)
{

}

ViewWindowManager::~ViewWindowManager()
{

}

ViewWindowManager* ViewWindowManager::GetInstance()
{
    static ViewWindowManager sInstance;
    return &sInstance;
}

void ViewWindowManager::CreateViewWindow(const char* name)
{
    auto iter = m_ViewWindows.find(name);
    if (iter == m_ViewWindows.end())
    {
        ViewWindow* obj = new ViewWindow(name);
        m_ViewWindows.insert(std::make_pair(name, obj));
    }
}

void ViewWindowManager::RemoveViewWindow(const char* name)
{
    {
        auto iter = m_ViewWindows.find(name);
        if (iter != m_ViewWindows.end())
        {
            ViewWindow* obj = iter->second;
            m_ViewWindows.erase(iter);
            delete obj;
            obj = nullptr;
        }
    }
    {
        auto iter = std::find(m_ViewWindowNameList.begin(), m_ViewWindowNameList.end(), name);
        if (iter != m_ViewWindowNameList.end())
        {
            m_ViewWindowNameList.erase(iter);
        }
    }
}

void ViewWindowManager::RemoveViewWindow(int index)
{
    RemoveViewWindow(m_ViewWindowNameList[index].c_str());
    m_DefualtViewWindow = index == m_DefualtViewWindow ? -1 : m_DefualtViewWindow;
}

void ViewWindowManager::ActiveWindow(int index)
{
    auto iter = m_ViewWindows.find(m_ViewWindowNameList[index].c_str());
    if (iter != m_ViewWindows.end())
    {
        iter->second->SetActive(true);
    }
}

void ViewWindowManager::SetDefualtViewWindow(int index)
{
    m_DefualtViewWindow = index;
}

ViewWindow* ViewWindowManager::GetDefualtViewWindow()
{
    if (m_DefualtViewWindow != -1)
    {
        auto iter = m_ViewWindows.find(m_ViewWindowNameList[m_DefualtViewWindow].c_str());
        if (iter != m_ViewWindows.end())
        {
            return iter->second;
        }
    }

    return nullptr;
}

ViewWindow* ViewWindowManager::GetViewWindow(int index)
{
    auto iter = m_ViewWindows.find(m_ViewWindowNameList[index].c_str());
    if (iter != m_ViewWindows.end())
    {
        return iter->second;
    }
    return nullptr;
}

const char* ViewWindowManager::GetDefualtViewWindowName()
{
    auto window = GetDefualtViewWindow();
    if (window)
    {
        return window->GetName().c_str();
    }
    return "Unkown";
}

void ViewWindowManager::Update()
{
    m_ViewWindowNameList.clear();
    for (auto iter : m_ViewWindows)
    {
        m_ViewWindowNameList.push_back(iter.first);
        if (iter.second->IsActive())
        {
            iter.second->Update();
        }
    }
}

void* ViewWindowManager::GetViewWindowNameList()
{
    return &m_ViewWindowNameList;
}

int ViewWindowManager::GetViewWindowCount()
{
    return m_ViewWindowNameList.size();
}

bool ViewWindowManager::ItemGetter(void* data, int idx, const char** out_str)
{
    std::vector<std::string>& list = *reinterpret_cast<std::vector<std::string>*>(data);
    *out_str = list[idx].c_str();
    return true;
}

ViewWindow::ViewWindow(const std::string& name)
    : m_Name(name)
    , m_IsActive(false)
    , m_Width(640)
    , m_Height(480)
{

}

ViewWindow::~ViewWindow()
{

}

void ViewWindow::Update()
{
    ImGui::Begin(m_Name.c_str(), &m_IsActive, ImGuiWindowFlags_AlwaysAutoResize);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::Image(m_TexID, ImVec2(m_Width, m_Height), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        float region_sz = 32.0f;
        float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
        if (region_x < 0.0f)
            region_x = 0.0f;
        else if (region_x > m_Width - region_sz)
            region_x = m_Width - region_sz;
        float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
        if (region_y < 0.0f)
            region_y = 0.0f;
        else if (region_y > m_Height - region_sz)
            region_y = m_Height - region_sz;
        float zoom = 4.0f;
        //ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
        //ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);

        ImVec2 uv0 = ImVec2((region_x) / m_Width, (region_y) / m_Height);
        ImVec2 uv1 = ImVec2((region_x + region_sz) / m_Width, (region_y + region_sz) / m_Height);
        //ImGui::Text("uv: (%.2f, %.2f)", uv0.x, uv0.y);
        ImGui::Image(m_TexID, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
        ImGui::EndTooltip();
    }
    ImGui::End();
}

bool ViewWindow::IsActive()
{
    return m_IsActive;
}


