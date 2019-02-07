#version 450 core

out vec4 FragColor;

in vec3 pos_fs;
in vec3 nrm_fs;

struct PointLight {
	vec3 position;
	vec3 color;
	float att_constant;
	float att_linear;
	float att_quadratic;
};

struct Material {
	vec3 color;
	float ambient_str;
	float diffuse_str;
	float specular_str;
	float shininess;
};

#define MAX_LIGHTS 10
uniform PointLight[MAX_LIGHTS] lighting;
uniform int lighting_size;

uniform Material material; 
uniform vec3 viewPos; 
layout(RGBA8) uniform image3D tex3D;

void main() {
	vec3 result = vec3(0,0,0);

	for(int i=0; i<MAX_LIGHTS && i<lighting_size; i++) {
		PointLight light = lighting[i];

		//>>Diffuse Lighting Calculation
		// ambient
		vec3 ambient = material.ambient_str * light.color;
  	
		// diffuse 
		vec3 norm = normalize(nrm_fs);
		vec3 lightDir = normalize(light.position - pos_fs);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = material.diffuse_str * diff * light.color;

		//specular should not be required here.
        
		result += (ambient + diffuse) * material.color;
	}

	//>>Voxelization specific
	ivec3 voxSize = imageSize(tex3D);  //get Voxelization Size
	vec3 pos_fs_normalized = 0.5f * (pos_fs + vec3(1.0f)); //Since we are in the fragment shader, we will get coordinates in Clip Space, meaning ranging from -1 to 1. In order to store them at the right place we convert them to coordinates ranging from 0 to 1.
	ivec3 location = ivec3(pos_fs_normalized * voxSize); //Location on 3d Texture to store data
	imageStore(tex3D, location, vec4(result, 1.0f));


}