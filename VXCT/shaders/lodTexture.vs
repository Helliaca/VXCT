#version 450 core

layout (location = 0) in vec3 aPos;

out vec3 FragPos;

void main()
{
    FragPos = aPos;
    
    gl_Position = vec4( FragPos, 1.0f );
}
