#include "RaytraceManager.h"
#include <Core/Raytrace.h>
#include <Core/Camera.h>

float RadicalInverse(uint32_t bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10f;

}

void Hammersley(uint32_t i, uint32_t N, float& x, float& y)
{
    x = float(i) / float(N);
    y = RadicalInverse(i);
}



RaytraceManager& RaytraceManager::GetInstance()
{
    static RaytraceManager sInstance;
    return sInstance;
}

void RaytraceManager::Initialize()
{

    mQueueMutex.Initialize(false);

    ThreadInitializeDescription desc;
    desc.EntryFunc = SyncRenderPixel;
    desc.Parameter = this;
    desc.Affinity = GE_THREAD_CORE_4 | GE_THREAD_CORE_3 | GE_THREAD_CORE_2 | GE_THREAD_CORE_1;
    desc.Priority = GE_THREAD_PRIORITY_NORMAL;
    desc.StackSize = GE_DEFUALT_THRED_STACK_SIZE;
    desc.StackAddress = nullptr;
    mThread.Initialize(desc);


}

void RaytraceManager::RenderScene(const Scene& scene, const TraceDescription& desc, Image* image)
{
    auto& camera = scene.camera;
    uint32_t width = camera.getImageWidth();
    uint32_t height = camera.getImageHeight();

    // Generate image.
    for (int y = 0; y < height; y++)
    {
        double pixelPosY = y + 0.5;

        for (int x = 0; x < width; x++)
        {
            double pixelPosX = x + 0.5;
            Ray ray = scene.camera.getRay(pixelPosX, pixelPosY);
            Color pixelColor = Raytrace::TraceRay(ray, scene, desc.num_reflection, desc.HasShadow);
            pixelColor.clamp();
            image->setPixel(x, y, pixelColor);
        }
        // printf( "%d ", y );
    }
}

void RaytraceManager::RenderSceneAsyn(Scene& scene, const TraceDescription& desc, Image* image)
{
    auto& camera = scene.camera;
    uint32_t width = camera.getImageWidth();
    uint32_t height = camera.getImageHeight();

    // Generate image.
    for (int y = 0; y < height; y++)
    {
        double pixelPosY = y + 0.5;

        for (int x = 0; x < width; x++)
        {
            RenderPixelData data;
            data.x = x+0.5f;
            data.y = y+0.5f;
            data.output = image->getPixel(x, y);
            data.scene = &scene;
            data.callback = nullptr;
            mQueueMutex.Lock();
            mQueue.push(data);
            mQueueMutex.Unlock();
            //double pixelPosX = x + 0.5;
            //Ray ray = scene.camera.getRay(pixelPosX, pixelPosY);
            //Color pixelColor = Raytrace::TraceRay(ray, scene, desc.num_reflection, desc.HasShadow);
            //pixelColor.clamp();
            //texture->setPixel(x, y, pixelColor);
        }
        // printf( "%d ", y );
    }
}

bool RaytraceManager::GetRenderPixelData(RenderPixelData& data)
{
    mQueueMutex.Lock();
    if (mQueue.empty())
    {
        mQueueMutex.Unlock();
        return false;
    }
    data = mQueue.front();
    mQueue.pop();
    mQueueMutex.Unlock();
    return true;
}

void RaytraceManager::SyncRenderPixel(void* param)
{
    RaytraceManager* manager = (RaytraceManager*)param;
    while (true)
    {
        RenderPixelData render_data;
        bool res = manager->GetRenderPixelData(render_data);
        if (res == false)
        {
            ::Sleep(1000);
            continue;
        }
        Ray ray = render_data.scene->camera.getRay(render_data.x, render_data.y);
        Color pixelColor = Raytrace::TraceRay(ray, *render_data.scene, 2, true);
        pixelColor.clamp();
        *render_data.output = pixelColor;
    }

}

