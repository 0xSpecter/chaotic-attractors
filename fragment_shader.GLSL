#version 330 core

in vec4 vertexColor;

in vec2 textureCoords;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, textureCoords), texture(texture2, textureCoords), 0.6);
}