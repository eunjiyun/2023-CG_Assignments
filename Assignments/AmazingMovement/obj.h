#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include<random>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <Windows.h>
#include <MMSystem.h> 

#include <stdlib.h>
#include <math.h>
#include<time.h>
#include<stdio.h>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "gl/glm/glm.hpp"
#include "gl/glm/ext.hpp"
#include "gl/glm/gtc/matrix_transform.hpp"


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")

using namespace std;

class Hexahedron {
public:
	bool is;
	bool show;

	int l; // low num
	int c; // col num
	GLfloat w; // x 너비
	GLfloat h; // y 너비
	float start_Animation{};
	GLfloat max_scale;
	GLfloat min_scale;

	GLfloat velocity;

	GLfloat scale;

	int dir; // -2 : moving down, -1 : stop, heading down /// 2 : moving up, 1 : stop, heading up 
};

class Me {
public:
	GLfloat x;
	GLfloat y;

	int dir;
	int moving_dir;

	GLfloat s_value;

	int cnt;
};

class Rgb {
public:
	float r;
	float g;
	float b;
};

class ObjectIn {
public:
	vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	vector< glm::vec3 > temp_vertices;
	vector< glm::vec2 > temp_uvs;
	vector< glm::vec3 > temp_normals;
	vector< glm::vec3 > outvertex, outnormal;
	vector< glm::vec2 > outuv;

	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	float aveX, aveY, aveZ;
	float scaleX, scaleY, scaleZ;
	float minX = 0.0, minY = 0.0, minZ = 0.0;
	float maxX = 0.0, maxY = 0.0, maxZ = 0.0;
	float scaleAll;

	float sizeX, sizeY, sizeZ;

};





