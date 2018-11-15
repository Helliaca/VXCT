#pragma once

#include "../base.h"

struct VoxSettings_struct {
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

	bool phong;
	bool phong_ambient;
	bool phong_diffuse;
	bool phong_specular;
	bool vox_diffuse;
	bool vox_shadows;
	bool vox_specular;
};

class VoxSettings
{
public:
	VoxSettings();
	~VoxSettings();
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

	bool phong;
	bool phong_ambient;
	bool phong_diffuse;
	bool phong_specular;
	bool vox_diffuse;
	bool vox_shadows;
	bool vox_specular;

	VoxSettings_struct to_struct();
};

