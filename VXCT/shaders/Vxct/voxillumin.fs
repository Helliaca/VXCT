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

struct VoxSettings {
	float diffuse_dist_factor;
	float occlusion_dist_factor;
	float specular_dist_factor;
	
	float diffuse_offset;
	float occlusion_offset;
	float specular_offset;

	float diffuse_apperture;
	float occlusion_apperture;
	float specular_apperture;

	float shadow_str;
	float shininess_falloff;

	bool phong;
	bool phong_ambient;
	bool phong_diffuse;
	bool phong_specular;
	bool vox_diffuse;
	bool vox_shadows;
	bool vox_specular;

	bool front_cone;
	bool side_cones;
	bool intermediate_cones;
};

const float PI = 3.14159;

#define MAX_LIGHTS 10
uniform PointLight[MAX_LIGHTS] lighting;
uniform int lighting_size;

uniform vec3 viewPos;
uniform VoxSettings settings;
uniform Material material;
uniform sampler3D tex3D;

vec3 nrm = normalize(nrm_fs);
float vox_size = textureSize(tex3D, 0).x; //Assuming that all dimensions of voxel map are the same as x at LOD 0.

vec3 indirectDiffuse();
vec3 directLight();

vec3 voxelTraceCone(const vec3 from, vec3 direction);
vec3 voxelTraceSpecularCone(const vec3 from, vec3 direction);
float voxelTraceOcclusionCone(const vec3 from, vec3 direction, float max_dist);

// Scales and bias a given vector (i.e. from [-1, 1] to [0, 1]).
vec3 scaleAndBias(const vec3 p) { return 0.5f * p + vec3(0.5f); }

// Returns a vector that is perpendicular/orthogonal to u (and q).
vec3 perp(vec3 v, vec3 q=vec3(0.0,0.0,1.0)){
	v = normalize(v);
	q = normalize(q);
	return cross(v, q);
}

PointLight light = lighting[0];

void main()
{
	const vec3 viewDirection = normalize(pos_fs - viewPos);
	vec4 result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	float min_occ = 1.0f;

	for(int i=0; i<MAX_LIGHTS && i<lighting_size; i++) {
		light = lighting[i];

		vec4 tmp_l = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		// Direct phong light
		if(settings.phong) tmp_l.rgb += directLight();

		// specular
		const vec3 reflectDir = normalize(reflect(viewDirection, nrm));
		if(settings.vox_specular) tmp_l.rgb += voxelTraceSpecularCone(pos_fs, reflectDir);

		// shadows
		float occ = voxelTraceOcclusionCone(pos_fs, light.position - pos_fs, length(light.position - pos_fs));
		if(occ<min_occ) min_occ = occ;

		if(settings.vox_shadows && !settings.vox_diffuse && !settings.vox_specular && !settings.phong) tmp_l = vec4(1.0f); //for showing only shadows
		if(settings.vox_shadows) tmp_l.rgb *= 1.0f - occ;

		result += tmp_l;
	}

	// Indirect diffuse light
	if(settings.vox_diffuse) result.rgb += indirectDiffuse() * (1.0f - min_occ);
	//Note: we are keeping the indirect light artifically lower with minimum occlusion because
	//the voxelTraceCone function doesnt factor in occlusion.

	FragColor = result;
}

// Calculates indirect diffuse light using voxel cone tracing.
// The current implementation uses 9 cones. I think 5 cones should be enough, but it might generate
// more aliasing and bad blur.
vec3 indirectDiffuse(){
	const vec3 origin = pos_fs + nrm * 0.05;

	vec3 y = nrm; //Front axis
	vec3 x = perp(y); //1st side axis
	vec3 z = perp(y, x); //2nd side axis

	vec3 ret = vec3(0.0f);
	float cone_count = 0;

	//front cone
	if(settings.front_cone) {
		ret += voxelTraceCone(origin, y);
		cone_count += 1;
	}

	//Side cones
	if(settings.side_cones) {
		ret += voxelTraceCone(origin, x);
		ret += voxelTraceCone(origin, -x);
		ret += voxelTraceCone(origin, z);
		ret += voxelTraceCone(origin, -z);
		cone_count += 4;
	}

	//Intermediate cones:
	if(settings.intermediate_cones) {
		float deg_mix = 0.5f;
		ret += voxelTraceCone(origin, mix(y, x, deg_mix));
		ret += voxelTraceCone(origin, mix(y, -x, deg_mix));
		ret += voxelTraceCone(origin, mix(y, z, deg_mix));
		ret += voxelTraceCone(origin, mix(y, -z, deg_mix));
		cone_count += 4;
	}

	

	return ret * (5.0f / cone_count); //All testing was done with 5 cones, so default factor is 5
} 

float voxelTraceOcclusionCone(const vec3 origin, vec3 dir, float max_dist=1.0f) {
	dir = normalize(dir);

	float current_dist = settings.occlusion_offset;

	float apperture_angle = settings.occlusion_apperture; //Angle in Radians. Will affect softness of shadows

	float occlusion = 0.0f;

	while(current_dist < max_dist && occlusion < 1) {
		//Get cone diameter (tan = cathetus / cathetus)
		float current_coneDiameter = 2.0f * current_dist * tan(apperture_angle * 0.5f);

		//Get mipmap level which should be sampled according to the cone diameter
		//log2(vox_size) returns the maximum mipmap level
		float vlevel = log2(current_coneDiameter * vox_size);
		vlevel = min( 6.0f, vlevel ); //vlevel hardcap at 3

		vec3 pos_worldspace = origin + dir * current_dist;
		vec3 pos_texturespace = (pos_worldspace + vec3(1.0f)) * 0.5f; //[-1,1] Coordinates to [0,1]

		vec4 voxel = textureLod(tex3D, pos_texturespace, vlevel);

		float shadow_str = settings.shadow_str;
		float occlusion_read = voxel.a * smoothstep(0.0f, max_dist, sqrt(current_dist)*shadow_str); //Using sqrt as all distances are <1.0; Gives us sqrt-falloff by distance

		occlusion = occlusion + (1 - occlusion) * occlusion_read;

		current_dist += current_coneDiameter * settings.occlusion_dist_factor;
	}

	return occlusion;
}

vec3 voxelTraceSpecularCone(const vec3 origin, vec3 dir) {
	float max_dist = 1.0f;
	dir = normalize(dir);

	float current_dist = settings.specular_offset;

	float apperture_angle = settings.specular_apperture; //Angle in Radians. Will affect softness of specular reflections

	vec3 color = vec3(0.0f);
	float occlusion = 0.0f;

	while(current_dist < max_dist && occlusion < 1) {
		//Get cone diameter (tan = cathetus / cathetus)
		float current_coneDiameter = 2.0f * current_dist * tan(apperture_angle * 0.5f);

		//Get mipmap level which should be sampled according to the cone diameter
		//log2(vox_size) returns the maximum mipmap level
		float vlevel = log2(current_coneDiameter * vox_size);
		vlevel = min( 6.0f, vlevel ); //vlevel hardcap at 3

		vec3 pos_worldspace = origin + dir * current_dist;
		vec3 pos_texturespace = (pos_worldspace + vec3(1.0f)) * 0.5f; //[-1,1] Coordinates to [0,1]

		vec4 voxel = textureLod(tex3D, pos_texturespace, vlevel);

		vec3 color_read = voxel.rgb;
		float occlusion_read = voxel.a;

		//if(occlusion_read>0.01f) { color = color_read; return color; } //Cheap alternative: simply return first color we find

		color = occlusion*color + (1 - occlusion) * occlusion_read * color_read;
		occlusion = occlusion + (1 - occlusion) * occlusion_read;

		current_dist += current_coneDiameter * settings.specular_dist_factor;
	}

	//Angular shininess
	vec3 frag_to_light = normalize(light.position - pos_fs);
	float angle_pr = material.shininess * 0.008f * PI; //Playing room for angle to be in without loosing brightness. 0.008 is a value chosen to give sensible values for material.shininess in range [0, 256]
	float angle = max(0.0f, acos(dot(dir, frag_to_light)) - angle_pr);
	float angle_str = pow (1 - (angle / PI), settings.shininess_falloff);
	//angle_str = 1.0f; //Un-comment to turn angular shininess off

	return material.specular_str * color * angle_str;
}

vec3 voxelTraceCone(const vec3 origin, vec3 dir) {
	float max_dist = 1.0f;
	dir = normalize(dir); //just to be safe

	float current_dist = settings.diffuse_offset;

	float apperture_angle = settings.diffuse_apperture; //Angle in Radians.

	vec3 color = vec3(0.0f);
	float occlusion = 0.0f;

	while(current_dist < max_dist && occlusion < 1) {
		//Get cone diameter (tan = cathetus / cathetus)
		float current_coneDiameter = 2.0f * current_dist * tan(apperture_angle * 0.5f);

		//Get mipmap level which should be sampled according to the cone diameter
		//log2(vox_size) returns the maximum mipmap level
		float vlevel = log2(current_coneDiameter * vox_size);
		vlevel = min( 6.0f, vlevel ); //vlevel hardcap at 3

		vec3 pos_worldspace = origin + dir * current_dist;
		vec3 pos_texturespace = (pos_worldspace + vec3(1.0f)) * 0.5f; //[-1,1] Coordinates to [0,1]

		vec4 voxel = textureLod(tex3D, pos_texturespace, vlevel);

		vec3 color_read = voxel.rgb;
		float occlusion_read = voxel.a;

		if(occlusion_read>0.01f) { color = color_read; return color; } //Cheap alternative: simply return first color we find

		//color = occlusion*color + (1 - occlusion) * occlusion_read * color_read;
		//color *= 3; //Enhance color
		//occlusion = 3 * (occlusion + (1 - occlusion) * occlusion_read);

		occlusion = occlusion + (1 - occlusion) * occlusion_read;

		current_dist += current_coneDiameter * settings.diffuse_dist_factor;
	}

	return color;
}

vec3 directLight() {
	// ambient
	vec3 ambient = vec3(0.0f);
    if(settings.phong_ambient) ambient = material.ambient_str * light.color;
  	
    // diffuse 
	vec3 diffuse = vec3(0.0f);
	vec3 lightDir = normalize(light.position - pos_fs);
	if(settings.phong_diffuse) {
		float diff = max(dot(nrm, lightDir), 0.0);
		diffuse = material.diffuse_str * diff * light.color;
	}
    
    // specular
	vec3 specular = vec3(0.0f);
	if(settings.phong_specular) {
		vec3 viewDir = normalize(viewPos - pos_fs);
		vec3 reflectDir = reflect(-lightDir, nrm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specular = material.specular_str * spec * light.color;  
	}


	// Attenuation
	float distance = length(light.position - pos_fs);
	float attenuation = 1.0f / (light.att_constant + light.att_linear * distance + light.att_quadratic * (distance * distance));


	return (ambient + diffuse + specular) * material.color * attenuation;
}