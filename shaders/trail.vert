#version 410 core
layout (location = 0) in vec3 Pos; 
  
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec4 color;

void main()
{
    gl_Position = projection * view * model * vec4(Pos, 1.0);
    color = normalize(abs(model * vec4(Pos, 0.7)));
}