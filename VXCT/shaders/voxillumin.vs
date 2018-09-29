//Applies model, view and projection matrices

#version 450 core

layout (location = 0) in vec3 pos_vs; //Vertex position
layout (location = 1) in vec3 nrm_vs; //Vertex normal

out vec3 pos_fs; //Position in world coordinates
out vec3 nrm_fs;

uniform mat4 model_u;
uniform mat4 view_u;
uniform mat4 proj_u;

void main() {
	pos_fs = vec3( model_u * vec4( pos_vs, 1.0f ) );
	nrm_fs = mat3( transpose( inverse(model_u) ) ) * nrm_vs; 
    
    gl_Position = proj_u * view_u * vec4( pos_fs , 1.0);
}