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

uniform PointLight light;
uniform Material material; 
uniform vec3 viewPos; 
layout(RGBA8) uniform image3D tex3D;

void main() {

	//>>Diffuse Lighting Calculation
    // ambient
    vec3 ambient = material.ambient_str * light.color;
  	
    // diffuse 
    vec3 norm = normalize(nrm_fs);
    vec3 lightDir = normalize(light.position - pos_fs);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = material.diffuse_str * diff * light.color;
    
    // specular
    vec3 viewDir = normalize(viewPos - pos_fs);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = material.specular_str * spec *light.color;  
        
    vec3 result = (ambient + diffuse + specular) * material.color;

	//>>Voxelization specific
	ivec3 voxSize = imageSize(tex3D);  //get Voxelization Size
	vec3 pos_fs_normalized = 0.5f * (pos_fs + vec3(1.0f)); //Since we are in the fragment shader, we will get coordinates in Clip Space, meaning ranging from -1 to 1. In order to store them at the right place we convert them to coordinates ranging from 0 to 1.
	ivec3 location = ivec3(pos_fs_normalized * voxSize); //Location on 3d Texture to store data
	imageStore(tex3D, location, vec4(result, 1.0f)); //All values go to 0 0 0 as of right now

    //FragColor = vec4(result, 1.0);


}