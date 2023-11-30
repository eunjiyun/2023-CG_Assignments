#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "gl/glm/glm.hpp"
#include "gl/glm/ext.hpp"
#include "gl/glm/gtc/matrix_transform.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <MMSystem.h> 

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")

using namespace std;

class objRead {
public:
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	std::vector< glm::vec3 > outvertex, outnormal;
	std::vector< glm::vec2 > outuv;

	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	float aveX, aveY, aveZ;
	float scaleX, scaleY, scaleZ;
	float minX = 0.0, minY = 0.0, minZ = 0.0;
	float maxX = 0.0, maxY = 0.0, maxZ = 0.0;
	float scaleAll;

	float sizeX, sizeY, sizeZ;

	int loadObj_normalize_center(const char* filename);
};

#define SWAP(a, b, type) do { \
    type temp; \
    temp = a;  \
    a = b;     \
    b = temp;  \
} while (0)

#define TRI_COUNT 500
#define SCREEN_W (GLint)1000
#define SCREEN_H (GLint)1000

bool left_button = 0;
bool right_button = 0;

class COLORVALUE {
public:
	float r;
	float g;
	float b;
};

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* filetobuf(const char* file);
void random_color(COLORVALUE* color);
void Mouse(int button, int state, int x, int y);
void con_D_to_Ogl(int x, int y, float* ox, float* oy);
void con_Ogl_to_D(int* x, int* y, float ox, float oy);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
GLvoid KeyboardSpecial(int key, int x, int y);
void Motion(int x, int y);
void TimerFunction(int value);

void subDFS(char** maze, bool** visited, int y, int x);
void DFS(char** maze, bool** visited, int y, int x);
char** setMaze(int size);
void printMaze(char*** maze);

void SetCamera();
void SetProjection();
void SetTransform();
void SetTransform_FPS();
void set_radians();

void draw_pillar(glm::mat4 TR, unsigned int modelLocation);
void draw_player(glm::mat4 TR, unsigned int modelLocation);
void draw_rect();
void draw_rect_line();

void PrintInstruction();

void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
void InitBuffer();
void InitShader();

void init_LOW_COL();
void init_PILLARS();
void init_PLAYER();
void init_ALL();

void move_LEFT();
void move_RIGHT();
void move_UP();
void move_DOWN();

void ChangeVelocity(int d);
void LowHeight();

GLchar* vertexSource, * fragmentSource;
GLuint vertexShader;
GLuint fragmentShader;
GLuint s_program;
GLuint vao, vbo[3];

// ���� ���� ����
int enable_depth = true;

GLfloat DEGREES = 0.0f;
GLfloat cameraPosZ = 2.0f;
GLfloat cameraPosX = 0.0f;

int Rotate_Y_Anime = 0;
GLfloat cameraY = 0.0f;

int perspective_view = 1;
GLfloat perspective_Z = -5.0f;

GLfloat Svalue;

int UpDownMoveAnime = false;

GLfloat ra = 0.0f;
int show_player = false;
int view_fps = false;

bool off{ true };
int lightColorLocation{};
int chooseCol{ 1 };
bool initLight{ false };

GLfloat pos[TRI_COUNT * 3][3] =
{
	// �Ʒ� 0
	{-1.0, -1.0, -1.0}, {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0},
	{1.0, -1.0, 1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0},

	// ���� 6
	{-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0},
	{-1.0, 1.0, -1.0}, {-1.0, -1.0, 1.0}, {-1.0, -1.0, -1.0},

	// right 12
	{1.0, -1.0, -1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, 1.0},
	{1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, -1.0},

	// behind 18
	{-1.0, 1.0, -1.0}, {-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0},
	{-1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {1.0, 1.0, -1.0},

	// front 24
	{-1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0},
	{-1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, 1.0},

	// top 30
	{-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0}, {1.0, 1.0, 1.0},
	{1.0, 1.0, 1.0}, {1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0},

	{-1.0, -1.0, -1.0}, {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0},
	{1.0, -1.0, 1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0},
	{-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0},
	{-1.0, 1.0, -1.0}, {-1.0, -1.0, 1.0}, {-1.0, -1.0, -1.0},
	{1.0, -1.0, -1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, 1.0},
	{1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, -1.0},
	{-1.0, 1.0, -1.0}, {-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0},
	{-1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {1.0, 1.0, -1.0},
	{-1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0},
	{-1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, 1.0},
	{-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0}, {1.0, 1.0, 1.0},
	{1.0, 1.0, 1.0}, {1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0},


	//bottom, 
	{-1.0, -1.0, 1.0}, {0.0, -1.0, 0.5}, {0.0, -1.0, -1.0}, // left
	{0.0, -1.0, 0.5}, {0.0, -1.0, -1.0}, {1.0, -1.0, 1.0},	// right
	// top
	{-1.0, -1.0, 1.0}, {0.0, 0.0, 0.5}, {0.0, -1.0, -1.0}, // left
	{0.0, 0.0, 0.5}, {0.0, -1.0, -1.0}, {1.0, -1.0, 1.0},  // right
	// behind
	{-1.0, -1.0, 1.0}, {0.0, -1.0, 0.5}, {0.0, 0.0, 0.5}, // left
	{1.0, -1.0, 1.0}, {0.0, -1.0, 0.5}, {0.0, 0.0, 0.5},  // right
};

GLfloat col[TRI_COUNT * 3][3] =
{
	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, // down
	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f},

	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, // left
	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f},

	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, // right
	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f},

	{0.6f, 0.6f, 0.6f}, {0.6f, 0.6f, 0.6f}, {0.6f, 0.6f, 0.6f}, // behind
	{0.6f, 0.6f, 0.6f}, {0.6f, 0.6f, 0.6f}, {0.6f, 0.6f, 0.6f},

	{0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, // front
	{0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f},

	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, // top
	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},


	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},

	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},


	// bottom
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
	// top
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
	{1.0f, 0.1f, 0.1f}, {1.0f, 0.1f, 0.1f}, {1.0f, 0.1f, 0.1f},
	// behind
	{1.0f, 0.3f, 0.3f}, {1.0f, 0.3f, 0.3f}, {1.0f, 0.3f, 0.3f},
	{1.0f, 0.2f, 0.2f}, {1.0f, 0.2f, 0.2f}, {1.0f, 0.2f, 0.2f},
};

class PILLAR {
public:
	bool exist;
	bool show;

	int l; // low num
	int c; // col num
	GLfloat w; // x �ʺ�
	GLfloat h; // y �ʺ�

	GLfloat max_scale;
	GLfloat min_scale;

	GLfloat velocity;

	GLfloat scale;

	int dir; // -2 : moving down, -1 : stop, heading down /// 2 : moving up, 1 : stop, heading up 
};

class PLAYER {
public:
	GLfloat x;
	GLfloat y;

	int dir;
	int moving_dir;

	GLfloat s_value;

	int cnt;
};

// 2

PILLAR B[26][26];
PLAYER P;

int LOW = 0, COL = 0;


GLvoid main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{
	PlaySound(L"inGame.wav", NULL, SND_ASYNC | SND_LOOP);

	init_LOW_COL();

	init_PILLARS();
	init_PLAYER();

	PrintInstruction();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(800, 50);
	glutInitWindowSize(SCREEN_H, SCREEN_W);
	glutCreateWindow("Amazing Movement");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GAME START!\n";

	make_vertexShaders();
	make_fragmentShaders();
	s_program = make_shaderProgram();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(KeyboardSpecial);

	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);

	srand(unsigned int(time(NULL)));
	rand();


	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

void init_LOW_COL() {
	printf("Enter Low : ");
	fseek(stdin, 0, SEEK_END);
	scanf("%d", &LOW);

	printf("Enter Col : ");
	fseek(stdin, 0, SEEK_END);
	scanf("%d", &COL);

	if (!(5 <= LOW and LOW <= 25)) {
		printf("not appropriate value \n");
		init_LOW_COL();
	}
	//COL = LOW;
	Svalue = 2.0f / LOW;
	if (Svalue <= 0) printf("Side Value Error = %f\n", Svalue);
}

void init_PILLARS() {
	// LOW�� COL�� ����, exist �ο�.

	for (int c = 0; c < COL; c++) {     // 0 ~ COL
		for (int l = 0; l < LOW; l++) { // 0 ~ LOW

			B[c][l].show = true;
			B[c][l].exist = true;

			B[c][l].c = c;
			B[c][l].l = l;
			B[c][l].w = Svalue, B[c][l].h = Svalue;

			switch (rand() % 2) {
			case 0: B[c][l].dir = -2; break;
			case 1: B[c][l].dir = 2; break;
			}

			// scale => 0.0f => 1.0f
			B[c][l].max_scale = 0.5f + ((float)rand() / (RAND_MAX) / 2);
			B[c][l].min_scale = 0.1f + ((float)rand() / (RAND_MAX) / 4);

			if (B[c][l].max_scale < B[c][l].min_scale) printf("\n\n\n\n!!!! MIN SCALE ERROR !!!!\n\n\n\n");

			B[c][l].scale = B[c][l].max_scale - 0.2f;



			// velocity, ������ value�� �������ֱ�.
			B[c][l].velocity = 0.002f + (float)rand() / (RAND_MAX) / 300;
			// printf("|%d|%d| max (%f) / min (%f) / vel (%f)\n", c, l, B[c][l].max_scale, B[c][l].min_scale, B[c][l].velocity);

			// printf("%f    ", B[c][l].velocity);
		}
	}
}

void init_PLAYER() {
	// -1 : left, 1: right, 2: top, -2: bottom
	P.dir = 2;
	P.moving_dir = 0;
	P.x = float(LOW) - 2.0f;
	P.y = 0.0f;

	P.s_value = Svalue / 2;
	P.cnt = 0;
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{

	// glViewport(0, 0, w, h);
	;
}

int top_view = 0;

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);

	if (enable_depth) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	set_radians();

	glViewport(0, 0, SCREEN_W, SCREEN_H);
	top_view = false;
	SetCamera();
	SetProjection();
	if (view_fps) SetTransform_FPS();
	else SetTransform();

	if (view_fps) glDisable(GL_DEPTH_TEST);
	glViewport(700, 700, 300, 300);
	top_view = true;
	SetCamera();
	SetProjection();
	SetTransform();

	glBindVertexArray(vao);

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLfloat Zmove = 0.0f;

void SetTransform() {
	glm::mat4 TR = glm::mat4(1.0f);

	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");

	/* ------------------------------ ���� ������ ���� �̵� ------------------------------ */
	if (perspective_view)
		if (top_view) TR = glm::translate(TR, glm::vec3(0.0, 0.0, -2.0));
		else TR = glm::translate(TR, glm::vec3(0.0, 0.0, perspective_Z));
	/* ---------------------------------------------------------------------------- */



	/* ------------------------------ y�� ���Ͽ� ȸ�� ------------------------------ */
	TR = glm::translate(TR, glm::vec3(-cameraPosX * 2, 0.0f, 0.0));
	//TR = glm::rotate(TR, glm::radians(cameraY), glm::vec3(0.0f, 1.0f, 0.0f));
	TR = glm::translate(TR, glm::vec3(cameraPosX * 2, 0.0f, 0.0));
	/* ---------------------------------------------------------------------------- */



	/* ------------------------------ �⺻ ȸ�� ------------------------------ */
	if (top_view) {
		TR = glm::rotate(TR, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else TR = glm::rotate(TR, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR = glm::rotate(TR, glm::radians(cameraY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	/* ---------------------------------------------------------------------------- */



	/* ------------------------------ �ٴ� ------------------------------ */
	// �ִ����� �ٴڸ��� �׸�.

	// printf("%f, %d, %d\n", FloorScaleValue, COL, LOW);
	TR = glm::translate(TR, glm::vec3(0.0f, -1.0f, 0.0f));
	TR = glm::scale(TR, glm::vec3(1.0f, 0.02f, 1.0f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = 36; i < 36 + 33 + 1; i += 3) {
		glDrawArrays(GL_TRIANGLES, i, 3);
	}

	TR = glm::scale(TR, glm::vec3(1.0f, 1.0f / 0.02f, 1.0f));
	TR = glm::translate(TR, glm::vec3(0.0f, 1.0f, 0.0f));
	/* ---------------------------------------------------------------------------- */



	/* ------------------------------ �÷��̾�, ��� ------------------------------ */
	draw_player(TR, modelLocation);
	draw_pillar(TR, modelLocation);
	/* ---------------------------------------------------------------------------- */
}




void set_radians() {
	switch (P.dir) {
	case 1: // right
		ra = 270.0f;
		break;
	case -1: // left
		ra = 90.0f;
		break;
	case 2:
		ra = 0.0f;
		break;
	case -2:
		ra = 180.0f;
		break;
	}
}

void SetTransform_FPS() {
	glm::mat4 TR = glm::mat4(1.0f);

	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");

	/* ------------------------------ �⺻ �̵� ------------------------------ */
	GLfloat Xvalue = -1.0f + (Svalue / 2) + (Svalue * P.x); // ���� ������ �̵� + ���� 1/2��ŭ �̵� + (LOW��ŭ ����������)
	GLfloat Zvalue = 1.0f - (Svalue / 2) - (Svalue * P.y); // ���� ������ �̵� + ���� 1/2��ŭ �̵� + (COL��ŭ ��������)

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	TR = glm::rotate(TR, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	if (P.dir == 2) {
		TR = glm::rotate(TR, glm::radians(ra), glm::vec3(0.0f, 1.0f, 0.0f));
		TR = glm::translate(TR, glm::vec3(-Xvalue, 1.2f, -Zvalue + 1.8f));
	}
	else if (P.dir == -2) {
		TR = glm::rotate(TR, glm::radians(ra), glm::vec3(0.0f, 1.0f, 0.0f));
		TR = glm::translate(TR, glm::vec3(-Xvalue, 1.2f, -Zvalue - 1.8f));
	}
	else if (P.dir == 1) {
		TR = glm::rotate(TR, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		TR = glm::translate(TR, glm::vec3(-Xvalue - 1.8, 1.2f, -Zvalue));
	}
	else if (P.dir == -1) {
		TR = glm::rotate(TR, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		TR = glm::translate(TR, glm::vec3(-Xvalue + 1.8, 1.2f, -Zvalue));
	}
	/* ---------------------------------------------------------------------------- */



	/* ------------------------------ �ٴ� ------------------------------ */
	// �ִ����� �ٴڸ��� �׸�.

	// printf("%f, %d, %d\n", FloorScaleValue, COL, LOW);
	TR = glm::translate(TR, glm::vec3(0.0f, -1.0f, 0.0f));
	TR = glm::scale(TR, glm::vec3(1.0f, 0.02f, 1.0f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = 36; i < 36 + 33 + 1; i += 3) {
		glDrawArrays(GL_TRIANGLES, i, 3);
	}

	TR = glm::scale(TR, glm::vec3(1.0f, 1.0f / 0.02f, 1.0f));
	TR = glm::translate(TR, glm::vec3(0.0f, 1.0f, 0.0f));
	/* ---------------------------------------------------------------------------- */



	/* ------------------------------ �÷��̾�, ��� ------------------------------ */
	draw_player(TR, modelLocation);
	draw_pillar(TR, modelLocation);
	/* ---------------------------------------------------------------------------- */



	TR = glm::translate(TR, glm::vec3(Xvalue, -0.9f, Zvalue));
}


void draw_pillar(glm::mat4 TR, unsigned int modelLocation) {
	// ���� ���� ���ϱ�.

	GLfloat ScaleValue = Svalue / 2;

	for (int c = 0; c < COL; c++) {
		for (int l = 0; l < LOW; l++) {
			if (B[c][l].show == true and B[c][l].exist == true) {
				GLfloat Xvalue = -1.0f + (Svalue / 2) + (Svalue * B[c][l].l); // ���� ������ �̵� + ���� 1/2��ŭ �̵� + (LOW��ŭ ����������)
				GLfloat Zvalue = 1.0f - (Svalue / 2) - (Svalue * B[c][l].c); // ���� ������ �̵� + ���� 1/2��ŭ �̵� + (COL��ŭ ��������)

				TR = glm::translate(TR, glm::vec3(Xvalue, 0.0f, Zvalue));
				TR = glm::scale(TR, glm::vec3(ScaleValue, 1.0f, ScaleValue));

				// scale��ŭ ũ�� ����.
				TR = glm::translate(TR, glm::vec3(0.0f, -1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(1.0f, B[c][l].scale, 1.0f));
				TR = glm::translate(TR, glm::vec3(0.0f, 1.0f, 0.0f));

				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

				draw_rect();
				//glDrawArrays(GL_TRIANGLES, 30, 3);
				//glDrawArrays(GL_TRIANGLES, 33, 3); // ��� ���κ�
				// draw_rect_line();

				TR = glm::translate(TR, glm::vec3(0.0f, -1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(1.0f, 1.0f / B[c][l].scale, 1.0f));
				TR = glm::translate(TR, glm::vec3(0.0f, 1.0f, 0.0f));

				TR = glm::scale(TR, glm::vec3(1.0f / ScaleValue, 1.0f, 1.0f / ScaleValue));
				TR = glm::translate(TR, glm::vec3(-Xvalue, 0.0f, -Zvalue)); // ���� 1/2��ŭ �߰��� �̵�
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			}
		}
	}
}


void draw_player(glm::mat4 TR, unsigned int modelLocation) {
	if (show_player) {
		GLfloat Xvalue = -1.0f + (Svalue / 2) + (Svalue * P.x); // ���� ������ �̵� + ���� 1/2��ŭ �̵� + (LOW��ŭ ����������)
		GLfloat Zvalue = 1.0f - (Svalue / 2) - (Svalue * P.y); // ���� ������ �̵� + ���� 1/2��ŭ �̵� + (COL��ŭ ��������)

		TR = glm::translate(TR, glm::vec3(Xvalue, -0.9f, Zvalue));
		TR = glm::scale(TR, glm::vec3(P.s_value, P.s_value, P.s_value));
		TR = glm::rotate(TR, glm::radians(ra), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

		for (int i = 72; i < 87 + 1; i += 3) {
			glDrawArrays(GL_TRIANGLES, i, 3);
		}

		TR = glm::rotate(TR, glm::radians(-ra), glm::vec3(0.0f, 1.0f, 0.0f));

		TR = glm::scale(TR, glm::vec3(1.0f / P.s_value, 1.0f / P.s_value, 1.0f / P.s_value));
		TR = glm::translate(TR, glm::vec3(-Xvalue, 0.9f, -Zvalue));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

	}
}

void move_RIGHT() {
	if (P.moving_dir == 0 && (int)(P.x + 0.5f) + 1 <= LOW) { // �����̰� ���� �������� && 
		//printf("\nMoving Right, P.x(%f) <= LOW(%d)\n", P.x, LOW);
		if (B[(int)(P.y + 0.5f)][(int)(P.x + 0.5f) + 1].show == 0) {
			//printf("Going, B[%d][%d].show == 0 true\n", (int)(P.y + 0.5f), (int)(P.x + 0.5f) + 1);
			P.moving_dir = 1;
			P.dir = 1;

		}
		else printf("   Can't move, B[%d][%d].show == 1\n", (int)(P.y + 0.5f), (int)(P.x + 0.5f) + 1);

	}
}

void move_LEFT() {
	if (P.moving_dir == 0 && (int)(P.x + 0.5f) - 1 >= 0) { // �����̰� ���� �������� ����
		//printf("\nMoving Left, P.x(%f) <= LOW(%d)\n", P.x, LOW);
		if (B[(int)(P.y + 0.5f)][(int)(P.x + 0.5f) - 1].show == 0) {
			//printf("Going, B[%d][%d].show == 0 true\n", (int)(P.y + 0.5f), (int)(P.x + 0.5f) - 1);
			P.moving_dir = -1;
			P.dir = -1;

		}
		else printf("   Can't move, B[%d][%d].show == 1\n", (int)(P.y + 0.5f), (int)(P.x + 0.5f) - 1);
	}
}

void move_UP() {
	if (P.moving_dir == 0 && (int)(P.y + 0.5f) + 1 <= LOW) { // �����̰� ���� �������� ����
		//printf("\nMoving Up, P.y(%f) <= LOW(%d)\n", P.y, LOW);
		if (B[(int)(P.y + 0.5f) + 1][(int)(P.x + 0.5f)].show == 0) {
			//printf("Going, B[%d][%d].show == 0 true\n", (int)(P.y + 0.5f) + 1, (int)(P.x + 0.5f));

			if (B[(int)(P.y + 0.5f) + 1][(int)(P.x + 0.5f)].exist == false) { // ������ ����, exist�� �Ǻ�
				MessageBox(NULL, L"You arrived at destination", L"Game Clear",
					MB_OK | MB_ICONINFORMATION);
			}
			else {
				P.moving_dir = 2;
				P.dir = 2;
			}
		}
		else printf("   Can't move, B[%d][%d].show == 1\n", (int)(P.y + 0.5f) + 1, (int)(P.x + 0.5f));

	}
}
void move_DOWN() {
	printf("P.y = %d \n", (int)(P.y + 0.5f));
	if (P.moving_dir == 0 && (int)(P.y + 0.5f) - 1 >= 0) { // �����̰� ���� �������� ����
		//printf("\nMoving Up, P.y(%f) <= LOW(%d)\n", P.y, LOW);
		if (B[(int)(P.y + 0.5f) - 1][(int)(P.x + 0.5f)].show == 0) {
			//printf("Going, B[%d][%d].show == 0 true\n", (int)(P.y + 0.5f) - 1, (int)(P.x + 0.5f));

			P.moving_dir = -2;
			P.dir = -2;

		}
		else printf("   Can't move, B[%d][%d].show == 1\n", (int)(P.y + 0.5f) - 1, (int)(P.x + 0.5f));

	}
}

void draw_rect() {
	for (int i = 0; i < 33 + 1; i += 3) {
		glDrawArrays(GL_TRIANGLES, i, 3);
	}
}

void draw_rect_line() {
	for (int i = 0; i < 33 - 1; i++) {
		glDrawArrays(GL_LINES, i, 2);
	}
}

void gen_maze() {
	for (int c = 0; c < COL; c++) {
		for (int l = 0; l < LOW; l++) {
			B[c][l].show = true;
		}
	}

	if (LOW % 2 == 0) { // ¦�����
		B[1][LOW - 2].show = false;
		B[0][LOW - 2].show = false;
		B[LOW - 1][1].show = false;
		B[LOW - 2][1].show = false;
	}
	else {
		B[0][LOW - 2].show = false;
		B[LOW - 1][1].show = false;
	}

	int maze_LOW = LOW / 2 - 1;
	if (LOW % 2 == 0) maze_LOW--;

	char** maze = setMaze(maze_LOW);
	// printMaze(&maze);
}


void SetCamera() {
	glm::mat4 VR = glm::mat4(1.0f);

	glm::vec3 cameraPos = glm::vec3(cameraPosX, 0.0f, cameraPosZ); //--- ī�޶� ��ġ
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	VR = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(VR));
}

void SetProjection() {
	glm::mat4 PR = glm::mat4(1.0f);
	unsigned int projLocation = glGetUniformLocation(s_program, "projectionTransform");

	/* ------------------------------ ���� ���� ------------------------------ */
	if (perspective_view)
		PR = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	else
		PR = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, -2.0f, 100.0f);

	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(PR));
	/* ---------------------------------------------------------------------------- */
}

void LowHeight() {
	UpDownMoveAnime = false;
	for (int c = 0; c < COL; c++) {
		for (int l = 0; l < LOW; l++) {
			if (B[c][l].exist == true) {
				B[c][l].scale = B[c][l].min_scale + B[c][l].velocity + 0.001f;
			}
		}
	}
}

void ChangeVelocity(int d) {
	for (int c = 0; c < COL; c++) {
		for (int l = 0; l < LOW; l++) {
			if (B[c][l].exist == true) {
				if (d == -1) {
					printf(" - pressed : ");
					if (B[c][l].velocity - 0.0005f > 0.0f) {
						printf("Velocity reduction\n");
						B[c][l].velocity -= 0.0005f;
					}
					else {
						printf("Velocity reduction limit\n");
					}
				}
				else if (d == 1) {
					printf(" + pressed : ");
					if (B[c][l].velocity + 0.001f < 0.05f) {
						printf("Velocity increase\n");
						B[c][l].velocity += 0.001f;
					}
					else {
						printf("Velocity increase limit\n");
					}

				}
			}
		}
	}

}

void init_ALL() {
	init_PLAYER();
	init_PILLARS();
	cameraPosZ = 2.0f;
	cameraPosX = 0.0f;

	Rotate_Y_Anime = 0;
	cameraY = 0.0f;

	perspective_view = 1;
	perspective_Z = -5.0f;

	UpDownMoveAnime = false;

	ra = 0.0f;
	show_player = false;
	view_fps = false;
}

void TimerFunction(int value) {

	// scale ����. -> B[c][l] ������ max_scale if�� �޾ƾ� ��.
	// scale �󸶳�? -> velocity ���� ����. dir���� �������� �������� �����ֱ�.
	for (int c = 0; c < COL; c++) {
		for (int l = 0; l < LOW; l++) {
			if (B[c][l].show == true and B[c][l].exist == true) {

				if (UpDownMoveAnime) {
					if (B[c][l].dir == -2) { // �������� ������ ��. so ����
						if (B[c][l].scale <= B[c][l].min_scale) { // �������� �۾�����
							B[c][l].dir *= -1; // �ݴ� ��������
						}
						else {
							B[c][l].scale -= B[c][l].velocity;
						}
					}
					else if (B[c][l].dir == 2) { // �ö󰡴� ������ ��. so ����
						if (B[c][l].scale >= B[c][l].max_scale) { // �������� �۾�����
							B[c][l].dir *= -1; // �ݴ� ��������
						}
						else {
							B[c][l].scale += B[c][l].velocity;
						}
					}
				}

			}
		}
	}


	switch (P.moving_dir) {
	case 0: break;
	case -1: // Left
		if (P.cnt <= 19) {
			P.x -= 0.05f; P.cnt++;
		}
		else {
			P.moving_dir = 0;
			P.cnt = 0;
		}
		break;
	case 1: // Right
		if (P.cnt <= 19) {
			P.x += 0.05f; P.cnt++;
		}
		else {
			P.moving_dir = 0;
			P.cnt = 0;
		}
		break;
	case -2: // Down
		if (P.cnt <= 19) {
			P.y -= 0.05f; P.cnt++;
		}
		else {
			P.moving_dir = 0;
			P.cnt = 0;
		}
		break;
	case 2: // Up
		if (P.cnt <= 19) {
			P.y += 0.05f; P.cnt++;
		}
		else {
			P.moving_dir = 0;
			P.cnt = 0;
		}
		break;
	}

	if (Rotate_Y_Anime == 1) {
		cameraY += 0.3f;
	}
	else if (Rotate_Y_Anime == -1) {
		cameraY -= 0.3f;
	}

	glutPostRedisplay();
	InitBuffer();
	glutTimerFunc(10, TimerFunction, 1);
}

int pressed_V = false;

void PrintInstruction() {
	printf("\n -------------------- Key Instruction --------------------\n");

	printf("\n\tO : Ortho projection\n");
	printf("\tP : Perspective projection\n");

	printf("\tM : Play Pillar animation\n");

	printf("\ty : Y rotate (negative) \n");
	printf("\tY : Y rotate (positive) \n");

	printf("\tR : Make maze\n");

	printf("\t+ : Increase Pillar velocity\n");
	printf("\t- : Decrease Pillar velocity\n");

	printf("\tV : Stop pillar animation, translate to low height\n");

	printf("\tS : Show player\n");


	printf("\t1 : View on FPS (only in Perspective proj)\n");
	printf("\t3 : View on TPS\n");
	printf("\tARROW KEYS : Move player\n");
	printf("\t\t (is different between FPS & TPS)\n");

	printf("\tC : Initialize entire values\n");
	printf("\tQ : Quit program\n");

	printf("\n\tH : Look Key Guide\n");

	printf("\n ---------------------------------------------------------\n");
};

GLvoid Keyboard(unsigned char key, int x, int y) {

	// �ܼ�â���� ����, ���� ������ ������ �Է� ����.

	switch (key) {
		//�ִϸ��̼� 1 2 3
	case '1': // 1��Ī
		view_fps = true;
		perspective_view = true;
		printf(" 1 pressed : View on FPS\n");
		break;
	case '2':
		break;
	case '3': // ���ͺ�
		view_fps = false;
		printf(" 3 pressed : View on TPS\n");
		break;
		//���� on off
	case 't':

		lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���

		if (!off){
			if (1 == chooseCol)
				glUniform3f(lightColorLocation, 0.3, 0.3, 0.3);
			else if (2 == chooseCol)
				glUniform3f(lightColorLocation, 0.3, 0.0, 0.0);
			else if (3 == chooseCol)
				glUniform3f(lightColorLocation, 0.0, 0.3, 0.0);
			else if (4 == chooseCol)
				glUniform3f(lightColorLocation, 0.3, 0.0, 0.3);
			off = true;
		}
		else{
			if (1 == chooseCol)
				glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
			else if (2 == chooseCol)
				glUniform3f(lightColorLocation, 1.0, 0.0, 0.0);
			else if (3 == chooseCol)
				glUniform3f(lightColorLocation, 0.0, 1.0, 0.0);
			else if (4 == chooseCol)
				glUniform3f(lightColorLocation, 0.3, 0.0, 1.0);
			off = false;
		}
		break;
		//���� �� ����
	case 'c':
		lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���


		if (1 == chooseCol){
			glUniform3f(lightColorLocation, 1.0, 0.0, 0.0);
			chooseCol = 2;
		}
		else if (2 == chooseCol){
			glUniform3f(lightColorLocation, 0.0, 1.0, 0.0);
			chooseCol = 3;
		}
		else if (3 == chooseCol){
			glUniform3f(lightColorLocation, 0.0, 0.0, 1.0);
			chooseCol = 4;
		}
		else if (4 == chooseCol){
			glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
			chooseCol = 1;
		}
		break;
		//ī�޶� �ٴ��� y�� ���� ��/�� ����, �ٽ� ������ ���� 
	case 'y': // �ٴ� y�� ����, �� / �� ���� ȸ��
		if (Rotate_Y_Anime == 0) {
			printf(" y pressed : Y rotate animation play, Negative direction\n");
			Rotate_Y_Anime = -1;
		}
		break;

	case 'Y':
		if (Rotate_Y_Anime == 0) {
			printf(" Y pressed : Y rotate animation play, Positive direction\n");
			Rotate_Y_Anime = 1;
		}
		break;
	case '+': // ����ü �̵��ϴ� �ӵ� ����
		ChangeVelocity(1);
		break;
	case '-': // ����ü �̵��ϴ� �ӵ� ����
		ChangeVelocity(-1);
		break;
	case 'r':  // ��� �� �ʱ�ȭ
		printf(" C pressed : Initialize entire values\n");

		init_LOW_COL();
		init_ALL();

		perspective_view = true;
		perspective_Z = -5.0f;
		break;
		//����
	case 'Q': case 'q':
		printf(" Q pressed : Quit program\n");
		exit(0);
		break;

	case 'o': case 'O': // ���� ����
		printf(" O pressed : Ortho View\n");
		perspective_view = false;
		break;

	case 'p': case 'P': // ���� ����
		printf(" P pressed : Perspective View\n");
		perspective_view = true;
		break;

	case 'z': // ���� ���� �� z�� �̵�, �۾���
		if (perspective_view) {
			printf(" z pressed : Z value goes down\n");
			perspective_Z += 0.1f;
		}
		break;
	case 'Z': // Ŀ��
		if (perspective_view) {
			printf(" Z pressed : Z value goes up\n");
			perspective_Z -= 0.1f;
		}
		break;

	case 'm': case 'M':
		if (UpDownMoveAnime) {
			printf(" M pressed : Pillar animation stop\n");
			UpDownMoveAnime = false;
		}
		else {
			printf(" M pressed : Pillar animation play\n");
			UpDownMoveAnime = true;
		}
		break;


	

	case 'R': // �̷� ���� --> �̷ΰ� �� �κ� ����ü�� ������
		printf(" R pressed : Make Maze\n");
		gen_maze();
		break;

	

	case 'v': case 'V': // ����ü�� �������� ���߰� ���� ���̷� ����
		if (pressed_V) {
			printf(" V pressed : Play Pillar animation\n");
			pressed_V = false;
			UpDownMoveAnime = true;
		}
		else {
			printf(" V pressed : Stop Pillar animation, translate to low height\n");
			pressed_V = true;
			LowHeight();
		}

		break;

	

	case 's': case 'S': // �̷ο��� ��ü�� ��Ÿ�� (�÷��̾�)
		if (show_player) {
			show_player = false;
			printf(" S pressed : Hide player\n");
		}
		else {
			show_player = true;
			printf(" S pressed : Show player\n");
		}
		break;

	

	

	case 'h': case 'H':
		PrintInstruction();
		break;

	
	}
	glutPostRedisplay();
}

GLvoid KeyboardSpecial(int key, int x, int y) {
	if (show_player)
		switch (key) {
		case GLUT_KEY_UP:
			if (!view_fps) {
				move_UP();
				printf("�� pressed : Move player to upside\n");
			}
			else {
				printf("�� pressed : Move player to front\n");
				if (P.dir == 2) move_UP();
				else if (P.dir == -2) move_DOWN();
				else if (P.dir == 1) move_RIGHT();
				else if (P.dir == -1) move_LEFT();
			}
			break;
		case GLUT_KEY_DOWN:
			if (!view_fps) {
				move_DOWN();
				printf("�� pressed : Move player to downside\n");
			}
			else {
				printf("�� pressed : Move player to back\n");
				if (P.dir == 2) move_DOWN();
				else if (P.dir == -2) move_UP();
				else if (P.dir == 1) move_LEFT();
				else if (P.dir == -1) move_RIGHT();
			}
			break;
		case GLUT_KEY_LEFT:
			if (!view_fps) {
				move_LEFT();
				printf("�� pressed : Move player to leftside\n");
			}
			else {
				printf("�� pressed : Move player to left\n");
				if (P.dir == 2) move_LEFT();
				else if (P.dir == -2) move_RIGHT();
				else if (P.dir == 1) move_UP();
				else if (P.dir == -1) move_DOWN();
			}
			break;
		case GLUT_KEY_RIGHT:
			if (!view_fps) {
				move_RIGHT();
				printf("�� pressed : Move player to rightside\n");
			}
			else {
				printf("�� pressed : Move player to right\n");
				if (P.dir == 2) move_RIGHT();
				else if (P.dir == -2) move_LEFT();
				else if (P.dir == 1) move_DOWN();
				else if (P.dir == -1) move_UP();
			}
			break;
		}
}

GLvoid KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'y': // �ٴ� y�� ����, �� / �� ���� ȸ��
		if (Rotate_Y_Anime == -1) Rotate_Y_Anime = 0;

		break;

	case 'Y':
		if (Rotate_Y_Anime == 1) Rotate_Y_Anime = 0;

		break;
	}
}

void subDFS(char** maze, bool** visited, int y, int x) {
	int i, j, dx[4] = { -2,2,0,0 }, dy[4] = { 0,0,-2,2 }, wx[4] = { -1,1,0,0 }, wy[4] = { 0,0,-1,1 }, size = _msize(*maze) / sizeof(char), num = rand() % 24;
	int arr[24][4] = {
		{ 0,1,2,3 },{ 0,1,3,2 },{ 0,2,1,3 },{ 0,2,3,1 },{ 0,3,1,2 },{ 0,3,2,1 },
		{ 1,0,2,3 },{ 1,0,3,2 },{ 1,2,0,3 },{ 1,2,3,0 },{ 1,3,0,2 },{ 1,3,2,0 },
		{ 2,1,0,3 },{ 2,1,3,0 },{ 2,0,1,3 },{ 2,0,3,1 },{ 2,3,1,0 },{ 2,3,0,1 },
		{ 3,1,2,0 },{ 3,1,0,2 },{ 3,2,1,0 },{ 3,2,0,1 },{ 3,0,1,2 },{ 3,0,2,1 }
	};
	for (i = 0; i < 4; i++) {
		if (y + dy[arr[num][i]] >= 1 && y + dy[arr[num][i]] < size - 1 && x + dx[arr[num][i]] >= 1 && x + dx[arr[num][i]] < size - 1 && visited[y + dy[arr[num][i]]][x + dx[arr[num][i]]] == false) {
			*(*(maze + y + wy[arr[num][i]]) + x + wx[arr[num][i]]) = '.';
			B[y + wy[arr[num][i]]][x + wx[arr[num][i]]].show = false;
			*(*(maze + y + dy[arr[num][i]]) + x + dx[arr[num][i]]) = '.';
			B[y + dy[arr[num][i]]][x + dx[arr[num][i]]].show = false;
			*(*(visited + y + wy[arr[num][i]]) + x + wx[arr[num][i]]) = true;
			*(*(visited + y + dy[arr[num][i]]) + x + dx[arr[num][i]]) = true;
			subDFS(maze, visited, y + dy[arr[num][i]], x + dx[arr[num][i]]);
		}
	}
}

void DFS(char** maze, bool** visited, int y, int x) {
	*(*(maze + y) + x) = '0';
	*(*(visited + y) + x) = true;
	subDFS(maze, visited, y, x);
}

char** setMaze(int size) {
	srand(time(NULL));
	char** p = (char**)malloc(sizeof(char*) * (size * 2 + 3));
	bool** visited = (bool**)malloc(sizeof(bool*) * (size * 2 + 3));
	for (int i = 0; i < size * 2 + 3; i++) {
		*(p + i) = (char*)malloc(sizeof(char) * (size * 2 + 3));
		*(visited + i) = (bool*)malloc(sizeof(bool) * (size * 2 + 3));
		for (int j = 0; j < size * 2 + 3; j++) {
			*(*(p + i) + j) = '#';
			*(*(visited + i) + j) = false;
		}
	}
	DFS(p, visited, 1, 1);
	*(*(p + size * 2 + 1) + size * 2 + 1) = '1';

	free(visited);
	return p;
}

void printMaze(char*** maze) {
	for (int i = 0; i < _msize(*maze) / sizeof(char*); i++) {
		for (int j = 0; j < _msize(*(*maze + i)) / sizeof(char); j++) {
			printf("%c", *(*(*maze + i) + j));
		}
		printf("\n");
	}
}

void random_color(COLORVALUE* color) {
	color->r = (float)rand() / (RAND_MAX);
	color->g = (float)rand() / (RAND_MAX);
	color->b = (float)rand() / (RAND_MAX);
}

void con_D_to_Ogl(int x, int y, float* ox, float* oy) {
	int w = SCREEN_W;
	int h = SCREEN_H;
	*ox = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	*oy = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}

void con_Ogl_to_D(int* x, int* y, float ox, float oy) {
	float w = SCREEN_W;
	float h = SCREEN_H;
	*x = (int)((ox * w + w) / 2);
	*y = h - (int)((oy * h + h) / 2);
}

float vertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f

};

void InitBuffer()
{

	




	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(3, vbo);


	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, TRI_COUNT * 9 * sizeof(GLfloat), pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, TRI_COUNT * 9 * sizeof(GLfloat), col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);




	//unsigned int VBO, VAO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //--- ��ġ �Ӽ�
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //--- �븻 �Ӽ�
	glEnableVertexAttribArray(3);
	glUseProgram(s_program);

	unsigned int lightPosLocation = glGetUniformLocation(s_program, "lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 0.0, 5.0);

	if (!initLight) {
		lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
		glUniform3f(lightColorLocation, 0.3, 0.3, 0.3);
		initLight = true;
	}

	//int objColorLocation = glGetUniformLocation(shaderProgram, ��objectColor��); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
	//glUniform3f(objColorLocation, 1.0, 0.5, 0.3);

	unsigned int viewPosLocation = glGetUniformLocation(s_program, "viewPos"); //--- viewPos �� ����: ī�޶� ��ġ
	glUniform3f(viewPosLocation, cameraPosX, cameraY, cameraPosZ);


}

void InitShader()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	


	//checkCompileErrors(s_program, "PROGRAM");
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	


	//--- Shader Program ����ϱ�
	glUseProgram(s_program);

	
}

int beforeX = 0, beforeY = 0;

void Mouse(int button, int state, int x, int y) {
	//GLfloat ox = 0, oy = 0;
	//con_D_to_Ogl(x, y, &ox, &oy);

	if (!left_button && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		beforeX = x, beforeY = y;
		left_button = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = false;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		right_button = true;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		right_button = false;
	}
	glutSwapBuffers();
	glutPostRedisplay();
}

void Motion(int x, int y) {

	// ���� ���콺�� ������ ��
	if (left_button == true) {

		int afterX = x - beforeX;
		int afterY = y - beforeY;
		float f_afterX, f_afterY;

		beforeX = x, beforeY = y;
	}

	// ������ ���콺�� ������ ��
	if (right_button == true) {
		;
	}
	else {
		;
	}

	glutPostRedisplay();
}

GLchar* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}

void make_vertexShaders() {

	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl"); // �����׼��̴� �о����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

GLuint make_shaderProgram()
{
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- ���̴� ���α׷� �����
	glAttachShader(ShaderProgramID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(ShaderProgramID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
	glLinkProgram(ShaderProgramID); //--- ���̴� ���α׷� ��ũ�ϱ�
	glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
	glDeleteShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
	}
	glUseProgram(ShaderProgramID); //--- ������� ���̴� ���α׷� ����ϱ�
	//--- ���� ���� ���̴����α׷� ���� �� �ְ�, �� �� �Ѱ��� ���α׷��� ����Ϸ���
	//--- glUseProgram �Լ��� ȣ���Ͽ� ��� �� Ư�� ���α׷��� �����Ѵ�.
	//--- ����ϱ� ������ ȣ���� �� �ִ�.
	return ShaderProgramID;
}
