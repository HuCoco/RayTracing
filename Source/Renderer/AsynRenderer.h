#pragma once
#include <Platform/Platform.h>
#include <Platform/Thread/Thread.h>
#include <atomic>

typedef void(*FinishTaskCallback)(void);
class Image;
namespace PBR
{
    struct Scene;
    struct AsynSceneRenderTask
    {
        Scene* scene;
        Image* output;
        uint32_t width;
        uint32_t height;
        uint32_t beginHeight;
        uint32_t endHeight;
        uint32_t processerId;
        FinishTaskCallback callback;
        uint32_t numSample;
        uint32_t reflectLevels;
        bool hasShadow;

    };
    class AsynRenderer
    {
    protected:
        AsynRenderer();
        ~AsynRenderer();
    public:
        static AsynRenderer* GetInstance();
        void Initialize();
        void Finalize();
        void RenderScene(Scene* scene, Image* output, uint32_t numProcesser, uint32_t hasShadow, uint32_t reflectLevels, uint32_t numSample);
        bool IsFinishAllTask();
        void ClearTasks();
        static void DoTask(void* param);
        static void StartTask();
        static void FinishTask();

    private:
        uint32_t mMaxProcesser;
        uint32_t mActivateThreads;
        ThreadWindows* mThreads;
        AsynSceneRenderTask* mTaskList;
        static std::atomic<uint32_t> sNumFinishTasks;
    };
}

namespace Phong
{
    struct Scene;
    struct AsynSceneRenderTask
    {
        Scene* scene;
        Image* output;
        uint32_t width;
        uint32_t height;
        uint32_t beginHeight;
        uint32_t endHeight;
        uint32_t processerId;
        FinishTaskCallback callback;
        uint32_t numSample;
        uint32_t reflectLevels;
        bool hasShadow;

    };
    class AsynRenderer
    {
    protected:
        AsynRenderer();
        ~AsynRenderer();
    public:
        static AsynRenderer* GetInstance();
        void Initialize();
        void Finalize();
        void RenderScene(Scene* scene, Image* output, uint32_t numProcesser, uint32_t hasShadow, uint32_t reflectLevels, uint32_t numSample);
        bool IsFinishAllTask();
        void ClearTasks();
        static void DoTask(void* param);
        static void StartTask();
        static void FinishTask();

    private:
        uint32_t mMaxProcesser;
        uint32_t mActivateThreads;
        ThreadWindows* mThreads;
        AsynSceneRenderTask* mTaskList;
        static std::atomic<uint32_t> sNumFinishTasks;
    };
}







