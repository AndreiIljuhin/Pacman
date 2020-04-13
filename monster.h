#pragma once
#include "character.h"
#include <ctime>

class Monster :
	public Character
{
public:
	Monster();

	void move(int** map) override;
	void set_i_j(int i, int j) override;

private:
	bool catchUp = 0;//�������������
	int pursuitTime = 0, distancePC;//����� �������������
};