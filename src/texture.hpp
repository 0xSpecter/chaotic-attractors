#include <iostream>
#pragma once

class Texture
{   
    public:
        unsigned int ID;

        Texture(std::string imagePath, std::string imageType = "png");
    
    private:
        void loadTexture(std::string imagePath, std::string imageType);
};
