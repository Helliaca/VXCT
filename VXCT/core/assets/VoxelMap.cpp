#include "VoxelMap.h"



VoxelMap::VoxelMap(const std::vector<GLubyte> & textureBuffer, const std::vector<GLubyte> & lod_textureBuffer) : IOobject("Unnamed VoxelMap")
{
	saved_lod_level = 0;
	int voxelTextureSize = VOX_SIZE;
	//const std::vector<GLfloat> texture3D(4 * voxelTextureSize * voxelTextureSize * voxelTextureSize, 0.0f); //4 because RGBA

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	
	//Set wrapping options
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	
	//LOD and Texture settings
	//smooth version:
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Mipmaps
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Texture
	//non-smooth version
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); //Mipmaps
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //Texture
	
	//TODO: What does this do and is it necessary?
	//Specify Storage for all levels of 3d texture
	const GLint levels = 7;
	const GLint width = voxelTextureSize;
	const GLint height = voxelTextureSize;
	const GLint depth = voxelTextureSize;
	
	glTexStorage3D(GL_TEXTURE_3D, levels, GL_RGBA8, width, height, depth); //Specify storage

	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth, GL_RGBA, GL_UNSIGNED_BYTE, &textureBuffer[0]); //Set colors for lowest mipmap level (all black)
	
	glGenerateMipmap(GL_TEXTURE_3D); //No need to generate mipmaps right here ass the texture is completely black

	glBindTexture(GL_TEXTURE_3D, 0); //Unbind Texture

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
	retrieveData(0);

	glGenerateMipmap(GL_TEXTURE_3D); //Re-generate mimaps after having drawn to them. Note: Imagelod will not work without running this.

	print(this, "Voxelization Complete:");
	print(this, "\tVoxelMap size (bytes): " + std::to_string(numBytes));

	checkErrors("VoxelMap updateMemory");
	return; //Remove this to re-enable voxel visualzation Details

	{
		//Write to CPU memory
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

	checkErrors("VoxelMap updateMemory Details");
}

// voxelModel : Model to draw in place of each voxel
// shaderReference : Where to write colors of each voxel to
void VoxelMap::visualize(Model* voxelModel, glm::vec3* shaderReference, int lod_level) {
	GLint w, h, d;
	GLubyte* draw_data;

	if (voxelModel == nullptr) { print(this, "Visualization Error: voxelModel is NULL"); return; }
	if (lod_level != 0 && lod_level != saved_lod_level) {
		print(this, "Lod Level " + std::to_string(lod_level) + " not cached. Retrieving...");
		retrieveData(lod_level);
		saved_lod_level = lod_level;
		print(this, "Retrieved: " + std::to_string(lod_numBytes) + " bytes");
	}

	if (lod_level == 0) {
		w = width;
		h = height;
		d = depth;
		draw_data = data;
	}
	else {
		w = lod_width;
		h = lod_height;
		d = lod_depth;
		draw_data = lod_data;
	}

	float voxelmap_size = w; //NOTE: We assume that the voxelmap is cubic, meaning w = h = d
	voxelModel->resetPSR();
	voxelModel->setPosition(0, 0, 0);
	voxelModel->scale((MAX_X - MIN_X) / voxelmap_size); //Note: only MAX/MIN_X is taken into account when sclaing voxel representatives
	float dontDraw = 0.0; //Alpha threshold for which not to draw a voxel
	glm::vec3 voxelOffset(1.0f / voxelmap_size, 1.0f / voxelmap_size, 1.0f / voxelmap_size); //Origin of our voxel-cube model is centered, thus we need to offset to set it to the actual position of the voxel

	int arrayPos = 0; //Declaring these variables outside the upcoming loop gives us a (quite small) performance boost
	float alpha = 0;

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			for (int z = 0; z < d; z++) {

				arrayPos = 4 * (x + y * voxelmap_size + z * voxelmap_size * voxelmap_size); //4 due to rgba
				
				alpha = draw_data[arrayPos + 3]; //get alpha value

				if (alpha > dontDraw) {
					shaderReference->r = draw_data[arrayPos] / 255.0f;
					shaderReference->g = draw_data[arrayPos + 1] / 255.0f;
					shaderReference->b = draw_data[arrayPos + 2] / 255.0f;
					voxelModel->setPosition(voxelOffset + vC_to_wC(glm::vec3(x, y, z), voxelmap_size));
					voxelModel->draw();
				}
			}
		}
	}
}

void VoxelMap::retrieveData(int level) {
	if(level == 0) free(data); //De-allocate previously saved pixel data
	else free(lod_data);

	GLubyte* putdatahere;

	GLint w, h, d, format, byte_amount; //width, height, depth
	w = h = d = 0;
	format = GL_RGBA8;

	glBindTexture(GL_TEXTURE_3D, textureID);

	glGetTexLevelParameteriv(GL_TEXTURE_3D, level, GL_TEXTURE_INTERNAL_FORMAT, &format);

	glGetTexLevelParameteriv(GL_TEXTURE_3D, level, GL_TEXTURE_WIDTH, &w);
	glGetTexLevelParameteriv(GL_TEXTURE_3D, level, GL_TEXTURE_HEIGHT, &h);
	glGetTexLevelParameteriv(GL_TEXTURE_3D, level, GL_TEXTURE_DEPTH, &d);

	byte_amount = 0;

	if (format == GL_RGB8) byte_amount = w * h * d * 3;
	else if (format == GL_RGBA8) byte_amount = w * h * d * 4;
	else { print(this, "FATAL: Texture Format Unsupported. Assuming RGBA8"); format = GL_RGBA8; byte_amount = w * h * d * 4; }

	size_t mem_size = byte_amount * sizeof(GLubyte);
	putdatahere = (GLubyte*)malloc(mem_size); //Allocate image data into memory
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT); //Explicit Synchronization, as ImageStore is not always memory coherent
	glGetTexImage(GL_TEXTURE_3D, level, GL_RGBA, GL_UNSIGNED_BYTE, putdatahere); //Retrieve image data from GPU

	//update object data if level is 0
	if (level == 0) {
		this->height = h;
		this->width = w;
		this->depth = d;
		this->internalFormat = format;
		this->numBytes = byte_amount;
		this->data = putdatahere;
	}
	else {
		this->lod_height = h;
		this->lod_width = w;
		this->lod_depth = d;
		this->lod_internalFormat = format;
		this->lod_numBytes = byte_amount;
		this->lod_data = putdatahere;
	}

	checkErrors("VoxelMap Retrieve Data");
}