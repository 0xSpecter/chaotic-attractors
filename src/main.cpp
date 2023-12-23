#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <cmath>
#include "main.hpp"
#include "shaders.hpp"

int main()
{
    init();
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); 
    glfwMakeContextCurrent(window);
    gladLoadGL();

    GLfloat vertices[] = {
        0.0f,  0.5f, // Vertex 1 
        0.5f, -0.5f, // Vertex 2 
        -0.5f, -0.5f  // Vertex 3
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    std::cout << loadShader("fragment_shader.GLSL");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    cleanup();
    return 0;
}

void init()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

}


void cleanup()
{
    glfwTerminate();
}


void processInput(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}