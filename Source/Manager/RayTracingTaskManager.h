#pragma once
#include <Core/Scene.h>
#include <Platform/Thread/Thread.h>
#include <queue>

typedef void(*RenderPixelCallback)();

struct RayTracingTaskData
{
    float x{ 0.0f };
    float y{ 0.0f };
    Scene* scene{ nullptr };
    Color* output{ nullptr };
    RenderPixelCallback callback{ nullptr };
};

class RayTracingTaskManager
{
public:
    RayTracingTaskManager();
    ~RayTracingTaskManager();
    void Initialize(uint32_t numThreads);
    void Finalize();
    void PushTask(const RayTracingTaskData& data);
    bool PopTask(RayTracingTaskData& data);
    inline bool IsQuit() { return mQuit; }
private:
    static void DoTask(void* param);

private:
    ThreadWindows* mThread;
    uint32_t mNumThreads;
    std::queue<RayTracingTaskData> mQueue;
    MutexWindows mQueueMutex;
    CriticalSectionWindows mCriticalSection;
    bool mQuit;
};