//============================================================
// STUDENT NAME: HU KE
// NUS User ID.: NUSRI1613
// COMMENTS TO GRADER: 
// 
// 
// ============================================================
//
// FILE: Main.cpp


#include <Platform/Platform.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "Util.h"
#include "Vector3d.h"
#include "Color.h"
#include "Image.h"
#include "Ray.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Surface.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Scene.h"
#include "Raytrace.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <GLFW/glfw3.h>

#include <Renderer/AsynRenderer.h>
#include <Core/Texture.h>
#include "Renderer/ComputeShaderRenderer.h"
#include "UI/RenderUI.h"
using namespace std;


// Constants for Scene 1.
static const uint32_t imageWidth1 = 640;
static const uint32_t imageHeight1 = 480;
static const uint32_t reflectLevels1 = 2;  // 0 -- object does not reflect scene.
static const uint32_t numSample1 = 100;
static const bool hasShadow1 = true;

// Constants for Scene 2.
static const uint32_t imageWidth2 = 640;
static const uint32_t imageHeight2 = 480;
static const uint32_t reflectLevels2 = 2;  // 0 -- object does not reflect scene.
static const uint32_t numSample2 = 100;
static const bool hasShadow2 = true;

static const uint32_t imageWidth3 = 640;
static const uint32_t imageHeight3 = 480;
static const uint32_t reflectLevels3 = 0;  // 0 -- object does not reflect scene.
static const uint32_t numSample3 = 200;
static const bool hasShadow3 = false;


Image gImage;
Texture lut_tex;
///////////////////////////////////////////////////////////////////////////
// Raytrace the whole image of the scene and write it to a file.
///////////////////////////////////////////////////////////////////////////
//template<class T>
//void RenderImage( const char *imageFilename, const Scene &scene, int reflectLevels, bool hasShadow )
//{
//	int imgWidth = scene.camera.getImageWidth();
//	int imgHeight = scene.camera.getImageHeight();
//
//	//Image image( imgWidth, imgHeight );	// To store the result of ray tracing.
//    gImage.setImage(imgWidth, imgHeight);
//	double startTime = Util::GetCurrRealTime();
//	double startCPUTime = Util::GetCurrCPUTime();
//
//	// Generate image.
//	for ( int y = 0; y < imgHeight; y++ )
//	{
//		double pixelPosY = y + 0.5;
//
//		for ( int x = 0; x < imgWidth; x++ )
//		{
//			double pixelPosX = x + 0.5;
//			Ray ray = scene.camera.getRay( pixelPosX, pixelPosY );
//			Color pixelColor = T::TraceRay( ray, scene, reflectLevels, hasShadow );
//			pixelColor.clamp();
//            gImage.setPixel( x, y, pixelColor );
//		}
//		// printf( "%d ", y );
//	}
//	double stopCPUTime = Util::GetCurrCPUTime();
//	double stopTime = Util::GetCurrRealTime();
//	printf( "CPU time taken = %.1f sec\n", stopCPUTime - startCPUTime );
//	printf( "Real time taken = %.1f sec\n", stopTime - startTime ); 
//
//	// Write image to file.
//	//image.writeToFile( imageFilename );
//}




// Forward declarations. These functions are defined later in the file.
namespace PBR
{
    void DefineScene1(Scene &scene, int imageWidth, int imageHeight);
    void DefineScene2(Scene &scene, int imageWidth, int imageHeight);
    void DefineScene3(Scene &scene, int imageWidth, int imageHeight);
}


namespace Phong
{
    void DefineScene1(Scene &scene, int imageWidth, int imageHeight);
    void DefineScene2(Scene &scene, int imageWidth, int imageHeight);
    void DefineScene3(Scene &scene, int imageWidth, int imageHeight);
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

Texture tex;




int main()
{
    RenderUI::GetInstance()->Initialize();
    PBR::AsynRenderer::GetInstance()->Initialize();
    Phong::AsynRenderer::GetInstance()->Initialize();

	Phong::Scene scene1;
	DefineScene1( scene1, imageWidth1, imageHeight1 );

	Phong::Scene scene2;
	DefineScene2( scene2, imageWidth2, imageHeight2 );

    PBR::Scene scene3;
    DefineScene3(scene3, imageWidth3, imageHeight3);

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui GLFW+OpenGL3 example", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    gl3wInit();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfwGL3_Init(window, true);

    ImGui::StyleColorsDark();

    io.Fonts->AddFontFromFileTTF("Resource/Fonts/Roboto-Medium.ttf", 16.0f);

    gImage.setImage(scene2.camera.getImageWidth(), scene2.camera.getImageHeight());
    

    lut_tex.Load("D:/RayTracing/Resource/Textures/pbdf_lut/ibl_brdf_lut.png");
    PBR::ComputeShaderRenderer::GetInstance()->Initialize();
    PBR::ComputeShaderRenderer::GetInstance()->CreateOutputImage(640, 480);
    PBR::ComputeShaderRenderer::GetInstance()->GenerateShaderData(scene3);
    PBR::ComputeShaderRenderer::GetInstance()->PrepareShaderData();
    //RenderImage("C:/Users/Huke/Desktop/ImageTest/out2.tga", scene2, reflectLevels2, hasShadow2);
    while (!glfwWindowShouldClose(window))
    {   
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        RenderUI::GetInstance()->Update();

        ImGui::Begin("Render Option");
        {
            static int light_mode = 0;
            ImGui::Text("Lighting Model:");
            ImGui::Separator();
            
            ImGui::RadioButton("Traditional Rendering", &light_mode, 1);
            ImGui::RadioButton("Physicall Base Rendering", &light_mode, 2);
          
        }



        ImGui::End();
        ImGui::ShowDemoWindow();
        gImage.UpdateTexture();
        {
            static uint32_t aaa = 0;
            ImGui::Begin("Option");
            if (ImGui::Button("Start"))
            {
                //AsynRenderer::GetInstance()->RenderScene(&scene3, &gImage, 8, hasShadow3, reflectLevels3, numSample3);
            }
            if (ImGui::Button("Start2"))
            {
               // AsynRenderer::GetInstance()->RenderScene(&scene2, &gImage, 8, hasShadow2, reflectLevels2, numSample2);
            }
            if (ImGui::Button("Gamma"))
            {
                gImage.gammaCorrect();
            }
            ImGui::End();
        }

        {
            ImGui::Begin("Option");
            PBR::ComputeShaderRenderer::GetInstance()->GenerateShaderData(scene3);
            PBR::ComputeShaderRenderer::GetInstance()->PassShaderData();
            PBR::ComputeShaderRenderer::GetInstance()->Render();
            glBindTexture(GL_TEXTURE_2D, PBR::ComputeShaderRenderer::GetInstance()->GetOutputImageHandle());
            ImTextureID my_tex_id = reinterpret_cast<ImTextureID>(PBR::ComputeShaderRenderer::GetInstance()->GetOutputImageHandle());// (gImage.GetGLTextureHandle());//io.Fonts->TexID;
            //ImTextureID my_tex_id = reinterpret_cast<ImTextureID>(gImage.GetGLTextureHandle());
            float my_tex_w = (float)gImage.GetWidth();//(float)io.Fonts->TexWidth;
            float my_tex_h = (float)gImage.GetHeight();//(float)io.Fonts->TexHeight;
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
            
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                float region_sz = 32.0f;
                float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
                if (region_x < 0.0f)
                    region_x = 0.0f;
                else if (region_x > my_tex_w - region_sz)
                    region_x = my_tex_w - region_sz;
                float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
                if (region_y < 0.0f)
                    region_y = 0.0f;
                else if (region_y > my_tex_h - region_sz)
                    region_y = my_tex_h - region_sz;
                float zoom = 4.0f;
                //ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                //ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);

                ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
                ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
                //ImGui::Text("uv: (%.2f, %.2f)", uv0.x, uv0.y);
                ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
                ImGui::EndTooltip();
            }
            ImGui::End();
        }
        
        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        if (PBR::AsynRenderer::GetInstance()->IsFinishAllTask())
        {
            PBR::AsynRenderer::GetInstance()->ClearTasks();
        }

    }

    PBR::AsynRenderer::GetInstance()->Finalize();
    PBR::ComputeShaderRenderer::GetInstance()->Finalize();
    RenderUI::GetInstance()->Finalize();
    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

	return 0;
}





///////////////////////////////////////////////////////////////////////////
// Modeling of Scene 1.
///////////////////////////////////////////////////////////////////////////
namespace Phong
{
    void DefineScene1(Scene &scene, int imageWidth, int imageHeight)
    {
        scene.backgroundColor = Color(0.2f, 0.3f, 0.5f);

        scene.amLight.I_a = Color(1.0f, 1.0f, 1.0f) * 0.25f;

        // Define materials.

        scene.numMaterials = 5;
        scene.material = new Material[scene.numMaterials];
        Material* mat = reinterpret_cast<Material*>(scene.material);

        // Light red.
        scene.material[0].diffuse = Color(0.8f, 0.4f, 0.4f);
        scene.material[0].ambient = Color(0.8f, 0.4f, 0.4f);
        scene.material[0].specluar = Color(0.8f, 0.8f, 0.8f) / 1.5f;
        scene.material[0].reflection = Color(0.8f, 0.8f, 0.8f) / 3.0f;
        scene.material[0].shininess = 32.0f;

        // Light green.
        scene.material[1].diffuse = Color(0.4f, 0.8f, 0.4f);
        scene.material[1].ambient = Color(0.4f, 0.8f, 0.4f);
        scene.material[1].specluar = Color(0.8f, 0.8f, 0.8f) / 1.5f;
        scene.material[1].reflection = Color(0.8f, 0.8f, 0.8f) / 3.0f;
        scene.material[1].shininess = 64.0f;

        // Light blue.
        scene.material[2].diffuse = Color(0.4f, 0.4f, 0.8f) * 0.9f;
        scene.material[2].ambient = Color(0.4f, 0.4f, 0.8f) * 0.9f;
        scene.material[2].specluar = Color(0.8f, 0.8f, 0.8f) / 1.5f;
        scene.material[2].reflection = Color(0.8f, 0.8f, 0.8f) / 2.5f;
        scene.material[2].shininess = 64.0f;

        // Yellow.
        scene.material[3].diffuse = Color(0.6f, 0.6f, 0.2f);
        scene.material[3].ambient = Color(0.6f, 0.6f, 0.2f);
        scene.material[3].specluar = Color(0.8f, 0.8f, 0.8f) / 1.5f;
        scene.material[3].reflection = Color(0.8f, 0.8f, 0.8f) / 3.0f;
        scene.material[3].shininess = 64.0f;

        // Gray.
        scene.material[4].diffuse = Color(0.6f, 0.6f, 0.6f);
        scene.material[4].ambient = Color(0.6f, 0.6f, 0.6f);
        scene.material[4].specluar = Color(0.6f, 0.6f, 0.6f);
        scene.material[4].reflection = Color(0.8f, 0.8f, 0.8f) / 3.0f;
        scene.material[4].shininess = 128.0f;


        // Define point light sources.

        scene.numPtLights = 2;
        scene.ptLight = new PointLightSource[scene.numPtLights];

        scene.ptLight[0].I_source = Color(1.0f, 1.0f, 1.0f) * 0.6f;
        scene.ptLight[0].position = Vector3d(100.0, 120.0, 10.0);

        scene.ptLight[1].I_source = Color(1.0f, 1.0f, 1.0f) * 0.6f;
        scene.ptLight[1].position = Vector3d(5.0, 80.0, 60.0);


        // Define surface primitives.

        scene.numSurfaces = 15;
        scene.surfacep = new SurfacePtr[scene.numSurfaces];

        scene.surfacep[0] = new Plane(0.0, 1.0, 0.0, 0.0, &(scene.material[2])); // Horizontal plane.
        scene.surfacep[1] = new Plane(1.0, 0.0, 0.0, 0.0, &(scene.material[4])); // Left vertical plane.
        scene.surfacep[2] = new Plane(0.0, 0.0, 1.0, 0.0, &(scene.material[4])); // Right vertical plane.
        scene.surfacep[3] = new Sphere(Vector3d(40.0, 20.0, 42.0), 22.0, &(scene.material[0])); // Big sphere.
        scene.surfacep[4] = new Sphere(Vector3d(75.0, 10.0, 40.0), 12.0, &(scene.material[1])); // Small sphere.

                                                                                                // Cube +y face.
        scene.surfacep[5] = new Triangle(Vector3d(50.0, 20.0, 90.0), Vector3d(50.0, 20.0, 70.0),
            Vector3d(30.0, 20.0, 70.0), &(scene.material[3]));
        scene.surfacep[6] = new Triangle(Vector3d(50.0, 20.0, 90.0), Vector3d(30.0, 20.0, 70.0),
            Vector3d(30.0, 20.0, 90.0), &(scene.material[3]));

        // Cube +x face.
        scene.surfacep[7] = new Triangle(Vector3d(50.0, 0.0, 70.0), Vector3d(50.0, 20.0, 70.0),
            Vector3d(50.0, 20.0, 90.0), &(scene.material[3]));
        scene.surfacep[8] = new Triangle(Vector3d(50.0, 0.0, 70.0), Vector3d(50.0, 20.0, 90.0),
            Vector3d(50.0, 0.0, 90.0), &(scene.material[3]));

        // Cube -x face.
        scene.surfacep[9] = new Triangle(Vector3d(30.0, 0.0, 90.0), Vector3d(30.0, 20.0, 90.0),
            Vector3d(30.0, 20.0, 70.0), &(scene.material[3]));
        scene.surfacep[10] = new Triangle(Vector3d(30.0, 0.0, 90.0), Vector3d(30.0, 20.0, 70.0),
            Vector3d(30.0, 0.0, 70.0), &(scene.material[3]));

        // Cube +z face.
        scene.surfacep[11] = new Triangle(Vector3d(50.0, 0.0, 90.0), Vector3d(50.0, 20.0, 90.0),
            Vector3d(30.0, 20.0, 90.0), &(scene.material[3]));
        scene.surfacep[12] = new Triangle(Vector3d(50.0, 0.0, 90.0), Vector3d(30.0, 20.0, 90.0),
            Vector3d(30.0, 0.0, 90.0), &(scene.material[3]));

        // Cube -z face.
        scene.surfacep[13] = new Triangle(Vector3d(30.0, 0.0, 70.0), Vector3d(30.0, 20.0, 70.0),
            Vector3d(50.0, 20.0, 70.0), &(scene.material[3]));
        scene.surfacep[14] = new Triangle(Vector3d(30.0, 0.0, 70.0), Vector3d(50.0, 20.0, 70.0),
            Vector3d(50.0, 0.0, 70.0), &(scene.material[3]));


        // Define camera.

        scene.camera = Camera(Vector3d(150.0, 120.0, 150.0), Vector3d(45.0, 22.0, 55.0), Vector3d(0.0, 1.0, 0.0),
            (-1.0 * imageWidth) / imageHeight, (1.0 * imageWidth) / imageHeight, -1.0, 1.0, 3.0,
            imageWidth, imageHeight);
    }


    void DefineScene2(Scene &scene, int imageWidth, int imageHeight)
    {
        //***********************************************
        //*********** WRITE YOUR CODE HERE **************
        scene.backgroundColor = Color(0.2f, 0.3f, 0.5f);

        scene.amLight.I_a = Color(1.0f, 1.0f, 1.0f) * 0.35f;

        scene.numMaterials = 5;
        scene.material = new Material[scene.numMaterials];

        // Light red.
        scene.material[0].diffuse = Color(0.8f, 0.4f, 0.4f);
        scene.material[0].ambient = Color(0.8f, 0.4f, 0.4f);
        scene.material[0].specluar = Color(0.8f, 0.8f, 0.8f) / 1.5f;
        scene.material[0].reflection = Color(0.8f, 0.8f, 0.8f) / 3.0f;
        scene.material[0].shininess = 32.0f;

        // Light green.
        scene.material[1].diffuse = Color(0.4f, 0.8f, 0.4f);
        scene.material[1].ambient = Color(0.4f, 0.8f, 0.4f);
        scene.material[1].specluar = Color(0.8f, 0.8f, 0.8f) / 1.5f;
        scene.material[1].reflection = Color(0.8f, 0.8f, 0.8f) / 3.0f;
        scene.material[1].shininess = 64.0f;

        // Light blue.
        scene.material[2].diffuse = Color(0.4f, 0.4f, 0.8f) * 0.9f;
        scene.material[2].ambient = Color(0.4f, 0.4f, 0.8f) * 0.9f;
        scene.material[2].specluar = Color(0.8f, 0.8f, 0.8f) / 1.5f;
        scene.material[2].reflection = Color(0.8f, 0.8f, 0.8f) / 2.5f;
        scene.material[2].shininess = 64.0f;

        // Yellow.
        scene.material[3].diffuse = Color(0.6f, 0.6f, 0.2f);
        scene.material[3].ambient = Color(0.6f, 0.6f, 0.2f);
        scene.material[3].specluar = Color(0.8f, 0.8f, 0.8f) / 1.5f;
        scene.material[3].reflection = Color(0.8f, 0.8f, 0.8f) / 3.0f;
        scene.material[3].shininess = 64.0f;

        // Gray.
        scene.material[4].diffuse = Color(0.6f, 0.6f, 0.6f);
        scene.material[4].ambient = Color(0.6f, 0.6f, 0.6f);
        scene.material[4].specluar = Color(0.6f, 0.6f, 0.6f);
        scene.material[4].reflection = Color(0.8f, 0.8f, 0.8f) / 3.0f;
        scene.material[4].shininess = 128.0f;

        scene.numPtLights = 2;
        scene.ptLight = new PointLightSource[scene.numPtLights];

        scene.ptLight[0].I_source = Color(1.0f, 1.0f, 1.0f) * 0.6f;
        scene.ptLight[0].position = Vector3d(40.0, 220.0, 10.0);

        scene.ptLight[1].I_source = Color(1.0f, 1.0f, 1.0f) * 0.6f;
        scene.ptLight[1].position = Vector3d(175.0, 80.0, 60.0);

        scene.numSurfaces = 20;
        scene.surfacep = new SurfacePtr[scene.numSurfaces];

        scene.surfacep[0] = new Plane(0.0, 1.0, 0.0, 0.0, &(scene.material[3]));
        scene.surfacep[1] = new Plane(0.0, 0.0, 1.0, 0.0, &(scene.material[4]));

        for (int i = 0; i < 14; i++)
        {
            scene.surfacep[2 + i] = new Sphere(Vector3d(-320 + 40 * i, 20.0, 80.0 + 20 * sin(M_PI / 1.8 * i)), 20.0, &(scene.material[rand() % scene.numMaterials]));
        }

        scene.surfacep[16] = new Triangle(Vector3d(0.0, 50.0, 0.0), Vector3d(50.0, 0.0, 0.0),
            Vector3d(0.0, 0.0, 50.0), &(scene.material[0]));

        scene.surfacep[17] = new Triangle(Vector3d(0.0, 50.0, 0.0), Vector3d(-50.0, 0.0, 0.0),
            Vector3d(0.0, 0.0, 50.0), &(scene.material[0]));

        scene.surfacep[18] = new Triangle(Vector3d(0.0, 50.0, 0.0), Vector3d(-50.0, 0.0, 0.0),
            Vector3d(50.0, 0.0, 0.0), &(scene.material[0]));

        scene.surfacep[19] = new Triangle(Vector3d(0.0, 0.0, 50.0), Vector3d(-50.0, 0.0, 0.0),
            Vector3d(50.0, 0.0, 0.0), &(scene.material[0]));

        scene.camera = Camera(Vector3d(300.0, 240.0, 300.0), Vector3d(45.0, 22.0, 55.0), Vector3d(0.0, 1.0, 0.0),
            (-1.0 * imageWidth) / imageHeight, (1.0 * imageWidth) / imageHeight, -1.0, 1.0, 3.0,
            imageWidth, imageHeight);;
        //***********************************************


    }
}





///////////////////////////////////////////////////////////////////////////
// Modeling of Scene 2.
///////////////////////////////////////////////////////////////////////////


namespace PBR
{
    void DefineScene3(Scene &scene, int imageWidth, int imageHeight)
    {
        scene.backgroundColor = Color(0.0f, 0.0f, 0.0f);

        scene.amLight.I_a = Color(1.0f, 1.0f, 1.0f) * 0.55f;


        scene.numMaterials = 30;
        scene.material = new Material[scene.numMaterials];
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                scene.material[j * 6 + i].albedo = Color(1.0f, 0.0f, 0.0f);
                scene.material[j * 6 + i].metallic = 0.25f + i * 0.15f;// i * (1.0f / 5.0f);
                scene.material[j * 6 + i].roughness = 0.1f + j * 0.2f;
            }
        }


        scene.numSurfaces = 30;
        scene.surfacep = new SurfacePtr[scene.numSurfaces];


        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                scene.surfacep[j * 6 + i] = new Sphere(Vector3d(35.0 - i * 14, 25.0 - j * 12, 100.0), 5.0, &(scene.material)[j * 6 + i]);
            }

        }


        scene.numPtLights = 2;
        scene.ptLight = new PointLightSource[scene.numPtLights];

        scene.ptLight[0].I_source = Color(300.0, 300.0, 300.0);
        scene.ptLight[0].position = Vector3d(30.0, 40.0, 80.0);

        scene.ptLight[1].I_source = Color(300.0, 300.0, 300.0);
        scene.ptLight[1].position = Vector3d(-30.0, 40.0, 80.0);

        scene.camera = Camera(Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, 10.0), Vector3d(0.0, 1.0, 0.0),
            (-1.0 * imageWidth) / imageHeight, (1.0 * imageWidth) / imageHeight, -1.0, 1.0, 3.0,
            imageWidth, imageHeight);;
    }
}

