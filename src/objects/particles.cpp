#include "particles.hpp"

Particles::Particles(Gui* gui, float minmax, float step)
{
    guiPtr = gui;
    shader = Shader("shaders/particle.vert", "shaders/particle.frag");

    for(float vx = -minmax; vx < minmax; vx += step) {
        for(float vy = -minmax; vy < minmax; vy += step) {
            for(float vz = -minmax; vz < minmax; vz += step) {
                Points.push_back(Point(glm::vec3(vx, vy, vz)));
            }
        }
    }
    PointsInital = Points;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };  

    getSphereVertices();

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);


    glEnable(GL_POINT_SMOOTH);
}

void Particles::renderPoints(float deltatime)
{
    updateScalingConstants();

    glPointSize(PointSize);
    float timestep = deltatime / 2 * Speed;    

    std::vector<glm::mat4> modelMatrices;

    for(unsigned int i = 0; i < Points.size(); i++)
    {
        if (doCull && Points[i].magnitude() > 30000) { // random high number. higher = more time until lost point is culled
            LossCount++;
            Points.erase(Points.begin() + i);
            continue;
        }

        if (!Paused) movePointByEquation(timestep, &Points[i]);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(Scale));
        model = glm::translate(model, Points[i].Pos);
        
        modelMatrices.push_back(model);
    }
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, Points.size() * sizeof(glm::mat4), &modelMatrices[0], GL_DYNAMIC_DRAW);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, Points.size());
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
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Particles::getSphereVertices()
{
    std::vector<float> Sphere;

    for (float x = 0; x < M_PI * 2; x += M_PI * 2 / 360){
        for (float y = 0; y < M_PI * 2; y += M_PI * 2 / 360){
            for (float z = 0; z < M_PI * 2; z += M_PI * 2 / 360)
            {
                Sphere.push_back(x);
                Sphere.push_back(y);
                Sphere.push_back(z);
            }
        }
    }
}