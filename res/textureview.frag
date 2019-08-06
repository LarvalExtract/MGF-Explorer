#version 400 core

in vec2 texCoord;

out vec4 colour;

uniform sampler2D test;

void main()
{
    colour = texture(test, texCoord);
}