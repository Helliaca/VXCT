# VXCT

Implementation of Voxel Cone Tracing using OpenGL 4.5

The available shell commands are the following (vxct is short for voxel cone tracing):

* `objs`: Toggle visibility of all models in the scene.
* `voxs`: Toggle visibility of all voxels.
* `objsW`: Toggle wireframe mode for all models.
* `voxs`: Toggle wireframe mode for all voxels.
* `sfMode`: Toggle single-frame rendering mode.
* `sf`: Render a single frame if sfMode is active.
* `iLight`: Toggle indirect light (voxel cone tracing).
* `overlayW`: Overlay a wireframe on top of all models.
* `pos1`: Move camera to position 1.
* `pos2`: Move camera to position 2.
* `ray`: Cast a ray from the cameras origin, marking the surface normal of the triangle hit.
* `loclod`: Enable localized Lod values as shader.
* `avgf`: Print the average frametime of the last 100 frames.
* `clearf`: Clear all recorded frametimes.
* `dynamic`: Toggle dynamic mode (Continous revoxelization).

* `phong`: Toggle phong shading component in vxct.
* `phong_ambient`: Toggle phong ambient component in vxct.
* `phong_specular`: Toggle phong specular component in vxct.
* `phong_diffuse`: Toggle phong diffuse component in vxct.

* `front_cone`: Toggle front cone.
* `side_cones`: Toggle side cones.
* `intermediate_cones`: Toggle intermediate cones.

* `phong_only`: Display only phong component in vxct.
* `shadows_only`: Display only occlusion component in vxct.
* `specular_only`: Display only specular component in vxct.
* `diffuse_only`: Display only diffuse component in vxct.
* `all`: Enable all 4 components in vxct.

* `vox_diffuse` or `vdiff`: Toggle vxct diffuse component.
* `vox_specular` or `vspec`: Toggle vxct specular component.
* `vox_shadows` or `vshad`: Toggle vxct occlusion component.

* `diffuse_offset` or `do`: Print diffuse cone offset.
* `specular_offset` or `so`: Print specular cone offset.
* `occlusion_offset` or `oo`: Print occlusion cone offset.


* `diffuse_dist_factor [x]` or `ddf [x]`: Set vxct diffuse distance factor to x.
* `occlusion_dist_factor [x]` or `ddf [x]`: Set vxct occlusion distance factor to x.
* `specular_dist_factor [x]` or `ddf [x]`: Set vxct specular distance factor to x.

* `diffuse_offset [x]` or `do [x]`: Set vxct diffuse offset to x.
* `occlusion_offset [x]` or `oo [x]`: Set vxct occlusion offset to x.
* `specular_offset [x]` or `so [x]`: Set vxct specular offset to x.

* `diffuse_aperture [x]` or `da [x]`: Set vxct diffuse aperture to x.
* `diffuse_aperture [x]` or `da [x]`: Set vxct diffuse aperture to x.
* `diffuse_aperture [x]` or `da [x]`: Set vxct diffuse aperture to x.

* `shadow_str [x]` or `ss [x]`: Set vxct shadow strength to x.
* `shininess_falloff [x]` or `sf [x]`: Set vxct shininess falloff to x.

* `lod [x]`: Set LOD of visualized voxels or loclod shader.
* `lastframes [x]`: Print frametimes of the last x frames.

* `load [x]`: Loads a scene file with the name [x].txt located in the scenes directory.
* `vox_freq [x]`: Set revoxelization time-interval of dynamic mode to x.

* `setmat [name] [property] [x]`: Sets the specified property of the material of a model with the name [name] to x. [property] can be one of the following: `shininess, ambient_str, diffuse_str, specular_str`


