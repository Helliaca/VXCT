# VXCT

## What does it do?

VXCT is an OpenGL 4.5 based implementation of the Voxel Cone Tracing rendering algorithm. 

It allows for easy visualization of all data utilized during the process such as 3D textures, individual lighting components and more. Customized scenes and models can be defined and loaded into the rendering environment with ease.

The development took place as part of a Bachelor's thesis at the [Freie Universität Berlin](https://www.fu-berlin.de/). A revised and corrected version of said thesis can be viewed and used as a form of documentation [here.](https://drive.google.com/file/d/1ZS57rgKCYl-T-sqEzVSDuj0E7VQEB6EY/view?usp=sharing)

![Example Pictures](https://i.imgur.com/TSqa8XK.jpg)

# Instructions

## Setup

If you are looking to only run this application without making code-based changes, simply head on over to the github releases tab and download the newest version. Make sure your GPU and driver are compatible with OpenGL 4.5.

A compressed folder will include three executables for 64x64x64, 128x128x128 and 256x256x256 sized 3D textures (voxelmaps) respectively. The higher resolution ones may cause some strain to your GPU, so I suggest commencing with the 64 variant.

Alternatively, if you are looking to compile the program yourself in Visual Studio, make sure you inlcude following dependencies:

- [Assimp 3.1.1](https://github.com/assimp/assimp/releases/tag/v3.3.1/)
- [GLFW 3.2.1](https://www.glfw.org/download.html)
- [GLM 9.8.5](https://github.com/g-truc/glm/releases/tag/0.9.8.5)
- [An OpenGL 4.5 loader generated with glad 0.1.16a0](https://glad.dav1d.de/)

Whichever method you opt for, once executed, you should be greeted by a commandline window together with an OpenGL rendering window displaying the default scene consisting of a simple Cornell Box and a low-poly version of 'Lucy', from the [Stanford 3D Scanning Repository](http://graphics.stanford.edu/data/3Dscanrep/). All other complex models shipped with this application also originate form the Stanford repository.

## Creating/Modifying a scene

If the default scene is not to your liking or you simply wish to render a scene of your own, head on over to the `/scenes` directory, where you'll find a variety of `.txt` files, each corresponding to a pre-defined scene.

You may open, edit and remove these files at your own pleasure, but keep in mind that the file `scene.txt` will be loaded by default at program start. So ensure to keep it around, even if just as an empty file.

The scene descriptions follow a simple, C-like syntax. Simply inspecting any of the pre-defined files should give you a pretty good overview of what options are available. A minimalist scene consisting of only one model and lighsource might look a little something like this:

```
scene = {
	name = mainScene
	
	model = {
		name = MyModel
		active = true
    		static = false
		path = models/my_model.obj
		position = { x = 0.0f y = 0.0f z = 0.0f }
		rotation = { x = 0.0f y = 0.0f z = 0.0f }
		scale = 1.0f
		shader = COLOR
		material = {
			shininess = 32.0f
			specular_str = 1.0f
			ambient_str = 1.0f
			diffuse_str = 1.0f
			color = { r = 1.0f g = 0.0f b = 0.0f }
		}
	}
	
	light = {
		position = { x = 0.0f y = 0.5f z = 0.0f }
		color = { r = 1.0f g = 1.0f b = 1.0f }
		att_quadratic = 1.0f
		att_linear = 1.0f
		att_constant = 1.0f
	}
}
```

Note that all models should posses an identifier `name`, which can later be used to adjust any of its attributes. Models with the `active` attribute as false will not be rendered. Set `shader` to COLOR for all regular objects and EMIT for objects like light-sources. The `static` attribute determines whether if models are included in voxel cone tracing or not. In particular all objects with a non COLOR shader should be declared as static.

The material attributes correspond to their Phong-reflection-model counterparts and will also influence the results obtained through voxel cone tracing. Textured objects are not supported at this moment. Instead, a flat color is assigned through the material's `color` attribute.

The `att_consant`, `att_linear` and `att_quadratic` attributes determine the light-sources quadratic, linear and constant attenuation values. Using their default values by simply omitting them should work for most purposes.

Keep in mind that the scene will only be voxelized within the coordinate bounds of [-1, 1] and all objects should be contained here. If your models are too large to accommodate this constraint, scale them down using the `scale` attribute.

Once you have your desired scene, simply running `load [scene_name]` will bring up your scene in the rendering window. Note that the `.txt` file extension is omitted in this command, but is still mandatory for the files in the `/scenes` directory.

## Manipulating a scene

Once you have an active scene running, you may make further adjustments at any point during runtime. However, any changes made will not persist after exiting the application.

Use `setmat [name] [property] [value]` to set any of the material properties except color (shininess, specular_str, ambient_str, diffuse_str) of a model identified by the name passed as the first argument.

The `rotate [name] [x|y|z] [value]` and `translate [name] [x|y|z] [value]` can be used to rotate or otherwise move a model along/around the specified axis. Furthermore `scale [name] [value]` will scale a model and `toggle [name]` will invert its `active` attribute.

## Voxelizing a scene

Up until now we've only looked at models rendered through a rudimentary Phong shader. To allow for voxel cone tracing to be performed, the scene must first be voxelized. There are two alternative approaches to accomplish this:

- *Manual Voxelization*: Simply running the command `vox` will voxelize the scene in its current state. Should any changes to scene be made or a different scene be loaded, you will have to run this command again to keep the voxelmap updated.
- *Dynamic Voxelization*: The command `dynamic` will toggle on or off dynamic mode, which will continuously re-voxelize the scene. The frequency of revoxelization (default is every 0.5s) can be adjusted with the `vox_freq [time-intervall]` command.  Keep in mind that dynamically revoxelizing can be a heavy strain on your GPU if you are running a high voxelmap resolution and a low `vox_freq` value.

![Example Pictures](https://i.imgur.com/AMzx3cR.jpg)

## Visualizing a voxelmap

This feature is only available, if the scene was voxelized *manually*. Copying a whole 3D texture back to the CPU is costly and thus not adequate for dynamic revoxelization. Also, note that visualizing voxels may cause really low framerates, in particular on large voxelmaps. Consider using single-frame mode.

Typing in `voxs` will toggle voxel visualization and `voxsW` will toggle wireframe mode.

If your scene is enclosed in a set of walls, as most default scenes are, you will have to remove the front wall in order to actually see anything useful. Running `toggle FrontWall` for any default scene should do the trick. Remember to revoxelize for it to take effect.

The various LOD levels of the voxelmap can retrieved with the `lod [level]` command.

Alternatively to `voxs`, you may use localized LOD mode which runs at far higher framerates. Unlike the previous method, voxels are not being rendered as individual cubes. Instead, the surfaces of every (non-static) object are given the color contained in the voxelmap at that position. The `loclod` command will toggle this mode respectively. Changing the LOD value will also work here.

## Voxel Cone Tracing

The command `iLight` will toggle between the voxel cone tracing shader and phong shader. If the scene has not been voxelized, this will result in completely black surfaces.

Once in this mode, a plethora of commands is available for the fine-tuning of individual algorithm parameters, which are all listed in the 'List of available command' below.

Most importantly, `diffuse_only`, `specular_only`, `shadows_only`, `phong_only` and `all` will allow you to look at the individual components taken into account during the rendering algorithm.

You can toggle the sets of indirect diffuse cones traced with the `front_cone`, `side_cones` and `intermediate_cones` commands.

![Example Pictures](https://i.imgur.com/RaYpTrE.jpg)

# List of available Commands

The available shell commands are the following (vxct is short for voxel cone tracing):

### Regular commands:

* `objs`: Toggle visibility of all models in the scene.
* `objsW`: Toggle wireframe mode for all models.
* `sfMode`: Toggle single-frame rendering mode.
* `sf`: Render a single frame if sfMode is active.
* `overlayW`: Overlay a wireframe on top of all models.
* `pos1`: Move camera to position 1.
* `pos2`: Move camera to position 2.
* `exit` or `quit` or `q`: Exit the application.

### Scene manipulation


* `load [x]`: Loads a scene file with the name [x].txt located in the scenes directory.
* `setmat [name] [property] [x]`: Sets the specified property of the material of a model with the name [name] to x. [property] can be one of the following: `shininess, ambient_str, diffuse_str, specular_str`
* `translate [name] [axis] [x]`: Translates the model along the specified axis by x units. `axis` can be `x`, `y` or `z`.
* `rotate [name] [axis] [x]`: Rotates the model around the specified axis by x degree. `axis` can be `x`, `y` or `z`.
* `scale [name] [x]`: Scales the model to x times its size.
* `toggle [name]`: Show/Hide the specified model.

### Voxeliaztion and voxel visualization:

* `vox`: Voxelize the scene in its current state.
* `dynamic`: Toggle dynamic mode (Continuous revoxelization).
* `voxs`: Toggle visibility of all voxels.
* `voxsW`: Toggle wireframe mode for all voxels.
* `loclod`: Enable localized Lod values as shader.
* `vox_freq [x]`: Set revoxelization time-interval of dynamic mode to x.
* `lod [x]`: Set LOD of visualized voxels or loclod shader.

### Voxel Cone Tracing

* `iLight`: Toggle indirect light (voxel cone tracing).

--

* `phong`: Toggle direct light shading component in vxct.
* `phong_ambient`: Toggle direct light ambient component in vxct.
* `phong_specular`: Toggle direct light specular/glossy component in vxct.
* `phong_diffuse`: Toggle direct light diffuse component in vxct.

--

* `front_cone`: Toggle front cone.
* `side_cones`: Toggle side cones.
* `intermediate_cones`: Toggle intermediate cones.

--

* `phong_only`: Display only phong component in vxct.
* `shadows_only`: Display only occlusion component in vxct.
* `specular_only`: Display only specular component in vxct.
* `diffuse_only`: Display only diffuse component in vxct.
* `all`: Enable all 4 components in vxct.

--

* `vox_diffuse` or `vdiff`: Toggle vxct diffuse component.
* `vox_specular` or `vspec`: Toggle vxct specular component.
* `vox_shadows` or `vshad`: Toggle vxct occlusion component.

--

* `diffuse_offset` or `do`: Print diffuse cone offset.
* `specular_offset` or `so`: Print specular cone offset.
* `occlusion_offset` or `oo`: Print occlusion cone offset.

--

* `diffuse_dist_factor [x]` or `ddf [x]`: Set vxct diffuse distance factor to x.
* `occlusion_dist_factor [x]` or `ddf [x]`: Set vxct occlusion distance factor to x.
* `specular_dist_factor [x]` or `ddf [x]`: Set vxct specular distance factor to x.

--

* `diffuse_offset [x]` or `do [x]`: Set vxct diffuse offset to x.
* `diffuse_origin_offset [x]` or `doo [x]`: Set the offset of vxct diffuse cones' origin to x.
* `occlusion_offset [x]` or `oo [x]`: Set vxct occlusion offset to x.
* `specular_offset [x]` or `so [x]`: Set vxct specular offset to x.

--

* `diffuse_aperture [x]` or `da [x]`: Set vxct diffuse cone aperture to x.
* `occlusion_aperture [x]` or `oa [x]`: Set vxct occlusion cone aperture to x.
* `specular_aperture [x]` or `sa [x]`: Set vxct specular cone aperture to x.

--

* `shadow_str [x]` or `ss [x]`: Set vxct shadow strength to x.
* `shininess_falloff [x]` or `sf [x]`: Set vxct shininess falloff to x.


### Utilities

* `ray`: Cast a ray from the cameras origin, marking the surface normal of the triangle hit.
* `avgf`: Print the average frametime of the last 100 frames.
* `clearf`: Clear all recorded frametimes.
* `lastframes [x]`: Print frametimes of the last x frames.
* `draw_block`: Will force CPU to wait for GPU to finish drawing. Allows GPU performance measurements to be made with `avgf` and `clearf`.


