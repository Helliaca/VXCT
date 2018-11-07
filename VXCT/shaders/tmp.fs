#version 450 core

out vec4 FragColor;

in vec3 pos_fs;
in vec3 nrm_fs;

uniform sampler3D tex3D_in;
layout(RGBA8) uniform image3D tex3D;

void main() {
	
	//Only on model voxels:
	//ivec3 voxSize = imageSize(tex3D);  
	//vec3 pos_fs_normalized = 0.5f * (pos_fs + vec3(1.0f)); 
	//ivec3 location = ivec3(pos_fs_normalized * voxSize); 
	//imageStore(tex3D, location, textureLod(tex3D_in, pos_fs_normalized, 1.0f));

	
	//Everywhere
	ivec3 voxSize = imageSize(tex3D);
	float voxSize_f = 64.0f;
	vec3 mapSize = vec3(1.0f, 1.0f, 1.0f);

	for(int x=0; x<voxSize.x; x++) {
		for(int y=0; y<voxSize.y; y++) {
			for(int z=0; z<voxSize.z; z++) {
				vec3 location = vec3( x*mapSize.x/voxSize_f, y*mapSize.y/voxSize_f, z*mapSize.z/voxSize_f );
				imageStore(tex3D, ivec3(x, y, z), textureLod(tex3D_in, location, 1.0f));
			}
		}
	}
	


}