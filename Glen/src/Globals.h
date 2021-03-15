#pragma once

#ifndef Globals_h
#define Globals_h

#define GLM_ENABLE_EXPERIMENTAL

#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <map>
#include <algorithm>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Constants.h"
#include "Core.h"

extern std::string dpath;

#define ALIGNED(x) __declspec(align(x))

#endif /* Globals_h */
