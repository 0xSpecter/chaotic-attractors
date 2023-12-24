#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shaders.hpp"


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    const char* vShaderCode = loadShader(vertexPath);
    const char* fShaderCode = loadShader(fragmentPath);

    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() 
    { 
        glUseProgram(ID); 
    }

void Shader::setBool(const std::string &name, bool value)
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value)
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value)
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

const char* Shader::loadShader(const char* name)
{
    try {
        std::ifstream shaderFile(name);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();

        std::string tmp = shaderStream.str();
        char* shaderCode = new char[tmp.size() + 1];
        std::strcpy(shaderCode, tmp.c_str());

        return shaderCode;
    }
    catch (std::ifstream::failure& error) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << error.what() << std::endl;
        return "fuck";
    }
}