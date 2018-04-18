#pragma once

#include <iostream>
#include <string>
#include "globals.h"
#include "settings\defs.h"

class IOobject;

inline void print(IOobject* source, std::string out);

class IOobject {
public:
	std::string name="unnamed";
	IOobject(std::string name) { this->name = name; }

	void checkErrors(std::string context="") {
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			print(this, "OpenGL Error:");
			if(context!="") print(this, "\t CONTEXT: " + context);
			if (err == GL_INVALID_ENUM) print(this, "\t TYPE: Invalid Enum");
			else if (err == GL_INVALID_VALUE) print(this, "\t TYPE: Invalid Value");
			else if (err == GL_INVALID_OPERATION) print(this, "\t TYPE: Invalid Operation");
			//else if (err == GL_STACK_OVERFLOW) print(this, "\t TYPE: Stack Overflow"); //not defined
			//else if (err == GL_STACK_UNDERFLOW) print(this, "\t TYPE: Stack Underflow"); //not defined
			else if (err == GL_OUT_OF_MEMORY) print(this, "\t TYPE: Out of Memory");
			else if (err == GL_INVALID_FRAMEBUFFER_OPERATION) print(this, "\t TYPE: Invalid Framebuffer Operation");
			else if (err == GL_CONTEXT_LOST) print(this, "\t TYPE: Context Lost");
			else print(this, "\t TYPE: Undefined Error");
		}
	}

};

inline void print(IOobject* source, std::string out) { std::cout << source->name << " >> " << out << std::endl; }