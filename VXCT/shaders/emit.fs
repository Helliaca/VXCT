#version 450 core

out vec4 FragColor;

uniform vec3 emitColor;

void main()
{
    FragColor = vec4(emitColor, 1.0);
}