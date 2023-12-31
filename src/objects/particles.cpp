#include "particles.hpp"

Particles::Particles(Gui* gui, float minmax, float step)
{
    guiPtr = gui;
    shader = Shader("shaders/shader.vert", "shaders/shader.frag");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    for(float vx = -minmax; vx < minmax; vx += step) {
        for(float vy = -minmax; vy < minmax; vy += step) {
            for(float vz = -minmax; vz < minmax; vz += step) {
                Points.push_back(Point(glm::vec3(vx, vy, vz)));
            }
        }
    }
    PointsInital = Points;
}

void Particles::renderPoints(float deltatime)
{
    glPointSize(PointSize);
    float timestep = deltatime * Speed;
    for(unsigned int i = 0; i < Points.size(); i++)
    {
        if (doCull && Points[i].magnitude() > 300000) {
            LossCount++;
            Points.erase(Points.begin() + i);
            continue;
        }

        switch(equation) {
            case LORENZ:
                Points[i].Pos.x += (constants["a"].value * (Points[i].Pos.y - Points[i].Pos.x)) * timestep;
                Points[i].Pos.y += (Points[i].Pos.x * (constants["b"].value - Points[i].Pos.z) - Points[i].Pos.y) * timestep;
                Points[i].Pos.z += (Points[i].Pos.x * Points[i].Pos.y - 8/3 * Points[i].Pos.z) * timestep;

                break;

            case AIZAWA: case AIZAWA_CIRCLE:
                Points[i].Pos.x += ((Points[i].Pos.z - constants["b"].value) * Points[i].Pos.x - constants["d"].value * Points[i].Pos.y) * timestep;
                Points[i].Pos.y += (constants["d"].value * Points[i].Pos.x + (Points[i].Pos.z - constants["b"].value) * Points[i].Pos.y) * timestep;
                Points[i].Pos.z += (constants["c"].value + constants["a"].value * Points[i].Pos.z - (Points[i].Pos.z * Points[i].Pos.z * Points[i].Pos.z / 3) - (Points[i].Pos.x * Points[i].Pos.x + Points[i].Pos.y * Points[i].Pos.y) * (1 + constants["e"].value * Points[i].Pos.z) + constants["f"].value * Points[i].Pos.z * (Points[i].Pos.x * Points[i].Pos.x * Points[i].Pos.x)) * timestep;
                /*
                dx = (z-b) * x - d*y

                dy = d * x + (z-b) * y

                dz = c + a*z - z3 /3 - x2 + f * z * x3
                */
                break;
            
            case CHEN:
                Points[i].Pos.x += (constants["a"].value * (Points[i].Pos.y - Points[i].Pos.x)) * timestep;
                Points[i].Pos.y += ((constants["c"].value - constants["a"].value) * Points[i].Pos.x - Points[i].Pos.x * Points[i].Pos.z + constants["c"].value * Points[i].Pos.y) * timestep;
                Points[i].Pos.z += (Points[i].Pos.x * Points[i].Pos.y - constants["b"].value * Points[i].Pos.z) * timestep;
                // dx/dt = a(y - x)
                // dy/dt = (c - a)x - xz + cy
                // dz/dt = xy - bz
                break;
            
            case LUCHEN:
                Points[i].Pos.x += (constants["a"].value * (Points[i].Pos.y - Points[i].Pos.x)) * timestep;
                Points[i].Pos.y += (Points[i].Pos.x - Points[i].Pos.x * Points[i].Pos.z + constants["c"].value * Points[i].Pos.y + constants["d"].value) * timestep;
                Points[i].Pos.z += (Points[i].Pos.x * Points[i].Pos.y - constants["b"].value * Points[i].Pos.z) * timestep;
                break;
            
            case NOSE_HOOVER:
                Points[i].Pos.x += (Points[i].Pos.y) * timestep;
                Points[i].Pos.y += (-Points[i].Pos.x + Points[i].Pos.y * Points[i].Pos.z) * timestep;
                Points[i].Pos.z += (constants["a"].value - (Points[i].Pos.y * Points[i].Pos.y)) * timestep;
                break;
            
            case HALVORSEN:
                Points[i].Pos.x += (-constants["a"].value * Points[i].Pos.x - 4 * Points[i].Pos.y - 4 * Points[i].Pos.z - Points[i].Pos.y * Points[i].Pos.y) * timestep;
                Points[i].Pos.y += (-constants["a"].value * Points[i].Pos.y - 4 * Points[i].Pos.z - 4 * Points[i].Pos.x - Points[i].Pos.z * Points[i].Pos.z) * timestep;
                Points[i].Pos.z += (-constants["a"].value * Points[i].Pos.z - 4 * Points[i].Pos.x - 4 * Points[i].Pos.y - Points[i].Pos.x * Points[i].Pos.x) * timestep;
                break;
            
            case CHEN_LEE:
                Points[i].Pos.x += (constants["a"].value * Points[i].Pos.x - Points[i].Pos.y * Points[i].Pos.z) * timestep;
                Points[i].Pos.y += (constants["b"].value * Points[i].Pos.y + Points[i].Pos.x * Points[i].Pos.z) * timestep;
                Points[i].Pos.z += (constants["c"].value * Points[i].Pos.z + Points[i].Pos.x * (Points[i].Pos.y / 3)) * timestep;
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

        shader.setVec3("globalPosition", Points[i].Pos);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(Scale));
        model = glm::translate(model, Points[i].Pos);

        shader.setMat4("model", model);

        glDrawArrays(GL_POINTS, 0, 1);
    }
}

void Particles::clean()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}