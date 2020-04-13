#pragma once
#include "character.h"
#include <conio.h>

class Pacman :
	public Character
{
public:
	void move(int** map) override;
	void set_i_j(int i, int j) override;

private:
	int newMove = 0;
};