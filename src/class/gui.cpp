#include "gui.hpp"

Gui::Gui(GLFWwindow* window, std::string GLSL_version)
{
    Gui::window = window;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(Gui::window, true);
    ImGui_ImplOpenGL3_Init(GLSL_version.c_str());

    ImGui::StyleColorsDark();
}

void Gui::newframe()
{
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}

void Gui::render(float* scalar, float* speed, int* lossCount, float* pointSize)
{
    if (open)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        ImGui::Begin(" Config ", nullptr, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) 
        {
            if (ImGui::MenuItem("Close")) open = false; 

            if (ImGui::BeginMenu("Menu")) 
            {
                if (!constantsOpen) {
                    if (ImGui::MenuItem("View Constants")) constantsOpen = true; 
                } else if (ImGui::MenuItem("close Constants")) constantsOpen = false; 
                
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Settings")) 
            {
                if (doCull) {
                    if (ImGui::MenuItem("Disable particle Culling")) doCull = false; 
                } else if (ImGui::MenuItem("Enable particle Culling")) doCull = true; 
                
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::SliderFloat("Scalar", scalar, 0.1, 10.0);
        ImGui::SliderFloat("speed", speed, 0.0, 10.0);
        ImGui::SliderFloat("Point Size", pointSize, 0.1, 100.0);

        if (doCull) {
            ImGui::Text("Loss Count");
            ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.2f, 1.0f), "Loss %s / %s Total", std::to_string(*lossCount).c_str(), std::to_string(PointsInital.size()).c_str());
        } else ImGui::Text("Culling is Disabled");

        if (ImGui::Button("Reset")) {
            *Points = PointsInital;
            *lossCount = 0; 
        }

        ImGui::End();

        if (constantsOpen)
        {
            ImGui::Begin(" Constants ", nullptr, ImGuiWindowFlags_MenuBar);
            if (ImGui::BeginMenuBar()) 
            {
                if (ImGui::BeginMenu("Menu")) 
                {
                    if (ImGui::MenuItem("Close")) constantsOpen = false; 
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            if (constants.size() > 0) {
                for (const auto& pair : constants) {
                    ImGui::SliderFloat(pair.first.c_str(), &constants[pair.first].value, constants[pair.first].min, constants[pair.first].max);
                    if (ImGui::Button((std::string("Reset ") + pair.first).c_str())) constants[pair.first].value = constants[pair.first].inital;
                }
            } else ImGui::Text("No constants created");

            ImGui::End();
        }
    } 
    else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) open = true;
}

void Gui::setPointsArray(std::vector<glm::vec3>* PointsRef)
{
    Points = PointsRef;
    PointsInital = *PointsRef;
}

void Gui::addConstant(std::string name, float value, float min, float max)
{
    constants[name] = Constant{value, min, max, value};
}