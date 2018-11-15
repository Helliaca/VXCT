#pragma once

#define VXCT_OPGENGL_VERSION_MAJ 4
#define VXCT_OPGENGL_VERSION_MIN 5
#define VXCT_MAC_OS 0

#define VXCT_CULLING 1

#define LOG_WINDOW 1
#define LOG_ENGINE 1
#define LOG_ERROR 1
#define LOG_SHADER 1

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define WIN_NAME "VXCT"

#define VOX_SIZE 64

//World Coordinate Bounds (Changing Values might not work)
#define MIN_X -1.0f
#define MIN_Y -1.0f
#define MIN_Z -1.0f

#define MAX_X 1.0f
#define MAX_Y 1.0f
#define MAX_Z 1.0f

//Paths
#define COLORSHADER_VS "../VXCT/shaders/color.vs"
#define COLORSHADER_FS "../VXCT/shaders/color.fs"

#define EMITSHADER_VS "../VXCT/shaders/emit.vs"
#define EMITSHADER_FS "../VXCT/shaders/emit.fs"

#define EMITRGBASHADER_VS "../VXCT/shaders/emit_rgba.vs"
#define EMITRGBASHADER_FS "../VXCT/shaders/emit_rgba.fs"

#define VOXSHADER_VS "../VXCT/shaders/vox.vs"
#define VOXSHADER_GS "../VXCT/shaders/vox.gs"
#define VOXSHADER_FS "../VXCT/shaders/vox.fs"

#define VOXILLUMINSHADER_VS "../VXCT/shaders/voxillumin.vs"
#define VOXILLUMINSHADER_FS "../VXCT/shaders/voxillumin.fs"

#define LODTEXTURESHADER_VS "../VXCT/shaders/lodTexture.vs"
#define LODTEXTURESHADER_FS "../VXCT/shaders/lodTexture.fs"

#define LINESHADER_VS "../VXCT/shaders/lineshader.vs"
#define LINESHADER_FS "../VXCT/shaders/lineshader.fs"

#define OBJ_SPHERE "models/voxel_test.obj"
#define OBJ_SCENE "models/voxel_scene.obj"

//Base Scene Objects
#define OBJ_SCENE_SPHERE1 "models/voxel_scene_sphere1.obj"
#define OBJ_SCENE_SPHERE2 "models/voxel_scene_sphere2.obj"
#define OBJ_SCENE_CUBE1 "models/voxel_scene_cube1.obj"

#define OBJ_SCENE_WALL1 "models/voxel_scene_wall1.obj"
#define OBJ_SCENE_WALL2 "models/voxel_scene_wall2.obj"
#define OBJ_SCENE_WALL3 "models/voxel_scene_wall3.obj"
#define OBJ_SCENE_WALL4 "models/voxel_scene_wall4.obj"
#define OBJ_SCENE_WALL5 "models/voxel_scene_wall5.obj"
#define OBJ_SCENE_WALL6 "models/voxel_scene_wall6.obj"

#define OBJ_VIS_CONE "models/cone.obj"

#define OBJ_SCENE_BUNNY "models/stan_bunny.obj"

//#define OBJ_SCENE_LUCY "models/stan_lucy_smooth.obj"
#define OBJ_SCENE_LUCY "models/stan_lucy_lowpoly_smooth.obj" //Low poly version to decrease loading time