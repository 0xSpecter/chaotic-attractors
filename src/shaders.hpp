#pragma once
#include <glad/glad.h>
#include <string>

class Shader
{
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath);

        void use();
        
        void setBool(const std::string& name, bool value);
        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);

    private:
        void checkCompileErrors(unsigned int shader, std::string type);
        const char* loadShader(const char* name);
};