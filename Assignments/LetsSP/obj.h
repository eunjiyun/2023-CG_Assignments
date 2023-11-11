#pragma once

class Floor
{
public:
	int act{};
};

class Particles
{
public:
	float down{};
	float velocity{};
	float fourthCol{};
	float xPos{};
	float yPos{};
};

class Fruits
{
public:
	float xPos{ -100 };
	float yPos{ 45 };
	int rotation{};
	int act{ 1 };
	float moveX{};
	float moveY{};
	int movingCnt{};
	float origin{ 1 };
	int originSw{};
	int shape;
};

class Slices
{
public:
	float ptX{};
	float ptY{};
	float moveX{};
	float moveY{};
	int movingCnt{};
	float rot{};
	float xPos{ -100 };
	float yPos{ 45 };
	int rotation{};
	int act{ 1 };
	float oldX{};
	float oldY{};
};

class Cutter
{
public:
	int bounce{};
	int change{};
	int cutPos{};
	float xPos[2];
	float yPos[2];
	int act{};
	int glow{ 150 };
	int glowSw{};
	int moveX[2];
	int moveY[2];
	int movingCnt[2];
};