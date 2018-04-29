#version 450 core

out vec4 FragColor;

in vec3 pos_fs;
in vec3 nrm_fs;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
layout(RGBA8) uniform image3D tex3D;

vec3 indirectDiffuse();

// Scales and bias a given vector (i.e. from [-1, 1] to [0, 1]).
vec3 scaleAndBias(const vec3 p) { return 0.5f * p + vec3(0.5f); }

void main() {
	vec4 defColor = vec4(0, 0, 0, 1.0);

	//const vec3 viewDirection = normalize(pos_fs - cameraPosition);

	//Indirect Diffuse Lighting
	defColor += indirectDiffuse(viewDirection);

	//Indirect Specular Lighting

	//Emissivity

	//Transparency

	//Direct Light
}

vec3 indirectDiffuse() {
	
}

// origin, dir, and maxDist are in texture space
// dir should be normalized
// coneRatio is the cone diameter to height ratio (2.0 for 90-degree cone)
vec4 voxelTraceCone(vec3 origin, vec3 dir, float coneRatio, float maxDist)
{
	vec3 samplePos = origin;
	vec3 accum = vec3(0.0);
	float alpha = 0.0;

	// the starting sample diameter
	float minDiameter = minVoxelDiameter;

	// push out the starting point to avoid self-intersection
	float startDist = minDiameter * 10;
	
	float dist = startDist;
	while (dist <= maxDist && alpha < 1.0)
	{
		// ensure the sample diameter is no smaller than the min
		// desired diameter for this cone (ensuring we always
		// step at least minDiameter each iteration, even for tiny
		// cones - otherwise lots of overlapped samples)
		float sampleDiameter = max(minDiameter, coneRatio * dist);
		
		// convert diameter to LOD
		// for example:
		// log2(1/256 * 256) = 0
		// log2(1/128 * 256) = 1
		// log2(1/64 * 256) = 2
		float sampleLOD = log2(sampleDiameter * minVoxelDiameterInv);
		
		vec3 samplePos = origin + dir * dist;
		
		vec4 sampleValue = textureLod (volumeTexture, samplePos, min (sampleLOD, VOXEL_MIPMAP_COUNT - 1.0));
		
		accum = accum + (1.0 - alpha) * sampleValue.a * sampleValue.rgb;
		alpha = alpha + (1.0 - alpha) * sampleValue.a;

		dist += sampleDiameter;
	}
	
	// decompress color range to decode limited HDR
	// accum *= 2.0;
	
	return vec4 (accum, alpha);
}