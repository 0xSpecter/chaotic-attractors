class Particles;
#pragma once
#include "../imgui/imgui.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "camera.hpp"

#include "GLFW/glfw3.h"
#include <iostream>
#include <map>
#include <stdio.h>

#include <glm/glm.hpp>

#include "../equation.hpp"
#include "../objects/point.hpp"
#include "../objects/particles.hpp"

// Gui class  !! INITIATE AFTER MAIN INIT FUNCTION !!
class Gui
{
    public:
        bool open = false;
        bool attractorSelect = true;
        bool cameraConfig = false;
        bool graphicsConfig = false;

        bool constantsOpen = false;

        ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);


        Gui(GLFWwindow* window, Camera* camera, std::string GLSL_version = "#version 410");

        void newframe();

        void render();

        void ProcessInput();

        void setParticles(Particles* ParticlesPtr);

        void setEquation(Equations tynewEquationpe);

        void updateScalingConstants();

    private:
        GLFWwindow* window;
        Camera* camera;
        
        Particles* ParticlesPtr;

        void renderConstants();
        void renderAttractorSelect();
        void renderCameraConfig();
        void renderGraphicsConfig();
};
