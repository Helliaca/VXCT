#version 450 core

out vec4 FragColor;

uniform vec4 emitColor;

void main()
{
    FragColor = emitColor; //Emit a specified RGBA color
}