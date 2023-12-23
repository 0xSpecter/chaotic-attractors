#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:
        unsigned int ID;
        Shader(const char* vertexPath, const char* fragmentPath) {

            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vertexShaderFile;
            std::ifstream fragmentShaderFile;

            vertexShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            fragmentShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

            try {
                vertexShaderFile.open(vertexPath);
                fragmentShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;

                vShaderStream << vertexShaderFile.rdbuf();
                fShaderStream << fragmentShaderFile.rdbuf();

                vertexShaderFile.close();
                fragmentShaderFile.close();

                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }

            catch (std::ifstream::failure& e) {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
            }

            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();
            
            unsigned int vertex, fragment;

            // vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            
            // fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);

            // shader Program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);

            // delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        // activate the shader
        void use() { 
            glUseProgram(ID); 
        }
        
        // utility uniform functions
        void setBool(const std::string &name, bool value) const {         
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
        }

        void setInt(const std::string &name, int value) const { 
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
        }

        void setFloat(const std::string &name, float value) const { 
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
        }
};
#endif
