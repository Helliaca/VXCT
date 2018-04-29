#include "VoxelMap.h"



VoxelMap::VoxelMap(const std::vector<GLubyte> & textureBuffer) : IOobject("Unnamed VoxelMap")
{
	int voxelTextureSize = VOX_SIZE;
	//const std::vector<GLfloat> texture3D(4 * voxelTextureSize * voxelTextureSize * voxelTextureSize, 0.0f); //4 because RGBA

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	
	//Set wrapping options
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	
	//LOD settings
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	//TODO: What does this do and is it necessary?
	//Specify Storage for all levels of 3d texture
	const GLint levels = 1;
	const GLint width = voxelTextureSize;
	const GLint height = voxelTextureSize;
	const GLint depth = voxelTextureSize;
	
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, &textureBuffer[0]);
	glTexStorage3D(GL_TEXTURE_3D, levels, GL_RGBA8, width, height, depth); //TODO: Vital for program to work, but why?
	
	//glGenerateMipmap(GL_TEXTURE_3D); //If we want mipmaps

	glBindTexture(GL_TEXTURE_3D, 0); //Unbind Texture (?)

	checkErrors("VoxelMap Initialization");
}


VoxelMap::~VoxelMap()
{
	free(data); //de-allocate pixel data
}

void VoxelMap::clear() {
	//TODO
}

void VoxelMap::activate(const int shaderProgramID, const std::string glSamplerName, const int textureUnit) {
	//?
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_3D, textureID);
	glUniform1i(glGetUniformLocation(shaderProgramID, glSamplerName.c_str()), textureUnit);
}

void VoxelMap::updateMemory() {
	free(data); //De-allocate previously saved pixel data

	width = height = depth = 0;
	internalFormat = GL_RGBA8;
	
	glBindTexture(GL_TEXTURE_3D, textureID);
	
	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
	
	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_HEIGHT, &height);
	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_DEPTH, &depth);
	
	numBytes = 0;

	if (internalFormat == GL_RGB8) numBytes = width * height * depth * 3;
	else if (internalFormat == GL_RGBA8) numBytes = width * height * depth * 4;
	else { print(this, "FATAL: Texture Format Unsupported. Assuming RGBA8"); internalFormat = GL_RGBA8; numBytes = width * height * depth * 4; }

	size_t mem_size = numBytes * sizeof(GLubyte);
	data = (GLubyte*)malloc(mem_size); //Allocate image data into memory
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT); //Explicit Synchronization, as ImageStore is not always memory coherent
	glGetTexImage(GL_TEXTURE_3D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //Retrieve image data from GPU
	{
		//NOTE: This output is only correct for RGBA8 and outputs only the rist 100 values.
		print(this, "Voxelization Complete:");
		print(this, "\tVoxelMap size (bytes): " + std::to_string(numBytes));
		long int ar, ag, ab, aa, values;
		ar = ag = ab = aa = values = 0;
		for (int i = 0; i+3 < numBytes; i+=4) {
			if (std::to_string(data[i]) != "0" || std::to_string(data[i + 1]) != "0" || std::to_string(data[i + 2]) != "0" || std::to_string(data[i + 3]) != "0") {
				values++;
				ar += data[i];
				ag += data[i + 1];
				ab += data[i + 2];
				aa += data[i + 3];
				//print(this, "\tValue: R" + std::to_string(data[i]) + "G" + std::to_string(data[i+1]) + "B" + std::to_string(data[i+2]) + "A" + std::to_string(data[i+3]));
			}
		}
		print(this, "\tNon-0 Value Amount: " + std::to_string(values));
		if(values!=0) print(this, "\tAvarage Non-0 Value: R" + std::to_string(ar / values) + "G" + std::to_string(ag / values) + "B" + std::to_string(ab /values) + "A" + std::to_string(aa / values));
		print(this, "\tTotal Value: R" + std::to_string(ar) + "G" + std::to_string(ag) + "B" + std::to_string(ab) + "A" + std::to_string(aa));

		/*
		//ACCESING INDIVIDUAL COORDINATES:
		int x, y, z;
		x = 1;
		y = 7;
		z = 13;
		int arrayPos = 4 * (x + y * VOX_SIZE + z * VOX_SIZE * VOX_SIZE );
		std::string r = std::to_string(pixels[arrayPos]); 
		*/
	}

	checkErrors("VoxelMap updateMemory");
}

// voxelModel : Model to draw in place of each voxel
// shaderReference : Where to write colors of each voxel to
void VoxelMap::visualize(Model* voxelModel, glm::vec3* shaderReference) {
	if (voxelModel == nullptr) { print(this, "Visualization Error: voxelModel is NULL"); return; }

	voxelModel->setPosition(0, 0, 0);
	glm::vec3 dontDraw(0, 0, 0);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			for (int z = 0; z < depth; z++) {

				int arrayPos = 4 * (x + y * VOX_SIZE + z * VOX_SIZE * VOX_SIZE);
				
				shaderReference->r = data[arrayPos] / 255.0f;
				shaderReference->g = data[arrayPos + 1] / 255.0f;
				shaderReference->b = data[arrayPos + 2] / 255.0f;
				//shaderReference->a = data[arrayPos + 3];

				voxelModel->setPosition(vC_to_wC(glm::vec3(x, y, z)));

				if(shaderReference->r != dontDraw.r && shaderReference->g != dontDraw.g && shaderReference->g != dontDraw.b) voxelModel->draw();
			}
		}
	}
}