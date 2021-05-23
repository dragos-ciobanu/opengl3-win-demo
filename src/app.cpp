#include <iostream>
#include "Renderer.h"

#include "VertexArray.h"

#include "Texture.h"

#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <fstream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

unsigned int width = 1920;
unsigned int height = 1080;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(void)
{
    glfwSetErrorCallback(glfw_error_callback);

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(width, height, "Dragos Ciobanu - Grafica IFR 2021", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        std::cout << "Glew Error" << std::endl;

    glfwSwapInterval(1);
    glViewport(0, 0, width, height);


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
    ImGui_ImplOpenGL3_Init();


    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    {
        float positions[] = {
             200.0f,    0.0f,  -200.0f, 0.0f, 0.0f,
             600.0f,    0.0f,  -200.0f, 5.0f, 0.0f,
             600.0f,    0.0f,  -600.0f, 0.0f, 0.0f,
             200.0f,    0.0f,  -600.0f, 5.0f, 0.0f,
             400.0f,  400.0f,  -400.0f, 2.5f, 5.0f,

               0.0f,    0.0f,     0.0f, 0.0f, 0.0f,
            1000.0f,    0.0f,     0.0f, 5.0f, 0.0f,
            1000.0f,    0.0f, -1000.0f, 5.0f, 5.0f,
               0.0f,    0.0f, -1000.0f, 0.0f, 5.0f,



        };

        GLuint indices[] =
        {
            // draw pyramid
            0, 1, 2,
            0, 2, 3,
            0, 1, 4,
            1, 2, 4,
            2, 3, 4,
            3, 0, 4,
        };

        GLuint landIndices[] = {
            // draw land
            5, 6, 7,
            5, 7, 8
        };


        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


        VertexArray va;
        VertexBuffer vb(positions, (4 + 5) * 5 * sizeof(float));
        IndexBuffer ib(indices, 18);
        IndexBuffer ibLand(landIndices, 6);

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);

        va.addBuffer(vb, layout);

        Shader shader("res/shaders/basic.shader");
        shader.Bind();


        Texture brickTexture("res/textures/bricks.jpg", GL_REPEAT);
        brickTexture.Bind();
        shader.setUniform1i("u_Texture", 0);
        //shader.setUniform1i("u_Texture", 1);

        Texture trekTexture("res/textures/trek.png", GL_REPEAT);
        //trekTexture.Bind(1);

        //unsigned int positionLocation = shader.getAttributeLocation("v_pos");
        //unsigned int texLocation = shader.getAttributeLocation("texCoord");

        ib.Unbind();
        ibLand.Unbind();
        vb.Unbind();
        shader.Unbind();

        Renderer renderer;

        double prevTime = glfwGetTime();

        float fova = 60.0f;
        float znear = 0.1f;
        float zfar = 2000.0f;

        glm::vec3 eye = glm::vec3(1200.0f, 1000.0f, 850.0f);
        glEnable(GL_DEPTH_TEST);

        Camera camera(width, height, eye, Camera::OrientationBottomLeft);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();
            // Simple timer
            double crntTime = glfwGetTime();
            if (crntTime - prevTime >= 1 / 60)
            {
                prevTime = crntTime;
            }
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Initializes matrices so they are not the null matrix
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 proj = glm::mat4(1.0f);

            // capture user inputs
            camera.Inputs(window);
            glm::mat4 cameraMatrix = camera.getCameraMatrix(fova, znear, zfar);
            glm::mat4 mvp = cameraMatrix * model;

            shader.Bind();
            shader.setUniformMat4f("u_MVP", mvp);

            shader.setUniform1i("u_Texture", 0);
            brickTexture.Bind();
            renderer.Draw(va, ib, shader);
            shader.setUniform1i("u_Texture", 1);
            trekTexture.Bind(1);
            renderer.Draw(va, ibLand, shader);

            {
                ImGui::SliderFloat3("EYE", &eye.x, -500.0f, 14000.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("FOV", &fova, 0.0f, 540.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("ZNear", &znear, -60.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("ZFar", &zfar, 10.0f, 2000.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }
            // Rendering
            ImGui::Render();
            //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}