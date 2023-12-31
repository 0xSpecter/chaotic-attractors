#include "main.hpp"

const float WIDTH = 1300.0f;
const float HEIGHT = 850.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLFWwindow* window = init();

Camera camera(window, 95.0f);
Shader shader("shaders/shader.vert", "shaders/shader.frag");
Gui gui(window, &camera);

int main()
{
    float point[] = { -0.5f, -0.5f, -0.5f };

    std::vector<Point> Points;

    for(float vx = -0.0017; vx < 0.0017; vx += 0.0001f) {
        for(float vy = -0.0017; vy < 0.0017; vy += 0.0001) {
            for(float vz = -0.0017; vz < 0.0017; vz += 0.0001f) {
                Points.push_back(Point(glm::vec3(vx, vy, vz)));
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
    gui.setEquation(LORENZ);
    
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        if (deltaTime > 0.3f) deltaTime = 0.3f;

        processInput();
        gui.newframe();

        glClearColor(gui.clearColor.x, gui.clearColor.y, gui.clearColor.z, gui.clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("view", camera.GetViewMatrix());

        glm::mat4 projection = glm::mat4(1.0f); 
        projection = glm::perspective(glm::radians(camera.getZoom()), WIDTH / HEIGHT, 0.01f, 100000.0f);
        shader.setMat4("projection", projection);

        float timestep = deltaTime / 2 * speed;
        
        glPointSize(pointSize);
        for(unsigned int i = 0; i < Points.size(); i++)
        {
            if (gui.doCull && Points[i].magnitude() > 300000) {
                lossCount++;
                Points.erase(Points.begin() + i);
                continue;
            }

            switch(gui.equation) {
                case LORENZ:
                    Points[i].Pos.x += (gui.constants["a"].value * (Points[i].Pos.y - Points[i].Pos.x)) * timestep;
                    Points[i].Pos.y += (Points[i].Pos.x * (gui.constants["b"].value - Points[i].Pos.z) - Points[i].Pos.y) * timestep;
                    Points[i].Pos.z += (Points[i].Pos.x * Points[i].Pos.y - 8/3 * Points[i].Pos.z) * timestep;

                    break;

                case AIZAWA: case AIZAWA_CIRCLE:
                    Points[i].Pos.x += ((Points[i].Pos.z - gui.constants["b"].value) * Points[i].Pos.x - gui.constants["d"].value * Points[i].Pos.y) * timestep;
                    Points[i].Pos.y += (gui.constants["d"].value * Points[i].Pos.x + (Points[i].Pos.z - gui.constants["b"].value) * Points[i].Pos.y) * timestep;
                    Points[i].Pos.z += (gui.constants["c"].value + gui.constants["a"].value * Points[i].Pos.z - (Points[i].Pos.z * Points[i].Pos.z * Points[i].Pos.z / 3) - (Points[i].Pos.x * Points[i].Pos.x + Points[i].Pos.y * Points[i].Pos.y) * (1 + gui.constants["e"].value * Points[i].Pos.z) + gui.constants["f"].value * Points[i].Pos.z * (Points[i].Pos.x * Points[i].Pos.x * Points[i].Pos.x)) * timestep;
                    /*
                    dx = (z-b) * x - d*y

                    dy = d * x + (z-b) * y

                    dz = c + a*z - z3 /3 - x2 + f * z * x3
                    */
                    break;
                
                case CHEN:
                    Points[i].Pos.x += (gui.constants["a"].value * (Points[i].Pos.y - Points[i].Pos.x)) * timestep;
                    Points[i].Pos.y += ((gui.constants["c"].value - gui.constants["a"].value) * Points[i].Pos.x - Points[i].Pos.x * Points[i].Pos.z + gui.constants["c"].value * Points[i].Pos.y) * timestep;
                    Points[i].Pos.z += (Points[i].Pos.x * Points[i].Pos.y - gui.constants["b"].value * Points[i].Pos.z) * timestep;
                    // dx/dt = a(y - x)
                    // dy/dt = (c - a)x - xz + cy
                    // dz/dt = xy - bz
                    break;
                
                case LUCHEN:
                    Points[i].Pos.x += (gui.constants["a"].value * (Points[i].Pos.y - Points[i].Pos.x)) * timestep;
                    Points[i].Pos.y += (Points[i].Pos.x - Points[i].Pos.x * Points[i].Pos.z + gui.constants["c"].value * Points[i].Pos.y + gui.constants["d"].value) * timestep;
                    Points[i].Pos.z += (Points[i].Pos.x * Points[i].Pos.y - gui.constants["b"].value * Points[i].Pos.z) * timestep;
                    break;
                
                case NOSE_HOOVER:
                    Points[i].Pos.x += (Points[i].Pos.y) * timestep;
                    Points[i].Pos.y += (-Points[i].Pos.x + Points[i].Pos.y * Points[i].Pos.z) * timestep;
                    Points[i].Pos.z += (gui.constants["a"].value - (Points[i].Pos.y * Points[i].Pos.y)) * timestep;
                    break;
                
                case HALVORSEN:
                    Points[i].Pos.x += (-gui.constants["a"].value * Points[i].Pos.x - 4 * Points[i].Pos.y - 4 * Points[i].Pos.z - Points[i].Pos.y * Points[i].Pos.y) * timestep;
			        Points[i].Pos.y += (-gui.constants["a"].value * Points[i].Pos.y - 4 * Points[i].Pos.z - 4 * Points[i].Pos.x - Points[i].Pos.z * Points[i].Pos.z) * timestep;
			        Points[i].Pos.z += (-gui.constants["a"].value * Points[i].Pos.z - 4 * Points[i].Pos.x - 4 * Points[i].Pos.y - Points[i].Pos.x * Points[i].Pos.x) * timestep;
                    break;
                
                case CHEN_LEE:
                    Points[i].Pos.x += (gui.constants["a"].value * Points[i].Pos.x - Points[i].Pos.y * Points[i].Pos.z) * timestep;
                    Points[i].Pos.y += (gui.constants["b"].value * Points[i].Pos.y + Points[i].Pos.x * Points[i].Pos.z) * timestep;
                    Points[i].Pos.z += (gui.constants["c"].value * Points[i].Pos.z + Points[i].Pos.x * (Points[i].Pos.y / 3)) * timestep;
                    break;
                
                case 104:
                    Points[i].Pos.x += (Points[i].Pos.x) * timestep;
                    Points[i].Pos.y += (Points[i].Pos.y) * timestep;
                    Points[i].Pos.z += (Points[i].Pos.z) * timestep;
                    break;
                    

                case CUBE:
                    Points[i].Pos.x += Points[i].Pos.x * timestep;
                    Points[i].Pos.y += Points[i].Pos.y * timestep;
                    Points[i].Pos.z += Points[i].Pos.z * timestep;

                    break;

                default:
                    break;
            }


            shader.setVec3("globalPosition", Points[i].Pos);
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(scalar));
            model = glm::translate(model, Points[i].Pos);
            
            shader.setMat4("model", model);

            glDrawArrays(GL_POINTS, 0, 1);
        }

        /*
        for(unsigned int i = 0; i < TrailPoints.size(); i++)
        {
            shader.setVec3("globalPosition", TrailPoints[i]);
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(scalar));
            model = glm::translate(model, TrailPoints[i]);
            
            shader.setMat4("model", model);

            glDrawArrays(GL_POINTS, 0, 1);
        }
        */

        gui.updateScalingConstants();
        gui.render(&scalar, &speed, &lossCount, &pointSize);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr); 
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