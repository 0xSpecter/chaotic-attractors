#pragma once
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "class/shaders.hpp"
#include "class/camera.hpp"
#include "class/gui.hpp"
#include "objects/point.hpp"
#include "objects/particles.hpp"

#include "equation.hpp"

void Launcher();
void Mainloop();

GLFWwindow* init();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput();