#include "gui.hpp"

Gui::Gui(GLFWwindow* window, Camera* camera, std::string GLSL_version)
{
    Gui::window = window;
    Gui::camera = camera;

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

void Gui::render(float deltaTime)
{
    static double minmax = ParticlesPtr->setMinmax;
    static double step = ParticlesPtr->setStep;

    if (open)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        ImGui::Begin(" Config ", nullptr, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) 
        {
            if (ImGui::MenuItem("Close")) open = false; 

            if (ImGui::BeginMenu("Tools")) 
            {
                if (!constantsOpen) {
                    if (ImGui::MenuItem("View Constants")) constantsOpen = true; 
                } else if (ImGui::MenuItem("close Constants")) constantsOpen = false; 

                if (!attractorSelect) {
                    if (ImGui::MenuItem("View Attractors")) attractorSelect = true; 
                } else if (ImGui::MenuItem("close Attractors")) attractorSelect = false; 

                if (!cameraConfig) {
                    if (ImGui::MenuItem("Camera")) cameraConfig = true; 
                } else if (ImGui::MenuItem("close Camera")) cameraConfig = false; 

                if (!graphicsConfig) {
                    if (ImGui::MenuItem("Graphics")) graphicsConfig = true; 
                } else if (ImGui::MenuItem("close Graphics")) graphicsConfig = false; 

                if (!worldTransform) {
                    if (ImGui::MenuItem("World Transform")) worldTransform = true; 
                } else if (ImGui::MenuItem("close World Transform")) worldTransform = false; 

                if (!setColors) {
                    if (ImGui::MenuItem("Colors")) setColors = true; 
                } else if (ImGui::MenuItem("close Colors")) setColors = false;
                
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Settings")) 
            {
                if (ParticlesPtr->doCull) {
                    if (ImGui::MenuItem("Disable particle Culling")) ParticlesPtr->doCull = false; 
                } else if (ImGui::MenuItem("Enable particle Culling")) ParticlesPtr->doCull = true; 
                
                ImGui::EndMenu();
            }
            
            ImGui::TextColored(ImVec4(0.5f, 0.15f, 0.5f, 1.0f), "Fps: %i", (int)(1.0f / deltaTime));
            
            ImGui::EndMenuBar();
        }

        ImGui::Text("Chaotic Attractors");

        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Dummy(ImVec2(0, 2));
        ImGui::SameLine();
        if (!ParticlesPtr->Paused && ImGui::Button("Pause")) ParticlesPtr->Paused = true;
        if (ParticlesPtr->Paused && ImGui::Button("Start")) ParticlesPtr->Paused = false;

        ImGui::SameLine();
        ImGui::Dummy(ImVec2(5, 0));
        ImGui::SameLine();

        if (ImGui::Button("Reset Points")) {
            ParticlesPtr->Points = ParticlesPtr->PointsInital;
            ParticlesPtr->LossCount = 0;
        }

        ImGui::Dummy(ImVec2(0, 12));

        if (ParticlesPtr->doCull) {
            ImGui::Text("Loss Count");
            ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.2f, 1.0f), "Loss %i / %lu Total", ParticlesPtr->LossCount, ParticlesPtr->PointsInital.size());
        } else ImGui::Text("%lu Total, Culling is disabled", ParticlesPtr->PointsInital.size());

        ImGui::Dummy(ImVec2(0, 10));

        ImGui::SliderFloat("Scalar", &ParticlesPtr->Scale, 0.1, 10.0);
        ImGui::SliderFloat("Speed", &ParticlesPtr->Speed, 0.0, 10.0);
        ImGui::SliderFloat("Point Size", &ParticlesPtr->PointSize, 0.1, 100.0);

        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Checkbox("Render Points", &ParticlesPtr->doRenderPoints);
        ImGui::Checkbox("Render Trails", &ParticlesPtr->doRenderTrails);

        ImGui::Dummy(ImVec2(0, 10));

        if (ImGui::Button("Redefine Points")) {
            ParticlesPtr->definePoints(static_cast<float>(minmax), static_cast<float>(step));
        }

        ImGui::InputDouble("MinMax", &minmax);
        ImGui::InputDouble("Step", &step);

        ImGui::End();

        if (constantsOpen) 
            renderConstants();
        
        if (attractorSelect) 
            renderAttractorSelect();

        if (graphicsConfig) 
            renderGraphicsConfig();
        
        if (worldTransform) 
            renderWorldTransform();
        
        if (setColors)
            renderColor();
        
    } 
    else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // non tab gui
    if (cameraConfig) 
        renderCameraConfig();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) open = true;
}

void Gui::setParticles(Particles* ParticlesPtr)
{
    Gui::ParticlesPtr = ParticlesPtr;
}


void Gui::setEquation(Equations newEquation)
{
    ParticlesPtr->equation = newEquation;
    
    ParticlesPtr->constants.clear();

    std::string letters = "abcdef";
    for (unsigned int i = 0; i < ParticlesPtr->equationConstants[newEquation].size(); i++)
    {
        datapoint data = ParticlesPtr->equationConstants[newEquation][i];
        ParticlesPtr->constants[letters.substr(i, 1)] = Constant{data.value, data.min, data.max, data.value};
    }
}

void Gui::renderConstants()
{
    ImGui::Begin(" Constants ", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) 
    {
        if (ImGui::MenuItem("Close")) 
            constantsOpen = false; 
        ImGui::EndMenuBar();
    }


    if (ParticlesPtr->constants.size() > 0) {
        for (const auto& pair : ParticlesPtr->constants) 
        {
            ImGui::SliderFloat(pair.first.c_str(), &ParticlesPtr->constants[pair.first].value, ParticlesPtr->constants[pair.first].min, ParticlesPtr->constants[pair.first].max);
            
            if (ImGui::Button((std::string("Reset ") + pair.first).c_str())) 
                ParticlesPtr->constants[pair.first].value = ParticlesPtr->constants[pair.first].inital;
                        
        }
    } else ImGui::Text("No constants created");

    ImGui::End();
}


void Gui::renderAttractorSelect()
{
    ImGui::Begin(" Attractors ", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) 
    {
        if (ImGui::MenuItem("Close")) 
            attractorSelect = false; 
        
        ImGui::EndMenuBar();
    }

    if (ImGui::BeginCombo("Options", ParticlesPtr->equationInfo[ParticlesPtr->equation][0])) {
        for (int i = 0; i < Equations::CUBE + 1; ++i) {
            bool isSelected = (ParticlesPtr->equation == Equations(i));

            if (ImGui::Selectable(ParticlesPtr->equationInfo[i][0], isSelected)) setEquation(Equations(i));
        }

        ImGui::EndCombo();
    }

    if (ImGui::Button("Optimal Camera Position")) {
        camera->Position = optimalCameraPositions[ParticlesPtr->equation].Position;
        camera->Right = optimalCameraPositions[ParticlesPtr->equation].Right;
        camera->Yaw = optimalCameraPositions[ParticlesPtr->equation].Yaw;
        camera->Pitch = optimalCameraPositions[ParticlesPtr->equation].Pitch;
        open = false;
    }

    ImGui::Text("%s", ParticlesPtr->equationInfo[ParticlesPtr->equation][1]);

    ImGui::End();
}

void Gui::renderCameraConfig()
{
    static bool sticky = true;

    if (open || sticky) 
    {
        ImGui::Begin(" Camera ", nullptr, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) 
        {
            if (ImGui::MenuItem("Close")) 
                cameraConfig = false; 
            
            ImGui::EndMenuBar();
        }

        ImGui::Text("Position: %f, %f, %f", camera->Position.x, camera->Position.y, camera->Position.z);  
        ImGui::Text("Right: %f, %f, %f", camera->Right.x, camera->Right.y, camera->Right.z);  
        ImGui::Text("Yaw: %f, Pitch, %f", camera->Yaw, camera->Pitch);    

        ImGui::Checkbox("Sticky", &sticky);
        ImGui::End();
    }
}

void Gui::renderGraphicsConfig()
{
    static bool BLEND = glIsEnabled(GL_BLEND);
    static bool DEPTH_TEST = glIsEnabled(GL_DEPTH_TEST);

    ImGui::Begin(" Graphic ", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) 
    {
        if (ImGui::MenuItem("Close")) 
            graphicsConfig = false; 
        
        ImGui::EndMenuBar();
    }

    ImGui::Checkbox("GL Depth Test", &DEPTH_TEST);
    ImGui::Checkbox("GL Blend", &BLEND);
    
    if (ImGui::BeginCombo("sfactor", currentSfactor.name)) {
        for (int i = 0; i < sfactorOptions.size(); ++i) 
        {
            if (ImGui::Selectable(sfactorOptions[i].name)) {
                GLint currentDstFactor;
                glGetIntegeri_v(GL_BLEND_DST, 0, &currentDstFactor);
                glBlendFunc(sfactorOptions[i].value, currentDstFactor);
                currentSfactor = sfactorOptions[i];
            };
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("dfactor", currentDfactor.name)) {
        for (int i = 0; i < dfactorOptions.size(); ++i) 
        {
            if (ImGui::Selectable(dfactorOptions[i].name)) {
                GLint currentSstFactor;
                glGetIntegeri_v(GL_BLEND_SRC, 0, &currentSstFactor);
                glBlendFunc(currentSstFactor, dfactorOptions[i].value);
                currentDfactor = dfactorOptions[i];
            };
        }
        ImGui::EndCombo();
    }

    ImGui::Dummy(ImVec2(0, 8));

    if (ImGui::BeginCombo("Equation", currentBlendEquation.name)) {
        for (int i = 0; i < blendEquationOptions.size(); ++i) 
        {
            if (ImGui::Selectable(blendEquationOptions[i].name)) {
                glBlendEquation(blendEquationOptions[i].value);
                currentBlendEquation = blendEquationOptions[i];
            };
        }
        ImGui::EndCombo();
    }

    ImGui::End();

    if (BLEND != glIsEnabled(GL_BLEND)) BLEND ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    if (DEPTH_TEST != glIsEnabled(GL_DEPTH_TEST)) DEPTH_TEST ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void Gui::renderWorldTransform()
{
    ImGui::Begin(" World Transform ", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) 
    {
        if (ImGui::MenuItem("Close")) 
            worldTransform = false; 
        
        ImGui::EndMenuBar();
    }

    ImGui::SliderFloat("Rotate X", &camera->RotateX, 0.0f, 360.0f);
    ImGui::SliderFloat("Rotate Y", &camera->RotateY, 0.0f, 360.0f);
    ImGui::SliderFloat("Rotate Z", &camera->RotateZ, 0.0f, 360.0f);

    ImGui::Dummy(ImVec2(0, 10));

    ImGui::Checkbox("Pan Around", &camera->doPanRot);
    ImGui::SliderFloat("Pan Speed", &camera->panSpeed, 0.0f, 1.0f);
    ImGui::InputFloat3("Pan Axie", &camera->panAxie[0]);
    ImGui::InputFloat3("Pan Offset", &camera->panOffset[0]);

    ImGui::End();
}

void Gui::renderColor()
{
    ImGui::Begin(" Color ", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) 
    {
        if (ImGui::MenuItem("Close")) 
            setColors = false; 
        
        ImGui::EndMenuBar();
    }

    ImGui::ColorEdit4("Background Color", (float*)&clearColor);

    ImGui::Dummy(ImVec2(0, 20));

    if (ImGui::Button(("Color Mode: %s", (ParticlesPtr->Colormode ? "positional" : "color blend"))))
        ParticlesPtr->Colormode = !ParticlesPtr->Colormode;
    
    ImGui::Dummy(ImVec2(0, 7));

    ImGui::ColorEdit4("From <- color", (float*)&ParticlesPtr->from_color);
    ImGui::ColorEdit4("To -> color", (float*)&ParticlesPtr->to_color);

    ImGui::Dummy(ImVec2(0, 5));

    ImGui::InputFloat("From Scale", &ParticlesPtr->fromScale, 0.0, 1000);
    ImGui::InputInt("Exponent", &ParticlesPtr->exponent, 0, 10);

    ImGui::End();
}

void Gui::renderSetup(bool* confirmed)
{
    static double minmax = ParticlesPtr->setMinmax;
    static double step = ParticlesPtr->setStep;
    static bool blend = glIsEnabled(GL_BLEND);

    ImGui::Begin(" Setup ");

    ImGui::InputDouble("MinMax", &minmax);
    ImGui::InputDouble("Step", &step);

    ImGui::Dummy(ImVec2(0, 3));

    ImGui::Checkbox("Render Points", &ParticlesPtr->doRenderPoints);
    ImGui::Checkbox("Render Trails", &ParticlesPtr->doRenderTrails);

    ImGui::Dummy(ImVec2(0, 4));

    ImGui::Checkbox("Blending", &blend);

    ImGui::Dummy(ImVec2(0, 4));

    if (ImGui::Button("Confirm")) {
        ParticlesPtr->definePoints(static_cast<float>(minmax), static_cast<float>(step));
        if (blend) glEnable(GL_BLEND);
        *confirmed = false;
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}