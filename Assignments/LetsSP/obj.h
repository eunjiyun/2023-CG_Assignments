#pragma once

class Fruits
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
	int shape;
};

class Baskets
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

	float turn = 0;//100���� ���������
};

class Floor
{
public:
	int active = 0;//0������� 1�ϳ������� 2����������������
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
	int move = 0;//1�������� 2����������

	int semo_you = 0;//1���ʿ��� ¥�� 2�����ʿ��� ¥�� 3����¥�� 4����¥��

	int star_count = 0;//���° ��
	int star_color[3];

	int move_x[2];
	int move_y[2];
	int move_count[2];

	int shine = 150;
	int shine_sw = 0;
};