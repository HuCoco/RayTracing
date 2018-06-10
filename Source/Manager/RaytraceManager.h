#pragma once
#include <Core/Scene.h>
#include <Core/Ray.h>
#include <Core/Texture.h>
#include <Platform/Thread/Thread.h>
#include <Manager/RayTracingTaskManager.h>
struct TraceDescription
{
    uint32_t num_ray_per_pixel;
    uint32_t num_reflection;
    bool HasShadow;
};



class RaytraceManager
{
public:
    RaytraceManager() = default;
    ~RaytraceManager() = default;

    static RaytraceManager& GetInstance();

    void Initialize();
    void Finalize();
    void RenderScene(const Scene& scene, const TraceDescription& desc, Image* texture);
    void RenderSceneAsyn(Scene& scene, const TraceDescription& desc, Image* texture);
    static void SyncRenderPixel(void* data);
private:
    RayTracingTaskManager mRayTracingTaskManager;
};