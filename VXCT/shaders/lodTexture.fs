#version 450 core

out vec4 FragColor;

in vec3 FragPos;

layout(RGBA8) uniform image3D tex3D;
//layout(RGBA8) uniform image3D lod_tex3D;
uniform float lod_level;

void main() {
	ivec3 tex3D_size = imageSize(tex3D);
	imageStore(tex3D, ivec3(0, 0, 0), vec4(0.0f, 0.0f, 1.0f, 1.0f));
	for(int x=0; x<tex3D_size.x; x++) {
		for(int y=0; y<tex3D_size.y; y++) {
			for(int z=0; z<tex3D_size.z; z++)	{
				imageStore(tex3D, ivec3(x, y, z), vec4(0.0f, 0.0f, 1.0f, 1.0f));
			}
		}
	}

	/*
	ivec3 og_size = imageSize(og_tex3D);
	ivec3 lod_size = imageSize(lod_tex3D); //this should be log2(og_size)

	for(int x=0; x<lod_size.x; x++) {
		for(int y=0; y<lod_size.y; y++) {
			for(int z =0; z<lod_size.z; z++) {
			
			}
		}
	}

	vec3 pos_fs_normalized = 0.5f * (pos_fs + vec3(1.0f)); //Since we are in the fragment shader, we will get coordinates in Clip Space, meaning ranging from -1 to 1. In order to store them at the right place we convert them to coordinates ranging from 0 to 1.
	ivec3 location = ivec3(pos_fs_normalized * voxSize); //Location on 3d Texture to store data
	imageStore(tex3D, location, vec4(result, 1.0f)); //All values go to 0 0 0 as of right now
	*/

    //FragColor = vec4(result, 1.0);


}