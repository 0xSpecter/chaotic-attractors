#include "point.hpp"

Point::Point(glm::vec3 Position)
{
    Pos = Position;
}

float Point::magnitude()
{
    return glm::length(Pos);
}