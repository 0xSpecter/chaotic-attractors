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

class Camera
{
    public:
        glm::vec3 Position = glm::vec3(0.0f, 0.0f, 5.0f);
        glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 Up;
        glm::vec3 Right;

        // camera options
        float MovementSpeed = 2.5f;
        float MouseSensitivity = 0.1f;
        float Zoom = 45.0f;

        float RotateX = 0.0f;
        float RotateY = 0.0f; // is deg
        float RotateZ = 0.0f;

        Camera(GLFWwindow* window, float cameraInitalDistance = 5.0f);
        glm::mat4 GetViewMatrix();

        void ProcessInput(float deltaTime);
        void ProcessMouseInput(double xpos, double ypos, bool IgnoreMouse);

        float getZoom();

    private:
        GLFWwindow* window;
        glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        // euler Angles
        float Yaw = -90.0f;
        float Pitch = 0.0f;

        // mouse values
        bool firstMouseMovement = true;
        double lastX;
        double lastY;

        void updateCameraVectors();
};