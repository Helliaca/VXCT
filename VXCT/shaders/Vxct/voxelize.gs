# version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 pos_gs[];
in vec3 nrm_gs[];

out vec3 pos_fs;
out vec3 nrm_fs;

void main() {
	vec3 faceNormal = abs( cross( pos_gs[1] - pos_gs[0], pos_gs[2] - pos_gs[0] ) );		//Get vector that is orthogonal to both axes of the triangle (pos1-pos0 and pos2-pos0)

	float dominantAxis = max( faceNormal.x, max( faceNormal.y, faceNormal.z ) );		//Dominant Axis selection

	
	for(uint i=0; i<3; i++) {															//For each of the three vertices (pos_gs[0-2], nrm_gs[0-2])
		pos_fs = pos_gs[i];																//Set output variables of fragmentshader identical to initial values, as we need those values there
		nrm_fs = nrm_gs[i];

		if (dominantAxis == faceNormal.x) gl_Position = vec4(pos_fs.zyx, 1);			//Rotate *actual* vertex Position so that dominant axis faces world Z axis. (Project along dominant axis with Z axis being the canvas)
		else if (dominantAxis == faceNormal.y) gl_Position = vec4(pos_fs.xzy, 1);
		else if (dominantAxis == faceNormal.z) gl_Position = vec4(pos_fs.xyz, 1);

		EmitVertex();																	//Hand vertex to fragment shader
	}

	EndPrimitive();																		//Complete output on current triangle
}