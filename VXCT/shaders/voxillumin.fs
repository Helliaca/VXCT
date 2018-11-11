#version 450 core

out vec4 FragColor;

in vec3 pos_fs;
in vec3 nrm_fs;

struct Material {
	vec3 color;
	float ambient_str;
	float diffuse_str;
	float specular_str;
	float shininess;
};

struct PointLight {
	vec3 position;
	vec3 color;
	float att_constant;
	float att_linear;
	float att_quadratic;
};

vec3 nrm = normalize(nrm_fs);

uniform vec3 viewPos;
uniform PointLight light;
uniform Material material;
uniform sampler3D tex3D;

vec3 indirectDiffuse();
vec3 directLight();

#define TSQRT2 2.828427
#define SQRT2 1.414213
#define ISQRT2 0.707106

#define VOXEL_SIZE (1/32.0)

#define DIFFUSE_INDIRECT_FACTOR 0.9f

#define MIPMAP_HARDCAP 5.4f

vec3 voxelTraceCone(const vec3 from, vec3 direction);

// Scales and bias a given vector (i.e. from [-1, 1] to [0, 1]).
vec3 scaleAndBias(const vec3 p) { return 0.5f * p + vec3(0.5f); }

// Returns a vector that is perpendicular/orthogonal to u (and q).
vec3 perp(vec3 v, vec3 q=vec3(0.0,0.0,1.0)){
	v = normalize(v);
	q = normalize(q);
	return cross(v, q);
}



void main()
{
	FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	const vec3 viewDirection = normalize(pos_fs - viewPos);

	// Indirect diffuse light
	FragColor.rgb += indirectDiffuse() * 1.0f;

	// Direct phong light
	//FragColor.rgb += directLight();
}

// Calculates indirect diffuse light using voxel cone tracing.
// The current implementation uses 9 cones. I think 5 cones should be enough, but it might generate
// more aliasing and bad blur.
vec3 indirectDiffuse(){
	const vec3 origin = pos_fs + nrm * 0.05;
	//const vec3 origin = pos_fs;

	vec3 y = nrm; //Front axis
	vec3 x = perp(y); //1st side axis
	vec3 z = perp(y, x); //2nd side axis

	//front cone
	vec3 ret = voxelTraceCone(origin, y);

	//Side cones left out due to self-collisions
	//ret += voxelTraceCone(origin, x);
	//ret += voxelTraceCone(origin, -x);
	//ret += voxelTraceCone(origin, z);
	//ret += voxelTraceCone(origin, -z);

	//Intermediate cones:
	float deg_mix = 0.5f;
	ret += voxelTraceCone(origin, mix(y, x, deg_mix));
	ret += voxelTraceCone(origin, mix(y, -x, deg_mix));
	ret += voxelTraceCone(origin, mix(y, z, deg_mix));
	ret += voxelTraceCone(origin, mix(y, -z, deg_mix));

	

	return ret * 1.0f;
} 

vec3 voxelTraceCone(const vec3 origin, vec3 dir) {
	float max_dist = 1.0f;
	dir = normalize(dir); //just to be safe

	float current_dist = 0.19f;

	//float apperture_angle = 0.523f; //Angle in Radians.
	float apperture_angle = 0.55f; //Angle in Radians.
	//float apperture_angle = 0.01f; //Angle in Radians.

	vec3 color = vec3(0.0f);
	float occlusion = 0.0f;

	float vox_size = 64.0f; //voxel map size

	while(current_dist < max_dist && occlusion < 1) {
		//Get cone diameter (tan = cathetus / cathetus)
		float current_coneDiameter = 2.0f * current_dist * tan(apperture_angle * 0.5f);

		//Get mipmap level which should be sampled according to the cone diameter
		//log2(vox_size) returns the maximum mipmap level
		float vlevel = log2(current_coneDiameter * vox_size);
		vlevel = min( 3.0f, vlevel ); //vlevel hardcap at 3

		vec3 pos_worldspace = origin + dir * current_dist;
		vec3 pos_texturespace = (pos_worldspace + vec3(1.0f)) * 0.5f; //[-1,1] Coordinates to [0,1]

		vec4 voxel = textureLod(tex3D, pos_texturespace, vlevel);

		vec3 color_read = voxel.rgb;
		float occlusion_read = voxel.a;

		//if(occlusion_read>0.01f) { color = color_read; return color; } //Cheap alternative: simply return first color we find

		color = occlusion*color + (1 - occlusion) * occlusion_read * color_read;
		//color *= 3; //Enhance color
		occlusion = occlusion + (1 - occlusion) * occlusion_read;

		float dist_factor = 1.0f; //Lower = better results but higher performance hit
		current_dist += current_coneDiameter * dist_factor;
	}

	return color; //TODO: Return vec4 with occlusion (?)
}

vec3 directLight() {
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
        
    return (ambient + diffuse + specular) * material.color;



	// Attenuation
	float distance = length(light.position - pos_fs);
	float attenuation = 1.0f / (light.att_constant + light.att_linear * distance + light.att_quadratic * (distance * distance));


	return (ambient + diffuse + specular) * material.color * attenuation;
}