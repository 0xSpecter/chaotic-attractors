class Gui;
#pragma once
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <vector>
#include <math.h>

#include "point.hpp"
#include "../class/gui.hpp"
#include "../class/shaders.hpp"

struct Constant
{
    float value;
    float min;
    float max;
    float inital;
    
    bool scaling;
    float scalingSpeed;
};

struct datapoint
{
    float value;
    float min;
    float max;
};

class Particles
{
    public:
        std::vector<Point> Points;
        std::vector<Point> PointsInital;
        Shader particleShader;
        Shader trailShader;

        float Scale = 1.0f;
        float Speed = 1.0f;
        bool Paused = false;
        float PointSize = 1.0f;
        int LossCount = 0;

        bool doCull = true;
        bool doRenderPoints = false;
        bool doRenderTrails = true;

        float setMinmax;
        float setStep;

        glm::vec4 from_color = glm::vec4(0.0, 0.8157, 0.8824, 1.0);
        glm::vec4 to_color = glm::vec4(0.9373, 0.5882, 0.9137, 1.0);
        float fromScale = 150;

        std::map<std::string, Constant> constants;

        Equations equation = LORENZ;

        std::map<Equations, std::vector<datapoint>> equationConstants = {
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
            {AIZAWA_CIRCLE, {
                {1.9f, 0.0f, 10.0f},
                {1.4f, 0.0f, 10.0f},
                {1.2f, 0.0f, 10.0f},
                {7.0f, 0.0f, 100.0f},
                {0.5f, 0.0f, 10.0f},
                {0.2f, 0.0f, 10.0f}
            }},
            {CHEN, {
                {40.0f, 0.0f, 100.0f},
                {3.0f, 0.0f, 10.0f},
                {28.0f, 0.0f, 100.0f}
            }},
            {LUCHEN, {
                {36.0f, 0.0f, 100.0f},
                {3.0f, 0.0f, 10.0f},
                {20.0f, 0.0f, 100.0f},
                {-15.15f, -50.0f, 50.0f}
            }},
            {NEWTON_LEIPNIK, {
                {0.4f, 0.0f, 10.0f},
                {0.175f, 0.0f, 5.0f}
            }},
            {NOSE_HOOVER, {
                {1.5f, 0.0f, 10.0f},
            }},
            {HALVORSEN, {
                {1.4f, 0.0f, 10.0f},
            }},
            {CHEN_LEE, {
                {5.0f, 0.0f, 50.0f},
                {-10.0f, -50.0f, 50.0f},
                {-0.38f, -1.0f, 1.0f},
            }},
            {BOUALI, {
                {0.3f, 0.0f, 10.0f},
                {1.0f, 0.0f, 10.0f},
            }},
            {FINANCE, {
                {0.001f, 0.0f, 0.1f},
                {0.2f, 0.0f, 1.0f},
                {1.1f, 0.0f, 10.0f}
            }},
            {THOMAS, {
                {0.19f, 0.0f, 1.0f},
            }},
            {ROSSLER, {
                {0.2f, 0.0f, 1.0f},
                {0.2f, 0.0f, 1.0f},
                {5.7f, 0.0f, 25.0f},
            }},
            {DADRAS, {
                {3.0f, 0.0f, 10.0f},
                {2.7f, 0.0f, 10.0f},
                {1.7f, 0.0f, 10.0f},
                {2.0f, 0.0f, 10.0f},
                {9.0f, 0.0f, 100.0f},
            }},
            {RABINOVICH_FABRIKANT, {
                {0.14f, 0.0f, 1.0f},
                {0.10f, 0.0f, 1.0f},
            }},
            {THREE_SCROLL_UNIFIED_CHAOTIC_SYSTEM, {
                {32.48f, 0.0f, 100.0f},
                {45.84f, 0.0f, 100.0f},
                {1.18f, 0.0f, 10.0f},
                {0.13f, 0.0f, 1.0f},
                {0.57f, 0.0f, 1.0f},
                {14.7f, 0.0f, 100.0f},
            }},
            {SPROTT, {
                {2.07f, 0.0f, 10.0f},
                {1.79f, 0.0f, 10.0f},
            }},
            {FOUR_WING, {
                {0.2f, 0.0f, 1.0f},
                {0.01f, 0.0f, 1.0f},
                {-0.4f, -1.0f, 1.0f},
            }},
            {CUBE, {}} 
        };

        const char* equationInfo[20][2] {
            {"Lorenz", "Classic three-dimensional attractor known for its butterfly shape."},
            {"Aizawa", "Compact attractor with intriguing patterns and smaller spatial footprint."},
            {"Aizawa Circle", "Circular variant of the Aizawa attractor, forming distinct circular trajectories."},
            {"Chen", "Dynamic attractor with potential need for speed adjustment to reveal intricate details."},
            {"Lu Chen", "Complex attractor that may benefit from speed adjustment for enhanced visualization."},
            {"Newton Leipnik", "Fascinating attractor exhibiting unique geometric patterns."},
            {"Nose Hoover", "Intricately structured attractor with captivating and varied trajectories."},
            {"Halvorsen", "Attractor known for producing triangular patterns and interesting spatial dynamics."},
            {"Chen Lee", "Attractor with nuanced behavior that requires some observation time."},
            {"Bouali", "Memory-laden attractor, details are not easily recalled."},
            {"Finance", "Financially-inspired attractor with patterns resembling market fluctuations."},
            {"Thomas", "Attractor that might benefit from increased initial speed, exhibiting gradual unfolding."},
            {"Rössler", "Classic attractor characterized by its simple yet captivating dynamics."},
            {"Dadras", "Details about this attractor are to be determined."},
            {"Rabinovich Fabrikant", "Details about this attractor are yet to be explored."},
            {"Three Scroll Unified Chaotic System", "Details about this attractor are currently under investigation."},
            {"Sprott", "Details about this attractor are yet to be discovered."},
            {"Four Wing", "Details about this attractor are awaiting exploration."},
            {"Cube", "Bruv is tha MATRIX!"}
        };


        Particles(Gui* gui, float minmax = 0.017f, float step = 0.0008f);

        void renderPoints(float deltatime);
        void definePoints(float minmax, float step);

        void assignUniforms(glm::mat4 view, glm::mat4 projection);

        void clean();

    private:
        Gui* guiPtr;

        unsigned int particleVBO, particleVAO, trailVBO, trailVAO;
        std::vector<float> vectorVertices;

        void movePointByEquation(float timestep, Point* point);
        void updateScalingConstants();
        std::vector<float> getSphereVertices();

        void configureShaderMatrix(unsigned int location = 0);
};