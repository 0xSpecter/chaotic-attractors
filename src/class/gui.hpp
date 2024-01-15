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
        bool worldTransform = false;

        ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);


        Gui(GLFWwindow* window, Camera* camera, std::string GLSL_version = "#version 410");

        void newframe();

        void render(float deltatime);

        void ProcessInput();

        void setParticles(Particles* ParticlesPtr);

        void setEquation(Equations tynewEquationpe);

    private:
        GLFWwindow* window;
        Camera* camera;
        
        Particles* ParticlesPtr;

        std::map<Equations, std::map<std::string, glm::vec3>> optimalCameraPositions = {
            {LORENZ, {
                {"Position", glm::vec3(0.0f, 0.0f, 95.0f)},
                {"Front", glm::vec3(0.0f, 0.0f, -1.0f)},
                {"Right", glm::vec3(-1.0f, 0.0f, 0.0f)}
            }},
            {AIZAWA, {
                {"Position", glm::vec3(-4.67f, -0.14f, 0.33f)},
                {"Front", glm::vec3(0.995f, -0.078f, -0.056f)},
                {"Right", glm::vec3(0.03f, 0.0f, 0.99f)}
            }},
            {AIZAWA_CIRCLE, {
                {"Position", glm::vec3(-0.528f, 0.611f, -3.235f)},
                {"Front", glm::vec3(-0.001f, -0.225f, 0.974f)},
                {"Right", glm::vec3(-0.999f, 0.0f, -0.001f)}
            }},
            {CHEN, {
                {"Position", glm::vec3(0.0f, 0.0f, 95.0f)},
                {"Front", glm::vec3(0.0f, 0.0f, -1.0f)},
                {"Right", glm::vec3(-1.0f, 0.0f, 0.0f)}
            }},
            {LUCHEN, {
                {"Position", glm::vec3(0.0f, 0.0f, 95.0f)},
                {"Front", glm::vec3(0.0f, 0.0f, -1.0f)},
                {"Right", glm::vec3(-1.0f, 0.0f, 0.0f)}
            }},
            {NEWTON_LEIPNIK, {
                {"Position", glm::vec3(0.0f, 0.0f, 95.0f)},
                {"Front", glm::vec3(0.0f, 0.0f, -1.0f)},
                {"Right", glm::vec3(-1.0f, 0.0f, 0.0f)}
            }},
            {NOSE_HOOVER, {
                {"Position", glm::vec3(-11.17f, 5.219f, 15.39f)},
                {"Front", glm::vec3(0.509f, -0.307f, -0.803f)},
                {"Right", glm::vec3(0.844f, 0.0f, 0.535f)}
            }},
            {HALVORSEN, {
                {"Position", glm::vec3(12.67f, 16.55f, 15.107f)},
                {"Front", glm::vec3(0.53f, -0.596f, -0.596f)},
                {"Right", glm::vec3(0.742f, 0.0f, -0.669f)}
            }},
            {CHEN_LEE, {
                {"Position", glm::vec3(-94.59f, 6.52f, 11.0f)},
                {"Front", glm::vec3(0.984f, -0.078f, -0.157f)},
                {"Right", glm::vec3(0.158f, 0.0f, 0.987f)}
            }},
            {BOUALI, {
                {"Position", glm::vec3(41.86f, 13.89f, 52.87f)},
                {"Front", glm::vec3(-0.600f, -0.074f, -0.796f)},
                {"Right", glm::vec3(0.798f, 0.0f, -0.601f)}
            }},
            {FINANCE, {
                {"Position", glm::vec3(41.86f, 13.89f, 52.87f)},
                {"Front", glm::vec3(-0.600f, -0.074f, -0.796f)}, // tbd
                {"Right", glm::vec3(0.798f, 0.0f, -0.601f)}
            }},
            {THOMAS, {
                {"Position", glm::vec3(41.86f, 13.89f, 52.87f)},
                {"Front", glm::vec3(-0.600f, -0.074f, -0.796f)}, // tbd
                {"Right", glm::vec3(0.798f, 0.0f, -0.601f)}
            }},
            {ROSSLER, {
                {"Position", glm::vec3(41.86f, 13.89f, 52.87f)},
                {"Front", glm::vec3(-0.600f, -0.074f, -0.796f)}, // tbd
                {"Right", glm::vec3(0.798f, 0.0f, -0.601f)}
            }},
            {CUBE, {
                {"Position", glm::vec3(0.0f, 0.0f, 95.0f)},
                {"Front", glm::vec3(0.0f, 0.0f, -1.0f)},
                {"Right", glm::vec3(-1.0f, 0.0f, 0.0f)}
            }}
        };


        struct BlendFactor {
            GLenum value;
            const char* name;
        };

        BlendFactor currentSfactor = {GL_SRC_ALPHA, "GL_SRC_ALPHA"};
        std::vector<BlendFactor> sfactorOptions = {
            {GL_ZERO, "GL_ZERO"},
            {GL_ONE, "GL_ONE"},
            {GL_SRC_COLOR, "GL_SRC_COLOR"},
            {GL_ONE_MINUS_SRC_COLOR, "GL_ONE_MINUS_SRC_COLOR"},
            {GL_DST_COLOR, "GL_DST_COLOR"},
            {GL_ONE_MINUS_DST_COLOR, "GL_ONE_MINUS_DST_COLOR"},
            {GL_SRC_ALPHA, "GL_SRC_ALPHA"},
            {GL_ONE_MINUS_SRC_ALPHA, "GL_ONE_MINUS_SRC_ALPHA"},
            {GL_DST_ALPHA, "GL_DST_ALPHA"},
            {GL_ONE_MINUS_DST_ALPHA, "GL_ONE_MINUS_DST_ALPHA"},
            {GL_CONSTANT_COLOR, "GL_CONSTANT_COLOR"},
            {GL_ONE_MINUS_CONSTANT_COLOR, "GL_ONE_MINUS_CONSTANT_COLOR"},
            {GL_CONSTANT_ALPHA, "GL_CONSTANT_ALPHA"},
            {GL_ONE_MINUS_CONSTANT_ALPHA, "GL_ONE_MINUS_CONSTANT_ALPHA"},
            {GL_SRC_ALPHA_SATURATE, "GL_SRC_ALPHA_SATURATE"}
        };

        BlendFactor currentDfactor = {GL_ONE, "GL_ONE"};
        std::vector<BlendFactor> dfactorOptions = {
            {GL_ZERO, "GL_ZERO"},
            {GL_ONE, "GL_ONE"},
            {GL_SRC_COLOR, "GL_SRC_COLOR"},
            {GL_ONE_MINUS_SRC_COLOR, "GL_ONE_MINUS_SRC_COLOR"},
            {GL_DST_COLOR, "GL_DST_COLOR"},
            {GL_ONE_MINUS_DST_COLOR, "GL_ONE_MINUS_DST_COLOR"},
            {GL_SRC_ALPHA, "GL_SRC_ALPHA"},
            {GL_ONE_MINUS_SRC_ALPHA, "GL_ONE_MINUS_SRC_ALPHA"},
            {GL_DST_ALPHA, "GL_DST_ALPHA"},
            {GL_ONE_MINUS_DST_ALPHA, "GL_ONE_MINUS_DST_ALPHA"},
            {GL_CONSTANT_COLOR, "GL_CONSTANT_COLOR"},
            {GL_ONE_MINUS_CONSTANT_COLOR, "GL_ONE_MINUS_CONSTANT_COLOR"},
            {GL_CONSTANT_ALPHA, "GL_CONSTANT_ALPHA"},
            {GL_ONE_MINUS_CONSTANT_ALPHA, "GL_ONE_MINUS_CONSTANT_ALPHA"}
        };

        void renderConstants();
        void renderAttractorSelect();
        void renderCameraConfig();
        void renderGraphicsConfig();
        void renderWorldTransform();
};
