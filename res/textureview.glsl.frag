#version 430 core

in vec2 texCoord;

out vec4 colour;

uniform sampler2D u_texture;

void main()
{
    colour = texture(u_texture, texCoord);
}