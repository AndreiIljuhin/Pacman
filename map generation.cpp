#include "map generation.h"

Worm::Worm() {
	i = 1;
	j = 1;
	width = 0;
	height = 0;
	unreachedPoints = nullptr;
}

void Worm::set(int i, int j, int w, int h) {
	this->i = i;
	this->j = j;
	width = w;
	height = h;
}

void Worm::shift_u_p(int position) {
	delete[] unreachedPoints[position];
	for (int i = position; i < pN - 1; i++)
		unreachedPoints[i] = unreachedPoints[i + 1];
	unreachedPoints[pN - 1] = nullptr;
	pN--;
}

void Worm::move(int** map) {
	int destination, destI, destJ, distance, count = 0;
	do {
		destination = rand() % pN;
		destI = unreachedPoints[destination][0];
		destJ = unreachedPoints[destination][1];
		distance = sqrt(pow(i - destI, 2) + pow(j - destJ, 2));
		count++;
		if (count == 20) {
			if (destI == i or destJ == j) {
				count = 0;
				do {
					destination = rand() % pN;
					destI = unreachedPoints[destination][0];
					destJ = unreachedPoints[destination][1];
					count++;
				} while ((destI == i or destJ == j) and count < 10);
				count = 20;
			}
		}
	} while (distance > 15 and count < 20);

	if (rand() % 2) {
		if (destI != i)
			destI > i ? dir = DOWN : dir = UP;
		else
			destJ > j ? dir = RIGHT : dir = LEFT;
	}
	else {
		if (destJ != j)
			destJ > j ? dir = RIGHT : dir = LEFT;
		else
			destI > i ? dir = DOWN : dir = UP;
	}

	while (destI != i or destJ != j) {
		if (!(rand() % 10) or destI == i or destJ == j) {
			if (rand() % 2) {
				if (destI != i)
					destI > i ? dir = DOWN : dir = UP;
				else
					destJ > j ? dir = RIGHT : dir = LEFT;
			}
			else {
				if (destJ != j)
					destJ > j ? dir = RIGHT : dir = LEFT;
				else
					destI > i ? dir = DOWN : dir = UP;
			}
		}

		switch (dir) {
		case LEFT:
			j--;
			if (i % 2 == 0 and j % 2 == 0) {
				if (destJ != j)
					j--;
				else
					destI > i ? i++ : i--;
			}
			break;
		case RIGHT:
			j++;
			if (i % 2 == 0 and j % 2 == 0) {
				if (destJ != j)
					j++;
				else
					destI > i ? i++ : i--;
			}
			break;
		case UP:
			i--;
			if (i % 2 == 0 and j % 2 == 0) {
				if (destI != i)
					i--;
				else
					destJ > j ? j++ : j--;
			}
			break;
		case DOWN:
			i++;
			if (i % 2 == 0 and j % 2 == 0) {
				if (destI != i)
					i++;
				else
					destJ > j ? j++ : j--;
			}
			break;
		}
		//удаление пройденной точки из unreachedPoints
		if (i % 2 != 0 and j % 2 != 0 and !(destI == i and destJ == j)) {
			int number = -1;
			for (int q = 0; q < pN; q++) {
				if (unreachedPoints[q][0] == i and unreachedPoints[q][1] == j) {
					shift_u_p(q);
					if (destination > q)
						destination--;
					break;
				}
			}
		}
		map[i][j] = 7;
	}
	shift_u_p(destination);
};

bool Worm::dig(int** map) {
	if (width <= 0 or height <= 0 or i <= 0 or i > height - 1 or j <= 0 or j > width - 1) {
		//cout << "Set the correct data\n";
		return 0;
	}
	pN = (width / 2) * (height / 2);
	unreachedPoints = new int*[pN];
	for (int i = 0; i < pN; i++)
		unreachedPoints[i] = new int[2];
	int** upPtr = unreachedPoints;
	for (int i = 1; i < height; i++)
		for (int j = 1; j < width; j++)
			if (map[i][j] == 7) {
				upPtr[0][0] = i;
				upPtr[0][1] = j;
				upPtr++;
			}

	while (*unreachedPoints != nullptr) {
		move(map);
	}
	delete[] unreachedPoints;
	unreachedPoints = nullptr;
	return 1;
};

void map_generation(int**& map, int& w, int& h) {
	if (map != nullptr) {
		for (int i = 0; i < h; i++)
			delete[] map[i];
		delete[] map;
	}

	w = rand() % 20 + 16;
	if (w % 2 == 0)
		w++;
	h = rand() % 20 + 16;
	if (h % 2 == 0)
		h++;

	map = new int*[h];
	for (int i = 0; i < h; i++)
		map[i] = new int[w];

	for (int i = 1; i < h - 1; i++)
		for (int j = 1; j < w - 1; j++)
			map[i][j] = 7;

	for (int i = 0; i < h; i += 2)
		for (int j = 0; j < w; j++)
			map[i][j] = 1;

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j += 2)
			map[i][j] = 1;

	Worm worm;
	worm.set(1, 1, w, h);
	worm.dig(map);

	//убрать одиночные стенки
	for (int i = 2; i < h - 2; i++)
		for (int j = 2; j < w - 2; j++) {

			if (map[i][j] == 1) {
				if (map[i - 1][j] == 7 and map[i + 1][j] == 7 and map[i][j - 1] == 7 and map[i][j + 1] == 7) {
					switch (rand() % 4) {
					case 0:
						map[i - 1][j] = 1;
						break;
					case 1:
						map[i + 1][j] = 1;
						break;
					case 2:
						map[i][j - 1] = 1;
						break;
					case 3:
						map[i][j + 1] = 1;
						break;
					}
				}
			}

		}

	//отформатировать карту
	for (int i = 0; i < h; i++) {
		map[i][0] = 2;
		map[i][w - 1] = 3;
	}
	for (int j = 1; j < w; j++) {
		if (map[1][j] == 1)
			map[0][j] = 4;
		if (map[h - 2][j] == 1)
			map[h - 1][j] = 5;
	}
	for (int i = 1; i < h - 1; i++)
		for (int j = 1; j < w - 1; j++) {
			if (map[i][j] == 1) {
				if (map[i][j - 1] == 7 and map[i][j + 1] == 7) {
					map[i][j] = 3;
					continue;
				}
				if (map[i - 1][j] == 7 and map[i + 1][j] == 7) {
					continue;
				}
				if (map[i - 1][j] == 7) {
					map[i][j] = 4;
					continue;
				}
				map[i][j] = 5;
			}
		}

	//убрать короткие горизонтальные стенки
	for (int i = 2; i < h - 2; i++) {
		int start = 0, end = 0;
		for (int j = 2; j < w - 2; j++) {
			if (map[i][j] == 1) {
				if (map[i][j - 1] == 7)
					start = j;
				if (map[i][j + 1] == 7)
					end = j;
			}
			else
				start = end = 0;
			if (start and end) {
				if (end - start < 7) {
					if (rand() % 2) {
						map[i][end + 1] = 1;
						if (map[i][end + 2] == 3 and end + 2 != w - 1) {
							if (map[i - 1][end + 2] == 7)
								map[i][end + 2] = 4;
							else
								map[i][end + 2] = 5;
						}
					}
					else {
						map[i][start - 1] = 1;
						if (map[i][start - 2] == 3 and start - 2 != 0) {
							if (map[i - 1][start - 2] == 7)
								map[i][start - 2] = 4;
							else
								map[i][start - 2] = 5;
						}
					}
					start = end = 0;
				}
				else
					start = end = 0;
			}
		}
	}

	//убрать короткие вертикальные стенки
	for (int j = 2; j < w - 2; j++) {
		int start = 0, end = 0;
		for (int i = 2; i < h - 2; i++) {
			if (map[i][j] == 3) {
				if (map[i - 1][j] == 7)
					start = i;
				if (map[i + 1][j] == 7)
					end = i;
			}
			else
				start = end = 0;

			if (start and end) {
				if (end - start < 7) {
					if (rand() % 2) {
						map[end + 1][j] = 3;
						if (map[end + 2][j] == 1)
							map[end + 2][j] = 5;
					}
					else {
						map[start - 1][j] = 3;
						if (map[start - 2][j] == 1)
							map[start - 2][j] = 4;
					}
					start = end = 0;
				}
				else
					start = end = 0;
			}

		}
	}

	switch (rand() % 4) {
	case 0:
		map[1][1] = 8;//вход
		map[h - 2][w - 1] = 6;//выход

		for (int i = 5; i < h - 1; i += 2) {//монстры
			for (int j = 5; j < w - 1; j += 2) {
				if (!(rand() % 12)) {
					map[i][j] = 9;
					i += 2;
					break;
				}
			}
		}
		break;
	case 1:
		map[h - 2][w - 2] = 8;
		map[1][0] = 6;

		for (int i = 1; i < h - 5; i += 2) {
			for (int j = 1; j < w - 5; j += 2) {
				if (!(rand() % 12)) {
					map[i][j] = 9;
					i += 2;
					break;
				}
			}
		}
		break;
	case 2:
		map[h - 2][1] = 8;
		map[1][w - 1] = 6;

		for (int i = 1; i < h - 5; i += 2) {
			for (int j = 5; j < w - 1; j += 2) {
				if (!(rand() % 12)) {
					map[i][j] = 9;
					i += 2;
					break;
				}
			}
		}
		break;
	case 3:
		map[1][w - 2] = 8;
		map[h - 2][0] = 6;

		for (int i = 5; i < h - 1; i += 2) {
			for (int j = 1; j < w - 5; j += 2) {
				if (!(rand() % 12)) {
					map[i][j] = 9;
					i += 2;
					break;
				}
			}
		}
		break;
	}
}