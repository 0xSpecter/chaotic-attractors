#include <iostream>
#pragma once

class Texture
{   
    public:
        unsigned int ID;

        Texture(std::string imagePath, unsigned int RGB_CONFIG);
    
    private:
        void loadTexture(std::string imagePath, unsigned int RGB_CONFIG);
};
