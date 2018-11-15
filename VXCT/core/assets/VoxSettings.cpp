#include "VoxSettings.h"



VoxSettings::VoxSettings()
{
	diffuse_dist_factor = 1.0f;
	occlusion_dist_factor = 0.6f;
	specular_dist_factor = 1.0f;

	diffuse_offset = 0.05f;
	occlusion_offset = 0.08f;
	specular_offset = 0.10f;

	diffuse_apperture = 0.7f;
	occlusion_apperture = 0.25f;
	specular_apperture = 0.02f;

	shadow_str = 1.5f;

	phong = true;
	phong_ambient = true;
	phong_diffuse = true;
	phong_specular = true;
	vox_diffuse = true;
	vox_shadows = true;
	vox_specular = true;

	if (true) {
		diffuse_dist_factor = 0.1f;
		diffuse_apperture = 0.55f;
		diffuse_offset = 0.19f;
	}
}


VoxSettings::~VoxSettings()
{
}

VoxSettings_struct VoxSettings::to_struct()
{
	VoxSettings_struct ret;
	ret.diffuse_dist_factor = diffuse_dist_factor;
	ret.occlusion_dist_factor = occlusion_dist_factor;
	ret.specular_dist_factor = specular_dist_factor;

	ret.diffuse_offset = diffuse_offset;
	ret.occlusion_offset = occlusion_offset;
	ret.specular_offset = specular_offset;

	ret.diffuse_apperture = diffuse_apperture;
	ret.occlusion_apperture = occlusion_apperture;
	ret.specular_apperture = specular_apperture;

	ret.shadow_str = shadow_str;

	ret.phong = phong;
	ret.phong_ambient = phong_ambient;
	ret.phong_diffuse = phong_diffuse;
	ret.phong_specular = phong_specular;
	ret.vox_diffuse = vox_diffuse;
	ret.vox_shadows = vox_shadows;
	ret.vox_specular = vox_specular;

	return ret;
}