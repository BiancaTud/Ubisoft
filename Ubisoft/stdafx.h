// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW32/glfw3.h> // GLFW helper library
#include <iostream>
#include <stdlib.h>
#include <Windows.h>

#include <glm\glm\glm\glm.hpp>
#include <glm\glm\glm\gtc\type_ptr.hpp>
#include <glm\glm\glm\gtc\matrix_transform.hpp>

#define pi 3.1415926
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
using namespace std;


// TODO: reference additional headers your program requires here
