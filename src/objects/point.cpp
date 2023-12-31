#include "point.hpp"

Point::Point(glm::vec3 Position)
{
    Pos = Position;
}

void Point::addTrailPoint(glm::vec3 Position)
{
    if (trail.size() >= capacity) trail.erase(trail.begin());
    
    trail.push_back(Position);
}

float Point::magnitude()
{
    return Pos.length();
}