class Gui;
#pragma once
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <vector>

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
        Shader shader;

        float Scale = 1.0f;
        float Speed = 1.0f;
        float PointSize = 1.0f;
        int LossCount = 0;

        bool doCull = true;


        std::map<std::string, Constant> constants;

        unsigned int equation = 0;

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
                {0.175f, 0.0f, 5.0f},
                {10.0f, 0.0f, 100.0f},
                {5.0f, 0.0f, 50.0f}
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
            {CUBE, {}} 
        };

        const char* equationNames[12] {
            "Lorenz",
            "Aizawa",
            "Aizawa Circle",
            "Chen",
            "Lu Chen",
            "Newton Leipnik",
            "Nose Hoover",
            "Halvorsen",
            "Chen Lee",
            "Cube"
        };


        Particles(Gui* gui, float minmax = 0.17f, float step = 0.001f);

        void renderPoints(float deltatime);

    private:
        Gui* guiPtr;

        float point[3] = { -0.5f, -0.5f, -0.5f };
        unsigned int VBO, VAO;
};