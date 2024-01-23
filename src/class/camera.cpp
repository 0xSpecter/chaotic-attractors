#include "camera.hpp"


Camera::Camera(GLFWwindow* window, float cameraInitalDistance)
{
    Camera::window = window;
    Position.z = cameraInitalDistance;

    updateCameraVectors();
}

// returns the view matrix
glm::mat4 Camera::GetViewMatrix()
{
    glm::mat4 view = glm::lookAt(Position, Position + Front, Up);
    view = glm::rotate(view, glm::radians(RotateX), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(RotateY), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, glm::radians(RotateZ), glm::vec3(0.0f, 0.0f, 1.0f));

    if (doPanRot) 
    {
        view = glm::rotate(prevView, glm::radians(panSpeed), glm::vec3(panAxie[0], panAxie[1], panAxie[2]));
        
        prevView = view;
        
        view = glm::translate(view, glm::vec3(panOffset[0], panOffset[1], panOffset[2]));
    } else prevView = view;

    return view;
}

// processes keyboard input
void Camera::ProcessInput(float deltaTime)
{
    float sprint = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ? 20.0 : (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 3.0 : 1.0));
    float velocity = deltaTime * MovementSpeed * sprint;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) Position += Front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) Position -= Front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) Position -= Right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) Position += Right * velocity;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) Zoom -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) Zoom += 1.0f;

    if (Zoom < 0.5f) Zoom = 0.5f;
    else if (Zoom > 75.0f) Zoom = 75.0f;
}

// processes mouse input
void Camera::ProcessMouseInput(double xpos, double ypos, bool IgnoreMouse)
{   
    if (IgnoreMouse) {
        firstMouseMovement = true;
        return;
    }

    if (firstMouseMovement) {
        lastX = xpos;
        lastY = ypos;
        firstMouseMovement = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f) Pitch = 89.0f;
    else if (Pitch < -89.0f) Pitch = -89.0f;

    updateCameraVectors();
}

// update values
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

float Camera::getZoom()
{
    return Zoom;
}