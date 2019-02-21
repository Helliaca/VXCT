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

#define VOX_SIZE 256

//Amount of frames to save in FrameTimeCounter
#define FRAMETIMES_SAVE 100

//World Coordinate Bounds (Changing Values might not work)
#define MIN_X -1.0f
#define MIN_Y -1.0f
#define MIN_Z -1.0f

#define MAX_X 1.0f
#define MAX_Y 1.0f
#define MAX_Z 1.0f

//Paths
#define SCENE_TXT "scenes/scene.txt"
#define SCENE_DIR "scenes/"

#define COLORSHADER_VS "shaders/Phong/color.vs"
#define COLORSHADER_FS "shaders/Phong/color.fs"

#define EMITSHADER_VS "shaders/Emit/emit.vs"
#define EMITSHADER_FS "shaders/Emit/emit.fs"

#define EMITRGBASHADER_VS "shaders/Emit/emit_rgba.vs"
#define EMITRGBASHADER_FS "shaders/Emit/emit_rgba.fs"

#define VOXSHADER_VS "shaders/Vxct/voxelize.vs"
#define VOXSHADER_GS "shaders/Vxct/voxelize.gs"
#define VOXSHADER_FS "shaders/Vxct/voxelize.fs"

#define VOXILLUMINSHADER_VS "shaders/Vxct/voxillumin.vs"
#define VOXILLUMINSHADER_FS "shaders/Vxct/voxillumin.fs"

#define LODTEXTURESHADER_VS "shaders/Misc/lodTexture.vs"
#define LODTEXTURESHADER_FS "shaders/Misc/lodTexture.fs"

#define LINESHADER_VS "shaders/Misc/lineshader.vs"
#define LINESHADER_FS "shaders/Misc/lineshader.fs"

//Primitive Objects
#define OBJ_SCENE_CUBE1 "models/voxel_scene_cube1.obj"