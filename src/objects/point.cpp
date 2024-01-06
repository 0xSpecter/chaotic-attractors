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
    
}