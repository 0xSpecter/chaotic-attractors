#include "camera.hpp"

Camera::Camera()
{
    
}

glm::mat4 Camera::rotate()
{
    const float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;
    return glm::lookAt(glm::vec3(camX, 0.0, camZ), cameraTarget, cameraUp);
}

glm::mat4 Camera::lookAt()
{
    return glm::lookAt(cameraPos, cameraPos - cameraFront, cameraUp);
}

void Camera::processInput(GLFWwindow* window)
{
    const float cameraSpeed = 0.05f; 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}