#version 410 core
layout (location = 0) in vec3 Pos; 
  
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform vec4 from_color; // if equal (0.0, 0.0, 0.0, 0.0) then runs std color code
uniform vec4 to_color; 
uniform float fromScale;

out vec4 color;

void main()
{
    gl_Position = projection * view * model * vec4(Pos, 1.0);

    if (to_color == vec4(0.0, 0.0, 0.0, 0.0)) 
    {
        color = vec4(normalize(abs(model * vec4(Pos, 1.0))).xyz, 0.005 * gl_VertexID);
    }
    else
    {   
        vec3 tf_color = normalize((model * from_color).xyz * fromScale + (model * to_color).xyz * (length(model * vec4(Pos, 1.0)) * length(model * vec4(Pos, 1.0)) * length(model * vec4(Pos, 1.0))));
        color = vec4(tf_color, 1.0);
    }
}