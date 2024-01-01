#include "point.hpp"

Point::Point(glm::vec3 Position)
{
    Pos = Position;
}

void Point::addTrailPoint()
{
    if (trail.size() >= capacity) trail.erase(trail.begin());
    
    trail.push_back(Pos);
}

float Point::magnitude()
{
    return Pos.length();
}