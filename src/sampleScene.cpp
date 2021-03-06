
// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs
#define GLEW_STATIC
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <stdio.h>



#include <GL/glew.h>            // Initialize with glewInit()


// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>
#include "Renderer/Shader.hpp"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions

#if defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader


    bool err = glewInit() != GLEW_OK;

    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Shader shader("../shader/VertexShader.glsl",  "../shader/FragmentShader.glsl");
    shader.use();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

//#define GLEW_STATIC
//
//#include <iostream>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <Eigen/Dense>
//#include "Renderer/imgui/imgui.h"
//#include "Renderer/imgui/imgui_impl_opengl3.h"
//#include "Renderer/imgui/imgui_impl_glfw.h"
//
//
//using namespace Eigen;
//using namespace std;
//
//int main()
//{
//
//
//    MatrixXd m = MatrixXd::Random(3,3);
//    m = (m + MatrixXd::Constant(3,3,1.2)) * 50;
//    cout << "m =" << endl << m << endl;
//    VectorXd v(3);
//    v << 1, 2, 3;
//    cout << "m * v =" << endl << m * v << endl;
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    if (glewInit() != GLEW_OK) { cout << "succeed\n"; }
//    /* Create a windowed mode window and its OpenGL context */
//
//    window = glfwCreateWindow(1640, 1480, "Hello World", NULL, NULL);
//
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;
//    }
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//
//    // Setup Dear ImGui context
//    IMGUI_CHECKVERSION();
//    cout << "47\n";
//    ImGui::CreateContext();
//    cout << "49\n";
//    ImGuiIO &io = ImGui::GetIO();
//    cout << "51\n";
//    // Setup Platform/Renderer bindings
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    cout << "54\n";
//    ImGui_ImplOpenGL3_Init("#version 330");
//    // Setup Dear ImGui style
//    cout << "57\n";
//    ImGui::StyleColorsDark();
//    cout << "47\n";
//    ImGui::Begin("Demo window");
//    cout << "47\n";
//    ImGui::Button("Hello!");
//    cout << "47\n";
//    ImGui::End();
//
//    // Render dear imgui into screen
//    cout << "47\n";
//    ImGui::Render();
//    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//    /* Loop until the user closes the window */
//    while (!glfwWindowShouldClose(window))
//    {
//        /* Render here */
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        /* Swap front and back buffers */
//        glfwSwapBuffers(window);
//
//
//        /* Poll for and process events */
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//    return 0;
//}


//
//// Include standard headers
//#include <stdio.h>
//#include <stdlib.h>
//
//// Include GLEW
//#include <GL/glew.h>
//
//// Include GLFW
//#include <GLFW/glfw3.h>
//
//
//// Include GLM
//#include <glm/glm.hpp>
//using namespace glm;
//
//#include "Renderer/Renderer.hpp"
//
//int main( void )
//{
//
//
//    Renderer *renderer = Renderer::Builder()
//            .init() //TODO: window parameter
//            .camera(glm::vec3(3., 2., 3.),glm::vec3(0))
//            .shader("../shader/VertexShader.glsl",  "../shader/FragmentShader.glsl")
//            .light(glm::vec3(5.,5.,5.),
//                   glm::vec3(0.,1.,1.),
//                   glm::vec3(1.,1.,1.),
//                   glm::vec3(1.,1.,1.),
//                   glm::vec3(0,0,0))
//            .build();
//
//    // Create and compile our GLSL program from the shaders
//
//
//    std::vector<glm::vec3> g_vertex_buffer_data;
//    g_vertex_buffer_data.emplace_back(-1.0f, -1.0f, 0.0f);
//    g_vertex_buffer_data.emplace_back(1.0f, -1.0f, 0.0f);
//    g_vertex_buffer_data.emplace_back(0.0f,  1.0f, 0.0f);
//
//
//
////    GLuint vertexbuffer;
////    glGenBuffers(1, &vertexbuffer);
////    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
////    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*g_pointer->size(), g_pointer->data(), GL_STATIC_DRAW);
//    GLuint vertexbuffer;
//    {
//        GLuint _vertexbuffer;
//        std::vector<glm::vec3>* g_pointer= &g_vertex_buffer_data;
//        glGenBuffers(1, &_vertexbuffer);
//        glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*g_pointer->size(), g_pointer->data(), GL_STATIC_DRAW);
//        vertexbuffer=_vertexbuffer;
//    }
//
//    //glBindVertexArray(renderer->getVAO());
//    while (!glfwWindowShouldClose(renderer->getWindow())) {
//
//        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glEnableVertexAttribArray(0);
//        glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
//        glVertexAttribPointer(
//                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//                3,                  // size
//                GL_FLOAT,           // type
//                GL_FALSE,           // normalized?
//                0,                  // stride
//                (void*)0            // array buffer offset
//        );
//        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,t_graphics_data.m_EBO);
//        //std::cout << &(*t_graphics_data.m_position)[0].x << "\n";
//
//
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        //glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
//
//        glDisableVertexAttribArray(0);
//
//        // Swap buffers
//        glfwSwapBuffers(renderer->getWindow());
//        glfwPollEvents();
//        int display_w, display_h;
//        glfwGetFramebufferSize(renderer->getWindow(), &display_w, &display_h);
//        glViewport(0, 0, display_w, display_h);
//    }
//
//
////    do{
////
////        // Clear the screen
////        glClear( GL_COLOR_BUFFER_BIT );
////
////        // Use our shader
////        renderer->getShader().use();
////
////        // 1rst attribute buffer : vertices
////        glEnableVertexAttribArray(0);
////        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
////        glVertexAttribPointer(
////                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
////                3,                  // size
////                GL_FLOAT,           // type
////                GL_FALSE,           // normalized?
////                0,                  // stride
////                (void*)0            // array buffer offset
////        );
////
////        // Draw the triangle !
////        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
////
////        glDisableVertexAttribArray(0);
////
////        // Swap buffers
////        glfwSwapBuffers(renderer->getWindow());
////        glfwPollEvents();
////
////    } // Check if the ESC key was pressed or the window was closed
////    while( glfwGetKey(renderer->getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
////           glfwWindowShouldClose(renderer->getWindow()) == 0 );
//
//    // Cleanup VBO
//    glDeleteBuffers(1, &vertexbuffer);
//
//    // Close OpenGL window and terminate GLFW
//    glfwTerminate();
//
//    return 0;
//}

