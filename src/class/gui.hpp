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
        void renderSetup(bool* confirmed);

        void ProcessInput();

        void setParticles(Particles* ParticlesPtr);

        void setEquation(Equations tynewEquationpe);

    private:
        GLFWwindow* window;
        Camera* camera;
        
        Particles* ParticlesPtr;

        struct positionData
        {
            glm::vec3 Position;
            glm::vec3 Right;
            float Yaw;
            float Pitch;
        };
        

        std::map<Equations, positionData> optimalCameraPositions = {
            {LORENZ, {
                glm::vec3(0.0f, 0.0f, 95.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f), 
                -90.0f, 
                0.0f
            }},
            {AIZAWA, {
                glm::vec3(-4.67f, -0.14f, 0.33f),
                glm::vec3(0.03f, 0.0f, 0.99f), 
                4.0f, 
                2.0f
            }},
            {AIZAWA_CIRCLE, {
                glm::vec3(-0.528f, 0.611f, -3.235f),
                glm::vec3(-0.999f, 0.0f, -0.001f), 
                -285.0f, 
                -10.0f
            }},
            {CHEN, {
                glm::vec3(0.0f, 0.0f, 95.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f), 
                -90.0f, 
                0.0f
            }},
            {LUCHEN, {
                glm::vec3(0.0f, 0.0f, 95.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                -90.0f,
                0.0f
            }},
            {NEWTON_LEIPNIK, {
                glm::vec3(0.0f, 0.0f, 95.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                -90.0f,
                3.0f
            }},
            {NOSE_HOOVER, {
                glm::vec3(-11.17f, 5.219f, 15.39f),
                glm::vec3(0.844f, 0.0f, 0.535f),
                -90.0f,
                6.0f
            }},
            {HALVORSEN, {
                glm::vec3(12.67f, 16.55f, 15.107f),
                glm::vec3(0.742f, 0.0f, -0.669f),
                -135.0f,
                -32.0f
            }},
            {CHEN_LEE, {
                glm::vec3(-94.59f, 6.52f, 11.0f),
                glm::vec3(0.158f, 0.0f, 0.987f),
                -8.0f,
                -8.0f
            }},
            {BOUALI, {
                glm::vec3(41.86f, 13.89f, 52.87f),
                glm::vec3(0.798f, 0.0f, -0.601f),
                -130.0f,
                -8.0f
            }},
            {FINANCE, {
                glm::vec3(41.86f, 13.89f, 52.87f),
                glm::vec3(0.798f, 0.0f, -0.601f),
                -90.0f,
                0.0f
            }},
            {THOMAS, {
                glm::vec3(41.86f, 13.89f, 52.87f),
                glm::vec3(0.798f, 0.0f, -0.601f),
                -90.0f,
                0.0f
            }},
            {ROSSLER, {
                glm::vec3(41.86f, 13.89f, 52.87f),
                glm::vec3(0.798f, 0.0f, -0.601f),
                -90.0f,
                0.0f
            }},
            {CUBE, {
                glm::vec3(0.0f, 0.0f, 95.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                0.0f,
                0.0f
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
