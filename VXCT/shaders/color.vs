#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model_u;
uniform mat4 view_u;
uniform mat4 proj_u;

void main()
{
    FragPos = vec3(model_u * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model_u))) * aNormal;  
    
    gl_Position = proj_u * view_u * vec4(FragPos, 1.0);
}
