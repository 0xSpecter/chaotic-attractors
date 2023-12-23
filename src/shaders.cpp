#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// "vertex_shader.GLSL"
std::string loadShader(const char* name)
{
    std::string shaderCode;
    std::ifstream shaderFile;
    std::stringstream shaderStream;

    shaderFile.open(name);
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    shaderCode = shaderStream.str();
    return shaderCode;
}