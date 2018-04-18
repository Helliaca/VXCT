#version 450 core

out vec4 FragColor;

in vec3 pos_fs;
in vec3 nrm_fs;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
layout(RGBA8) uniform image3D tex3D;

void main() {

	//>>Diffuse Lighting Calculation
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(nrm_fs);
    vec3 lightDir = normalize(lightPos - pos_fs);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - pos_fs);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;

	//>>Voxelization specific
	ivec3 voxSize = imageSize(tex3D);  //get Voxelization Size
	vec3 pos_fs_normalized = 0.5f * (pos_fs + vec3(1.0f)); //Since we are in the fragment shader, we will get coordinates in Clip Space, meaning ranging from -1 to 1. In order to store them at the right place we convert them to coordinates ranging from 0 to 1.
	ivec3 location = ivec3(pos_fs_normalized * voxSize); //Location on 3d Texture to store data
	imageStore(tex3D, location, vec4(result, 1.0f)); //All values go to 0 0 0 as of right now

    //FragColor = vec4(result, 1.0);


}