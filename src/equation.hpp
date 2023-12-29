#pragma once
#include <map>

enum equations 
{
    LORENZ,
    AIZAWA,
    CIRCLE,
    CUBE
};

struct datapoint
{
    float value;
    float min;
    float max;
};