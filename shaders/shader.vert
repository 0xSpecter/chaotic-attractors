#version 410 core
layout (location = 0) in vec3 position; 
layout (location = 1) in mat4 instanceModel; 
  
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;

vec3 point = vec3(1.0, 1.0, 1.0);

void main()
{
    gl_Position = projection * view * model * vec4(point, 1.0);
    color = normalize(abs(model * vec4(point, 0.7)));
}