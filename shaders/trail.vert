#version 410 core
layout (location = 0) in vec3 Pos; 
  
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform bool colorMode;
uniform vec4 from_color; 
uniform vec4 to_color; 
uniform float fromScale;
uniform int exponent;

out vec4 color;

void main()
{
    gl_Position = projection * view * model * vec4(Pos, 1.0);

    if (colorMode) 
    {
        color = vec4(normalize(abs(model * vec4(Pos, 1.0))).xyz, 0.005 * gl_VertexID);
    }
    else
    {   
        vec3 tf_color = normalize((model * from_color).xyz * fromScale + (model * to_color).xyz * pow(length(model * vec4(Pos, 1.0)), exponent));
        color = vec4(tf_color, 0.005 * gl_VertexID);
    }
}
