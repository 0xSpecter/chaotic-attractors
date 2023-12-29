#pragma once
#include "../imgui/imgui.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"
#include <iostream>
#include <map>
#include <stdio.h>

#include <glm/glm.hpp>

#include "../equation.hpp"

// Gui class  !! INITIATE AFTER MAIN INIT FUNCTION !!
class Gui
{
    public:
        bool open = false;

        bool constantsOpen = false;
        struct Constant
        {
            float value;
            float min;
            float max;
            float inital;
            
            bool scaling;
            float scalingSpeed;
        };
        std::map<std::string, Constant> constants;

        bool doCull = true;

        unsigned int equation = 0;


        Gui(GLFWwindow* window, std::string GLSL_version = "#version 410");

        void newframe();

        void render(float* scalar, float* speed, int* lossCount, float* pointSize);

        void ProcessInput();

        void setPointsArray(std::vector<glm::vec3>* PointsRef);

        void setEquation(equations tynewEquationpe);

        void updateScalingConstants();

    private:
        GLFWwindow* window;
        std::vector<glm::vec3>* Points;
        std::vector<glm::vec3> PointsInital;

        std::map<equations, std::vector<datapoint>> equationConstants = {
            {LORENZ, {
                {10.0f, 0.0f, 100.0f},
                {28.0f, 0.0f, 100.0f}
            }},
            {AIZAWA, {
                {0.95f, 0.0f, 10.0f},
                {0.7f, 0.0f, 10.0f},
                {0.6f, 0.0f, 10.0f},
                {3.5f, 0.0f, 100.0f},
                {0.25f, 0.0f, 10.0f},
                {0.1f, 0.0f, 10.0f}
            }},
            {CIRCLE, {
                {1.9f, 0.0f, 10.0f},
                {1.4f, 0.0f, 10.0f},
                {1.2f, 0.0f, 10.0f},
                {7.0f, 0.0f, 100.0f},
                {0.5f, 0.0f, 10.0f},
                {0.2f, 0.0f, 10.0f}
            }},
            {CUBE, {}} 
        };
};

/*
                {0.95f, 0.0f, 10.0f},
                {0.7f, 0.0f, 10.0f},
                {0.6f, 0.0f, 10.0f},
                {3.5f, 0.0f, 100.0f},
                {0.25f, 0.0f, 10.0f},
                {0.1f, 0.0f, 10.0f}
*/

