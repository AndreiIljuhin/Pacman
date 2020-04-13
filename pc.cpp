#include "pc.h"

void Pacman::move(int** map)
{
	lastI = i;//предыдущее положение в консоли
	lastJ = j;
	map[lastI][lastJ] = 0;//новое значение пройденной точки

	if (_kbhit())
		switch (_getch()) {
		case 'a':
			newMove = 1;
			break;
		case 'd':
			newMove = 2;
			break;
		case 'w':
			newMove = 3;
			break;
		case 's':
			newMove = 4;
			break;
		}

	//7 - монетка, 0 - пустота, 6 - выход
	switch (newMove)//newMove хранит предыдущее направление, если пользователь не нажимает клавиши
	{
	case 1://a
		if (map[i][j - 1] == 7 or map[i][j - 1] == 0 or map[i][j - 1] == 6)
			dir = LEFT;
		break;
	case 2://d
		if (map[i][j + 1] == 7 or map[i][j + 1] == 0 or map[i][j + 1] == 6)
			dir = RIGHT;
		break;
	case 3://w
		if (map[i - 1][j] == 7 or map[i - 1][j] == 0 or map[i - 1][j] == 6)
			dir = UP;
		break;
	case 4://s
		if (map[i + 1][j] == 7 or map[i + 1][j] == 0 or map[i + 1][j] == 6)
			dir = DOWN;
		break;
	}

	switch (dir)
	{
	case LEFT://a
		if (map[i][j - 1] == 7 or map[i][j - 1] == 0 or map[i][j - 1] == 6) {
			j--;
		}
		break;
	case RIGHT://d
		if (map[i][j + 1] == 7 or map[i][j + 1] == 0 or map[i][j + 1] == 6) {
			j++;
		}
		break;
	case UP://w
		if (map[i - 1][j] == 7 or map[i - 1][j] == 0 or map[i - 1][j] == 6) {
			i--;
		}
		break;
	case DOWN://s
		if (map[i + 1][j] == 7 or map[i + 1][j] == 0 or map[i + 1][j] == 6) {
			i++;
		}
		break;
	}

	lastPoint = map[i][j];
	map[i][j] = 8;
}

void Pacman::set_i_j(int i, int j)
{
	this->i = i;
	this->j = j;
	lastI = i;
	lastJ = j;
	lastPoint = 0;
	dir = STOP;
	newMove = 0;
}