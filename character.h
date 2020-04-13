#pragma once
#include <iostream>
class Character
{
protected:
	enum Direction { STOP, LEFT, RIGHT, UP, DOWN } dir = STOP;

public:
	int i = 1, j = 1, lastI = 1, lastJ = 1, lastPoint = 0;

	virtual void move(int** map) = 0;
	virtual void set_i_j(int i, int j) {
		this->i = i;
		this->j = j;
		lastI = i;
		lastJ = j;
		lastPoint = 0;
	}
};