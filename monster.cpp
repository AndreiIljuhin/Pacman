#include "monster.h"

Monster::Monster()
{
	dir = LEFT;
}

void Monster::move(int** map)
{
	srand(time(0));
	lastI = i;//предыдущее положение в консоли
	lastJ = j;
	map[lastI][lastJ] = lastPoint;//предыдущее значение пройденной точки

	//7 - монетка, 0 - пустота, 8 - пакман
	if ((catchUp and pursuitTime < 40) or (!catchUp and pursuitTime < 35)) {
		bool pacmanInSight = false;
		//поле видимости
		for (int q = 1; q < 14; q++) {
			if (map[i + q][j] == 8) {
				pacmanInSight = true;
				catchUp = pacmanInSight;
				distancePC = q - 1;
				dir = DOWN;
				i++;
				break;
			}
			else if (!(map[i + q][j] == 7 or map[i + q][j] == 0))
				break;
		}
		if (!pacmanInSight)
			for (int q = 1; q < 14; q++) {
				if (map[i - q][j] == 8) {
					pacmanInSight = true;
					catchUp = pacmanInSight;
					distancePC = q - 1;
					dir = UP;
					i--;
					break;
				}
				else if (!(map[i - q][j] == 7 or map[i - q][j] == 0))
					break;
			}
		if (!pacmanInSight)
			for (int q = 1; q < 14; q++) {
				if (map[i][j - q] == 8) {
					pacmanInSight = true;
					catchUp = pacmanInSight;
					distancePC = q - 1;
					dir = LEFT;
					j--;
					break;
				}
				else if (!(map[i][j - q] == 7 or map[i][j - q] == 0))
					break;
			}
		if (!pacmanInSight)
			for (int q = 1; q < 14; q++) {
				if (map[i][j + q] == 8) {
					pacmanInSight = true;
					catchUp = pacmanInSight;
					distancePC = q - 1;
					dir = RIGHT;
					j++;
					break;
				}
				else if (!(map[i][j + q] == 7 or map[i][j + q] == 0))
					break;
			}

		if (catchUp != pacmanInSight) {
			switch (dir)
			{
			case LEFT://a
				if (distancePC == 0) {
					catchUp = false;
					break;
				}
				else if (map[i][j - 1] == 7 or map[i][j - 1] == 0) {
					j--;
					distancePC--;
				}
				else
					catchUp = false;
				break;
			case RIGHT://d
				if (distancePC == 0) {
					catchUp = false;
					break;
				}
				else if (map[i][j + 1] == 7 or map[i][j + 1] == 0) {
					j++;
					distancePC--;
				}
				else
					catchUp = false;
				break;
			case UP://w
				if (distancePC == 0) {
					catchUp = false;
					break;
				}
				else if (map[i - 1][j] == 7 or map[i - 1][j] == 0) {
					i--;
					distancePC--;
				}
				else
					catchUp = false;
				break;
			case DOWN://s
				if (distancePC == 0) {
					catchUp = false;
					break;
				}
				else if (map[i + 1][j] == 7 or map[i + 1][j] == 0) {
					i++;
					distancePC--;
				}
				else
					catchUp = false;
				break;
			}
		}

	}
	else {
		catchUp = false;
		distancePC = -1;
	}

	if (catchUp or distancePC == 0)
		pursuitTime++;
	else {
		if (pursuitTime < 35)
			pursuitTime = 0;
		else
			pursuitTime--;
	}

	if (!catchUp) {//рандомный путь, если не догнал пакмана
		int left = map[i][j - 1], right = map[i][j + 1], up = map[i - 1][j], down = map[i + 1][j];

		switch (dir)
		{
		case LEFT://a
			if (int a = (int)(down == 0 or down == 7 or down == 8) + (int)(up == 0 or up == 7 or up == 8)) {
				if (rand() % 5 or distancePC == 0) {
					int b = rand() % a;
					if (b == 1) {
						if (down == 7 or down == 0 or down == 8) {
							dir = DOWN;
							i++;
						}
						else {
							dir = UP;
							i--;
						}
					}
					else {
						if (up == 7 or up == 0 or up == 8) {
							dir = UP;
							i--;
						}
						else {
							dir = DOWN;
							i++;
						}
					}
					if (distancePC == 0)
						distancePC = -1;
				}
				else if (left == 7 or left == 0 or left == 8) {
					j--;
				}
				else if (right == 7 or right == 0 or right == 8) {
					dir = RIGHT;
					j++;
				}
			}
			else if (left == 7 or left == 0 or left == 8) {
				j--;
			}
			else if (right == 7 or right == 0 or right == 8) {
				dir = RIGHT;
				j++;
			}
			break;
		case RIGHT://d
			if (int a = (int)(down == 0 or down == 7 or down == 8) + (int)(up == 0 or up == 7 or up == 8)) {
				if (rand() % 5 or distancePC == 0) {
					int b = rand() % a;
					if (b == 1) {
						if (down == 7 or down == 0 or down == 8) {
							dir = DOWN;
							i++;
						}
						else {
							dir = UP;
							i--;
						}
					}
					else {
						if (up == 7 or up == 0 or up == 8) {
							dir = UP;
							i--;
						}
						else {
							dir = DOWN;
							i++;
						}
					}
					if (distancePC == 0)
						distancePC = -1;
				}
				else if (right == 7 or right == 0 or right == 8) {
					j++;
				}
				else if (left == 7 or left == 0 or left == 8) {
					dir = LEFT;
					j--;
				}
			}
			else if (right == 7 or right == 0 or right == 8) {
				j++;
			}
			else if (left == 7 or left == 0 or left == 8) {
				dir = LEFT;
				j--;
			}
			break;
		case UP://w
			if (int a = (int)(right == 0 or right == 7 or right == 8) + (int)(left == 0 or left == 7 or left == 8)) {
				if (rand() % 5 or distancePC == 0) {
					int b = rand() % a;
					if (b == 1) {
						if (right == 7 or right == 0 or right == 8) {
							dir = RIGHT;
							j++;
						}
						else {
							dir = LEFT;
							j--;
						}
					}
					else {
						if (left == 7 or left == 0 or left == 8) {
							dir = LEFT;
							j--;
						}
						else {
							dir = RIGHT;
							j++;
						}
					}
					if (distancePC == 0)
						distancePC = -1;
				}
				else if (up == 7 or up == 0 or up == 8) {
					i--;
				}
				else if (down == 7 or down == 0 or down == 8) {
					dir = DOWN;
					i++;
				}
			}
			else if (up == 7 or up == 0 or up == 8) {
				i--;
			}
			else if (down == 7 or down == 0 or down == 8) {
				dir = DOWN;
				i++;
			}
			break;
		case DOWN://s
			if (int a = (int)(right == 0 or right == 7 or right == 8) + (int)(left == 0 or left == 7 or left == 8)) {
				if (rand() % 5 or distancePC == 0) {
					int b = rand() % a;
					if (b == 1) {
						if (right == 7 or right == 0 or right == 8) {
							dir = RIGHT;
							j++;
						}
						else {
							dir = LEFT;
							j--;
						}
					}
					else {
						if (left == 7 or left == 0 or left == 8) {
							dir = LEFT;
							j--;
						}
						else {
							dir = RIGHT;
							j++;
						}
					}
					if (distancePC == 0)
						distancePC = -1;
				}
				else if (down == 7 or down == 0 or down == 8) {
					i++;
				}
				else if (up == 7 or up == 0 or up == 8) {
					dir = UP;
					i--;
				}
			}
			else if (down == 7 or down == 0 or down == 8) {
				i++;
			}
			else if (up == 7 or up == 0 or up == 8) {
				dir = UP;
				i--;
			}
			break;
		}
	}

	if (map[i][j] != 9)
		lastPoint = map[i][j];
	map[i][j] = 9;
}

void Monster::set_i_j(int i, int j)
{
	this->i = i;
	this->j = j;
	lastI = i;
	lastJ = j;
	lastPoint = 0;
	distancePC = -1;
}
