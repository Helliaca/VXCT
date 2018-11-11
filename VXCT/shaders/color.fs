#version 450 core

out vec4 FragColor;

in vec3 nrm_fs;  
in vec3 pos_fs;

vec3 nrm = normalize(nrm_fs);

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
uniform vec3 objectColor;

void main()
{
	// ambient
    vec3 ambient = material.ambient_str * light.color;
  	
    // diffuse 
    vec3 lightDir = normalize(light.position - pos_fs);
    float diff = max(dot(nrm, lightDir), 0.0);
    vec3 diffuse = material.diffuse_str * diff * light.color;
    
    // specular
    vec3 viewDir = normalize(viewPos - pos_fs);
    vec3 reflectDir = reflect(-lightDir, nrm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular_str * spec * light.color;  

	// Attenuation
	float distance = length(light.position - pos_fs);
	float attenuation = 1.0f / (light.att_constant + light.att_linear * distance + light.att_quadratic * (distance * distance));


	vec3 result = (ambient + diffuse + specular) * material.color * attenuation;
	FragColor = vec4(result, 1.0);
}
