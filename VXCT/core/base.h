#pragma once

#include <iostream>
#include <string>
#include "settings\defs.h"

class IOobject {
public:
	std::string name="unnamed";
	IOobject(std::string name) { this->name = name; }
};

inline void print(IOobject* source, std::string out) { std::cout << source->name << " >> " << out << std::endl; }