//============================================================
// STUDENT NAME: HU KE
// NUS User ID.: NUSRI1613
// COMMENTS TO GRADER: 
// 
// 
// ============================================================
//
// FILE: Main.cpp



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

using namespace std;


// Constants for Scene 1.
static const int imageWidth1 = 512;
static const int imageHeight1 = 512;
static const int reflectLevels1 = 2;  // 0 -- object does not reflect scene.
static const int hasShadow1 = true;

// Constants for Scene 2.
static const int imageWidth2 = 640;
static const int imageHeight2 = 480;
static const int reflectLevels2 = 2;  // 0 -- object does not reflect scene.
static const int hasShadow2 = true;

Image gImage;

///////////////////////////////////////////////////////////////////////////
// Raytrace the whole image of the scene and write it to a file.
///////////////////////////////////////////////////////////////////////////

void RenderImage( const char *imageFilename, const Scene &scene, int reflectLevels, bool hasShadow )
{
	int imgWidth = scene.camera.getImageWidth();
	int imgHeight = scene.camera.getImageHeight();

	//Image image( imgWidth, imgHeight );	// To store the result of ray tracing.
    gImage.setImage(imgWidth, imgWidth);
	double startTime = Util::GetCurrRealTime();
	double startCPUTime = Util::GetCurrCPUTime();

	// Generate image.
	for ( int y = 0; y < imgHeight; y++ )
	{
		double pixelPosY = y + 0.5;

		for ( int x = 0; x < imgWidth; x++ )
		{
			double pixelPosX = x + 0.5;
			Ray ray = scene.camera.getRay( pixelPosX, pixelPosY );
			Color pixelColor = Raytrace::TraceRay( ray, scene, reflectLevels, hasShadow );
			pixelColor.clamp();
            gImage.setPixel( x, y, pixelColor );
		}
		// printf( "%d ", y );
	}
	double stopCPUTime = Util::GetCurrCPUTime();
	double stopTime = Util::GetCurrRealTime();
	printf( "CPU time taken = %.1f sec\n", stopCPUTime - startCPUTime );
	printf( "Real time taken = %.1f sec\n", stopTime - startTime ); 

	// Write image to file.
	//image.writeToFile( imageFilename );
}




// Forward declarations. These functions are defined later in the file.

void DefineScene1( Scene &scene, int imageWidth, int imageHeight );
void DefineScene2( Scene &scene, int imageWidth, int imageHeight );




void WaitForEnterKeyBeforeExit( void )
{
    fflush( stdin );
    getchar();
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}


int main()
{
//	atexit( WaitForEnterKeyBeforeExit );
//
//
//
// Define Scene 1.

	Scene scene1;
	DefineScene1( scene1, imageWidth1, imageHeight1 );

// Render Scene 1.

	printf( "Render Scene 1...\n" );
	RenderImage( "C:/Users/Huke/Desktop/ImageTest/out1.tga", scene1, reflectLevels1, hasShadow1 );
	printf( "Image completed.\n" );
//
//
//
//// Define Scene 2.
//
//	Scene scene2;
//	DefineScene2( scene2, imageWidth2, imageHeight2 );
//
//// Render Scene 2.
//
//	printf( "Render Scene 2...\n" );
//	RenderImage( "C:/Users/Huke/Desktop/ImageTest/out2.tga", scene2, reflectLevels2, hasShadow2 );
//	printf( "Image completed.\n" );
//
//
//	printf( "All done.\n" );


    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    uchar* tex_data = new uchar[imageWidth1 * imageHeight1 * 4];
    gImage.CreateTextureData(tex_data);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth1, imageHeight1, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            
            ImTextureID my_tex_id = (ImTextureID)texture;//io.Fonts->TexID;
            float my_tex_w = imageWidth1;//(float)io.Fonts->TexWidth;
            float my_tex_h = imageHeight1;//(float)io.Fonts->TexHeight;

            ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
            ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
        }

        // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
        if (show_demo_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
           // ImGui::ShowDemoWindow(&show_demo_window);
        }

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

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

void DefineScene1( Scene &scene, int imageWidth, int imageHeight )
{
	scene.backgroundColor = Color( 0.2f, 0.3f, 0.5f );

	scene.amLight.I_a = Color( 1.0f, 1.0f, 1.0f ) * 0.25f;

// Define materials.

	scene.numMaterials = 5;
	scene.material = new Material[ scene.numMaterials ];

	// Light red.
	scene.material[0].k_d = Color( 0.8f, 0.4f, 0.4f );
	scene.material[0].k_a = scene.material[0].k_d;
	scene.material[0].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
	scene.material[0].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
	scene.material[0].n = 64.0f;

	// Light green.
	scene.material[1].k_d = Color( 0.4f, 0.8f, 0.4f );
	scene.material[1].k_a = scene.material[0].k_d;
	scene.material[1].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
	scene.material[1].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
	scene.material[1].n = 64.0f;

	// Light blue.
	scene.material[2].k_d = Color( 0.4f, 0.4f, 0.8f ) * 0.9f;
	scene.material[2].k_a = scene.material[0].k_d;
	scene.material[2].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
	scene.material[2].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 2.5f;
	scene.material[2].n = 64.0f;

	// Yellow.
	scene.material[3].k_d = Color( 0.6f, 0.6f, 0.2f );
	scene.material[3].k_a = scene.material[0].k_d;
	scene.material[3].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
	scene.material[3].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
	scene.material[3].n = 64.0f;

	// Gray.
	scene.material[4].k_d = Color( 0.6f, 0.6f, 0.6f );
	scene.material[4].k_a = scene.material[0].k_d;
	scene.material[4].k_r = Color( 0.6f, 0.6f, 0.6f );
	scene.material[4].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
	scene.material[4].n = 128.0f;


// Define point light sources.

	scene.numPtLights = 2;
	scene.ptLight = new PointLightSource[ scene.numPtLights ];

	scene.ptLight[0].I_source = Color( 1.0f, 1.0f, 1.0f ) * 0.6f;
	scene.ptLight[0].position = Vector3d( 100.0, 120.0, 10.0 );

	scene.ptLight[1].I_source = Color( 1.0f, 1.0f, 1.0f ) * 0.6f;
	scene.ptLight[1].position = Vector3d( 5.0, 80.0, 60.0 );


// Define surface primitives.

	scene.numSurfaces = 15;
	scene.surfacep = new SurfacePtr[ scene.numSurfaces ];

	scene.surfacep[0] = new Plane( 0.0, 1.0, 0.0, 0.0, &(scene.material[2]) ); // Horizontal plane.
	scene.surfacep[1] = new Plane( 1.0, 0.0, 0.0, 0.0, &(scene.material[4]) ); // Left vertical plane.
	scene.surfacep[2] = new Plane( 0.0, 0.0, 1.0, 0.0, &(scene.material[4]) ); // Right vertical plane.
	scene.surfacep[3] = new Sphere( Vector3d( 40.0, 20.0, 42.0 ), 22.0, &(scene.material[0]) ); // Big sphere.
	scene.surfacep[4] = new Sphere( Vector3d( 75.0, 10.0, 40.0 ), 12.0, &(scene.material[1]) ); // Small sphere.

	// Cube +y face.
	scene.surfacep[5] = new Triangle( Vector3d( 50.0, 20.0, 90.0 ), Vector3d( 50.0, 20.0, 70.0 ),
		                              Vector3d( 30.0, 20.0, 70.0 ), &(scene.material[3]) );
	scene.surfacep[6] = new Triangle( Vector3d( 50.0, 20.0, 90.0 ), Vector3d( 30.0, 20.0, 70.0 ),
		                              Vector3d( 30.0, 20.0, 90.0 ), &(scene.material[3]) );

	// Cube +x face.
	scene.surfacep[7] = new Triangle( Vector3d( 50.0, 0.0, 70.0 ), Vector3d( 50.0, 20.0, 70.0 ),
		                              Vector3d( 50.0, 20.0, 90.0 ), &(scene.material[3]) );
	scene.surfacep[8] = new Triangle( Vector3d( 50.0, 0.0, 70.0 ), Vector3d( 50.0, 20.0, 90.0 ),
		                              Vector3d( 50.0, 0.0, 90.0 ), &(scene.material[3]) );

	// Cube -x face.
	scene.surfacep[9] = new Triangle( Vector3d( 30.0, 0.0, 90.0 ), Vector3d( 30.0, 20.0, 90.0 ),
		                              Vector3d( 30.0, 20.0, 70.0 ), &(scene.material[3]) );
	scene.surfacep[10] = new Triangle( Vector3d( 30.0, 0.0, 90.0 ), Vector3d( 30.0, 20.0, 70.0 ),
		                              Vector3d( 30.0, 0.0, 70.0 ), &(scene.material[3]) );

	// Cube +z face.
	scene.surfacep[11] = new Triangle( Vector3d( 50.0, 0.0, 90.0 ), Vector3d( 50.0, 20.0, 90.0 ),
		                              Vector3d( 30.0, 20.0, 90.0 ), &(scene.material[3]) );
	scene.surfacep[12] = new Triangle( Vector3d( 50.0, 0.0, 90.0 ), Vector3d( 30.0, 20.0, 90.0 ),
		                              Vector3d( 30.0, 0.0, 90.0 ), &(scene.material[3]) );

	// Cube -z face.
	scene.surfacep[13] = new Triangle( Vector3d( 30.0, 0.0, 70.0 ), Vector3d( 30.0, 20.0, 70.0 ),
		                              Vector3d( 50.0, 20.0, 70.0 ), &(scene.material[3]) );
	scene.surfacep[14] = new Triangle( Vector3d( 30.0, 0.0, 70.0 ), Vector3d( 50.0, 20.0, 70.0 ),
		                              Vector3d( 50.0, 0.0, 70.0 ), &(scene.material[3]) );


// Define camera.

	scene.camera = Camera( Vector3d( 150.0, 120.0, 150.0 ), Vector3d( 45.0, 22.0, 55.0 ), Vector3d( 0.0, 1.0, 0.0 ),
				   (-1.0 * imageWidth) / imageHeight, (1.0 * imageWidth) / imageHeight, -1.0, 1.0, 3.0, 
				   imageWidth, imageHeight );
}





///////////////////////////////////////////////////////////////////////////
// Modeling of Scene 2.
///////////////////////////////////////////////////////////////////////////

void DefineScene2( Scene &scene, int imageWidth, int imageHeight )
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    scene.backgroundColor = Color( 0.2f, 0.3f, 0.5f );
    
    scene.amLight.I_a = Color( 1.0f, 1.0f, 1.0f ) * 0.35f;
    
    scene.numMaterials = 5;
    scene.material = new Material[ scene.numMaterials ];
    
    scene.material[0].k_d = Color( 0.8f, 0.4f, 0.4f );
    scene.material[0].k_a = scene.material[0].k_d;
    scene.material[0].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
    scene.material[0].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
    scene.material[0].n = 64.0f;
    
    scene.material[1].k_d = Color( 0.4f, 0.8f, 0.4f );
    scene.material[1].k_a = scene.material[1].k_d;
    scene.material[1].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
    scene.material[1].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
    scene.material[1].n = 64.0f;
    
    scene.material[2].k_d = Color( 0.4f, 0.4f, 0.8f ) * 0.9f;
    scene.material[2].k_a = scene.material[2].k_d;
    scene.material[2].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
    scene.material[2].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 2.5f;
    scene.material[2].n = 64.0f;
    
    scene.material[3].k_d = Color( 0.6f, 0.6f, 0.2f );
    scene.material[3].k_a = scene.material[3].k_d;
    scene.material[3].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
    scene.material[3].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
    scene.material[3].n = 64.0f;
    
    scene.material[4].k_d = Color( 0.6f, 0.6f, 0.6f );
    scene.material[4].k_a = scene.material[4].k_d;
    scene.material[4].k_r = Color( 0.6f, 0.6f, 0.6f );
    scene.material[4].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
    scene.material[4].n = 128.0f;
    
    scene.numPtLights = 2;
    scene.ptLight = new PointLightSource[ scene.numPtLights ];
    
    scene.ptLight[0].I_source = Color( 1.0f, 1.0f, 1.0f ) * 0.6f;
    scene.ptLight[0].position = Vector3d( 40.0, 220.0, 10.0 );
    
    scene.ptLight[1].I_source = Color( 1.0f, 1.0f, 1.0f ) * 0.6f;
    scene.ptLight[1].position = Vector3d( 175.0, 80.0, 60.0 );
    
    scene.numSurfaces = 20;
    scene.surfacep = new SurfacePtr[ scene.numSurfaces ];
    
    scene.surfacep[0] = new Plane( 0.0, 1.0, 0.0, 0.0, &(scene.material[3]) );
    scene.surfacep[1] = new Plane( 0.0, 0.0, 1.0, 0.0, &(scene.material[4]) );

    for(int i = 0 ; i < 14 ; i++)
    {
        scene.surfacep[2+i] = new Sphere(Vector3d( -320 + 40 * i, 20.0, 80.0 + 20 * sin(M_PI/1.8 * i) ), 20.0, &(scene.material[rand()%scene.numMaterials]));
    }
    
    scene.surfacep[16] = new Triangle( Vector3d( 0.0, 50.0, 0.0 ), Vector3d( 50.0, 0.0, 0.0 ),
                                      Vector3d( 0.0, 0.0, 50.0 ), &(scene.material[0]) );
    
    scene.surfacep[17] = new Triangle( Vector3d( 0.0, 50.0, 0.0 ), Vector3d( -50.0, 0.0, 0.0 ),
                                      Vector3d( 0.0, 0.0, 50.0 ), &(scene.material[0]) );
    
    scene.surfacep[18] = new Triangle( Vector3d( 0.0, 50.0, 0.0 ), Vector3d( -50.0, 0.0, 0.0 ),
                                      Vector3d( 50.0, 0.0, 0.0 ), &(scene.material[0]) );
    
    scene.surfacep[19] = new Triangle( Vector3d( 0.0, 0.0, 50.0 ), Vector3d( -50.0, 0.0, 0.0 ),
                                      Vector3d( 50.0, 0.0, 0.0 ), &(scene.material[0]) );

    scene.camera = Camera( Vector3d( 300.0, 240.0, 300.0 ), Vector3d( 45.0, 22.0, 55.0 ), Vector3d( 0.0, 1.0, 0.0 ),
                          (-1.0 * imageWidth) / imageHeight, (1.0 * imageWidth) / imageHeight, -1.0, 1.0, 3.0,
                          imageWidth, imageHeight );;
    //***********************************************
    
    
}
