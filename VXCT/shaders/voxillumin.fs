#version 450 core

out vec4 FragColor;

in vec3 pos_fs;
in vec3 nrm_fs;

vec3 nrm = normalize(nrm_fs);

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler3D tex3D;

vec3 indirectDiffuse();
vec3 directLight();

#define TSQRT2 2.828427
#define SQRT2 1.414213
#define ISQRT2 0.707106

#define VOXEL_SIZE (1/32.0)

#define DIFFUSE_INDIRECT_FACTOR 0.9f

#define MIPMAP_HARDCAP 5.4f

//#define indirectDiffuseLight 1
//#define 

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

	// Indirect diffuse light.
	FragColor.rgb += indirectDiffuse();

	//FragColor.rgb += directLight();
	
	//FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

// Calculates indirect diffuse light using voxel cone tracing.
// The current implementation uses 9 cones. I think 5 cones should be enough, but it might generate
// more aliasing and bad blur.
vec3 indirectDiffuse_old(){
	const float ANGLE_MIX = 0.5f; // Angle mix (1.0f => orthogonal direction, 0.0f => direction of normal).

	const float w[3] = {1.0, 1.0, 1.0}; // Cone weights.

	// Find a base for the side cones with the normal as one of its base vectors.
	const vec3 ortho = normalize(perp(nrm));
	const vec3 ortho2 = normalize(cross(ortho, nrm));
	
	// Find base vectors for the corner cones too.
	const vec3 corner = 0.5f * (ortho + ortho2);
	const vec3 corner2 = 0.5f * (ortho - ortho2);

	// Find start position of trace (start with a bit of offset).
	const vec3 N_OFFSET = nrm * (1 + 4 * ISQRT2) * VOXEL_SIZE;
	const vec3 C_ORIGIN = pos_fs + N_OFFSET;

	// Accumulate indirect diffuse light.
	vec3 acc = vec3(0);

	// We offset forward in normal direction, and backward in cone direction.
	// Backward in cone direction improves GI, and forward direction removes
	// artifacts.
	const float CONE_OFFSET = -0.01;
	
	// Trace front cone
	acc += w[0] * voxelTraceCone(C_ORIGIN + CONE_OFFSET * nrm, nrm);

	// Trace 4 side cones.
	const vec3 s1 = mix(nrm, ortho, ANGLE_MIX);
	const vec3 s2 = mix(nrm, -ortho, ANGLE_MIX);
	const vec3 s3 = mix(nrm, ortho2, ANGLE_MIX);
	const vec3 s4 = mix(nrm, -ortho2, ANGLE_MIX);
	
	acc += w[1] * voxelTraceCone(C_ORIGIN + CONE_OFFSET * ortho, s1);
	acc += w[1] * voxelTraceCone(C_ORIGIN - CONE_OFFSET * ortho, s2);
	acc += w[1] * voxelTraceCone(C_ORIGIN + CONE_OFFSET * ortho2, s3);
	acc += w[1] * voxelTraceCone(C_ORIGIN - CONE_OFFSET * ortho2, s4);

	// Trace 4 corner cones.
	const vec3 c1 = mix(nrm, corner, ANGLE_MIX);
	const vec3 c2 = mix(nrm, -corner, ANGLE_MIX);
	const vec3 c3 = mix(nrm, corner2, ANGLE_MIX);
	const vec3 c4 = mix(nrm, -corner2, ANGLE_MIX);

	acc += w[2] * voxelTraceCone(C_ORIGIN + CONE_OFFSET * corner, c1);
	acc += w[2] * voxelTraceCone(C_ORIGIN - CONE_OFFSET * corner, c2);
	acc += w[2] * voxelTraceCone(C_ORIGIN + CONE_OFFSET * corner2, c3);
	acc += w[2] * voxelTraceCone(C_ORIGIN - CONE_OFFSET * corner2, c4);
	
	// Return result.
	// 1.0f = material.diffuseReflectivity
	return DIFFUSE_INDIRECT_FACTOR * 1.0f * acc * (objectColor + vec3(0.001f));
}

// Calculates indirect diffuse light using voxel cone tracing.
// The current implementation uses 9 cones. I think 5 cones should be enough, but it might generate
// more aliasing and bad blur.
vec3 indirectDiffuse(){
	const vec3 origin = pos_fs;

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
	float max_dist = 0.99f;
	dir = normalize(dir); //just to be safe

	float current_dist = 0.2f;

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

		vec4 voxel = textureLod(tex3D, pos_texturespace, vlevel); //get voxel TODO: perhaps set a level hardcap like so: min(hardcap, vlevel)

		vec3 color_read = voxel.rgb;
		float occlusion_read = voxel.a;

		if(occlusion_read>0.01f) { color = color_read; return color; } //Cheap alternative: simply return first color we find

		//color = occlusion*color + (1 - occlusion) * occlusion_read * color_read;
		//color *= 3; //Enhance color
		//occlusion = occlusion + (1 - occlusion) * occlusion_read;

		float dist_factor = 0.1f; //Lower = better results but higher performance hit
		current_dist += current_coneDiameter * dist_factor;
	}

	return color; //TODO: Return vec4 with occlusion (?)
}

vec3 directLight() {  	
    // diffuse 
    vec3 lightDir = normalize(lightPos - pos_fs);
    float diff = max(dot(nrm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - pos_fs);
    vec3 reflectDir = reflect(-lightDir, nrm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    return (diffuse + specular) * objectColor;
}