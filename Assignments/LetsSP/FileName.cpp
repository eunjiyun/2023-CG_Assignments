#include <GL/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <Windows.h>
#include <iostream>
#include<random>
#include <MMSystem.h> 
#include"obj.h"
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")
using namespace std;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int, int);
GLvoid KeyBoard(unsigned char, int, int);
GLvoid Mouse(int, int, int, int);
GLvoid Timer(int);
GLvoid Motion(int, int);
bool FindIntersection(glm::vec2, glm::vec2, glm::vec2 , glm::vec2 , glm::vec2& );

random_device gen;
uniform_int_distribution<int> dist(0, 1);
uniform_int_distribution<int> dist400(0, 399);
uniform_int_distribution<int> dist16(0, 15);
uniform_int_distribution<int> dist700(0, 699);
uniform_int_distribution<int> dist300(0, 299);
uniform_int_distribution<int> dist3(0, 2);
uniform_int_distribution<int> dist600(100, 500);

Fruits baskets;
Fruits fruit;
Slices slice[2];
Cutter cutter;
Floor ground[3][16];
Particles particle[10];
int speed{ 20 };
bool direction{ false };
int curSh{ -1 };

GLvoid main(int argc, char* argv[])
{
	PlaySound("inGame.wav", NULL, SND_ASYNC | SND_LOOP);

	fruit.xPos = 400;
	fruit.yPos = 600;
	slice[0].act = 0;
	slice[1].act = 0;
	baskets.rotation = 0;
	baskets.yPos = 550;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
	glutInitWindowPosition(100, 100); 
	glutInitWindowSize(800, 600); 
	glutCreateWindow("Let’s Slice Polygon!"); 
	glutDisplayFunc(drawScene); 
	glutTimerFunc(0, Timer, 1);
	glutKeyboardFunc(KeyBoard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT); 

	glPushMatrix();
	{
		if (0!=cutter.bounce){
			if (10 < cutter.bounce){
				glTranslatef((cutter.bounce - 20) * 2, (cutter.bounce - 20) * 2, 0.0);
			}else{
				glTranslatef((cutter.bounce) * -2, (cutter.bounce) * -2, 0.0);
			}
		}

		glShadeModel(GL_SMOOTH);
		glBegin(GL_POLYGON);

		glColor3ub(50, 50, 50);
		glVertex2f(0, 0);


		glColor3ub(50, 50, 50);
		glVertex2f(800, 0);


		glColor3ub(127, 153, 248);
		glVertex2f(800, 600);


		glColor3ub(127, 153, 248);
		glVertex2f(0, 600);


		glEnd();
		glShadeModel(GL_FLAT);

		for (int i{}; i < 10; ++i){
			if (0<particle[i].fourthCol){
				glPushMatrix();{
					glColor4f(1.0, 0.0, 0.0, particle[i].fourthCol);
					glTranslated(particle[i].xPos, particle[i].yPos, 0);
					glBegin(GL_POLYGON);
					glVertex2f(-2, -2);
					glVertex2f(2, -2);
					glVertex2f(2, 2);
					glVertex2f(-2, 2);
					glEnd();
				}
				glPopMatrix();
			}
		}

		glColor3ub(255, 255, cutter.glow);

		if (1==baskets.act ){
			glPushMatrix();{
				glTranslatef(baskets.xPos, baskets.yPos, 0.0);
				glBegin(GL_POLYGON);
				glColor3ub(0, 255, 0);
				glVertex2f(-120, -25);
				glVertex2f(-120, +25);
				glVertex2f(+120, +25);
				glVertex2f(+120, -25);
				glEnd();
			}
			glPopMatrix();
		}


		glPushMatrix();{
			glColor3ub(255, 0, 0);
			if (1==fruit.act ){
				glTranslatef(fruit.xPos, fruit.yPos, 0.0);
				glRotatef(fruit.rotation, 0, 0, 1);
				glBegin(GL_POLYGON);

				if (0 == fruit.shape){
					glVertex2f(-50, -50);
					glVertex2f(-50, +50);
					glVertex2f(+50, +50);
					glVertex2f(+50, -50);

					curSh = 0;
				}else if (1 == fruit.shape){
					glVertex2f(-50, -50);
					glVertex2f(50, 50);
					glVertex2f(-50, 50);

					curSh = 1;
				}else if (2 == fruit.shape){
					for (int i{}; i < 5; ++i){
						float theta = 2.0f * 3.14159265358979323846f * float(i) / 5.0f; // 5개의 정점을 나타냄
						float x = 50 * cos(theta); // 반지름이 25인 원의 x 좌표
						float y = 50 * sin(theta); // 반지름이 25인 원의 y 좌표
						glVertex2f(x, y);
					}
					curSh = 2;
				}
				glEnd();
			}
		}
		glPopMatrix();

		if (0==fruit.act){
			glColor3ub(255, 0, 0);{
				if (100!=slice[0].movingCnt ){
					glPushMatrix();{
						glTranslatef(slice[0].xPos, slice[0].yPos, 0.0);
						glRotatef(slice[0].rot * slice[0].movingCnt, 0, 0, 1);
						glBegin(GL_POLYGON);
						if (0 == curSh){
							glVertex2f(-50, -50);
							glVertex2f(50, 50);
							glVertex2f(-50, 50);
						}else if (1 == curSh){
							// First small triangle
							glVertex2f(0, 0);
							glVertex2f(50, 50);
							glVertex2f(-50, 50);
						}else if (2 == curSh){
							// Small triangle
							glVertex2f(0, 0);
							glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 1 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 1 / 5.0f));
							glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 2 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 2 / 5.0f));
						}
						glEnd();
					}
					glPopMatrix();
				}

				if (100!=slice[1].movingCnt ){
					glPushMatrix();{
						glTranslatef(slice[1].xPos, slice[1].yPos, 0.0);
						glRotatef(slice[1].rot * slice[1].movingCnt, 0, 0, 1);
						glBegin(GL_POLYGON);
						if (0 == curSh){
							glVertex2f(50, -50);
							glVertex2f(-50, -50);
							glVertex2f(50, 50);
						}else if (1 == curSh){
							// Second small triangle
							glVertex2f(-50, -50);
							glVertex2f(0, 0);
							glVertex2f(-50, 50);
						}else if (2 == curSh){
							// Small square
							glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 2 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 2 / 5.0f));
							glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 3 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 3 / 5.0f));
							glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 4 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 4 / 5.0f));
							glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 1 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 1 / 5.0f));
						}
						glEnd();
					}
					glPopMatrix();
				}
			}
		}

		if (1==cutter.act){
			glColor3ub(255, 0, 0);
			glBegin(GL_LINES);
			glVertex2f(cutter.xPos[0], cutter.yPos[0]);
			glVertex2f(cutter.xPos[1], cutter.yPos[1]);
			glEnd();
		}
	}
	glPopMatrix();
	glFlush(); // 화면에 출력하기 
}
GLvoid Timer(int value)
{
	for (int i{}; i < 10; ++i){
		if (particle[i].fourthCol > 0){
			particle[i].fourthCol -= 0.01;
			particle[i].down += 0.15;
			particle[i].xPos += particle[i].velocity;
			particle[i].yPos += particle[i].down;
		}
	}

	if (0==cutter.glowSw ){
		if (180 > cutter.glow) {
			++cutter.glow;
		}else {
			cutter.glowSw = 1;
		}
	}else if (1==cutter.glowSw ){
		if (cutter.glow > 120) {
			--cutter.glow;
		}else {
			cutter.glowSw = 0;
		}
	}


	if (825 < baskets.xPos)
		direction = true;
	else if (0 > baskets.xPos)
		direction = false;

	if (!direction)
		baskets.xPos += 5;
	else
		baskets.xPos -= 5;


	if (357 > fruit.rotation) {
		fruit.rotation += 2;
	}else {
		fruit.rotation = 0;
	}

	if (1==fruit.act && -50>fruit.yPos ){
		fruit.yPos = 600;
		int choose{ dist(gen) };
		if (1 == choose) {
			fruit.rotation = 0;
		}
		else {
			fruit.rotation = 45;
		}
	}
	else{
		int size{};
		if (0 == curSh)
			size = 35;
		else if (2 == curSh)
			size = 25;
		else
			size = 18;

		for (int b{}; b < 2; ++b)// 두 객체의 충돌 여부 확인
			if (baskets.xPos - size < slice[b].xPos + size && baskets.xPos + size > slice[b].xPos - size &&
				baskets.yPos - size < slice[b].yPos + size && baskets.yPos + size > slice[b].yPos - size)
				slice[b].act = 0;

		if (0 == slice[0].act && 0 == slice[1].act && 0 == fruit.act){
			fruit.act = 1;
			fruit.xPos = 0;
		}

		// 좌에서 우로 이동하는 코드 추가
		fruit.xPos += 3;
		// 화면 오른쪽 끝을 벗어나면 화면 왼쪽 끝으로 초기화
		if (825<fruit.xPos ){
			fruit.xPos = -25;
			int choose{ dist(gen) };
			if (1 == choose) {
				fruit.rotation = 0;
			}
			else {
				fruit.rotation = 45;
			}

			fruit.shape = dist3(gen);
		}
		// y 좌표는 그대로 유지
		fruit.yPos -= 1;
	}

	if (0 < cutter.bounce) {
		--cutter.bounce;
	}

	for (int i{}; i < 2; ++i){
		if (100!=slice[i].movingCnt  && 1==slice[i].act ){
			if (0==i && 1!=cutter.change){
				float p1X{ slice[i].oldX - ((slice[i].oldX - slice[i].ptX) / 100 * slice[i].movingCnt) };
				float p2X{ slice[i].ptX - ((slice[i].ptX - slice[i].moveX) / 100 * slice[i].movingCnt) };
				float p1Y{ slice[i].oldY - ((slice[i].oldY - slice[i].ptY) / 100 * slice[i].movingCnt) };
				float p2Y{ slice[i].ptY - ((slice[i].ptY - slice[i].moveY) / 100 * slice[i].movingCnt) };
				float movX{ p1X - ((p1X - p2X) / 100 * slice[i].movingCnt) };
				float movY{ p1Y - ((p1Y - p2Y) / 100 * slice[i].movingCnt) };
				slice[i].xPos = movX;
				slice[i].yPos = movY;
				++slice[i].movingCnt;
			}
			if (1==i  && 2!=cutter.change ){
				float p1X{ slice[i].oldX - ((slice[i].oldX - slice[i].ptX) / 100 * slice[i].movingCnt) };
				float p2X{ slice[i].ptX - ((slice[i].ptX - slice[i].moveX) / 100 * slice[i].movingCnt) };
				float p1Y{ slice[i].oldY - ((slice[i].oldY - slice[i].ptY) / 100 * slice[i].movingCnt) };
				float p2Y{ slice[i].ptY - ((slice[i].ptY - slice[i].moveY) / 100 * slice[i].movingCnt) };
				float movX{ p1X - ((p1X - p2X) / 100 * slice[i].movingCnt) };
				float movY{ p1Y - ((p1Y - p2Y) / 100 * slice[i].movingCnt) };
				slice[i].xPos = movX;
				slice[i].yPos = movY;
				++slice[i].movingCnt;
			}
		}
		else if (100==slice[i].movingCnt && 1==slice[i].act ){
			slice[i].act = 0;
			int posX =(slice[i].moveX - 25) / 50;
			int posY = -(slice[i].moveY - 575) / 50;

			++ground[posY][posX].act;
			
			if (0==slice[0].act && 0==slice[1].act )
			{
				fruit.act = 1;
				fruit.yPos = dist600(gen);
				fruit.xPos = 0;

				int choose{ dist(gen) };
				if (1==choose )
					fruit.rotation = 0;
				else
					fruit.rotation = 45;
			}
		}
	}


	glutPostRedisplay(); // 화면 재 출력 
	glutTimerFunc(speed, Timer, 1); // 타이머함수 재 설정
}
GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		if (1==fruit.act ){
			cutter.act = 1;
			if (1==cutter.act ){
				cutter.xPos[0] = x;
				cutter.yPos[0] = y;
				cutter.xPos[1] = x;
				cutter.yPos[1] = y;
			}
		}

		if (slice[0].act == 1 && slice[0].xPos - 50 < x && slice[0].xPos + 50 > x && slice[0].yPos - 50 < y && slice[0].yPos + 50 > y){
			cutter.change = 1;
			slice[0].xPos = x;
			slice[0].yPos = y;
		}
		if (slice[1].act == 1 && slice[1].xPos - 50 < x && slice[1].xPos + 50 > x && slice[1].yPos - 50 < y && slice[1].yPos + 50 > y){
			cutter.change = 2;
			slice[1].xPos = x;
			slice[1].yPos = y;
		}
	}
	else if ((button == GLUT_LEFT_BUTTON && state == GLUT_UP)){
		if (cutter.act == 1){
			float left_x, right_x, left_y, right_y;
			if (cutter.xPos[0] < cutter.xPos[1]){
				left_x = cutter.xPos[0];
				right_x = cutter.xPos[1];
				left_y = cutter.yPos[0];
				right_y = cutter.yPos[1];
			}
			else{
				left_x = cutter.xPos[1];
				right_x = cutter.xPos[0];
				left_y = cutter.yPos[1];
				right_y = cutter.yPos[0];
			}

			glm::vec2 intersection1, intersection2;

			if (FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.xPos - 50, fruit.yPos - 50), glm::vec2(fruit.xPos - 50, fruit.yPos + 50), intersection1) &&
				FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.xPos + 50, fruit.yPos - 50), glm::vec2(fruit.xPos + 50, fruit.yPos + 50), intersection2) ||
				FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.xPos - 50, fruit.yPos + 50), glm::vec2(fruit.xPos + 50, fruit.yPos + 50), intersection1) &&
				FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.xPos - 50, fruit.yPos - 50), glm::vec2(fruit.xPos + 50, fruit.yPos - 50), intersection2) ||

				FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.xPos - 50, fruit.yPos), glm::vec2(fruit.xPos, fruit.yPos + 50), intersection1) &&
				FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.xPos, fruit.yPos - 50), glm::vec2(fruit.xPos + 50, fruit.yPos), intersection2) ||
				FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.xPos - 50, fruit.yPos), glm::vec2(fruit.xPos, fruit.yPos - 50), intersection1) &&
				FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.xPos, fruit.yPos + 50), glm::vec2(fruit.xPos + 50, fruit.yPos), intersection2)){
				
				// Add logic to create two slices
				fruit.act = 0;
				slice[0].act = true;
				slice[1].act = true;
				slice[0].xPos = intersection1.x;
				slice[0].yPos = intersection1.y;
				slice[1].xPos = intersection2.x;
				slice[1].yPos = intersection2.y;

				fruit.shape = dist3(gen);
				
				for (int s{}; s < 10; ++s){
					float mtRand=dist400(gen) ;
					particle[s].fourthCol = 1;
					particle[s].xPos = slice[0].xPos;
					particle[s].yPos = slice[0].yPos;
					particle[s].velocity = -2 + mtRand / 100;
					particle[s].down = 0;
				}

				int goX{ -1 };
				for (int i{}; i < 2; ++i){
					int goY{ 2 };
					int through{ 1 };
					do{
						through = 1;
						while (true){
							int goX2{ dist16(gen) };
							if (goX2 != goX){
								goX = goX2;
								break;
							}
						}
						if (2!=ground[0][goX].act ){
							goY = 0;
							through = 0;
						}
						else if (2!=ground[1][goX].act ){
							goY = 1;
							through = 0;
						}
						else if (ground[2][goX].act != 2)
						{
							goY = 2;
							through = 0;
						}
					} while (through);

					slice[i].oldX = slice[i].xPos;
					slice[i].oldY = slice[i].yPos;
					if (i == 0)
						slice[i].ptX = 0;
					else
						slice[i].ptX = 800;
					slice[i].ptY = slice[i].yPos;
					slice[i].moveX = goX * 50 + 25;
					slice[i].moveY = 650;
					slice[i].movingCnt = 0;

					if (i == 0 && ground[goY][goX].act == 0)
						slice[i].rot = -0.9;
					else if (i == 0 && ground[goY][goX].act == 1)
						slice[i].rot = 0.9;
					else if (i == 1 && ground[goY][goX].act == 0)
						slice[i].rot = 0.9;
					else if (i == 1 && ground[goY][goX].act == 1)
						slice[i].rot = -0.9;
				}

				cutter.bounce = 20;
				cutter.cutPos = 1;
			}

			cutter.act = 0;
		}
		if (cutter.change == 1 || cutter.change == 2)
		{
			if (cutter.change == 1)
			{

				if (baskets.act == 1 && x > baskets.xPos - 25 && x < baskets.xPos + 25 && y > baskets.yPos - 25 && y < baskets.yPos + 25)
				{

					if (slice[0].act == 0 && slice[1].act == 0)
					{
						fruit.act = 1;
						fruit.yPos = 600;

						int ch = dist(gen);
						if (ch == 1)
							fruit.rotation = 0;
						else
							fruit.rotation = 45;
					}
				}

			}
			if (cutter.change == 2)
			{

				if (baskets.act == 1 && x > baskets.xPos - 25 && x < baskets.xPos + 25 && y > baskets.yPos - 25 && y < baskets.yPos + 25)
				{


					if (slice[0].act == 0 && slice[1].act == 0)
					{
						fruit.act = 1;
						fruit.yPos = 600;

						int choose = dist(gen);
						if (choose == 1)
							fruit.rotation = 0;
						else
							fruit.rotation = 45;
					}
				}

			}
			cutter.change = 0;
		}
	}
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0, 800, 600, 0, -1.0, 1.0);
}
GLvoid Motion(int x, int y)
{
	if (1==cutter.act ){
		cutter.xPos[1] = x;
		cutter.yPos[1] = y;
	}

	if (1==cutter.change ){
		slice[0].xPos = x;
		slice[0].yPos = y;
	}
	if (2==cutter.change ){
		slice[1].xPos = x;
		slice[1].yPos = y;
	}
}
bool FindIntersection(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec2& intersection) {
	float down = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);
	if (down == 0.0f)
		return false;

	float fir = (p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x);
	fir /= down;

	float sec = (p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x);
	sec /= down;

	if (fir < 0.0f || fir > 1.0f || sec < 0.0f || sec > 1.0f)
		return false;

	intersection = p1 * (1 - fir) + fir * p2;
	return true;
}
GLvoid KeyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'l':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'L':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case '+':
		speed /= 1.5f;
		break;
	case '-':
		speed *= 1.5f;
		break;
	case 'q':
		exit(-1);
	default:
		break;
	}
	glutPostRedisplay();
}