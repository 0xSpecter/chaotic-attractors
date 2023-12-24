#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "texture.hpp"
#include "stb_image.h"

Texture::Texture(std::string imagePath, std::string imageType)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    loadTexture(imagePath, imageType);
}

void Texture::loadTexture(std::string imagePath, std::string imageType)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);
    
    if (data) {
        stbi_set_flip_vertically_on_load(true);  
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (imageType.compare("png") ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}