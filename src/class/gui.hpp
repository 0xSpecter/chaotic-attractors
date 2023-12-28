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

        void render(float* scalar, float* speed, int* lossCount, float* pointSize);

        void ProcessInput();

        void setPointsArray(std::vector<glm::vec3>* PointsRef);

    private:
        GLFWwindow* window;
        std::vector<glm::vec3>* Points;
        std::vector<glm::vec3> PointsInital;
};

