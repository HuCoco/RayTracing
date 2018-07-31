#pragma once
#include <map>
#include <vector>
#include <ImGui/imgui.h>
class RenderUI
{
protected:
    RenderUI();
    ~RenderUI();
public:
    static RenderUI* GetInstance();
    void Initialize();
    void Finalize();
    void Update();

private:

    enum SETTING_PAGE_TYPE
    {
        RENDERING_PARAMETER_PAGE,
        PBR_VIEWER,
        RENDERING_RESULT_VIEWER,
        NUM_SETTING_PAGE_TYPE
    };
    const char* m_SettingPageTitle[NUM_SETTING_PAGE_TYPE] =
    {
        "Rendering Parameter Settings",
        "Physically Based Rendering Viewer",
        "Rendering Result Viewer",
    };
    const char* m_SettingPageSubTitle[NUM_SETTING_PAGE_TYPE] =
    {
        "Create New Render Scene Task",
        "Physically Based Rendering Viewer",
        "Display Rendering Result",
        
    };
    SETTING_PAGE_TYPE m_SelectedPage{ RENDERING_PARAMETER_PAGE };

    enum RENDERING_MODE
    {
        BLINN_PHONG,
        PHYSICALLY_BASED,
        NUM_RENDERING_MODE
    };
    const char* m_RenderModeTitle = "Rendering Mode";
    const char* m_RenderModeSubTitle[NUM_RENDERING_MODE] = 
    { 
        "Blinn-Phong",
        "Physically Based Rendering" 
    };
    RENDERING_MODE m_RenderMode{ BLINN_PHONG };

    enum ACCELERATION_MODE
    {
        NONE_ACCELERATION,
        CPU_MULTITHREAD,
        GPU_COMPUTE_SHADER,
        NUM_ACCELERATION_MODE
    };
    const char* m_AccelerationModeTitle = "Acceleration Modee";
    const char* m_AccelerationModeSubTitle[NUM_ACCELERATION_MODE] = 
    { 
        "None",
        "CPU Multithread",
        "Compute shader" 
    };
    ACCELERATION_MODE m_AccelerationMode{ NONE_ACCELERATION };

    enum SCENE_TYPE
    {
        SCENE_1,
        SCENE_2,
        SCENE_3,
        NUM_SCENE_TYPE
    };
    const char* m_SceneTypeTitle = "Target Scene";
    const char* m_SceneTypeSubTitle[NUM_SCENE_TYPE] =
    {
        "Scene 1",
        "Scene 2",
        "Scene 3"
    };
    SCENE_TYPE m_SceneType;
};


class ViewWindow
{
public:
    ViewWindow(const std::string& name);
    ~ViewWindow();
    void Update();
    bool IsActive();
    void SetActive(bool active) { m_IsActive = active; }
    const std::string& GetName() { return m_Name; }
    void SetImTextureID(ImTextureID id) { m_TexID = id; }
    inline int GetWidth() { return m_Width; }
    inline int GetHeight() { return m_Height; }
    void GetImTextureID(ImTextureID id) { m_TexID =  id; }
private:
    bool m_IsActive;
    std::string m_Name;
    int m_Width;
    int m_Height;
    ImTextureID m_TexID;
};


class ViewWindowManager
{
protected:
    ViewWindowManager();
    ~ViewWindowManager();
public:
    static ViewWindowManager* GetInstance();
    void CreateViewWindow(const char* name);
    void RemoveViewWindow(const char* name);
    void RemoveViewWindow(int index );
    void ActiveWindow(int index);
    void SetDefualtViewWindow(int index);
    ViewWindow* GetDefualtViewWindow();
    ViewWindow* GetViewWindow(int index);
    const char* GetDefualtViewWindowName();

    void Update();
    void* GetViewWindowNameList();
    int GetViewWindowCount();

    static bool ItemGetter(void* data, int idx, const char** out_str);
private:
    std::map<std::string, ViewWindow*> m_ViewWindows;
    std::vector<std::string> m_ViewWindowNameList;
    int m_DefualtViewWindow;
};