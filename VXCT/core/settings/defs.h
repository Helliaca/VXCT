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

#define VOX_SIZE 16

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

#define VOXSHADER_VS "../VXCT/shaders/vox.vs"
#define VOXSHADER_GS "../VXCT/shaders/vox.gs"
#define VOXSHADER_FS "../VXCT/shaders/vox.fs"

#define OBJ_SPHERE "voxel_test.obj"
#define OBJ_SCENE "voxel_scene.obj"

//Base Scene Objects
#define OBJ_SCENE_SPHERE1 "voxel_scene_sphere1.obj"
#define OBJ_SCENE_SPHERE2 "voxel_scene_sphere2.obj"
#define OBJ_SCENE_CUBE1 "voxel_scene_cube1.obj"

#define OBJ_SCENE_WALL1 "voxel_scene_wall1.obj"
#define OBJ_SCENE_WALL2 "voxel_scene_wall2.obj"
#define OBJ_SCENE_WALL3 "voxel_scene_wall3.obj"
#define OBJ_SCENE_WALL4 "voxel_scene_wall4.obj"
#define OBJ_SCENE_WALL5 "voxel_scene_wall5.obj"
#define OBJ_SCENE_WALL6 "voxel_scene_wall6.obj"