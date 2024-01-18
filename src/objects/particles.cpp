#include "particles.hpp"

Particles::Particles(Gui* gui, float minmax, float step)
{
    guiPtr = gui;
    particleShader = Shader("shaders/particle.vert", "shaders/particle.frag");
    trailShader = Shader("shaders/trail.vert", "shaders/trail.frag");

    definePoints(minmax, step);
    std::cout << Points.size() << " Points" << std::endl;
    
    
    glGenVertexArrays(1, &particleVAO);
    glGenBuffers(1, &particleVBO);

    glBindVertexArray(particleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);

    // is just set in shader does nothing as of rn
    vectorVertices = {0.5, 0.5, 0.5};

    glBufferData(GL_ARRAY_BUFFER, sizeof(vectorVertices.data()), vectorVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    configureShaderMatrix(1);


    glGenVertexArrays(1, &trailVAO);
    glGenBuffers(1, &trailVBO);

    glBindVertexArray(trailVAO);
    glBindBuffer(GL_ARRAY_BUFFER, trailVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
}

void Particles::renderPoints(float deltatime)
{
    updateScalingConstants();

    glPointSize(PointSize);
    float timestep = deltatime / 2 * Speed;    

    std::vector<glm::mat4> particalModels;
    std::vector<float> trailPoss;

    for(unsigned int i = 0; i < Points.size(); i++)
    {
        if (doCull && Points[i].magnitude() > 30000) { // random high number. higher = more time until lost point is culled
            LossCount++;
            Points.erase(Points.begin() + i);
            continue;
        }

        if (!Paused) {
            movePointByEquation(timestep, &Points[i]);
            if (doRenderTrails) Points[i].trailCompute();
        }
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(Scale));
        model = glm::translate(model, Points[i].Pos);
        
        particalModels.push_back(model);
    }

    if (doRenderPoints)
    {
        particleShader.use();
        glBindVertexArray(particleVAO);
        glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
        glBufferData(GL_ARRAY_BUFFER, Points.size() * sizeof(glm::mat4), &particalModels[0], GL_DYNAMIC_DRAW);

        glDrawArraysInstanced(GL_POINTS, 0, 1, Points.size());
    }

    if (doRenderTrails)
    {
        trailShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(Scale));
        trailShader.setMat4("model", model);
        trailShader.setVec4("from_color", from_color);
        trailShader.setVec4("to_color", to_color);
        trailShader.setFloat("fromScale", fromScale);
        
        glBindVertexArray(trailVAO);
        glBindBuffer(GL_ARRAY_BUFFER, trailVBO);

        for(unsigned int i = 0; i < Points.size(); i++)
        {
            size_t length = Points[i].trail.size();

            glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), Points[i].trail.data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_LINE_STRIP, 0, length / 3);
        }
    }
}

void Particles::movePointByEquation(float timestep, Point* point)
{
    switch(equation) {
        case LORENZ:
            point->Pos.x += (constants["a"].value * (point->Pos.y - point->Pos.x)) * timestep;
            point->Pos.y += (point->Pos.x * (constants["b"].value - point->Pos.z) - point->Pos.y) * timestep;
            point->Pos.z += (point->Pos.x * point->Pos.y - 8/3 * point->Pos.z) * timestep;

            break;

        case AIZAWA: case AIZAWA_CIRCLE:
            point->Pos.x += ((point->Pos.z - constants["b"].value) * point->Pos.x - constants["d"].value * point->Pos.y) * timestep;
            point->Pos.y += (constants["d"].value * point->Pos.x + (point->Pos.z - constants["b"].value) * point->Pos.y) * timestep;
            point->Pos.z += (constants["c"].value + constants["a"].value * point->Pos.z - (point->Pos.z * point->Pos.z * point->Pos.z / 3) - (point->Pos.x * point->Pos.x + point->Pos.y * point->Pos.y) * (1 + constants["e"].value * point->Pos.z) + constants["f"].value * point->Pos.z * (point->Pos.x * point->Pos.x * point->Pos.x)) * timestep;
            /*
            dx = (z-b) * x - d*y

            dy = d * x + (z-b) * y

            dz = c + a*z - z3 /3 - x2 + f * z * x3
            */
            break;
        
        case CHEN:
            point->Pos.x += (constants["a"].value * (point->Pos.y - point->Pos.x)) * timestep;
            point->Pos.y += ((constants["c"].value - constants["a"].value) * point->Pos.x - point->Pos.x * point->Pos.z + constants["c"].value * point->Pos.y) * timestep;
            point->Pos.z += (point->Pos.x * point->Pos.y - constants["b"].value * point->Pos.z) * timestep;
            // dx/dt = a(y - x)
            // dy/dt = (c - a)x - xz + cy
            // dz/dt = xy - bz
            break;
        
        case LUCHEN:
            point->Pos.x += (constants["a"].value * (point->Pos.y - point->Pos.x)) * timestep;
            point->Pos.y += (point->Pos.x - point->Pos.x * point->Pos.z + constants["c"].value * point->Pos.y + constants["d"].value) * timestep;
            point->Pos.z += (point->Pos.x * point->Pos.y - constants["b"].value * point->Pos.z) * timestep;
            break;
        
        case NEWTON_LEIPNIK:
            point->Pos.x += (-constants["a"].value * point->Pos.x + point->Pos.y + 10 * point->Pos.y * point->Pos.z) * timestep;
            point->Pos.y += (-point->Pos.x - 0.4 * point->Pos.y + 5 * point->Pos.x * point->Pos.z) * timestep;
            point->Pos.z += (constants["b"].value * point->Pos.z - 5 * point->Pos.x * point->Pos.y) * timestep;
            break;
        
        case NOSE_HOOVER:
            point->Pos.x += (point->Pos.y) * timestep;
            point->Pos.y += (-point->Pos.x + point->Pos.y * point->Pos.z) * timestep;
            point->Pos.z += (constants["a"].value - (point->Pos.y * point->Pos.y)) * timestep;
            break;
        
        case HALVORSEN:
            point->Pos.x += (-constants["a"].value * point->Pos.x - 4 * point->Pos.y - 4 * point->Pos.z - point->Pos.y * point->Pos.y) * timestep;
            point->Pos.y += (-constants["a"].value * point->Pos.y - 4 * point->Pos.z - 4 * point->Pos.x - point->Pos.z * point->Pos.z) * timestep;
            point->Pos.z += (-constants["a"].value * point->Pos.z - 4 * point->Pos.x - 4 * point->Pos.y - point->Pos.x * point->Pos.x) * timestep;
            break;
        
        case CHEN_LEE:
            point->Pos.x += (constants["a"].value * point->Pos.x - point->Pos.y * point->Pos.z) * timestep;
            point->Pos.y += (constants["b"].value * point->Pos.y + point->Pos.x * point->Pos.z) * timestep;
            point->Pos.z += (constants["c"].value * point->Pos.z + point->Pos.x * (point->Pos.y / 3)) * timestep;
            break;
        
        case BOUALI:
            point->Pos.x += (point->Pos.x * (4 - point->Pos.y) + constants["a"].value * point->Pos.z) * timestep;
            point->Pos.y += (-point->Pos.y * (1 - point->Pos.x * point->Pos.x)) * timestep;
            point->Pos.z += (-point->Pos.x * (1.5 - constants["b"].value * point->Pos.z) - 0.05 * point->Pos.z) * timestep;
            break;
        
        case FINANCE: 
            point->Pos.x += ((1 / constants["b"].value - constants["a"].value) * point->Pos.x + point->Pos.z + point->Pos.x * point->Pos.y) * timestep;
            point->Pos.y += (-constants["b"].value * point->Pos.y - point->Pos.x * point->Pos.x) * timestep;
            point->Pos.z += (-point->Pos.x - constants["c"].value * point->Pos.z) * timestep;
            break;

        case THOMAS: 
            point->Pos.x += (-constants["a"].value * point->Pos.x + sin(point->Pos.y)) * timestep;
            point->Pos.y += (-constants["a"].value * point->Pos.y + sin(point->Pos.z)) * timestep;
            point->Pos.z += (-constants["a"].value * point->Pos.z + sin(point->Pos.x)) * timestep;
            break;
        
        case ROSSLER:
            point->Pos.x += (-point->Pos.y - point->Pos.z) * timestep;
            point->Pos.y += (point->Pos.x + constants["a"].value * point->Pos.y) * timestep;
            point->Pos.z += (constants["b"].value + point->Pos.z * (point->Pos.x - constants["c"].value)) * timestep;
            break;

        case DADRAS:
            point->Pos.x += (point->Pos.y - constants["a"].value * point->Pos.x + constants["b"].value * point->Pos.y * point->Pos.z) * timestep;
            point->Pos.y += (constants["c"].value * point->Pos.y - point->Pos.x * point->Pos.z + point->Pos.z) * timestep;
            point->Pos.z += (constants["d"].value * point->Pos.x * point->Pos.y - constants["e"].value * point->Pos.z) * timestep;
            break;
        
        case RABINOVICH_FABRIKANT:
            point->Pos.x += (point->Pos.y * (point->Pos.z - 1 + point->Pos.x * point->Pos.x) + constants["a"].value * point->Pos.x) * timestep;
            point->Pos.y += (point->Pos.x * (3 * point->Pos.z + 1 - point->Pos.x * point->Pos.x) + constants["a"].value * point->Pos.y) * timestep;
            point->Pos.z += (-2 * point->Pos.z * (constants["b"].value + point->Pos.x * point->Pos.y)) * timestep;
            break;
        
        case THREE_SCROLL_UNIFIED_CHAOTIC_SYSTEM:
            point->Pos.x += (constants["a"].value * (point->Pos.y - point->Pos.x) + constants["d"].value * point->Pos.x * point->Pos.z) * timestep;
            point->Pos.y += (constants["b"].value * point->Pos.x - point->Pos.x * point->Pos.z + constants["f"].value * point->Pos.y) * timestep;
            point->Pos.z += (constants["c"].value * point->Pos.z + point->Pos.x * point->Pos.y - constants["e"].value * (point->Pos.x * point->Pos.x)) * timestep;
            break;
        
        case SPROTT:
            point->Pos.x += (point->Pos.y + constants["a"].value * point->Pos.x * point->Pos.y + point->Pos.x * point->Pos.z) * timestep;
            point->Pos.y += (1 - constants["b"].value * (point->Pos.x * point->Pos.x) + point->Pos.y * point->Pos.z) * timestep;
            point->Pos.z += (point->Pos.x - point->Pos.x * point->Pos.x - point->Pos.y * point->Pos.y) * timestep;
            break;
        
        case FOUR_WING:
            point->Pos.x += (constants["a"].value * point->Pos.x + point->Pos.y * point->Pos.z) * timestep;
            point->Pos.y += (constants["b"].value * point->Pos.x + constants["c"].value * point->Pos.y - point->Pos.x * point->Pos.z) * timestep;
            point->Pos.z += (-point->Pos.z - point->Pos.x * point->Pos.y) * timestep;
            break;

        case CUBE:
            point->Pos.x += point->Pos.x * timestep;
            point->Pos.y += point->Pos.y * timestep;
            point->Pos.z += point->Pos.z * timestep;
            break;

        default:
            break;
    }
}

void Particles::updateScalingConstants()
{
    for (const auto& pair : constants) 
    {
        if (constants[pair.first].scaling) {
            constants[pair.first].value += constants[pair.first].scalingSpeed;
            
            if (constants[pair.first].value > constants[pair.first].max) {
                constants[pair.first].value = constants[pair.first].max;
            }
        }
    }
}

void Particles::clean()
{
    glDeleteVertexArrays(1, &particleVAO);
    glDeleteBuffers(1, &particleVBO);
}

std::vector<float> Particles::getSphereVertices()
{
    std::vector<float> Sphere;

    const float PI = 3.14f;

    for (float angle = 0.0f; angle <= (2.0f * PI); angle += 0.01f)
    {
        Sphere.push_back(50.0f * sin(angle));
        Sphere.push_back(50.0f * cos(angle));
        Sphere.push_back(0.0);
    }   
    
    /*
    int stacks = 20;
    int slices = 20;
    const float PI = 3.14f;

    for (int i = 0; i <= stacks; ++i){

        float V = (float)i / (float)stacks;
        float phi = V * PI;

        for (int j = 0; j <= slices; ++j){

            float U = (float)j / (float)slices;
            float theta = U * (PI * 2);

            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            Sphere.push_back(x);
            Sphere.push_back(y);
            Sphere.push_back(z);
        }
    }
    */

    return Sphere;
}

void Particles::assignUniforms(glm::mat4 view, glm::mat4 projection)
{
    particleShader.use();
    particleShader.setMat4("view", view);
    particleShader.setMat4("projection", projection);

    trailShader.use();
    trailShader.setMat4("view", view);
    trailShader.setMat4("projection", projection);
}

void Particles::configureShaderMatrix(unsigned int location)
{
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    
    glEnableVertexAttribArray(location + 1);
    glVertexAttribPointer(location + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    
    glEnableVertexAttribArray(location + 2);
    glVertexAttribPointer(location + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    
    glEnableVertexAttribArray(location + 3);
    glVertexAttribPointer(location + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(location, 1);
    glVertexAttribDivisor(location + 1, 1);
    glVertexAttribDivisor(location + 2, 1);
    glVertexAttribDivisor(location + 3, 1);
}

void Particles::definePoints(float minmax, float step)
{
    Points.clear();
    for(float vx = -minmax; vx < minmax; vx += step) {
        for(float vy = -minmax; vy < minmax; vy += step) {
            for(float vz = -minmax; vz < minmax; vz += step) {
                Points.push_back(Point(glm::vec3(vx, vy, vz)));
            }
        }
    }
    PointsInital = Points;
    setMinmax = minmax;
    setStep = step;
}