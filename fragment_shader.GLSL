#version 330 core

in vec4 vertexColor;

in vec2 textureCoords;


out vec4 FragColor;


uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, textureCoords);
}