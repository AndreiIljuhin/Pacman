#pragma once
#include <iostream>
#include <ctime>

class Worm {
private:
	int i, j;
	int width, height;
	int** unreachedPoints, pN;
	enum Direction { LEFT, RIGHT, UP, DOWN } dir;

	void shift_u_p(int position);

	void move(int** map);

public:
	Worm();

	void set(int i, int j, int w, int h);
	bool dig(int** map);
};

void map_generation(int**& map, int& w, int& h);