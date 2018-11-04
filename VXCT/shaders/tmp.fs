#version 450 core

out vec4 FragColor;

in vec3 pos_fs;
in vec3 nrm_fs;

uniform sampler3D tex3D_in;
layout(RGBA8) uniform image3D tex3D;

void main() {
	//>>Voxelization specific
	ivec3 voxSize = imageSize(tex3D);  
	vec3 pos_fs_normalized = 0.5f * (pos_fs + vec3(1.0f)); 
	ivec3 location = ivec3(pos_fs_normalized * voxSize); 
	//imageStore(tex3D, location, vec4(0.0f, 0.0f, 1.0f, 1.0f));
	imageStore(tex3D, location, textureLod(tex3D_in, pos_fs_normalized, 4.0f));

	//imageStore(tex3D, location, vec4(0.0f, 0.0f, 1.0f, 1.0f)); //All values go to 0 0 0 as of right now
	//textureLod(tex3D_in, ivec3(0,0,0), 0)

    //FragColor = vec4(result, 1.0);


}