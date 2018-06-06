#pragma once
#include <Image/Image.h>
#include <Core/Scene.h>
#include <Core/Sphere.h>
#include <Core/Camera.h>
#include <Core/RayTracer.h>
int main()
{
    //char* data = new char[128 * 128 * 4];
    //Image::WriteImage("C:/Users/Huke/Desktop/ImageTest/test.tga", 128, 128, data, IMAGE_FORMAT::TGA_FORMAT);
    //return 0;

    uint32_t imageWidth = 1920;
    uint32_t imageHeight = 1080;

    Scene scene;
    scene.m_materials.resize(5);

    scene.m_materials[0].k_d = Vec3f(0.8f, 0.4f, 0.4f);
    scene.m_materials[0].k_a = scene.m_materials[0].k_d;
    scene.m_materials[0].k_r = Vec3f(0.8f, 0.8f, 0.8f) / 1.5f;
    scene.m_materials[0].k_rg = Vec3f(0.8f, 0.8f, 0.8f) / 3.0f;
    scene.m_materials[0].n = 64.0f;

    // Light green.
    scene.m_materials[1].k_d = Vec3f(0.4f, 0.8f, 0.4f);
    scene.m_materials[1].k_a = scene.m_materials[0].k_d;
    scene.m_materials[1].k_r = Vec3f(0.8f, 0.8f, 0.8f) / 1.5f;
    scene.m_materials[1].k_rg = Vec3f(0.8f, 0.8f, 0.8f) / 3.0f;
    scene.m_materials[1].n = 64.0f;

    // Light blue.
    scene.m_materials[2].k_d = Vec3f(0.4f, 0.4f, 0.8f) * 0.9f;
    scene.m_materials[2].k_a = scene.m_materials[0].k_d;
    scene.m_materials[2].k_r = Vec3f(0.8f, 0.8f, 0.8f) / 1.5f;
    scene.m_materials[2].k_rg = Vec3f(0.8f, 0.8f, 0.8f) / 2.5f;
    scene.m_materials[2].n = 64.0f;

    // Yellow.
    scene.m_materials[3].k_d = Vec3f(0.6f, 0.6f, 0.2f);
    scene.m_materials[3].k_a = scene.m_materials[0].k_d;
    scene.m_materials[3].k_r = Vec3f(0.8f, 0.8f, 0.8f) / 1.5f;
    scene.m_materials[3].k_rg = Vec3f(0.8f, 0.8f, 0.8f) / 3.0f;
    scene.m_materials[3].n = 64.0f;

    // Gray.
    scene.m_materials[4].k_d = Vec3f(0.6f, 0.6f, 0.6f);
    scene.m_materials[4].k_a = scene.m_materials[0].k_d;
    scene.m_materials[4].k_r = Vec3f(0.6f, 0.6f, 0.6f);
    scene.m_materials[4].k_rg = Vec3f(0.8f, 0.8f, 0.8f) / 3.0f;
    scene.m_materials[4].n = 128.0f;

    scene.m_Lights.resize(2);

    scene.m_Lights[0].color = Vec3f(1.0f, 1.0f, 1.0f) * 0.6f;
    scene.m_Lights[0].position = Vec3f(100.0, 120.0, 10.0);

    scene.m_Lights[1].color = Vec3f(1.0f, 1.0f, 1.0f) * 0.6f;
    scene.m_Lights[1].position = Vec3f(5.0, 80.0, 60.0);

    scene.m_surfaces.resize(2);
    scene.m_surfaces[0] = new Sphere(Vec3f(40.0, 20.0, 42.0), 22.0, &(scene.m_materials[0])); // Big sphere.
    scene.m_surfaces[1] = new Sphere(Vec3f(75.0, 10.0, 40.0), 12.0, &(scene.m_materials[1])); // Small sphere.

    scene.camera = Camera();
    scene.camera.Initialize(Vec3f(150.0, 120.0, 150.0), Vec3f(45.0, 22.0, 55.0), Vec3f(0.0, 1.0, 0.0),
        (-1.0f * imageWidth) / imageHeight, (1.0f * imageWidth) / imageHeight, -1.0f, 1.0f, 3.0f,
        imageWidth, imageHeight);

    Image image(imageWidth, imageHeight);
    for (int y = 0; y < imageHeight; y++)
    {
        double pixelPosY = y + 0.5;

        for (int x = 0; x < imageWidth; x++)
        {
            double pixelPosX = x + 0.5;
            Ray ray = scene.camera.GetRay(pixelPosX, pixelPosY);
            Vec3f pixelColor = RayTracer::TraceRay(ray, scene, 2);
            //pixelColor.clamp();
            image.SetPixel(x, y, pixelColor);
        }
        // printf( "%d ", y );
    }

    Image::WriteImage("C:/Users/Huke/Desktop/ImageTest/test.tga", imageWidth, imageHeight, image.GetData(), IMAGE_FORMAT::TGA_FORMAT);
    return 0;
}