#version 450 core

out vec4 FragColor;

in vec3 pos_fs;
in vec3 nrm_fs;

uniform sampler3D tex3D;
uniform int lod_level;

void main() {

	vec3 pos_texturespace = (pos_fs + vec3(1.0f)) * 0.5f; //[-1,1] Coordinates to [0,1]: Gives us our current position on the VoxelMap

	FragColor = textureLod(tex3D, pos_texturespace, lod_level);
}