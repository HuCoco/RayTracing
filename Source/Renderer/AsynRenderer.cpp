#include "AsynRenderer.h"
#include <Core/Scene.h>
#include <Core/Image.h>
#include <Math/Math.h>
#include <Core/Raytrace.h>

namespace PBR
{
    std::atomic<uint32_t> AsynRenderer::sNumFinishTasks = 0;

    AsynRenderer::AsynRenderer()
    {

    }

    AsynRenderer::~AsynRenderer()
    {

    }

    AsynRenderer* AsynRenderer::GetInstance()
    {
        static AsynRenderer sInstance;
        return &sInstance;
    }

    void AsynRenderer::Initialize()
    {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        mMaxProcesser = info.dwNumberOfProcessors;
        mThreads = new ThreadWindows[16];
        mTaskList = new AsynSceneRenderTask[16];
    }

    void AsynRenderer::Finalize()
    {
        if (mTaskList)
        {
            ClearTasks();
            delete[] mThreads;
            mThreads = nullptr;
        }

    }


    void AsynRenderer::RenderScene(PBR::Scene* scene, Image* output, uint32_t numProcesser, uint32_t hasShadow, uint32_t reflectLevels, uint32_t numSample)
    {
        if (mActivateThreads != 0)
        {
            return;
        }
        //Math::clamp(numProcesser, 1u, mMaxProcesser);
        uint32_t width = scene->camera.getImageWidth();
        uint32_t height = scene->camera.getImageHeight();

        uint32_t heightPerTask = height / numProcesser;
        mActivateThreads = numProcesser;
        StartTask();
        for (uint32_t i = 0; i < numProcesser; i++)
        {
            mTaskList[i].width = width;
            mTaskList[i].height = height;
            mTaskList[i].beginHeight = heightPerTask * i;
            mTaskList[i].endHeight = ((i < (numProcesser - 1)) ? heightPerTask * (i + 1) : height);
            mTaskList[i].scene = scene;
            mTaskList[i].output = output;
            mTaskList[i].processerId = i;
            mTaskList[i].callback = FinishTask;
            mTaskList[i].numSample = numSample;
            mTaskList[i].reflectLevels = reflectLevels;
            mTaskList[i].hasShadow = hasShadow;

            ThreadInitializeDescription desc;
            desc.Affinity = 1 << i;
            desc.EntryFunc = DoTask;
            desc.Parameter = &mTaskList[i];
            desc.Priority = GE_THREAD_PRIORITY_NORMAL;
            desc.StackSize = GE_DEFUALT_THRED_STACK_SIZE;
            desc.StackAddress = nullptr;
            mThreads[i].Initialize(desc);
        }

    }

    bool AsynRenderer::IsFinishAllTask()
    {
        return (sNumFinishTasks == mActivateThreads) && (mActivateThreads != 0);
    }

    void AsynRenderer::ClearTasks()
    {
        for (uint32_t i = 0; i < mActivateThreads; i++)
        {
            mThreads[i].Join();
            mThreads[i].Finalize();
        }
        memset(mTaskList, 0, sizeof(mTaskList) * mActivateThreads);
        mActivateThreads = 0;
    }

    void AsynRenderer::DoTask(void* param)
    {
        AsynSceneRenderTask* task = (AsynSceneRenderTask*)param;
        for (uint32_t y = task->beginHeight; y < task->endHeight; y++)
        {
            for (uint32_t x = 0; x < task->width; x++)
            {
                Color pixelColor = Color(0, 0, 0);
                for (uint32_t i = 0; i < task->numSample; i++)
                {
                    float xx;
                    float yy;
                    Math::Hammersley(i, task->numSample, &xx, &yy);
                    double pixelPosX = x + xx;
                    double pixelPosY = y + yy;
                    Ray ray = task->scene->camera.getRay(pixelPosX, pixelPosY);
                    pixelColor += Raytrace::TraceRay(ray, *task->scene, task->reflectLevels, task->hasShadow);
                }
                pixelColor /= (float)task->numSample;
                pixelColor.clamp();
                task->output->setPixel(x, y, pixelColor);

            }
            // printf( "%d ", y );
        }
        task->callback();
    }

    void AsynRenderer::StartTask()
    {
        sNumFinishTasks = 0;
    }

    void AsynRenderer::FinishTask()
    {
        sNumFinishTasks.fetch_add(1);
    }
}

namespace Phong
{
    std::atomic<uint32_t> AsynRenderer::sNumFinishTasks = 0;

    AsynRenderer::AsynRenderer()
    {

    }

    AsynRenderer::~AsynRenderer()
    {

    }

    AsynRenderer* AsynRenderer::GetInstance()
    {
        static AsynRenderer sInstance;
        return &sInstance;
    }

    void AsynRenderer::Initialize()
    {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        mMaxProcesser = info.dwNumberOfProcessors;
        mThreads = new ThreadWindows[16];
        mTaskList = new AsynSceneRenderTask[16];
    }

    void AsynRenderer::Finalize()
    {
        if (mTaskList)
        {
            ClearTasks();
            delete[] mThreads;
            mThreads = nullptr;
        }

    }


    void AsynRenderer::RenderScene(Scene* scene, Image* output, uint32_t numProcesser, uint32_t hasShadow, uint32_t reflectLevels, uint32_t numSample)
    {
        if (mActivateThreads != 0)
        {
            return;
        }
        //Math::clamp(numProcesser, 1u, mMaxProcesser);
        uint32_t width = scene->camera.getImageWidth();
        uint32_t height = scene->camera.getImageHeight();

        uint32_t heightPerTask = height / numProcesser;
        mActivateThreads = numProcesser;
        StartTask();
        for (uint32_t i = 0; i < numProcesser; i++)
        {
            mTaskList[i].width = width;
            mTaskList[i].height = height;
            mTaskList[i].beginHeight = heightPerTask * i;
            mTaskList[i].endHeight = ((i < (numProcesser - 1)) ? heightPerTask * (i + 1) : height);
            mTaskList[i].scene = scene;
            mTaskList[i].output = output;
            mTaskList[i].processerId = i;
            mTaskList[i].callback = FinishTask;
            mTaskList[i].numSample = numSample;
            mTaskList[i].reflectLevels = reflectLevels;
            mTaskList[i].hasShadow = hasShadow;

            ThreadInitializeDescription desc;
            desc.Affinity = 1 << i;
            desc.EntryFunc = DoTask;
            desc.Parameter = &mTaskList[i];
            desc.Priority = GE_THREAD_PRIORITY_NORMAL;
            desc.StackSize = GE_DEFUALT_THRED_STACK_SIZE;
            desc.StackAddress = nullptr;
            mThreads[i].Initialize(desc);
        }

    }

    bool AsynRenderer::IsFinishAllTask()
    {
        return (sNumFinishTasks == mActivateThreads) && (mActivateThreads != 0);
    }

    void AsynRenderer::ClearTasks()
    {
        for (uint32_t i = 0; i < mActivateThreads; i++)
        {
            mThreads[i].Join();
            mThreads[i].Finalize();
        }
        memset(mTaskList, 0, sizeof(mTaskList) * mActivateThreads);
        mActivateThreads = 0;
    }

    void AsynRenderer::DoTask(void* param)
    {
        AsynSceneRenderTask* task = (AsynSceneRenderTask*)param;
        for (uint32_t y = task->beginHeight; y < task->endHeight; y++)
        {
            for (uint32_t x = 0; x < task->width; x++)
            {
                Color pixelColor = Color(0, 0, 0);
                for (uint32_t i = 0; i < task->numSample; i++)
                {
                    float xx;
                    float yy;
                    Math::Hammersley(i, task->numSample, &xx, &yy);
                    double pixelPosX = x + xx;
                    double pixelPosY = y + yy;
                    Ray ray = task->scene->camera.getRay(pixelPosX, pixelPosY);
                    pixelColor += Raytrace::TraceRay(ray, *task->scene, task->reflectLevels, task->hasShadow);
                }
                pixelColor /= (float)task->numSample;
                pixelColor.clamp();
                task->output->setPixel(x, y, pixelColor);

            }
            // printf( "%d ", y );
        }
        task->callback();
    }

    void AsynRenderer::StartTask()
    {
        sNumFinishTasks = 0;
    }

    void AsynRenderer::FinishTask()
    {
        sNumFinishTasks.fetch_add(1);
    }
}