#include "particles.hpp"

Particles::Particles(Gui* gui, float minmax, float step)
{
    guiPtr = gui;
    shader = Shader("shaders/shader.vert", "shaders/shader.frag");

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

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
}

void Particles::renderPoints(float deltatime)
{
    glPointSize(PointSize);
    float timestep = deltatime * Speed;
    int amount = Points.size();
    
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];

    for(unsigned int i = 0; i < amount; i++)
    {
        if (doCull && Points[i].magnitude() > 300000) { // random high number. higher = more time until lost point is culled
            LossCount++;
            Points.erase(Points.begin() + i);
            continue;
        }

        movePointByEquation(timestep, &Points[i]);
        Points[i].addTrailPoint();
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(Scale));
        model = glm::translate(model, Points[i].Pos);
        
        modelMatrices[i] = model;
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_DYNAMIC_DRAW);

    glDrawArraysInstanced(GL_POINTS, 0, 1, amount);
}

/*
void Particles::renderPoints(float deltatime)
{
    glPointSize(PointSize);
    float timestep = deltatime * Speed;
    for(unsigned int i = 0; i < Points.size(); i++)
    {
        if (doCull && Points[i].magnitude() > 300000) { // random high number. higher = more time until lost point is culled
            LossCount++;
            Points.erase(Points.begin() + i);
            continue;
        }

        movePointByEquation(timestep, &Points[i]);

        Points[i].addTrailPoint();
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(Scale));
        model = glm::translate(model, Points[i].Pos);

        shader.setMat4("model", model);

        glDrawArrays(GL_POINTS, 0, 1);
    }
}
*/

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
        
        case 104:
            point->Pos.x += (point->Pos.x) * timestep;
            point->Pos.y += (point->Pos.y) * timestep;
            point->Pos.z += (point->Pos.z) * timestep;
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

void Particles::clean()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}