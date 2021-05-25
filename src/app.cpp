#include <iostream>
#include "Renderer.h"

#include "VertexArray.h"

#include "Texture.h"

#include "Camera.h"

#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <array>
#include "Shapes.cpp"

unsigned int width = 1920;
unsigned int height = 1080;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static float getRand(float start = 0.0f, float end = RAND_MAX)
{
    return std::rand() % (int)(end - start) + start;    
}

int main(void)
{
    glfwSetErrorCallback(glfw_error_callback);

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

    std::srand(std::time(nullptr));

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

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        const size_t maxQuadCount = 1000;
        const size_t maxVertexCount = maxQuadCount * 8;
        const size_t maxIndexCount = maxQuadCount * 36;


        VertexArray va;
//        VertexBuffer vb(positions, (4 + 5) * 6 * sizeof(float));
        VertexBuffer vb(nullptr, sizeof(Vertex) * maxVertexCount, GL_DYNAMIC_DRAW);

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        layout.Push<float>(1);

        va.addBuffer(vb, layout);

        Shader shader("res/shaders/basic.shader");
        shader.Bind();


        Texture brickTexture("res/textures/bricks.jpg", GL_REPEAT);
        brickTexture.Bind(1);
        Texture grassTexture("res/textures/grass.jpg", GL_REPEAT);
        grassTexture.Bind(2);
        Texture trekTexture("res/textures/trek.png", GL_REPEAT);
        trekTexture.Bind(3);
        Texture palmBarkTexture("res/textures/palm-bark.jpg", GL_REPEAT);
        palmBarkTexture.Bind(4);
        Texture palmLeafTexture("res/textures/palm-leaf.jpg", GL_CLAMP_TO_EDGE);
        palmLeafTexture.Bind(5);
        Texture houseTexture("res/textures/house.jpg", GL_CLAMP_TO_EDGE);
        houseTexture.Unbind();
        houseTexture.Bind(6);
        int samplers[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

        shader.setUniform1iv("u_Textures", 8, samplers);

        vb.Unbind();
        shader.Unbind();

        Renderer renderer;

        double prevTime = glfwGetTime();

        float fova = 45.0f;
        float znear = 0.1f;
        float zfar = 3000.0f;


        glm::vec3 eye = glm::vec3(1200.0f, 1000.0f, 850.0f);
        glEnable(GL_DEPTH_TEST);

        Camera camera(width, height, eye, Camera::OrientationBottomLeft);
        // generate specifications for our houses to be drawn
        float housesX[10];
        float housesHeight[10];
        for (unsigned int i = 0; i < 10; i++) {
            housesX[i] = getRand(600, 900);
            housesHeight[i] = getRand(50, 150);
        }
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            unsigned int vertexSize = 0;
            unsigned int indexCount = 0;
            std::array<Vertex, maxVertexCount> vertices;
            std::array<unsigned int, maxIndexCount> indices;
            Vertex* buffer = vertices.data();
            unsigned int* indexBuffer = indices.data();

            for (unsigned int i = 0; i < 10; i++) 
            {
                buffer = CreateCube(buffer, housesX[i], 0.0f, -50.0f - (i * 100), housesHeight[i], glm::vec3(1.0f), 6);
                indexBuffer = CreateCubeIndices(indexBuffer, vertexSize);
                vertexSize += 8;
                indexCount += 36;
                
            }
            // draw star trek cubes :)
            for (unsigned int i = 0; i < 7; i++) {
                buffer = CreateCube(buffer, 150.0f * i, 0.0f, 50.0f, 100.0f, glm::vec3(1.0f), 3);
                indexBuffer = CreateCubeIndices(indexBuffer, vertexSize);
                vertexSize += 8;
                indexCount += 36;
                
            }

            for (unsigned int i = 0; i < 6; i++) {
                // draw tree trunk
                float depth = -200.0f - (i * 100.0f);
                buffer = CreateCuboid(buffer, glm::vec3(100.0f, 0.0f, depth), glm::vec3(110.0f, 200.0f, depth-10.0f), glm::vec3(1.0f), 4);
                indexBuffer = CreateCubeIndices(indexBuffer, vertexSize);
                vertexSize += 8;
                indexCount += 36;
                
                // draw tree top
                depth = -170.0f - (i * 100.0f);
                buffer = CreateCuboid(buffer, glm::vec3(70.0f, 200.0f, depth), glm::vec3(140.0f, 210.0f, depth-70.0f), glm::vec3(1.0f), 5);
                indexBuffer = CreateCubeIndices(indexBuffer, vertexSize);
                vertexSize += 8;
                indexCount += 36;

            }  
            
            for (unsigned int i = 0; i < 8; i++) {
                // draw tree trunk
                float x = 100.0f + (i * 100.0f);
                buffer = CreateCuboid(buffer, glm::vec3(x, 0.0f, -900.0f), glm::vec3(x + 10.0f, 200.0f, -910.0f), glm::vec3(1.0f), 4);
                indexBuffer = CreateCubeIndices(indexBuffer, vertexSize);
                vertexSize += 8;
                indexCount += 36;
                
                // draw tree top
                x = 70.0f + (i * 100.0f);
                buffer = CreateCuboid(buffer, glm::vec3(x, 200.0f, -870.0f), glm::vec3(x + 70.0f, 210.0f, -940.0f), glm::vec3(1.0f), 5);
                indexBuffer = CreateCubeIndices(indexBuffer, vertexSize);
                vertexSize += 8;
                indexCount += 36;

            }

            {   //draw the big pyramid
                buffer = CreatePyramid(buffer, 200.0f, 0.0f, -200.0f, 400.0f, 400.0f, glm::vec3(1.0f), 1);
                indexBuffer = CreatePyramidIndices(indexBuffer, vertexSize);
                vertexSize += 5;
                indexCount += 18;
            }  
            {
                // draw the ground
                buffer = CreatePlane(buffer, 0.0f, 0.0f, 0.0f, 1000.0f, glm::vec3(1.0f), 2.0f);
                indexBuffer = CreatePlaneIndices(indexBuffer, vertexSize);
                vertexSize += 4;
                indexCount += 6;
            }



            IndexBuffer ib(indices.data(), indices.size());
            ib.Bind();
            // set dynamic vertex buffer
            vb.Bind();
            //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions2), positions2);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
            

            // clear the screen
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
            // draw big pyramid
            shader.Bind();
            shader.setUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
            //renderer.Draw(va, ibQuad, shader);


            {
                ImGui::SliderFloat3("EYE", &eye.x, -500.0f, 14000.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("FOV", &fova, 0.0f, 540.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("ZNear", &znear, -60.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("ZFar", &zfar, 10.0f, 2000.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }
            // Rendering
            ImGui::Render();
//            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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