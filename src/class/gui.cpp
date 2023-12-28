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
            if (ImGui::BeginMenu("Menu")) 
            {
                if (ImGui::MenuItem("Close")) open = false; 
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }


        ImGui::SliderFloat("Scalar", scalar, 0.1, 10.0);
        ImGui::SliderFloat("speed", speed, 0.0, 10.0);
        ImGui::SliderFloat("Point Size", pointSize, 0.1, 100.0);

        ImGui::Text("Loss Count");
        ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.2f, 1.0f), "Loss %s / %s Total", std::to_string(*lossCount).c_str(), std::to_string(PointsInital.size()).c_str());

        if (ImGui::Button("Reset")) {
            *Points = PointsInital;
            *lossCount = 0; 
        }
        
        ImGui::End();
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