#include "main.hpp"
#include <vector>

const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLFWwindow* window = init();

Camera camera(window, 100.0f);
Shader shader("shaders/shader.vert", "shaders/shader.frag");
Gui gui(window);

enum equations {
    lorenze,
    cube
};

int main()
{
    float point[] = {
        -0.5f, -0.5f, -0.5f,  
    };

    std::vector<glm::vec3> Points;
    
    if (true) {
        for(float vx = -5.0f; vx < 5.0f; vx += 0.3f) {
            for(float vy = -5.0f; vy < 5.0f; vy += 0.3) {
                for(float vz = -5.0f; vz < 5.0f; vz += 0.3f) {
                    Points.push_back(glm::vec3(vx, vy, vz));
                }
            }
        }
    } else {
        for(float vx = -2.0f; vx < 2.0f; vx += 1.0f) {
            for(float vy = -2.0f; vy < 2.0f; vy += 1.0) {
                for(float vz = -2.0f; vz < 2.0f; vz += 1.0f) {
                    Points.push_back(glm::vec3(vx, vy, vz));
                }
            }
        }
    }
    

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader.use();

    float scalar = 1.0f;
    float speed = 1.0f;
    float pointSize = 1.0f;
    int lossCount = 0;

    gui.setPointsArray(&Points);

    gui.addConstant("f", 28.0, 0.0, 100.0);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();
        gui.newframe();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("view", camera.GetViewMatrix());

        glm::mat4 projection = glm::mat4(1.0f); 
        projection = glm::perspective(glm::radians(camera.getFov()), WIDTH / HEIGHT, 0.1f, 10000.0f);
        shader.setMat4("projection", projection);

        float timestep = deltaTime * speed;
        glPointSize(pointSize);
        for(unsigned int i = 0; i < Points.size(); i++)
        {
            if (glm::length(Points[i]) > 100000 && gui.doCull) {
                lossCount++;
                Points.erase(Points.begin() + i);
                continue;
            }

            switch(gui.equation) {
                case lorenze:
                    Points[i].x += (10 * (Points[i].y - Points[i].x)) * timestep;
                    Points[i].y += (Points[i].x * (gui.constants["f"].value - Points[i].z) - Points[i].y) * timestep;
                    Points[i].z += (Points[i].x * Points[i].y - 8/3 * Points[i].z) * timestep;
                    break;
                case cube:

                    break;
                default:
                    Points[i].x += Points[i].x * timestep;
                    Points[i].y += Points[i].y * timestep;
                    Points[i].z += Points[i].z * timestep;
            }

            if (false)
            {
                Points[i].x += (10 * (Points[i].y - Points[i].x)) * timestep;
                Points[i].y += (Points[i].x * (gui.constants["f"].value - Points[i].z) - Points[i].y) * timestep;
                Points[i].z += (Points[i].x * Points[i].y - 8/3 * Points[i].z) * timestep;
            }
            else 
            {
                Points[i].x += Points[i].x * timestep;
                Points[i].y += Points[i].y * timestep;
                Points[i].z += Points[i].z * timestep;
            }   

            /*
            dx = (z-b) * x - d*y

            dy = d * x + (z-b) * y

            dz = c + a*z - z3 /3 - x2 + f * z * x3
            */

            shader.setVec3("globalPosition", Points[i]);

            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(scalar));
            model = glm::translate(model, Points[i]);
            
            shader.setMat4("model", model);
            glBindVertexArray(VAO);
            glDrawArrays(GL_POINTS, 0, 36);
        }

        gui.render(&scalar, &speed, &lossCount, &pointSize);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    ImGui::DestroyContext();
    ImGui_ImplGlfw_Shutdown();
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
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr); 
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetCursorPosCallback(window, mouse_callback);

    gladLoadGL();

    glEnable(GL_DEPTH_TEST);  

    if (true) {
        glEnable(GL_BLEND);
    }

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