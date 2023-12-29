#pragma once
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float FOV         =  45.0f;

class Camera
{
    public:
        Camera(GLFWwindow* window, float cameraInitalDistance = 5.0f);
        glm::mat4 GetViewMatrix();

        void ProcessInput(float deltaTime);
        void ProcessMouseInput(double xpos, double ypos, bool IgnoreMouse);
        void ProcessMouseScroll(double yoffset);

        float getFov();

    private:
        GLFWwindow* window;

        glm::vec3 Position = glm::vec3(0.0f, 0.0f, 5.0f);
        glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 Up;
        glm::vec3 Right;

        // euler Angles
        float Yaw = YAW;
        float Pitch = PITCH;

        // camera options
        float MovementSpeed = SPEED;
        float MouseSensitivity = SENSITIVITY;
        float Fov = FOV;
            // mouse values
            bool firstMouseMovement = true;
            double lastX;
            double lastY;

        void updateCameraVectors();
};