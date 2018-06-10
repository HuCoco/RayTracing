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
    mRayTracingTaskManager.Initialize(8);

}

void RaytraceManager::Finalize()
{
    mRayTracingTaskManager.Finalize();
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
            RayTracingTaskData data;
            data.x = x+0.5f;
            data.y = y+0.5f;
            data.output = image->getPixel(x, y);
            data.scene = &scene;
            data.callback = nullptr;
            mRayTracingTaskManager.PushTask(data);
            //double pixelPosX = x + 0.5;
            //Ray ray = scene.camera.getRay(pixelPosX, pixelPosY);
            //Color pixelColor = Raytrace::TraceRay(ray, scene, desc.num_reflection, desc.HasShadow);
            //pixelColor.clamp();
            //texture->setPixel(x, y, pixelColor);
        }
        // printf( "%d ", y );
    }
}

