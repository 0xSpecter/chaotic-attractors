#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
bool update_i(float i, bool backtrack);
float sin_of_i(float i);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "yeha baby", NULL, NULL);
    if (window == NULL) {
        std::cout << "bitch you done fuck it all up! with window spice" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL()) { // can be exchanged with: gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)
        std::cout << "bitch you done fuck it all up! with glad spice" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float i = 0.1f;
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();    

        i += 0.01f;
        glClearColor(0.0f, sin_of_i(i), 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

float sin_of_i(float i) {
    if (std::sinf(i) < 0) {
        return std::sinf(-i);
    }
    else {
        return std::sinf(i);
    }
}
