#include <iostream>
#include "Renderer.h"

#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <fstream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

float width = 1920.0f;
float height = 1080.0f;

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


//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);
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

    float positions[] = {
        100.0f, 100.0f, 0.0f,  0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
        200.0f, 100.0f, 0.0f,  0.83f, 0.70f, 0.44f, 1.0f, 0.0f,
        200.0f, 100.0f, -100.0f,  0.83f, 0.70f, 0.44f, 1.0f, 1.0f,
        100.0f, 100.0f, -100.0f,  0.83f, 0.70f, 0.44f, 0.0f, 1.0f,
        150.0f, 200.0f, -50.0f, 0.92f, 0.86f, 0.76f, 0.5f, 1.0f,


        900.0f, 900.0f, 0.0f,  0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
        1000.0f, 900.0f, 0.0f,  0.83f, 0.70f, 0.44f, 1.0f, 0.0f,
        1000.0f, 900.0f, -100.0f,  0.83f, 0.70f, 0.44f, 1.0f, 1.0f,
        900.0f, 900.0f, -100.0f,  0.83f, 0.70f, 0.44f, 0.0f, 1.0f,
        950.0f, 1000.0f, -50.0f, 0.92f, 0.86f, 0.76f, 0.5f, 1.0f
    };                            
    //unsigned int indices[] = {
    //        0, 1, 2,
    //        0, 1, 3,
    //        0, 2, 3,
    //        1, 2, 3
    //};


    //GLfloat positions[] =
    //{ //     COORDINATES     /        COLORS      /   TexCoord  //
    //    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
    //    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
    //     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
    //     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	1.0f, 0.0f,
    //     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	0.5f, 1.0f
    //};

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4,

        
        5, 6, 7,
        5, 7, 8,
        5, 6, 9,
        6, 7, 9,
        7, 8, 9,
        8, 5, 9
    };


    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexBuffer vb(positions, 10 * 8 * sizeof(float));
    IndexBuffer ib(indices, 36);

 
    Shader shader("res/shaders/basic.shader");
    shader.Bind();


    Texture texture("res/textures/trek.png");
    texture.Bind();
    shader.setUniform1i("u_Texture", 0);

    unsigned int positionLocation = shader.getAttributeLocation("v_pos");
    unsigned int texLocation = shader.getAttributeLocation("texCoord");
    GLCall(glEnableVertexAttribArray(positionLocation));
    GLCall(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr));
    GLCall(glEnableVertexAttribArray(texLocation));
    GLCall(glVertexAttribPointer(texLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)24));



    ib.Unbind();
    vb.Unbind();
    shader.Unbind();

    Renderer renderer;

    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    float fova = 45.0f;
    float znear = 0.1f;
    float zfar = 2000.0f;

    glm::vec3 translation = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 translation1 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 translation2 = glm::vec3(-550.0f, -550.0f, -1100.0f);
    glEnable(GL_DEPTH_TEST);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        // Simple timer
        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1 / 60)
        {
            rotation += 0.5f;
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


        // Assigns different transformations to each matrix
        //model = glm::rotate(model, glm::radians(rotation), translation);
        //model = glm::translate(model, translation1);
        view = glm::translate(view, translation2);
        proj = glm::perspective(glm::radians(fova), (float)(width / height), znear, zfar);
        //proj = glm::ortho(0.0f, 300.0f, 0.0f, 300.0f, -60.0f, 60.0f);

        glm::mat4 mvp = proj * view * model;
        shader.Bind();
        shader.setUniformMat4f("u_MVP", mvp);
        renderer.Draw(vb, ib, shader);

        //model = glm::translate(model, glm::vec3(-200.0f, -100.0f, 0.0f));
        //mvp = proj * view * model;
        //shader.Bind();
        //shader.setUniformMat4f("u_MVP", mvp);
        //renderer.Draw(vb, ib, shader);

        {
            ImGui::SliderFloat3("rotate vector", &translation.x, 0.0f, 200.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat3("translate model", &translation1.x, -500.0f, 200.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat3("translate view", &translation2.x, -1200.0f, 200.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("FOV", &fova, 0.0f, 540.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("ZNear", &znear, -60.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("ZFar", &zfar, 10.0f, 2000.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("rotation", &rotation, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }


        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}