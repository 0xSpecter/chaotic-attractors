#include "main.hpp"

const float WIDTH = 1300.0f;
const float HEIGHT = 850.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLFWwindow* window = init();

Camera camera(window, 95.0f);
Gui gui(window, &camera);
Particles particles(&gui, 0.0001, 0.0001); // 1 point with (0.0001, 0.0002), 300 with (0.001, 0.0003), 64000 with (0.001, 0.00005) -_-

int main()
{    
    glLineWidth(10.0);

    gui.setParticles(&particles);
    gui.setEquation(LORENZ);
    
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();
        gui.newframe();

        glClearColor(gui.clearColor.x, gui.clearColor.y, gui.clearColor.z, gui.clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::mat4(1.0f); 
        projection = glm::perspective(glm::radians(camera.getZoom()), WIDTH / HEIGHT, 0.01f, 100000.0f);

        particles.assignUniforms(camera.GetViewMatrix(), projection);

        particles.renderPoints(deltaTime);

        gui.render(deltaTime);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    particles.clean();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

GLFWwindow* init()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Chaotic Attractors", nullptr, nullptr); 
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetCursorPosCallback(window, mouse_callback);

    gladLoadGL();

    glEnable(GL_DEPTH_TEST);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

void processInput() 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    camera.ProcessInput(deltaTime);
    gui.ProcessInput();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.ProcessMouseInput(xpos, ypos, gui.open);
}