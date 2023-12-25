#pragma once
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

class Texture
{   
    public:
        unsigned int ID;

        Texture(std::string imagePath, unsigned int RGB_CONFIG);
    
    private:
        void loadTexture(std::string imagePath, unsigned int RGB_CONFIG);
};
