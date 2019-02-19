#version 450 core

layout (location = 0) in vec3 aPos;

uniform mat4 view_u;
uniform mat4 proj_u;

void main()
{
	gl_Position = proj_u * view_u * vec4(aPos, 1.0);
}