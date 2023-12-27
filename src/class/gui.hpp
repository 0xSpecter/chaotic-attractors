#pragma once
#include "../imgui/imgui.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"
#include <iostream>

#include <glm/glm.hpp>

// Gui class  !! INITIATE AFTER MAIN INIT FUNCTION !!
class Gui
{
    public:
        bool open = false;

        Gui(GLFWwindow* window, std::string GLSL_version = "#version 410");

        void newframe();

        void render(float* scalar);

        void ProcessInput();

    private:
        GLFWwindow* window;
};

