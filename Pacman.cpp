#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <windows.h>
#include <SFML/Audio.hpp>
#include "pc.h"
#include "monster.h"
#include "map generation.h"

using namespace std;
HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

template<typename T>
struct Node {
	T info;
	Node* next = nullptr;
};

template<typename T>
struct List {
	Node<T>* head = nullptr;
	Node<T>* tail = nullptr;
	int N = 0;
};

bool card_loading(int**& map, int& mHeight, int& mWidth, char* file) {
	ifstream in(file);
	if (!in.is_open()) {
		system("cls");
		SetConsoleTextAttribute(hConsoleHandle, 15);
		cout << "Error loading \"" << file << "\"\n";
		cin.get();
		return 0;
	}

	if (map == nullptr) {
		mHeight = 21;
		mWidth = 20;
		map = new int*[mHeight];
		for (int i = 0; i < mHeight; i++)
			map[i] = new int[mWidth];
	}
	else if (mHeight != 21 or mWidth != 20) {
		for (int i = 0; i < mHeight; i++)
			delete[] map[i];
		delete[] map;
		mHeight = 21;
		mWidth = 20;
		map = new int*[mHeight];
		for (int i = 0; i < mHeight; i++)
			map[i] = new int[mWidth];
	}

	for (int i = 0; i < mHeight; i++) {
		for (int j = 0; j < mWidth; j++)
			map[i][j] = in.get() - 48;
		in.get();
	}
	in.close();
	return 1;
}

void map_console_output(int** map, int mHeight, int mWidth, int score, Pacman& pc, List<Monster>& monsters) {
	system("cls");
	Node<Monster>* current = monsters.head, *temp;
	while (current != nullptr) {
		temp = current;
		current = current->next;
		delete temp;
	}
	monsters.head = monsters.tail = nullptr;
	monsters.N = 0;

	int color = rand() % 7 + 1;//цвет стенок
	if (color == 1)//темный синий меняется на светлый синий
		color = 9;
	SetConsoleTextAttribute(hConsoleHandle, color);
	for (int i = 0; i < mHeight; i++) {
		for (int j = 0; j < mWidth; j++) {
			int cell = map[i][j];

			if (cell == 0 or cell == 6)//пустота или выход
				cout << "  ";
			else if (cell == 1)//горизонтальная стенка
				cout << (char)205 << (char)205;
			else if (cell == 2)//левая вертикальная стенка
				cout << " " << (char)186;
			else if (cell == 3)//правая вертикальная стенка
				cout << (char)186 << " ";
			else if (cell == 4)//соединение горизонтальной и вертикальной стенки сверху
				cout << (char)203 << (char)205;
			else if (cell == 5)//соединение горизонтальной и вертикальной стенки снизу
				cout << (char)202 << (char)205;
			else if (cell == 7) {//монетка
				SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_GREEN | FOREGROUND_RED);
				cout << "® ";
				SetConsoleTextAttribute(hConsoleHandle, color);
			}
			else if (cell == 8) {//пакман
				SetConsoleTextAttribute(hConsoleHandle, 14);
				cout << "@ ";
				SetConsoleTextAttribute(hConsoleHandle, color);
				pc.set_i_j(i, j);
			}
			else if (cell == 9) {//монстр
				SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED);
				cout << "M ";
				SetConsoleTextAttribute(hConsoleHandle, color);
				current = new Node<Monster>;
				if (monsters.tail == nullptr)
					monsters.head = monsters.tail = current;
				else {
					monsters.tail->next = current;
					monsters.tail = current;
				}
				monsters.N++;
				current->info.set_i_j(i, j);
			}
		}
		cout << endl;
	}
	SetConsoleTextAttribute(hConsoleHandle, 15);
	cout << "Score: " << score << endl;
}

void character_update(Character* ch, int** map) {//указатель на родителя принимает указатель на любого наследника
	ch->move(map);//виртуальная функция
}

void setcur(int x, int y) {//наведение курсора в консоли
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void console_update(Pacman pc, bool& win, int& score, sf::Sound& coinS, int mHeight) {
	if (!(pc.lastJ == pc.j and pc.lastI == pc.i)) {
		setcur(pc.j * 2, pc.i);
		SetConsoleTextAttribute(hConsoleHandle, 14);
		cout << "@ ";
		setcur(pc.lastJ * 2, pc.lastI);
		cout << "  ";
	}
	if (pc.lastPoint == 7) {
		score += 10;
		SetConsoleTextAttribute(hConsoleHandle, 15);
		setcur(7, mHeight);
		cout << score;
		coinS.play();
	}
	if (pc.lastPoint == 6)
		win = true;
}

void console_update(Monster ms, int** map) {
	setcur(ms.j * 2, ms.i);
	SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED);
	cout << "M ";
	if (!(ms.i == ms.lastI and ms.j == ms.lastJ)) {
		setcur(ms.lastJ * 2, ms.lastI);
		if (map[ms.lastI][ms.lastJ] == 7) {
			SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_GREEN | FOREGROUND_RED);
			cout << "® ";
		}
		else
			cout << "  ";
	}
}

int char_to_int(char* str) {
	int value = 0;
	bool minus = false;
	char* sptr = str;
	if (*sptr == '-') {
		minus = true;
		sptr++;
	}
	while (*sptr >= 48 and *sptr <= 57) {
		value *= 10;
		value += *sptr - 48;
		sptr++;
	}
	if (minus)
		value = -value;

	return value;
}

struct Node_result {
	char name[50] = "Anonymous";
	int score = 0;
	Node_result* next = nullptr;
};

struct List_results {
	Node_result* head = nullptr;
	Node_result* tail = nullptr;
	int N = 0;
};

void load_highscores(List_results& highscores) {
	fstream file;
	if (highscores.head == nullptr) {
		file.open("highscores.txt", ios_base::in);
		if (!file) {
			file.open("highscores.txt", ios_base::out);
			file.close();
			file.open("highscores.txt", ios_base::in);
		}
		char line[50];
		Node_result* nNode;

		while (file >> line) {
			nNode = new Node_result;
			strncpy(nNode->name, line, sizeof(nNode->name) - 1);
			if (!(file >> line)) {
				cout << "Corrupted file\n";
				delete nNode;
				break;
			}
			nNode->score = char_to_int(line);

			if (highscores.head == nullptr)
				highscores.head = highscores.tail = nNode;
			else {
				highscores.tail->next = nNode;
				highscores.tail = nNode;
			}
			highscores.N++;
		}
		file.close();
	}
}

bool add_to_list(List_results& l, int score, char* name) {
	l.N++;
	Node_result* current = l.head, *nNode, *temp;
	nNode = new Node_result;
	if (*name != '\0')
		strncpy(nNode->name, name, sizeof(nNode->name) - 1);
	nNode->score = score;

	if (current != nullptr) {
		if (current->score <= score) {
			temp = l.head;
			l.head = nNode;
			nNode->next = temp;
			return 1;
		}

		while (current->next != nullptr and current->next->score > score)
			current = current->next;

		if (current->next == nullptr) {
			current->next = nNode;
			l.tail = nNode;
			return 1;
		}

		temp = current->next;
		current->next = nNode;
		nNode->next = temp;
	}
	else
		l.head = l.tail = nNode;
	return 1;
}

bool save_result(List_results& highscores, int score) {
	if (score < 500) {
		cin.get();
		return 0;
	}

	load_highscores(highscores);

	if (highscores.N < 20 or highscores.tail->score < score) {
		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 20;
		cci.bVisible = TRUE;
		SetConsoleCursorInfo(hConsoleHandle, &cci);
		char name[50];
		cout << "Enter your name to save the result: ";
		cin.getline(name, 50, '\n');
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		cci.dwSize = 1;
		cci.bVisible = FALSE;
		SetConsoleCursorInfo(hConsoleHandle, &cci);
		add_to_list(highscores, score, name);

		fstream file;
		file.open("highscores.txt", ios_base::out);
		Node_result* current = highscores.head;
		int count = 0;
		while (current != nullptr and count < 20) {
			file << current->name << endl;
			file << current->score << endl;
			current = current->next;
			count++;
		}
		file.close();
		return 1;
	}
	else {
		cin.get();
		return 0;
	}
}

void show_records(List_results& highscores) {
	load_highscores(highscores);

	Node_result* current = highscores.head;
	int count = 1;
	while (current != nullptr and count <= 20) {
		cout << " " << count << "    ";
		if (count < 10)
			cout << " ";

		cout << current->name;
		for (int i = 0; i < 50 - strlen(current->name); i++) {
			cout << " ";
		}
		cout << current->score << "\n\n";

		count++;
		current = current->next;
	}
	cout << endl << endl;
	cin.get();
}

int main()
{
	srand(time(0));
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;//Убираю курсор
	SetConsoleCursorInfo(hConsoleHandle, &cci);
	int door = 1, choice, score, clock;
	int **map = nullptr, mapH = 0, mapW = 0;
	Pacman pc;
	List<Monster> monsters;
	Node<Monster>* current;
	List_results highscores;

	sf::SoundBuffer bufferC, bufferW, bufferL;
	sf::Sound coinS, winS, loseS;

	door += bufferC.loadFromFile("sounds\\coin1.wav");
	door += bufferW.loadFromFile("sounds\\win.wav");
	door += bufferL.loadFromFile("sounds\\lose.wav");
	if (door < 3) {
		cout << "\nNot all files uploaded\n";
		cin.get();
	}
	coinS.setBuffer(bufferC);
	coinS.setVolume(60);//0-100
	winS.setBuffer(bufferW);
	loseS.setBuffer(bufferL);

Menu:
	door = 1;
	choice = 0;
	score = 0;
	system("cls");
	do {
		if (door > 4)
			door = 1;
		else if (door < 1)
			door = 4;

		setcur(0, 0);
		if (door == 1)
			SetConsoleTextAttribute(hConsoleHandle, 15);
		cout << " _____  _____   ___  ______  _____   _____   ___  ___  ___ _____ \n";
		cout << "/  ___||_   _| / _ \\ | ___ \\|_   _| |  __ \\ / _ \\ |  \\/  ||  ___|\n";
		cout << "\\ `--.   | |  / /_\\ \\| |_/ /  | |   | |  \\// /_\\ \\| .  . || |__  \n";
		cout << " `--. \\  | |  |  _  ||    /   | |   | | __ |  _  || |\\/| ||  __| \n";
		cout << "/\\__/ /  | |  | | | || |\\ \\   | |   | |_\\ \\| | | || |  | || |___ \n";
		cout << "\\____/   \\_/  \\_| |_/\\_|\\_|   \\_/    \\____/\\_| |_/\\_|  |_/\\____/\n\n";
		if (door == 1)
			SetConsoleTextAttribute(hConsoleHandle, 8);
		if (door == 2)
			SetConsoleTextAttribute(hConsoleHandle, 15);
		cout << " _____  _   _ ______  _      _____  _____  _____   _____   ___  ___  ___ _____ \n";
		cout << "|  ___|| \\ | ||  _  \\| |    |  ___|/  ___|/  ___| |  __ \\ / _ \\ |  \\/  ||  ___|\n";
		cout << "| |__  |  \\| || | | || |    | |__  \\ `--. \\ `--.  | |  \\// /_\\ \\| .  . || |__  \n";
		cout << "|  __| | . ` || | | || |    |  __|  `--. \\ `--. \\ | | __ |  _  || |\\/| ||  __|\n";
		cout << "| |___ | |\\  || |/ / | |____| |___ /\\__/ //\\__/ / | |_\\ \\| | | || |  | || |___ \n";
		cout << "\\____/ \\_| \\_/|___/  \\_____/\\____/ \\____/ \\____/   \\____/\\_| |_/\\_|  |_/\\____/ \n\n";
		if (door == 2)
			SetConsoleTextAttribute(hConsoleHandle, 8);
		if (door == 3)
			SetConsoleTextAttribute(hConsoleHandle, 15);
		cout << " _   _  _____  _____  _   _   _____  _____  _____ ______  _____  _____ \n";
		cout << "| | | ||_   _||  __ \\| | | | /  ___|/  __ \\|  _  || ___ \\|  ___|/  ___|\n";
		cout << "| |_| |  | |  | |  \\/| |_| | \\ `--. | /  \\/| | | || |_/ /| |__  \\ `--. \n";
		cout << "|  _  |  | |  | | __ |  _  |  `--. \\| |    | | | ||    / |  __|  `--. \\\n";
		cout << "| | | | _| |_ | |_\\ \\| | | | /\\__/ /| \\__/\\\\ \\_/ /| |\\ \\ | |___ /\\__/ /\n";
		cout << "\\_| |_/ \\___/  \\____/\\_| |_/ \\____/  \\____/ \\___/ \\_| \\_|\\____/ \\____/\n\n";
		if (door == 3)
			SetConsoleTextAttribute(hConsoleHandle, 8);
		if (door == 4)
			SetConsoleTextAttribute(hConsoleHandle, 15);
		cout << " _____ __   __ _____  _____   _____   ___  ___  ___ _____\n";
		cout << "|  ___|\\ \\ / /|_   _||_   _| |  __ \\ / _ \\ |  \\/  ||  ___|\n";
		cout << "| |__   \\ V /   | |    | |   | |  \\// /_\\ \\| .  . || |__ \n";
		cout << "|  __|  /   \\   | |    | |   | | __ |  _  || |\\/| ||  __| \n";
		cout << "| |___ / /^\\ \\ _| |_   | |   | |_\\ \\| | | || |  | || |___ \n";
		cout << "\\____/ \\/   \\/ \\___/   \\_/    \\____/\\_| |_/\\_|  |_/\\____/\n";
		if (door == 4)
			SetConsoleTextAttribute(hConsoleHandle, 8);
		cout << "\n                                                     Use english layout\n";

		choice = _getch();
		if (choice == 'w')
			door--;
		else if (choice != VK_RETURN)
			door++;

	} while (choice != VK_RETURN);

	SetConsoleTextAttribute(hConsoleHandle, 15);

	if (door == 1) {
		for (int i = 0; i < 5; i++) {
			char cardName[10] = { 'l', 'v', 'l', (unsigned char)(i + 49), '.', 't', 'x', 't', '\0' };
			if (!card_loading(map, mapH, mapW, cardName))
				break;
			map_console_output(map, mapH, mapW, score, pc, monsters);

			clock = 0;
			bool win = 0;
			while (!win) {
				current = monsters.head;
				int count = 0;
				while (current != nullptr and count <= clock / (15 + 15 * count)) {
					character_update(&current->info, map);
					console_update(current->info, map);
					if (current->info.j == pc.j and current->info.i == pc.i) {
						system("cls");
						SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED);
						cout << " _____   ___  ___  ___ _____   _____  _   _  _____ ______ \n";
						cout << "|  __ \\ / _ \\ |  \\/  ||  ___| |  _  || | | ||  ___|| ___ \\\n";
						cout << "| |  \\// /_\\ \\| .  . || |__   | | | || | | || |__  | |_/ /\n";
						cout << "| | __ |  _  || |\\/| ||  __|  | | | || | | ||  __| |    / \n";
						cout << "| |_\\ \\| | | || |  | || |___  \\ \\_/ /\\ \\_/ /| |___ | |\\ \\ \n";
						cout << " \\____/\\_| |_/\\_|  |_/\\____/   \\___/  \\___/ \\____/ \\_| \\_|\n";
						cout << "\n                       SCORE: " << score << "\n\n";
						SetConsoleTextAttribute(hConsoleHandle, 15);
						loseS.play();
						save_result(highscores, score);
						goto Menu;
					}
					count++;
					current = current->next;
				}
				character_update(&pc, map);
				console_update(pc, win, score, coinS, mapH);
				Sleep(150);
				clock++;
			}
		}
		system("cls");
		SetConsoleTextAttribute(hConsoleHandle, 2);
		cout << " __   __ _____  _   _   _    _  _____  _   _\n";
		cout << " \\ \\ / /|  _  || | | | | |  | ||_   _|| \\ | |\n";
		cout << "  \\ V / | | | || | | | | |  | |  | |  |  \\| |\n";
		cout << "   \\ /  | | | || | | | | |/\\| |  | |  | . ` |\n";
		cout << "   | |  \\ \\_/ /| |_| | \\  /\\  / _| |_ | |\\  |\n";
		cout << "   \\_/   \\___/  \\___/   \\/  \\/  \\___/ \\_| \\_/\n";
		cout << "\n                SCORE: " << score << "\n\n";
		SetConsoleTextAttribute(hConsoleHandle, 15);
		winS.play();
		save_result(highscores, score);
		goto Menu;
	}

	if (door == 2) {
		while (1) {
			map_generation(map, mapW, mapH);
			map_console_output(map, mapH, mapW, score, pc, monsters);
			bool win = false;
			while (!win) {
				current = monsters.head;
				while (current != nullptr) {
					character_update(&current->info, map);
					console_update(current->info, map);
					if (current->info.j == pc.j and current->info.i == pc.i) {
						system("cls");
						SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED);
						cout << " _____   ___  ___  ___ _____   _____  _   _  _____ ______ \n";
						cout << "|  __ \\ / _ \\ |  \\/  ||  ___| |  _  || | | ||  ___|| ___ \\\n";
						cout << "| |  \\// /_\\ \\| .  . || |__   | | | || | | || |__  | |_/ /\n";
						cout << "| | __ |  _  || |\\/| ||  __|  | | | || | | ||  __| |    / \n";
						cout << "| |_\\ \\| | | || |  | || |___  \\ \\_/ /\\ \\_/ /| |___ | |\\ \\ \n";
						cout << " \\____/\\_| |_/\\_|  |_/\\____/   \\___/  \\___/ \\____/ \\_| \\_|\n";
						cout << "\n                       SCORE: " << score << "\n\n";
						SetConsoleTextAttribute(hConsoleHandle, 15);
						loseS.play();
						save_result(highscores, score);
						goto Menu;
					}
					current = current->next;
				}
				character_update(&pc, map);
				console_update(pc, win, score, coinS, mapH);
				Sleep(150);
			}
		}
	}

	if (door == 3) {
		system("cls");
		show_records(highscores);
		goto Menu;
	}
}

//void list_bubbleSort(List l)
//{
//	int tempi;
//	char tempc[50];
//	Node* current;
//	bool b;
//	for (int i = 0; i < l.N - 1; i++) {
//		current = l.head;
//		b = 0;
//		for (int j = 0; j < l.N - i - 1; j++) {
//			if (current->score < current->next->score) {
//				tempi = current->score;
//				current->score = current->next->score;
//				current->next->score = tempi;
//
//				strncpy(tempc, current->name, sizeof(tempc) - 1);
//				strncpy(current->name, current->next->name, sizeof(current->name) - 1);
//				strncpy(current->next->name, tempc, sizeof(current->next->name) - 1);
//				b = 1;
//			}
//			current = current->next;
//		}
//		if (b)
//			break;
//	}
//}