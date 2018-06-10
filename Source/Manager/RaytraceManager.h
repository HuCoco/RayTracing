#pragma once
#include <Core/Scene.h>
#include <Core/Ray.h>
#include <Core/Texture.h>
#include <Platform/Thread/Thread.h>
#include <atomic>
#include <queue>
struct TraceDescription
{
    uint32_t num_ray_per_pixel;
    uint32_t num_reflection;
    bool HasShadow;
};
typedef void(*RenderPixelCallback)();
struct RenderPixelData
{
    float x;
    float y;
    Scene* scene;
    Color* output;
    RenderPixelCallback callback;
};

class RaytraceManager
{
public:
    RaytraceManager() = default;
    ~RaytraceManager() = default;

    static RaytraceManager& GetInstance();

    void Initialize();
    void RenderScene(const Scene& scene, const TraceDescription& desc, Image* texture);
    void RenderSceneAsyn(Scene& scene, const TraceDescription& desc, Image* texture);
    bool GetRenderPixelData(RenderPixelData& data);
    static void SyncRenderPixel(void* data);
private:
    ThreadWindows mThread;
    MutexWindows mQueueMutex;
    std::atomic<uint32_t> mPixelCounter{ 0 };
    std::queue<RenderPixelData> mQueue;
    
};