#include "RayTracingTaskManager.h"
#include <Core/Raytrace.h>
RayTracingTaskManager::RayTracingTaskManager()
{

}

RayTracingTaskManager::~RayTracingTaskManager()
{

}

void RayTracingTaskManager::Initialize(uint32_t numThreads)
{
    mQuit = false;
    mNumThreads = numThreads;
    mThread = new ThreadWindows[numThreads];
    ThreadInitializeDescription desc;
    desc.EntryFunc = DoTask;
    desc.Parameter = this;
    desc.Affinity = GE_THREAD_CORE_7 | GE_THREAD_CORE_6 | GE_THREAD_CORE_5 | GE_THREAD_CORE_4 | GE_THREAD_CORE_3 | GE_THREAD_CORE_2 | GE_THREAD_CORE_1 | GE_THREAD_CORE_0;
    desc.Priority = GE_THREAD_PRIORITY_NORMAL;
    desc.StackSize = GE_DEFUALT_THRED_STACK_SIZE;
    desc.StackAddress = nullptr;
    for (uint32_t i = 0; i < mNumThreads; ++i)
    {
        mThread[i].Initialize(desc);
    }
    mQueueMutex.Initialize(false);
}

void RayTracingTaskManager::Finalize()
{
    mQuit = true;
    for (uint32_t i = 0; i < mNumThreads; ++i)
    {
        mThread[i].Join();
        mThread[i].Finalize();
    }
}

void RayTracingTaskManager::PushTask(const RayTracingTaskData& data)
{
    mQueueMutex.Lock();
    mQueue.push(data);

    mQueueMutex.Unlock();
}

bool RayTracingTaskManager::PopTask(RayTracingTaskData& data)
{
    mQueueMutex.Lock();
    bool ret = false;
    if (!mQueue.empty())
    {
        data = mQueue.front();
        mQueue.pop();
        ret = true;
    }
    mQueueMutex.Unlock();
    return ret;
}

void RayTracingTaskManager::DoTask(void* param)
{
    RayTracingTaskManager* task_manager = (RayTracingTaskManager*)param;
    while (!task_manager->IsQuit())
    {
        RayTracingTaskData data;
        if (!task_manager->PopTask(data))
        {
            continue;
        }
        Ray ray = data.scene->camera.getRay(data.x, data.y);
        Color pixelColor = Raytrace::TraceRay(ray, *data.scene, 2, true);
        pixelColor.clamp();
        *data.output = pixelColor;
    }
}
