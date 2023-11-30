#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>
#include<random>

#include <Windows.h>
#include <MMSystem.h> 

#include <stdlib.h>
#include <math.h>
#include<time.h>
#include<stdio.h>

#include "gl/glm/glm.hpp"
#include "gl/glm/ext.hpp"
#include "gl/glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include "GL/freeglut.h"


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")

using namespace std;

class Hexahedron {
public:

	GLfloat Smax;
	GLfloat Smin;
	GLfloat s;

	bool is;
	bool canSee;

	GLfloat width;
	GLfloat height;
	int hor; 
	int ver; 

	
	int where;
	GLfloat speed;
	float aniSt{};
};










