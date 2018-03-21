#pragma once

#include <iostream>
#include <string>
#include "settings\defs.h"

class IOobject {

};

inline void print(IOobject* source, std::string out) { std::cout << out << std::endl; }