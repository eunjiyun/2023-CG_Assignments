#include <GL/freeglut.h>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <math.h>
#include <MMSystem.h> 
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")
using namespace std;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid KeyBoard(unsigned char key, int x, int y);

class Ob
{
public:
	float x = -100;
	float y = 20 + 25;
	int ro = 0;
	int active = 1;

	float move_x = 0;
	float move_y = 0;
	int move_count = 0;

	float big = 1;
	int big_sw = 0;
};

class Ob2
{
public:
	float x = -100;
	float y = 20 + 25;
	int ro = 0;
	int active = 1;

	float now_x = 0;
	float now_y = 0;
	float point_x = 0;
	float point_y = 0;
	float move_x = 0;
	float move_y = 0;
	int move_count = 0;

	float turn = 0;//100으로 나누어쓰세요
};

class Block
{
public:
	int active = 0;//0비어있음 1하나차있음 2꽉차있음오지마셈
};

class Light
{
public:
	float x = 0;
	float y = 0;
	float gravity = 0;
	float speed = 0;
	float alpha = 0;
};

class Any
{
public:
	float cut_x[2];
	float cut_y[2];
	int cut_active = 0;

	int shack = 0;
	int move = 0;//1왼쪽잡음 2오른쪽잡음

	int semo_you = 0;//1왼쪽에서 짜름 2오른쪽에서 짜름 3가로짜름 4세로짜름

	int star_count = 0;//몇번째 별
	int star_color[3];

	int move_x[2];
	int move_y[2];
	int move_count[2];

	int shine = 150;
	int shine_sw = 0;
};

Ob up_tr[1];
Ob nemo;
Ob2 semo[2];
Ob star[100];
Any any;
Block block[3][16];
Light light[10];
int speed{ 20 };
bool direction{ false };

void Timer(int value)
{
	for (int i{}; i < 10; ++i)
	{
		if (light[i].alpha > 0)
		{
			light[i].alpha -= 0.01;
			light[i].gravity += 0.15;
			light[i].x += light[i].speed;
			light[i].y += light[i].gravity;
		}
	}

	if (any.shine_sw == 0)
	{
		if (any.shine < 180)
			any.shine++;
		else
			any.shine_sw = 1;
	}
	else if (any.shine_sw == 1)
	{
		if (any.shine > 120)
			any.shine--;
		else
			any.shine_sw = 0;
	}


	if (up_tr[0].x > 825)
		direction = true;
	else if (0 > up_tr[0].x)
		direction = false;

	if (!direction)
		up_tr[0].x += 5;
	else
		up_tr[0].x -= 5;


	if (up_tr[0].ro < 357)
		up_tr[0].ro += 2;
	else
	{
		//up_tr[i].ro -= 2;
		up_tr[0].ro = 0;
	}


	//if (nemo.active == 1 && nemo.y < -50)
	////if (/*nemo.active == 1 &&*/ nemo.x > 500)
	//{
	//	nemo.y = 600;
	//	int ch = rand() % 2;
	//	if (ch == 1)
	//		nemo.ro = 0;
	//	else
	//		nemo.ro = 45;
	//}
	//else
	//	nemo.y -= 3;
	//	//nemo.x += 3;

	if (nemo.active == 1 && nemo.y < -50)
	{
		nemo.y = 600;
		int ch = rand() % 2;
		if (ch == 1)
			nemo.ro = 0;
		else
			nemo.ro = 45;
	}
	else
	{
		// nemo.x += 3; // 기존 코드 주석 처리

		// 좌에서 우로 이동하는 코드 추가
		nemo.x += 3;
		// 화면 오른쪽 끝을 벗어나면 화면 왼쪽 끝으로 초기화
		if (nemo.x > 825)
		{
			nemo.x = -25;
			int ch = rand() % 2;
			if (ch == 1)
				nemo.ro = 0;
			else
				nemo.ro = 45;
		}
		// y 좌표는 그대로 유지
		nemo.y -= 3;
	}

	if (any.shack > 0)
		any.shack--;

	for (int i{}; i < 3; ++i)
	{
		if (any.star_color[i] >= 255)
			any.star_color[i] = 0;
		else
			any.star_color[i] += 5;
	}

	for (int i{}; i < 100; ++i)
	{
		if (star[i].active == 1)
		{
			if (star[i].ro < 360)
				star[i].ro += 5;
			else
				star[i].ro = 0;

			if (star[i].move_count != 0)
			{
				star[i].x += star[i].move_x;
				star[i].y += star[i].move_y;
				star[i].move_count--;
			}

			if (star[i].big_sw == 0)
			{
				if (star[i].big < 1.5)
					star[i].big += 0.05;
				else
					star[i].big_sw = 1;
			}
			else if (star[i].big_sw == 1)
			{
				if (star[i].big > 0.5)
					star[i].big -= 0.05;
				else
					star[i].big_sw = 0;
			}
		}
	}

	for (int i{}; i < 2; ++i)
	{
		if (semo[i].move_count != 100 && semo[i].active == 1)
		{
			if (i == 0 && any.move != 1)
			{
				float set1_x = semo[i].now_x - ((semo[i].now_x - semo[i].point_x) / 100 * semo[i].move_count);
				float set2_x = semo[i].point_x - ((semo[i].point_x - semo[i].move_x) / 100 * semo[i].move_count);
				float set1_y = semo[i].now_y - ((semo[i].now_y - semo[i].point_y) / 100 * semo[i].move_count);
				float set2_y = semo[i].point_y - ((semo[i].point_y - semo[i].move_y) / 100 * semo[i].move_count);
				float new_x = set1_x - ((set1_x - set2_x) / 100 * semo[i].move_count);
				float new_y = set1_y - ((set1_y - set2_y) / 100 * semo[i].move_count);

				semo[i].x = new_x;
				semo[i].y = new_y;
				semo[i].move_count++;
			}
			if (i == 1 && any.move != 2)
			{
				float set1_x = semo[i].now_x - ((semo[i].now_x - semo[i].point_x) / 100 * semo[i].move_count);
				float set2_x = semo[i].point_x - ((semo[i].point_x - semo[i].move_x) / 100 * semo[i].move_count);
				float set1_y = semo[i].now_y - ((semo[i].now_y - semo[i].point_y) / 100 * semo[i].move_count);
				float set2_y = semo[i].point_y - ((semo[i].point_y - semo[i].move_y) / 100 * semo[i].move_count);
				float new_x = set1_x - ((set1_x - set2_x) / 100 * semo[i].move_count);
				float new_y = set1_y - ((set1_y - set2_y) / 100 * semo[i].move_count);

				semo[i].x = new_x;
				semo[i].y = new_y;
				semo[i].move_count++;
			}
		}
		else if (semo[i].move_count == 100 && semo[i].active == 1)
		{
			semo[i].active = 0;
			int xx = (semo[i].move_x - 25) / 50;
			int yy = -(semo[i].move_y - 575) / 50;

			block[yy][xx].active++;
			cout << xx << " " << yy << endl;

			if (semo[0].active == 0 && semo[1].active == 0)
			{
				nemo.active = 1;
				nemo.y = 600;
				//nemo.x = 0;

				int ch = rand() % 2;
				if (ch == 1)
					nemo.ro = 0;
				else
					nemo.ro = 45;
			}
		}
	}


	glutPostRedisplay(); // 화면 재 출력 
	glutTimerFunc(speed, Timer, 1); // 타이머함수 재 설정
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (nemo.active == 1)
		{
			any.cut_active = 1;
			if (any.cut_active == 1)
			{
				any.cut_x[0] = x;
				any.cut_y[0] = y;
				any.cut_x[1] = x;
				any.cut_y[1] = y;
			}
		}

		if (semo[0].active == 1 && semo[0].x - 25 < x && semo[0].x + 25 > x && semo[0].y - 25 < y && semo[0].y + 25 > y)
		{
			any.move = 1;
			semo[0].x = x;
			semo[0].y = y;
		}
		if (semo[1].active == 1 && semo[1].x - 25 < x && semo[1].x + 25 > x && semo[1].y - 25 < y && semo[1].y + 25 > y)
		{
			any.move = 2;
			semo[1].x = x;
			semo[1].y = y;
		}
	}
	else if ((button == GLUT_LEFT_BUTTON && state == GLUT_UP))
	{
		if (any.cut_active == 1)
		{
			float left_x, right_x, left_y, right_y;
			if (any.cut_x[0] < any.cut_x[1])
			{
				left_x = any.cut_x[0];
				right_x = any.cut_x[1];
				left_y = any.cut_y[0];
				right_y = any.cut_y[1];
			}
			else
			{
				left_x = any.cut_x[1];
				right_x = any.cut_x[0];
				left_y = any.cut_y[1];
				right_y = any.cut_y[0];
			}

			if (nemo.ro == 0 && nemo.y > 130)//네모 자름
			{
				if (left_x < nemo.x && right_x > nemo.x + 50 && left_y < nemo.y && right_y > nemo.y + 50)//왼쪽 위 자름
				{
					float far_x = abs(right_x - left_x);//선분x길이
					float far_y = abs(right_y - left_y);//선분y길이

					float check1_x = abs(nemo.x - 25 - left_x);//첫점까지x거리
					float be1_x = check1_x / far_x;//몇대 몇?
					float line1_y = left_y + far_y * be1_x;
					cout << "line1_y : " << line1_y << "     nemo.y - 25 : " << nemo.y - 25 << " /     " << abs(line1_y - (nemo.y - 25)) << endl;

					float check2_x = abs(nemo.x + 25 - left_x);//첫점까지x거리
					float be2_x = check2_x / far_x;//몇대 몇?
					float line2_y = left_y + far_y * be2_x;
					cout << "line2_y : " << line2_y << "     nemo.y + 25 : " << nemo.y + 25 << " /     " << abs(line2_y - (nemo.y + 25)) << endl;

					if (abs(line1_y - (nemo.y - 25)) < 20 && abs(line2_y - (nemo.y + 25)) < 20)
					{
						nemo.active = 0;
						semo[0].active = 1;
						semo[1].active = 1;
						semo[0].x = nemo.x;
						semo[1].x = nemo.x;
						semo[0].y = nemo.y;
						semo[1].y = nemo.y;

						//파티클도 만들어 줘야지~!
						for (int s{}; s < 10; ++s)
						{
							float rands = rand() % 400;
							light[s].alpha = 1;
							light[s].x = semo[0].x;
							light[s].y = semo[0].y;
							light[s].speed = -2 + rands / 100;
							light[s].gravity = 0;
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
								while (1)
								{
									int go_x2 = rand() % 16;
									if (go_x2 != go_x)
									{
										go_x = go_x2;
										break;
									}
								}
								if (block[0][go_x].active != 2)
								{
									go_y = 0;
									pass = 0;
								}
								else if (block[1][go_x].active != 2)
								{
									go_y = 1;
									pass = 0;
								}
								else if (block[2][go_x].active != 2)
								{
									go_y = 2;
									pass = 0;
								}
							} while (pass);

							semo[i].now_x = semo[i].x;
							semo[i].now_y = semo[i].y;
							if (i == 0)
								semo[i].point_x = 0;
							else
								semo[i].point_x = 800;
							semo[i].point_y = semo[i].y;
							semo[i].move_x = go_x * 50 + 25;
							//semo[i].move_y = 575 - go_y * 50;
							semo[i].move_y = 650;
							semo[i].move_count = 0;

							if (i == 0 && block[go_y][go_x].active == 0)
								semo[i].turn = -0.9;
							else if (i == 0 && block[go_y][go_x].active == 1)
								semo[i].turn = 0.9;
							else if (i == 1 && block[go_y][go_x].active == 0)
								semo[i].turn = 0.9;
							else if (i == 1 && block[go_y][go_x].active == 1)
								semo[i].turn = -0.9;
						}

						any.shack = 20;
						any.semo_you = 1;
					}
				}

				else if (left_x < nemo.x && right_x > nemo.x + 50 && right_y < nemo.y && left_y > nemo.y + 50)//오른쪽 위 자름
				{
					float far_x = abs(right_x - left_x);//선분x길이
					float far_y = abs(right_y - left_y);//선분y길이

					float check1_x = abs(right_x - (nemo.x + 25));//첫점까지x거리
					float be1_x = check1_x / far_x;//몇대 몇?
					float line1_y = right_y + far_y * be1_x;
					cout << "line1_y : " << line1_y << "     nemo.y - 25 : " << nemo.y - 25 << " /     " << abs(line1_y - (nemo.y - 25)) << endl;

					float check2_x = abs(right_x - (nemo.x - 25));//첫점까지x거리
					float be2_x = check2_x / far_x;//몇대 몇?
					float line2_y = right_y + far_y * be2_x;
					cout << "line2_y : " << line2_y << "     nemo.y + 25 : " << nemo.y + 25 << " /     " << abs(line2_y - (nemo.y + 25)) << endl;

					if (abs(line1_y - (nemo.y - 25)) < 20 && abs(line2_y - (nemo.y + 25)) < 20)
					{
						nemo.active = 0;
						semo[0].active = 1;
						semo[1].active = 1;
						semo[0].x = nemo.x;
						semo[1].x = nemo.x;
						semo[0].y = nemo.y;
						semo[1].y = nemo.y;

						//파티클도 만들어 줘야지~!
						for (int s{}; s < 10; ++s)
						{
							float rands = rand() % 400;
							light[s].alpha = 1;
							light[s].x = semo[0].x;
							light[s].y = semo[0].y;
							light[s].speed = -2 + rands / 100;
							light[s].gravity = 0;
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
								while (1)
								{
									int go_x2 = rand() % 16;
									if (go_x2 != go_x)
									{
										go_x = go_x2;
										break;
									}
								}
								if (block[0][go_x].active != 2)
								{
									go_y = 0;
									pass = 0;
								}
								else if (block[1][go_x].active != 2)
								{
									go_y = 1;
									pass = 0;
								}
								else if (block[2][go_x].active != 2)
								{
									go_y = 2;
									pass = 0;
								}
							} while (pass);

							semo[i].now_x = semo[i].x;
							semo[i].now_y = semo[i].y;
							if (i == 0)
								semo[i].point_x = 0;
							else
								semo[i].point_x = 800;
							semo[i].point_y = semo[i].y;
							semo[i].move_x = go_x * 50 + 25;
							//semo[i].move_y = 575 - go_y * 50;
							semo[i].move_y = 650;
							semo[i].move_count = 0;

							if (i == 0 && block[go_y][go_x].active == 0)
								semo[i].turn = 1.8;
							else if (i == 0 && block[go_y][go_x].active == 1)
								semo[i].turn = 0;
							else if (i == 1 && block[go_y][go_x].active == 0)
								semo[i].turn = 0;
							else if (i == 1 && block[go_y][go_x].active == 1)
								semo[i].turn = 1.8;
						}

						any.shack = 20;
						any.semo_you = 2;
					}
				}
			}
			if (nemo.ro == 45 && nemo.y > 130)//마름모 자름
			{
				if (left_x < nemo.x - 25 && right_x > nemo.x + 25 && left_y > nemo.y - 25 && left_y < nemo.y + 25 && right_y > nemo.y - 25 && right_y < nemo.y + 25)//가로 자름
				{
					if (abs((left_y - right_y) / (left_x - right_x)) < 0.25)
					{
						nemo.active = 0;
						semo[0].active = 1;
						semo[1].active = 1;
						semo[0].x = nemo.x;
						semo[1].x = nemo.x;
						semo[0].y = nemo.y;
						semo[1].y = nemo.y;

						//파티클도 만들어 줘야지~!
						for (int s{}; s < 10; ++s)
						{
							float rands = rand() % 400;
							light[s].alpha = 1;
							light[s].x = semo[0].x;
							light[s].y = semo[0].y;
							light[s].speed = -2 + rands / 100;
							light[s].gravity = 0;
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
								while (1)
								{
									int go_x2 = rand() % 16;
									if (go_x2 != go_x)
									{
										go_x = go_x2;
										break;
									}
								}
								if (block[0][go_x].active != 2)
								{
									go_y = 0;
									pass = 0;
								}
								else if (block[1][go_x].active != 2)
								{
									go_y = 1;
									pass = 0;
								}
								else if (block[2][go_x].active != 2)
								{
									go_y = 2;
									pass = 0;
								}
							} while (pass);

							semo[i].now_x = semo[i].x;
							semo[i].now_y = semo[i].y;
							if (i == 0)
								semo[i].point_x = 0;
							else
								semo[i].point_x = 800;
							semo[i].point_y = semo[i].y;
							semo[i].move_x = go_x * 50 + 25;
							//semo[i].move_y = 575 - go_y * 50;
							semo[i].move_y = 650;
							semo[i].move_count = 0;

							if (i == 0 && block[go_y][go_x].active == 0)
								semo[i].turn = 1.35;
							else if (i == 0 && block[go_y][go_x].active == 1)
								semo[i].turn = -0.45;
							else if (i == 1 && block[go_y][go_x].active == 0)
								semo[i].turn = -0.45;
							else if (i == 1 && block[go_y][go_x].active == 1)
								semo[i].turn = 1.35;
						}

						any.shack = 20;
						any.semo_you = 3;
					}
				}
				else if (left_y < nemo.y - 25 && right_y > nemo.y + 25 && left_x > nemo.x - 25 && left_x < nemo.x + 25 && right_x > nemo.x - 25 && right_x < nemo.x + 25)//세로 자름
				{
					if (abs((left_y - right_y) / (left_x - right_x)) > 4)
					{
						nemo.active = 0;
						semo[0].active = 1;
						semo[1].active = 1;
						semo[0].x = nemo.x;
						semo[1].x = nemo.x;
						semo[0].y = nemo.y;
						semo[1].y = nemo.y;

						//파티클도 만들어 줘야지~!
						for (int s{}; s < 10; ++s)
						{
							float rands = rand() % 400;
							light[s].alpha = 1;
							light[s].x = semo[0].x;
							light[s].y = semo[0].y;
							light[s].speed = -2 + rands / 100;
							light[s].gravity = 0;
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
								while (1)
								{
									int go_x2 = rand() % 16;
									if (go_x2 != go_x)
									{
										go_x = go_x2;
										break;
									}
								}
								if (block[0][go_x].active != 2)
								{
									go_y = 0;
									pass = 0;
								}
								else if (block[1][go_x].active != 2)
								{
									go_y = 1;
									pass = 0;
								}
								else if (block[2][go_x].active != 2)
								{
									go_y = 2;
									pass = 0;
								}
							} while (pass);

							semo[i].now_x = semo[i].x;
							semo[i].now_y = semo[i].y;
							if (i == 0)
								semo[i].point_x = 0;
							else
								semo[i].point_x = 800;
							semo[i].point_y = semo[i].y;
							semo[i].move_x = go_x * 50 + 25;
							//semo[i].move_y = 575 - go_y * 50;
							semo[i].move_y = 650;
							semo[i].move_count = 0;

							if (i == 0 && block[go_y][go_x].active == 0)
								semo[i].turn = -1.35;
							else if (i == 0 && block[go_y][go_x].active == 1)
								semo[i].turn = 0.45;
							else if (i == 1 && block[go_y][go_x].active == 0)
								semo[i].turn = 0.45;
							else if (i == 1 && block[go_y][go_x].active == 1)
								semo[i].turn = -1.35;
						}

						any.shack = 20;
						any.semo_you = 4;
					}
				}
			}

			any.cut_active = 0;
		}
		if (any.move == 1 || any.move == 2)//나뉜 삼각형 놓을때
		{
			if (any.move == 1)//왼쪽거
			{

				if (up_tr[0].active == 1 && x > up_tr[0].x - 25 && x < up_tr[0].x + 25 && y > up_tr[0].y - 25 && y < up_tr[0].y + 25)
				{
					star[any.star_count].x = up_tr[0].x;
					star[any.star_count].y = up_tr[0].y;
					up_tr[0].active = 0;
					semo[0].active = 0;
					semo[0].y = 800;
					star[any.star_count].active = 1;
					star[any.star_count].move_count = 50;
					star[any.star_count].move_x = (rand() % 700 + 50 - star[any.star_count].x) / 50;
					star[any.star_count].move_y = (rand() % 300 + 150 - star[any.star_count].y) / 50;
					any.star_count++;
					//i = 10;

					if (semo[0].active == 0 && semo[1].active == 0)
					{
						nemo.active = 1;
						nemo.y = 600;

						int ch = rand() % 2;
						if (ch == 1)
							nemo.ro = 0;
						else
							nemo.ro = 45;
					}
				}

			}
			if (any.move == 2)//오른쪽거
			{

				if (up_tr[0].active == 1 && x > up_tr[0].x - 25 && x < up_tr[0].x + 25 && y > up_tr[0].y - 25 && y < up_tr[0].y + 25)
				{
					star[any.star_count].x = up_tr[0].x;
					star[any.star_count].y = up_tr[0].y;
					up_tr[0].active = 0;
					semo[1].active = 0;
					semo[1].y = 800;
					star[any.star_count].active = 1;
					star[any.star_count].move_count = 50;
					star[any.star_count].move_x = (rand() % 700 + 50 - star[any.star_count].x) / 50;
					star[any.star_count].move_y = (rand() % 300 + 150 - star[any.star_count].y) / 50;
					any.star_count++;
					//i = 10;

					if (semo[0].active == 0 && semo[1].active == 0)
					{
						nemo.active = 1;
						nemo.y = 600;

						int ch = rand() % 2;
						if (ch == 1)
							nemo.ro = 0;
						else
							nemo.ro = 45;
					}
				}

			}
			any.move = 0;
		}
	}
}

void Motion(int x, int y)
{
	if (any.cut_active == 1)
	{
		any.cut_x[1] = x;
		any.cut_y[1] = y;
	}

	if (any.move == 1)
	{
		semo[0].x = x;
		semo[0].y = y;
	}
	if (any.move == 2)
	{
		semo[1].x = x;
		semo[1].y = y;
	}
}

void main(int argc, char* argv[])
{
	PlaySound("inGame.wav", NULL, SND_ASYNC | SND_LOOP);

	srand(time(NULL));

	//up_tr[i].x = i * 85;
	up_tr[0].ro = 0 * 36;
	up_tr[0].y = 550;

	nemo.x = 400;
	nemo.y = 600;
	semo[0].active = 0;
	semo[1].active = 0;
	for (int i{}; i < 100; ++i)
		star[i].active = 0;

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
		if (any.shack != 0)//화면흔들림
		{
			if (any.shack > 10)
				glTranslatef((any.shack - 20) * 2, (any.shack - 20) * 2, 0.0);
			else
				glTranslatef((any.shack) * -2, (any.shack) * -2, 0.0);
		}

		glShadeModel(GL_SMOOTH);//배경그리기

		glBegin(GL_POLYGON);
		glColor3ub(50, 50, 50);
		glVertex2f(0, 0);
		glColor3ub(50, 50, 50);
		glVertex2f(800, 0);
		glColor3ub(0, 0, 0);
		glVertex2f(800, 600);
		glColor3ub(0, 0, 0);
		glVertex2f(0, 600);
		glEnd();

		glShadeModel(GL_FLAT);

		for (int i{}; i < 10; ++i)//파티클
		{
			if (light[i].alpha > 0)
			{
				glPushMatrix();
				{
					glColor4f(1.0, 1.0, 0.4, light[i].alpha);
					glTranslated(light[i].x, light[i].y, 0);
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



		for (int i{}; i < 100; ++i)//별그리기
		{
			glPushMatrix();
			{
				if (star[i].active == 1)
				{
					glTranslatef(star[i].x, star[i].y, 0);
					glRotatef(star[i].ro, 0, 0, 1);
					glScalef(star[i].big, star[i].big, 1);

					glShadeModel(GL_SMOOTH);

					glBegin(GL_POLYGON);
					glColor3ub(any.star_color[0], any.star_color[1], 255);
					glVertex2f(0, -25);
					glColor3ub(any.star_color[1], 255, any.star_color[2]);
					glVertex2f(-25, 25);
					glColor3ub(255, any.star_color[2], any.star_color[0]);
					glVertex2f(25, 25);
					glEnd();
					glBegin(GL_POLYGON);
					glColor3ub(any.star_color[0], any.star_color[1], 255);
					glVertex2f(25, -10);
					glColor3ub(any.star_color[1], 255, any.star_color[2]);
					glVertex2f(0, 40);
					glColor3ub(255, any.star_color[2], any.star_color[0]);
					glVertex2f(-25, -10);
					glEnd();

					glShadeModel(GL_FLAT);
				}
			}
			glPopMatrix();
		}

		//위에 지나가는 삼각형

		glColor3ub(255, 255, any.shine);
		if (up_tr[0].active == 1)
		{
			glPushMatrix();
			{
				glTranslatef(up_tr[0].x, up_tr[0].y, 0.0);
				//glRotatef(up_tr[i].ro, 0.0, 0.0, 1.0);

				/*glBegin(GL_POLYGON);
				glVertex2f(0, 0 - 25);
				glVertex2f(0 - 25, 0 + 25);
				glVertex2f(0 + 25, 0 + 25);
				glEnd();*/

				glBegin(GL_POLYGON);
				glColor3ub(150, 150, 150);
				glVertex2f(-75, -25);
				glVertex2f(-75, +25);
				glVertex2f(+75, +25);
				glVertex2f(+75, -25);
				glEnd();
			}
			glPopMatrix();
		}


		glPushMatrix();//네모그리기
		{
			glColor3ub(255, 255, any.shine);
			if (nemo.active == 1)
			{
				//if (nemo.y > 130)
				{
					glTranslatef(nemo.x, nemo.y, 0.0);
					glRotatef(nemo.ro, 0, 0, 1);
					glBegin(GL_POLYGON);

					if (45 == nemo.ro)
					{
						glVertex2f(-25, -25);
						glVertex2f(-25, +25);
						glVertex2f(+25, +25);
						glVertex2f(+25, -25);
					}
					else
					{
						glVertex2f(-25, -25);
						glVertex2f(25, 25);
						glVertex2f(-25, 25);
					}
					glEnd();
				}

			}
		}
		glPopMatrix();

		if (nemo.active == 0)//새세모그리기!!
		{
			glColor3ub(255, 255, any.shine);
			if (any.semo_you == 1)//왼쪽 위 자름
			{
				if (semo[0].move_count != 100)
				{
					glPushMatrix();
					{
						glTranslatef(semo[0].x, semo[0].y, 0.0);
						glRotatef(semo[0].turn * semo[0].move_count, 0, 0, 1);
						glBegin(GL_POLYGON);
						glVertex2f(-25, -25);
						glVertex2f(25, 25);
						glVertex2f(-25, 25);
						glEnd();
					}
					glPopMatrix();
				}

				if (semo[1].move_count != 100)
				{
					glPushMatrix();
					{
						glTranslatef(semo[1].x, semo[1].y, 0.0);
						glRotatef(semo[1].turn * semo[1].move_count, 0, 0, 1);
						glBegin(GL_POLYGON);
						glVertex2f(25, -25);
						glVertex2f(-25, -25);
						glVertex2f(25, 25);
						glEnd();
					}
					glPopMatrix();
				}
			}
			else if (any.semo_you == 2)//오른쪽 위 자름
			{
				if (semo[0].move_count != 100)
				{
					glPushMatrix();
					{
						glTranslatef(semo[0].x, semo[0].y, 0.0);
						glRotatef(semo[0].turn * semo[0].move_count, 0, 0, 1);
						glBegin(GL_POLYGON);
						glVertex2f(-25, -25);
						glVertex2f(25, -25);
						glVertex2f(-25, 25);
						glEnd();
					}
					glPopMatrix();
				}

				if (semo[1].move_count != 100)
				{
					glPushMatrix();
					{
						glTranslatef(semo[1].x, semo[1].y, 0.0);
						glRotatef(semo[1].turn * semo[1].move_count, 0, 0, 1);
						glBegin(GL_POLYGON);
						glVertex2f(25, -25);
						glVertex2f(-25, 25);
						glVertex2f(25, 25);
						glEnd();
					}
					glPopMatrix();
				}
			}
			else if (any.semo_you == 3)//가로 자름
			{
				if (semo[0].move_count != 100)
				{
					glPushMatrix();
					{
						glTranslatef(semo[0].x, semo[0].y, 0.0);
						glRotatef(semo[0].turn * semo[0].move_count, 0, 0, 1);
						glRotatef(45, 0, 0, 1);
						glBegin(GL_POLYGON);
						glVertex2f(-25, -25);
						glVertex2f(25, -25);
						glVertex2f(-25, 25);
						glEnd();
					}
					glPopMatrix();
				}

				if (semo[1].move_count != 100)
				{
					glPushMatrix();
					{
						glTranslatef(semo[1].x, semo[1].y, 0.0);
						glRotatef(semo[1].turn * semo[1].move_count, 0, 0, 1);
						glRotatef(45, 0, 0, 1);
						glBegin(GL_POLYGON);
						glVertex2f(25, -25);
						glVertex2f(-25, 25);
						glVertex2f(25, 25);
						glEnd();
					}
					glPopMatrix();
				}
			}
			else if (any.semo_you == 4)//세로 자름
			{
				if (semo[0].move_count != 100)
				{
					glPushMatrix();
					{
						glTranslatef(semo[0].x, semo[0].y, 0.0);
						glRotatef(semo[0].turn * semo[0].move_count, 0, 0, 1);
						glRotatef(-45, 0, 0, 1);
						glBegin(GL_POLYGON);
						glVertex2f(-25, -25);
						glVertex2f(25, -25);
						glVertex2f(-25, 25);
						glEnd();
					}
					glPopMatrix();
				}

				if (semo[1].move_count != 100)
				{
					glPushMatrix();
					{
						glTranslatef(semo[1].x, semo[1].y, 0.0);
						glRotatef(semo[1].turn * semo[1].move_count, 0, 0, 1);
						glRotatef(-45, 0, 0, 1);
						glBegin(GL_POLYGON);
						glVertex2f(25, -25);
						glVertex2f(-25, 25);
						glVertex2f(25, 25);
						glEnd();
					}
					glPopMatrix();
				}
			}
		}

		if (any.cut_active == 1)
		{
			glColor3ub(255, 255, 0);
			glBegin(GL_LINES);
			//glBegin(GL_POLYGON);
			glVertex2f(any.cut_x[0], any.cut_y[0]);
			glVertex2f(any.cut_x[1], any.cut_y[1]);
			glEnd();
		}

	}glPopMatrix();

	glFlush(); // 화면에 출력하기 
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0, 800, 600, 0, -1.0, 1.0);
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