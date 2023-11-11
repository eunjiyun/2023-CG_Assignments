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
bool FindIntersection(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec2& intersection);

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

	baskets.ro = 0;
	baskets.y = 550;

	fruit.x = 400;
	fruit.y = 600;
	slice[0].active = 0;
	slice[1].active = 0;


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // 디스플레이 모드 설정 
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정 
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정 

	glutCreateWindow("Let’s Slice Polygon!"); // 윈도우 생성 (윈도우 이름) 
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutTimerFunc(0, Timer, 1);
	glutKeyboardFunc(KeyBoard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

// 윈도우 출력 함수 
GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기 

	glPushMatrix();
	{
		if (cutter.shack != 0)//화면흔들림
		{
			if (cutter.shack > 10)
				glTranslatef((cutter.shack - 20) * 2, (cutter.shack - 20) * 2, 0.0);
			else
				glTranslatef((cutter.shack) * -2, (cutter.shack) * -2, 0.0);
		}

		glShadeModel(GL_SMOOTH);//배경그리기

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

		for (int i{}; i < 10; ++i)//파티클
		{
			if (particle[i].alpha > 0)
			{
				glPushMatrix();
				{
					glColor4f(1.0, 0.0, 0.0, particle[i].alpha);
					glTranslated(particle[i].x, particle[i].y, 0);
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

		//위에 지나가는 삼각형

		glColor3ub(255, 255, cutter.shine);
		if (baskets.active == 1)
		{
			glPushMatrix();
			{
				glTranslatef(baskets.x, baskets.y, 0.0);

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


		glPushMatrix();//네모그리기
		{
			glColor3ub(255, 0, 0/*any.shine*/);
			if (fruit.active == 1)
			{

				glTranslatef(fruit.x, fruit.y, 0.0);
				glRotatef(fruit.ro, 0, 0, 1);
				glBegin(GL_POLYGON);

				if (0 == fruit.shape)
				{
					glVertex2f(-50, -50);
					glVertex2f(-50, +50);
					glVertex2f(+50, +50);
					glVertex2f(+50, -50);

					curSh = 0;
				}
				else if (1 == fruit.shape)
				{
					glVertex2f(-50, -50);
					glVertex2f(50, 50);
					glVertex2f(-50, 50);

					curSh = 1;
				}
				else if (2 == fruit.shape)
				{
					for (int i{}; i < 5; ++i)
					{
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

		if (fruit.active == 0)//새세모그리기!!
		{
			glColor3ub(255, 0, 0/*any.shine*/);
			//if (cutter.semo_you == 1)//왼쪽 위 자름
			{
				if (slice[0].move_count != 100)
				{
					glPushMatrix();
					{
						glTranslatef(slice[0].x, slice[0].y, 0.0);
						glRotatef(slice[0].turn * slice[0].move_count, 0, 0, 1);
						glBegin(GL_POLYGON);
						if (0== curSh)//4
						{
							glVertex2f(-50, -50);
							glVertex2f(50, 50);
							glVertex2f(-50, 50);
						}
						else if (1 == curSh)//3
						{
							// First small triangle
							glVertex2f(0, 0);
							glVertex2f(50, 50);
							glVertex2f(-50, 50);
						}
						else if (2 == curSh)//5
						{
							//// Small triangle
							//glVertex2f(0, 0);
							//glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 1 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 1 / 5.0f));
							//glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 2 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 2 / 5.0f));

							// Small triangle
							glVertex2f(0, 0);
							glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 1 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 1 / 5.0f));
							glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 2 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 2 / 5.0f));
						}
						glEnd();
					}
					glPopMatrix();
				}

				if (slice[1].move_count != 100)
				{
					glPushMatrix();
					{
						glTranslatef(slice[1].x, slice[1].y, 0.0);
						glRotatef(slice[1].turn * slice[1].move_count, 0, 0, 1);
						glBegin(GL_POLYGON);
						if (0== curSh)//4
						{
							glVertex2f(50, -50);
							glVertex2f(-50, -50);
							glVertex2f(50, 50);
						}
						else if (1 == curSh)//3
						{
							// Second small triangle
							glVertex2f(-50, -50);
							glVertex2f(0, 0);
							glVertex2f(-50, 50);
						}
						else if(2==curSh)//5
						{
							/*glVertex2f(0, 0);
							glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 2 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 2 / 5.0f));
							glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 3 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 3 / 5.0f));
							glVertex2f(50 * cos(2.0f * 3.14159265358979323846f * 4 / 5.0f), 50 * sin(2.0f * 3.14159265358979323846f * 4 / 5.0f));*/

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
		//	else if (cutter.semo_you == 2)//오른쪽 위 자름
		//	{
		//		if (slice[0].move_count != 100)
		//		{
		//			glPushMatrix();
		//			{
		//				glTranslatef(slice[0].x, slice[0].y, 0.0);
		//				glRotatef(slice[0].turn * slice[0].move_count, 0, 0, 1);
		//				glBegin(GL_POLYGON);
		//				glVertex2f(-50, -50);
		//				glVertex2f(50, -50);
		//				glVertex2f(-50, 50);
		//				glEnd();
		//			}
		//			glPopMatrix();
		//		}

		//		if (slice[1].move_count != 100)
		//		{
		//			glPushMatrix();
		//			{
		//				glTranslatef(slice[1].x, slice[1].y, 0.0);
		//				glRotatef(slice[1].turn * slice[1].move_count, 0, 0, 1);
		//				glBegin(GL_POLYGON);
		//				glVertex2f(50, -50);
		//				glVertex2f(-50, 50);
		//				glVertex2f(50, 50);
		//				glEnd();
		//			}
		//			glPopMatrix();
		//		}
		//	}
		//	else if (cutter.semo_you == 3)//가로 자름
		//	{
		//		if (slice[0].move_count != 100)
		//		{
		//			glPushMatrix();
		//			{
		//				glTranslatef(slice[0].x, slice[0].y, 0.0);
		//				glRotatef(slice[0].turn * slice[0].move_count, 0, 0, 1);
		//				glRotatef(45, 0, 0, 1);
		//				glBegin(GL_POLYGON);
		//				glVertex2f(-50, -50);
		//				glVertex2f(50, -50);
		//				glVertex2f(-50, 50);
		//				glEnd();
		//			}
		//			glPopMatrix();
		//		}

		//		if (slice[1].move_count != 100)
		//		{
		//			glPushMatrix();
		//			{
		//				glTranslatef(slice[1].x, slice[1].y, 0.0);
		//				glRotatef(slice[1].turn * slice[1].move_count, 0, 0, 1);
		//				glRotatef(45, 0, 0, 1);
		//				glBegin(GL_POLYGON);
		//				glVertex2f(50, -50);
		//				glVertex2f(-50, 50);
		//				glVertex2f(50, 50);
		//				glEnd();
		//			}
		//			glPopMatrix();
		//		}
		//	}
		//	else if (cutter.semo_you == 4)//세로 자름
		//	{
		//		if (slice[0].move_count != 100)
		//		{
		//			glPushMatrix();
		//			{
		//				glTranslatef(slice[0].x, slice[0].y, 0.0);
		//				glRotatef(slice[0].turn * slice[0].move_count, 0, 0, 1);
		//				glRotatef(-45, 0, 0, 1);
		//				glBegin(GL_POLYGON);
		//				glVertex2f(-50, -50);
		//				glVertex2f(50, -50);
		//				glVertex2f(-50, 50);
		//				glEnd();
		//			}
		//			glPopMatrix();
		//		}

		//		if (slice[1].move_count != 100)
		//		{
		//			glPushMatrix();
		//			{
		//				glTranslatef(slice[1].x, slice[1].y, 0.0);
		//				glRotatef(slice[1].turn * slice[1].move_count, 0, 0, 1);
		//				glRotatef(-45, 0, 0, 1);
		//				glBegin(GL_POLYGON);
		//				glVertex2f(50, -50);
		//				glVertex2f(-50, 50);
		//				glVertex2f(50, 50);
		//				glEnd();
		//			}
		//			glPopMatrix();
		//		}
		//	}
		}

		if (cutter.cut_active == 1)
		{
			glColor3ub(255, 0, 0);
			glBegin(GL_LINES);
			//glBegin(GL_POLYGON);
			glVertex2f(cutter.cut_x[0], cutter.cut_y[0]);
			glVertex2f(cutter.cut_x[1], cutter.cut_y[1]);
			glEnd();
		}

	}glPopMatrix();

	glFlush(); // 화면에 출력하기 
}
GLvoid Timer(int value)
{
	for (int i{}; i < 10; ++i)
	{
		if (particle[i].alpha > 0)
		{
			particle[i].alpha -= 0.01;
			particle[i].gravity += 0.15;
			particle[i].x += particle[i].speed;
			particle[i].y += particle[i].gravity;
		}
	}

	if (cutter.shine_sw == 0)
	{
		if (cutter.shine < 180)
			cutter.shine++;
		else
			cutter.shine_sw = 1;
	}
	else if (cutter.shine_sw == 1)
	{
		if (cutter.shine > 120)
			cutter.shine--;
		else
			cutter.shine_sw = 0;
	}


	if (825 < baskets.x)
		direction = true;
	else if (0 > baskets.x)
		direction = false;

	if (!direction)
		baskets.x += 5;
	else
		baskets.x -= 5;


	if (fruit.ro < 357)
		fruit.ro += 2;
	else
	{
		fruit.ro = 0;
	}

	
	if (fruit.active == 1 && fruit.y < -50)
	{
		
		fruit.y = 600;
		int ch = dist(gen);
		if (ch == 1)
			fruit.ro = 0;
		else
			fruit.ro = 45;

		
	}
	else
	{
		int size{};
		if (0 == curSh)
			size = 35;
		else if (2 == curSh)
			size = 25;
		else
			size = 18;

		for (int b{}; b < 2; ++b)
		{// 두 객체의 충돌 여부 확인
			if (baskets.x - size < slice[b].x + size && baskets.x + size > slice[b].x - size &&
				baskets.y - size < slice[b].y + size && baskets.y + size > slice[b].y - size)
			{

				slice[b].active = 0;
			}
		}

		if (0 == slice[0].active && 0 == slice[1].active && 0 == fruit.active)
		{
			fruit.active = 1;
			fruit.x = 0;
		}

		// 좌에서 우로 이동하는 코드 추가
		fruit.x += 3;
		// 화면 오른쪽 끝을 벗어나면 화면 왼쪽 끝으로 초기화
		if (fruit.x > 825)
		{
			fruit.x = -25;
			int ch = dist(gen);
			if (ch == 1)
				fruit.ro = 0;
			else
				fruit.ro = 45;

			fruit.shape = dist3(gen);
			cout << "모양 초기화" << endl;
		}
		// y 좌표는 그대로 유지
		fruit.y -= 1;
	}

	if (cutter.shack > 0)
		cutter.shack--;

	/*for (int i{}; i < 3; ++i)
	{
		if (cutter.star_color[i] >= 255)
			cutter.star_color[i] = 0;
		else
			cutter.star_color[i] += 5;
	}*/



	for (int i{}; i < 2; ++i)
	{
		if (slice[i].move_count != 100 && slice[i].active == 1)
		{
			if (i == 0 && cutter.move != 1)
			{
				float set1_x = slice[i].now_x - ((slice[i].now_x - slice[i].point_x) / 100 * slice[i].move_count);
				float set2_x = slice[i].point_x - ((slice[i].point_x - slice[i].move_x) / 100 * slice[i].move_count);
				float set1_y = slice[i].now_y - ((slice[i].now_y - slice[i].point_y) / 100 * slice[i].move_count);
				float set2_y = slice[i].point_y - ((slice[i].point_y - slice[i].move_y) / 100 * slice[i].move_count);
				float new_x = set1_x - ((set1_x - set2_x) / 100 * slice[i].move_count);
				float new_y = set1_y - ((set1_y - set2_y) / 100 * slice[i].move_count);

				slice[i].x = new_x;
				slice[i].y = new_y;
				slice[i].move_count++;
			}
			if (i == 1 && cutter.move != 2)
			{
				float set1_x = slice[i].now_x - ((slice[i].now_x - slice[i].point_x) / 100 * slice[i].move_count);
				float set2_x = slice[i].point_x - ((slice[i].point_x - slice[i].move_x) / 100 * slice[i].move_count);
				float set1_y = slice[i].now_y - ((slice[i].now_y - slice[i].point_y) / 100 * slice[i].move_count);
				float set2_y = slice[i].point_y - ((slice[i].point_y - slice[i].move_y) / 100 * slice[i].move_count);
				float new_x = set1_x - ((set1_x - set2_x) / 100 * slice[i].move_count);
				float new_y = set1_y - ((set1_y - set2_y) / 100 * slice[i].move_count);

				slice[i].x = new_x;
				slice[i].y = new_y;
				slice[i].move_count++;
			}
		}
		else if (slice[i].move_count == 100 && slice[i].active == 1)
		{
			slice[i].active = 0;
			int xx = (slice[i].move_x - 25) / 50;
			int yy = -(slice[i].move_y - 575) / 50;

			ground[yy][xx].active++;
			cout << xx << " " << yy << endl;

			if (slice[0].active == 0 && slice[1].active == 0)
			{
				fruit.active = 1;
				//nemo.y = 600;
				fruit.y = dist600(gen);
				fruit.x = 0;

				int ch = dist(gen);
				if (ch == 1)
					fruit.ro = 0;
				else
					fruit.ro = 45;
			}
		}
	}


	glutPostRedisplay(); // 화면 재 출력 
	glutTimerFunc(speed, Timer, 1); // 타이머함수 재 설정
}
GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (fruit.active == 1)
		{
			cutter.cut_active = 1;
			if (cutter.cut_active == 1)
			{
				cutter.cut_x[0] = x;
				cutter.cut_y[0] = y;
				cutter.cut_x[1] = x;
				cutter.cut_y[1] = y;
			}
		}

		if (slice[0].active == 1 && slice[0].x - 50 < x && slice[0].x +50 > x && slice[0].y - 50 < y && slice[0].y + 50 > y)
		{
			cutter.move = 1;
			slice[0].x = x;
			slice[0].y = y;
		}
		if (slice[1].active == 1 && slice[1].x - 50 < x && slice[1].x + 50 > x && slice[1].y - 50 < y && slice[1].y + 50 > y)
		{
			cutter.move = 2;
			slice[1].x = x;
			slice[1].y = y;
		}
	}
	else if ((button == GLUT_LEFT_BUTTON && state == GLUT_UP))
	{
		if (cutter.cut_active == 1)
		{
			float left_x, right_x, left_y, right_y;
			if (cutter.cut_x[0] < cutter.cut_x[1])
			{
				left_x = cutter.cut_x[0];
				right_x = cutter.cut_x[1];
				left_y = cutter.cut_y[0];
				right_y = cutter.cut_y[1];
			}
			else
			{
				left_x = cutter.cut_x[1];
				right_x = cutter.cut_x[0];
				left_y = cutter.cut_y[1];
				right_y = cutter.cut_y[0];
			}

			//if (fruit.ro == 0 && fruit.y > 130)//네모 자름
			{
				glm::vec2 intersection1, intersection2;

				//if (left_x < fruit.x && right_x > fruit.x + 50 && left_y < fruit.y && right_y > fruit.y + 50)//왼쪽 위 자름
				if (FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.x - 50, fruit.y - 50), glm::vec2(fruit.x - 50, fruit.y + 50), intersection1) &&
					FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.x + 50, fruit.y - 50), glm::vec2(fruit.x + 50, fruit.y + 50), intersection2)||
					FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.x - 50, fruit.y + 50), glm::vec2(fruit.x + 50, fruit.y + 50), intersection1) &&
					FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.x - 50, fruit.y - 50), glm::vec2(fruit.x + 50, fruit.y - 50), intersection2)||

					FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.x - 50, fruit.y ), glm::vec2(fruit.x , fruit.y + 50), intersection1) &&
					FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.x , fruit.y -50), glm::vec2(fruit.x + 50, fruit.y ), intersection2) ||
					FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.x - 50, fruit.y ), glm::vec2(fruit.x , fruit.y - 50), intersection1) &&
					FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.x , fruit.y +50), glm::vec2(fruit.x + 50, fruit.y), intersection2))
				{
					//FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.x - 50, fruit.y - 50), glm::vec2(fruit.x - 50, fruit.y + 50), intersection1);
					//FindIntersection(glm::vec2(left_x, left_y), glm::vec2(right_x, right_y), glm::vec2(fruit.x + 50, fruit.y - 50), glm::vec2(fruit.x + 50, fruit.y + 50), intersection2);

					float far_x = abs(right_x - left_x);//선분x길이
					float far_y = abs(right_y - left_y);//선분y길이

					float check1_x = abs(fruit.x - 25 - left_x);//첫점까지x거리
					float be1_x = check1_x / far_x;//몇대 몇?
					float line1_y = left_y + far_y * be1_x;
					cout << "line1_y : " << line1_y << "     nemo.y - 25 : " << fruit.y - 25 << " /     " << abs(line1_y - (fruit.y - 25)) << endl;

					float check2_x = abs(fruit.x + 25 - left_x);//첫점까지x거리
					float be2_x = check2_x / far_x;//몇대 몇?
					float line2_y = left_y + far_y * be2_x;
					cout << "line2_y : " << line2_y << "     nemo.y + 25 : " << fruit.y + 25 << " /     " << abs(line2_y - (fruit.y + 25)) << endl;

					//if (abs(line1_y - (fruit.y - 25)) < 20 && abs(line2_y - (fruit.y + 25)) < 20)
					{
						/*fruit.active = 0;
						slice[0].active = 1;
						slice[1].active = 1;
						slice[0].x = fruit.x;
						slice[1].x = fruit.x;
						slice[0].y = fruit.y;
						slice[1].y = fruit.y;*/

						// Add logic to create two slices
						fruit.active = 0;
						slice[0].active = true;
						slice[1].active = true;
						slice[0].x = intersection1.x;
						slice[0].y = intersection1.y;
						slice[1].x = intersection2.x;
						slice[1].y = intersection2.y;

						fruit.shape = dist3(gen);
						cout << "모양 초기화" << endl;

						//파티클도 만들어 줘야지~!
						for (int s{}; s < 10; ++s)
						{
							float rands = dist400(gen);
							particle[s].alpha = 1;
							particle[s].x = slice[0].x;
							particle[s].y = slice[0].y;
							particle[s].speed = -2 + rands / 100;
							particle[s].gravity = 0;
						}

						//넌 어디로 날라갈레 삼각형아?
						int go_x = -1;
						for (int i{}; i < 2; ++i)
						{
							int go_y = 2;
							int pass = 1;
							do
							{
								pass = 1;
								while (true)
								{
									int go_x2 = dist16(gen);
									if (go_x2 != go_x)
									{
										go_x = go_x2;
										break;
									}
								}
								if (ground[0][go_x].active != 2)
								{
									go_y = 0;
									pass = 0;
								}
								else if (ground[1][go_x].active != 2)
								{
									go_y = 1;
									pass = 0;
								}
								else if (ground[2][go_x].active != 2)
								{
									go_y = 2;
									pass = 0;
								}
							} while (pass);

							slice[i].now_x = slice[i].x;
							slice[i].now_y = slice[i].y;
							if (i == 0)
								slice[i].point_x = 0;
							else
								slice[i].point_x = 800;
							slice[i].point_y = slice[i].y;
							slice[i].move_x = go_x * 50 + 25;
							//semo[i].move_y = 575 - go_y * 50;
							slice[i].move_y = 650;
							slice[i].move_count = 0;

							if (i == 0 && ground[go_y][go_x].active == 0)
								slice[i].turn = -0.9;
							else if (i == 0 && ground[go_y][go_x].active == 1)
								slice[i].turn = 0.9;
							else if (i == 1 && ground[go_y][go_x].active == 0)
								slice[i].turn = 0.9;
							else if (i == 1 && ground[go_y][go_x].active == 1)
								slice[i].turn = -0.9;
						}

						cutter.shack = 20;
						cutter.semo_you = 1;
					}
				}

			//	else if (left_x < fruit.x && right_x > fruit.x + 50 && right_y < fruit.y && left_y > fruit.y + 50)//오른쪽 위 자름
			//	{
			//		float far_x = abs(right_x - left_x);//선분x길이
			//		float far_y = abs(right_y - left_y);//선분y길이

			//		float check1_x = abs(right_x - (fruit.x + 25));//첫점까지x거리
			//		float be1_x = check1_x / far_x;//몇대 몇?
			//		float line1_y = right_y + far_y * be1_x;
			//		cout << "line1_y : " << line1_y << "     nemo.y - 25 : " << fruit.y - 25 << " /     " << abs(line1_y - (fruit.y - 25)) << endl;

			//		float check2_x = abs(right_x - (fruit.x - 25));//첫점까지x거리
			//		float be2_x = check2_x / far_x;//몇대 몇?
			//		float line2_y = right_y + far_y * be2_x;
			//		cout << "line2_y : " << line2_y << "     nemo.y + 25 : " << fruit.y + 25 << " /     " << abs(line2_y - (fruit.y + 25)) << endl;

			//		if (abs(line1_y - (fruit.y - 25)) < 20 && abs(line2_y - (fruit.y + 25)) < 20)
			//		{
			//			fruit.active = 0;
			//			slice[0].active = 1;
			//			slice[1].active = 1;
			//			slice[0].x = fruit.x;
			//			slice[1].x = fruit.x;
			//			slice[0].y = fruit.y;
			//			slice[1].y = fruit.y;

			//			//파티클도 만들어 줘야지~!
			//			for (int s{}; s < 10; ++s)
			//			{
			//				float rands = dist400(gen);
			//				particle[s].alpha = 1;
			//				particle[s].x = slice[0].x;
			//				particle[s].y = slice[0].y;
			//				particle[s].speed = -2 + rands / 100;
			//				particle[s].gravity = 0;
			//			}

			//			//넌 어디로 날라갈레 삼각형아?
			//			int go_x = -1;
			//			for (int i{}; i < 2; ++i)
			//			{
			//				int go_y = 2;
			//				int pass = 1;
			//				do
			//				{
			//					pass = 1;
			//					while (true)
			//					{
			//						int go_x2 = dist16(gen);
			//						if (go_x2 != go_x)
			//						{
			//							go_x = go_x2;
			//							break;
			//						}
			//					}
			//					if (ground[0][go_x].active != 2)
			//					{
			//						go_y = 0;
			//						pass = 0;
			//					}
			//					else if (ground[1][go_x].active != 2)
			//					{
			//						go_y = 1;
			//						pass = 0;
			//					}
			//					else if (ground[2][go_x].active != 2)
			//					{
			//						go_y = 2;
			//						pass = 0;
			//					}
			//				} while (pass);

			//				slice[i].now_x = slice[i].x;
			//				slice[i].now_y = slice[i].y;
			//				if (i == 0)
			//					slice[i].point_x = 0;
			//				else
			//					slice[i].point_x = 800;
			//				slice[i].point_y = slice[i].y;
			//				slice[i].move_x = go_x * 50 + 25;
			//				//semo[i].move_y = 575 - go_y * 50;
			//				slice[i].move_y = 650;
			//				slice[i].move_count = 0;

			//				if (i == 0 && ground[go_y][go_x].active == 0)
			//					slice[i].turn = 1.8;
			//				else if (i == 0 && ground[go_y][go_x].active == 1)
			//					slice[i].turn = 0;
			//				else if (i == 1 && ground[go_y][go_x].active == 0)
			//					slice[i].turn = 0;
			//				else if (i == 1 && ground[go_y][go_x].active == 1)
			//					slice[i].turn = 1.8;
			//			}

			//			cutter.shack = 20;
			//			cutter.semo_you = 2;
			//		}
			//	}
			//}
			//if (fruit.ro == 45 && fruit.y > 130)//마름모 자름
			//{
			//	if (left_x < fruit.x - 25 && right_x > fruit.x + 25 && left_y > fruit.y - 25 && left_y < fruit.y + 25 && right_y > fruit.y - 25 && right_y < fruit.y + 25)//가로 자름
			//	{
			//		if (abs((left_y - right_y) / (left_x - right_x)) < 0.25)
			//		{
			//			fruit.active = 0;
			//			slice[0].active = 1;
			//			slice[1].active = 1;
			//			slice[0].x = fruit.x;
			//			slice[1].x = fruit.x;
			//			slice[0].y = fruit.y;
			//			slice[1].y = fruit.y;

			//			//파티클도 만들어 줘야지~!
			//			for (int s{}; s < 10; ++s)
			//			{
			//				float rands = dist400(gen);
			//				particle[s].alpha = 1;
			//				particle[s].x = slice[0].x;
			//				particle[s].y = slice[0].y;
			//				particle[s].speed = -2 + rands / 100;
			//				particle[s].gravity = 0;
			//			}

			//			//넌 어디로 날라갈레 삼각형아?
			//			int go_x = -1;
			//			for (int i{}; i < 2; ++i)
			//			{
			//				int go_y = 2;
			//				int pass = 1;
			//				do
			//				{
			//					pass = 1;
			//					while (true)
			//					{
			//						int go_x2 = dist16(gen);
			//						if (go_x2 != go_x)
			//						{
			//							go_x = go_x2;
			//							break;
			//						}
			//					}
			//					if (ground[0][go_x].active != 2)
			//					{
			//						go_y = 0;
			//						pass = 0;
			//					}
			//					else if (ground[1][go_x].active != 2)
			//					{
			//						go_y = 1;
			//						pass = 0;
			//					}
			//					else if (ground[2][go_x].active != 2)
			//					{
			//						go_y = 2;
			//						pass = 0;
			//					}
			//				} while (pass);

			//				slice[i].now_x = slice[i].x;
			//				slice[i].now_y = slice[i].y;
			//				if (i == 0)
			//					slice[i].point_x = 0;
			//				else
			//					slice[i].point_x = 800;
			//				slice[i].point_y = slice[i].y;
			//				slice[i].move_x = go_x * 50 + 25;
			//				//semo[i].move_y = 575 - go_y * 50;
			//				slice[i].move_y = 650;
			//				slice[i].move_count = 0;

			//				if (i == 0 && ground[go_y][go_x].active == 0)
			//					slice[i].turn = 1.35;
			//				else if (i == 0 && ground[go_y][go_x].active == 1)
			//					slice[i].turn = -0.45;
			//				else if (i == 1 && ground[go_y][go_x].active == 0)
			//					slice[i].turn = -0.45;
			//				else if (i == 1 && ground[go_y][go_x].active == 1)
			//					slice[i].turn = 1.35;
			//			}

			//			cutter.shack = 20;
			//			cutter.semo_you = 3;
			//		}
			//	}
				//else if (left_y < fruit.y - 25 && right_y > fruit.y + 25 && left_x > fruit.x - 25 && left_x < fruit.x + 25 && right_x > fruit.x - 25 && right_x < fruit.x + 25)//세로 자름
				//{
				//	if (abs((left_y - right_y) / (left_x - right_x)) > 4)
				//	{
				//		fruit.active = 0;
				//		slice[0].active = 1;
				//		slice[1].active = 1;
				//		slice[0].x = fruit.x;
				//		slice[1].x = fruit.x;
				//		slice[0].y = fruit.y;
				//		slice[1].y = fruit.y;

				//		//파티클도 만들어 줘야지~!
				//		for (int s{}; s < 10; ++s)
				//		{
				//			float rands = dist400(gen);
				//			particle[s].alpha = 1;
				//			particle[s].x = slice[0].x;
				//			particle[s].y = slice[0].y;
				//			particle[s].speed = -2 + rands / 100;
				//			particle[s].gravity = 0;
				//		}

				//		//넌 어디로 날라갈레 삼각형아?
				//		int go_x = -1;
				//		for (int i{}; i < 2; ++i)
				//		{
				//			int go_y = 2;
				//			int pass = 1;
				//			do
				//			{
				//				pass = 1;
				//				while (true)
				//				{
				//					int go_x2 = dist16(gen);
				//					if (go_x2 != go_x)
				//					{
				//						go_x = go_x2;
				//						break;
				//					}
				//				}
				//				if (ground[0][go_x].active != 2)
				//				{
				//					go_y = 0;
				//					pass = 0;
				//				}
				//				else if (ground[1][go_x].active != 2)
				//				{
				//					go_y = 1;
				//					pass = 0;
				//				}
				//				else if (ground[2][go_x].active != 2)
				//				{
				//					go_y = 2;
				//					pass = 0;
				//				}
				//			} while (pass);

				//			slice[i].now_x = slice[i].x;
				//			slice[i].now_y = slice[i].y;
				//			if (i == 0)
				//				slice[i].point_x = 0;
				//			else
				//				slice[i].point_x = 800;
				//			slice[i].point_y = slice[i].y;
				//			slice[i].move_x = go_x * 50 + 25;
				//			//semo[i].move_y = 575 - go_y * 50;
				//			slice[i].move_y = 650;
				//			slice[i].move_count = 0;

				//			if (i == 0 && ground[go_y][go_x].active == 0)
				//				slice[i].turn = -1.35;
				//			else if (i == 0 && ground[go_y][go_x].active == 1)
				//				slice[i].turn = 0.45;
				//			else if (i == 1 && ground[go_y][go_x].active == 0)
				//				slice[i].turn = 0.45;
				//			else if (i == 1 && ground[go_y][go_x].active == 1)
				//				slice[i].turn = -1.35;
				//		}

				//		cutter.shack = 20;
				//		cutter.semo_you = 4;
				//	}
				//}
			}

			cutter.cut_active = 0;
		}
		if (cutter.move == 1 || cutter.move == 2)//나뉜 삼각형 놓을때
		{
			if (cutter.move == 1)//왼쪽거
			{

				if (baskets.active == 1 && x > baskets.x - 25 && x < baskets.x + 25 && y > baskets.y - 25 && y < baskets.y + 25)
				{


					if (slice[0].active == 0 && slice[1].active == 0)
					{
						fruit.active = 1;
						fruit.y = 600;

						int ch = dist(gen);
						if (ch == 1)
							fruit.ro = 0;
						else
							fruit.ro = 45;
					}
				}

			}
			if (cutter.move == 2)//오른쪽거
			{

				if (baskets.active == 1 && x > baskets.x - 25 && x < baskets.x + 25 && y > baskets.y - 25 && y < baskets.y + 25)
				{


					if (slice[0].active == 0 && slice[1].active == 0)
					{
						fruit.active = 1;
						fruit.y = 600;

						int ch = dist(gen);
						if (ch == 1)
							fruit.ro = 0;
						else
							fruit.ro = 45;
					}
				}

			}
			cutter.move = 0;
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
	if (cutter.cut_active == 1)
	{
		cutter.cut_x[1] = x;
		cutter.cut_y[1] = y;
	}

	if (cutter.move == 1)
	{
		slice[0].x = x;
		slice[0].y = y;
	}
	if (cutter.move == 2)
	{
		slice[1].x = x;
		slice[1].y = y;
	}
}
bool FindIntersection(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec2& intersection) {
	float under = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);
	if (under == 0.0f)
		return false;

	float t = (p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x);
	t /= under;

	float s = (p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x);
	s /= under;

	if (t < 0.0f || t > 1.0f || s < 0.0f || s > 1.0f)
		return false;

	intersection = p1 * (1 - t) + t * p2;
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