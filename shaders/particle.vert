#version 410 core
layout (location = 0) in vec3 Pos; 
layout (location = 1) in mat4 instanceModel; 
  
uniform mat4 view;
uniform mat4 projection;

out vec4 color;

//placeholder
vec3 point = vec3(0.5, 0.5, 0.5);

void main()
{
    gl_Position = projection * view * instanceModel * vec4(point, 1.0);
    color = normalize(abs(instanceModel * vec4(point, 0.7)));
}