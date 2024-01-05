#include "point.hpp"

Point::Point(glm::vec3 Position)
{
    Pos = Position;
}

float Point::magnitude()
{
    return glm::length(Pos);
}

void Point::addTrailPoint()
{
    if (trail.size() >= capacity) trail.erase(trail.begin());
    
    trail.push_back(Pos);
}

// bruh this this is outdated AS fuck
void Point::renderTrail(Shader* shader)
{
    glLineWidth(5.0f);
    for(unsigned int i = 0; i < trail.size(); i++)
    {
        glVertex3f(trail[i].x, trail[i].y, trail[i].z);
    }
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, trail[0]);
    
    shader->setMat4("model", model);
}