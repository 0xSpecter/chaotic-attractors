#pragma once
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <vector>

#include "../class/shaders.hpp"

class Point
{
    public:
        glm::vec3 Pos;

        int capacity = 200; // max lenght of trail
        std::vector<float> trail;

        Point(glm::vec3 Position);
        
        void trailCompute();
        float magnitude();
    
    private:
        
};