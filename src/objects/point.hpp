#pragma once
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <vector>

class Point
{
    public:
        glm::vec3 Pos;
        int capacity = 100; // max lenght of trail

        Point(glm::vec3 Position);

        void addTrailPoint(glm::vec3 Position);

        float magnitude();
    
    private:
        std::vector<glm::vec3> trail;

};