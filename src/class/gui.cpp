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

void Gui::render(float* scalar)
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


        ImGui::SliderFloat("float", scalar, 0.0, 10.0);

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