#version 410 core
layout (location = 0) in vec3 position; 
  
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 globalPosition;

out vec4 color;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    color = normalize(abs(vec4(globalPosition, 0.7)));
}