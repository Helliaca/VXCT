#include "VoxSettings.h"



VoxSettings::VoxSettings()
{
	//These values are chosen to work for 64x64x64 voxelMaps
	diffuse_dist_factor = 0.1f;
	occlusion_dist_factor = 0.35f;
	specular_dist_factor = 0.8f;

	diffuse_offset = 0.19f;
	diffuse_origin_offset = 0.05f;
	occlusion_offset = 0.08f;
	specular_offset = 0.15f;

	diffuse_apperture = 0.55f;
	occlusion_apperture = 0.4f;
	specular_apperture = 0.04f;

	shadow_str = 1.5f;
	shininess_falloff = 3.0f;

	phong = true;
	phong_ambient = true;
	phong_diffuse = true;
	phong_specular = false; //turning this off, since cone-trace specular seems good enough on its own
	vox_diffuse = true;
	vox_shadows = true;
	vox_specular = true;

	bool front_cone = true;
	bool side_cones = false;
	bool intermediate_cones = true;

	//Different voxelmap sizes require different values.
	if (VOX_SIZE == 128) {
		diffuse_dist_factor = 0.01f;
		specular_apperture = 0.02f;
		specular_dist_factor = 0.3f;
	}

	//Different voxelmap sizes require different values.
	if (VOX_SIZE == 256) {
		occlusion_dist_factor = 0.1f;
		shininess_falloff = 2.0f;
		specular_offset = 0.05f;
		specular_apperture = 0.01f;
		specular_dist_factor = 0.15f;
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
	ret.diffuse_origin_offset = diffuse_origin_offset;
	ret.occlusion_offset = occlusion_offset;
	ret.specular_offset = specular_offset;

	ret.diffuse_apperture = diffuse_apperture;
	ret.occlusion_apperture = occlusion_apperture;
	ret.specular_apperture = specular_apperture;

	ret.shadow_str = shadow_str;
	ret.shininess_falloff = shininess_falloff;

	ret.phong = phong;
	ret.phong_ambient = phong_ambient;
	ret.phong_diffuse = phong_diffuse;
	ret.phong_specular = phong_specular;
	ret.vox_diffuse = vox_diffuse;
	ret.vox_shadows = vox_shadows;
	ret.vox_specular = vox_specular;

	ret.front_cone = front_cone;
	ret.side_cones = side_cones;
	ret.intermediate_cones = intermediate_cones;

	return ret;
}