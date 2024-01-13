#include "point.hpp"

Point::Point(glm::vec3 Position)
{
    Pos = Position;
}

float Point::magnitude()
{
    return glm::length(Pos);
}

void Point::trailCompute(glm::mat4 model)
{
    if (trail.size() >= capacity) trail.erase(trail.begin());
    trail.push_back(model);
}