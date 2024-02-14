#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <list>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstring>
#include <thread>
#include <conio.h>

class Board
{
private:
	std::string screen_frame;
	std::string rules;
	int forbirth[9] = { 0 };
	int forsurvival[9] = { 0 };
	int width;
	int height;
	char** field;

public:
	Board(int widthn = 50, int heightn = 20, std::string rules_inp = "#R B3/S23") : rules{ rules_inp }, width{ abs(widthn) }, height{ abs(heightn) } {
		get_rules_from_str(rules);
		field = new char* [width];
		for (int i = 0; i < width; i++) {
			field[i] = new char[height];
		}
		screen_frame = create_screen_border();
	}
	~Board() {
		if (field != nullptr) {
			for (int i = 0; i < width; i++) {
				delete[] field[i];
			}
			delete[] field;
		}
	}
	void get_rules_from_str(std::string rul) {
		int i = 4;
		while (rul[i] != '/') {
			forbirth[rul[i] - '0'] = 1;
			i++;
		}
		i += 2;
		while (rul[i] != '\0') {
			forsurvival[rul[i] - '0'] = 1;
			i++;
		}
	}

	void set_field_with_glider() {
		for (int i = 0; i < width; i++) {
			memset(field[i], 0, height);
		}
		field[5][7] = 1;
		field[6][5] = 1;
		field[6][7] = 1;
		field[7][6] = 1;
		field[7][7] = 1;
	}

	void set_field(std::list<std::pair<int, int>> life_list) {
		for (int i = 0; i < width; i++) {
			memset(field[i], 0, height);
		}
		for (auto& elem : life_list) {
			field[elem.first][elem.second] = 1;
		}
	}

	void print_field() {
		char life = '+';
		char dead = ' ';
		std::cout << screen_frame << '\n';
		for (int i = 0; i < height; i++) {
			std::cout << "|";
			for (int j = 0; j < width; j++) {
				if (field[j][i]) {
					std::cout << life;
				}
				else {
					std::cout << dead;
				}
			}
			std::cout << "|\n";
		}
		std::cout << screen_frame << '\n';
	}
	int number_of_neighbors(int i, int j, Board* prev_frame) {
		int count = 0;
		count += prev_frame->field[get_x(i)][get_y(j - 1)];
		count += prev_frame->field[get_x(i)][get_y(j + 1)];
		count += prev_frame->field[get_x(i - 1)][get_y(j)];
		count += prev_frame->field[get_x(i - 1)][get_y(j - 1)];
		count += prev_frame->field[get_x(i - 1)][get_y(j + 1)];
		count += prev_frame->field[get_x(i + 1)][get_y(j)];
		count += prev_frame->field[get_x(i + 1)][get_y(j - 1)];
		count += prev_frame->field[get_x(i + 1)][get_y(j + 1)];
		return count;
	}
	void step() {
		Board prev_frame(width, height, rules);
		for (int i = 0; i < width; i++)
			memcpy(prev_frame.field[i], field[i], height);
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				int count_of_neib = number_of_neighbors(i, j, &prev_frame);
				for (int l = 0; l < 9; l++) {
					if (forbirth[l] == 1 && count_of_neib == l){
						field[i][j] = 1;
						break;
					}
				}
				for (int l = 0; l < 9; l++) {
					if (prev_frame.field[i][j] == 1 && forsurvival[l] == 1 && count_of_neib == l) {
						field[i][j] = 1;
						break;
					}
					else if (prev_frame.field[i][j] == 1 && forsurvival[l] == 0 && count_of_neib == l) 
						field[i][j] = 0;
				}
			}
		}
	}

	void save_universe(std::string outpath, std::string name, std::string rules) {
		std::ofstream out(outpath);
		out << "#Life 1.06" << '\n' << name << '\n' << rules << '\n' << width << " " << height;
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				if (field[i][j] == 1) {
					out << '\n' << i << " " << j;
				}
			}
		}
	}
	std::string create_screen_border() {
		std::string str = "##";
		str.insert(1, width, '=');
		return str;
	}
	int get_x(int i) {
		return (width + i) % width;
	}
	int get_y(int i) {
		return (height + i) % height;
	}
};

class ParametrSetter
{
private:
	unsigned int iterations;
	int mode;
	int width, height;
	std::string inpath;
	std::string outpath;
	std::string rules;
	std::string name;
	std::list<std::pair<int, int>> life_list;

public:
	ParametrSetter() = default;
	~ParametrSetter() = default;
	int read_argv(int argc, char* argv[]) {
		if (argc == 1) //Basic parametrs
			mode = 1;
		else if (argc == 2) { //From file
			inpath = argv[1];
			mode = 0;
		}
		else { // offline game
			inpath = argv[1];
			mode = 2;
			for (int i = 2; i < argc; ++i) {
				if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--iterations") == 0) {
					if (i + 1 < argc) {
						++i;
						iterations = std::stoi(argv[i]);
					}
					else {
						std::cout << "wrong parametrs\n";
						exit(1);
					}
				}
				if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
					if (i + 1 < argc) {
						++i;
						outpath = argv[i];
					}
					else {
						std::cout << "wrong parametrs\n";
						exit(1);
					}
				}
			}
		}
		return mode;
	}

	std::list<std::pair<int, int>> read_file() {
		std::ifstream input(inpath);
		std::string format;
		int x, y;
		std::list<std::pair<int, int>> life_list;
		if (input.is_open())
		{
			std::getline(input, format);
			if (format == "#Life 1.06") {
				std::getline(input, name);
				std::getline(input, rules);
				input >> width >> height;
				while (input >> x >> y) {
					life_list.push_back(std::make_pair(abs(x), abs(y)));
				}
			}
			else {
				std::cout << "Invalid format of universe" << std::endl;
			}
		}
		input.close();
		return life_list;
	}

	void console_play(Board* board) {
		char key = 0;
		while (1) {
			board->print_field();
			if (_kbhit()) {
				key = _getch();
				if (key == '=')
					break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			system("cls");
			board->step();
		}
	}

	void offline_game(Board* board) {
		for (unsigned int i = 0; i < iterations; i++) {
			board->step();
		}
	}

	void start() {
		if (mode == 0) {
			life_list = read_file();
			Board board(width, height, rules);
			board.set_field(life_list);
			console_play(&board);
			get_after_game_command(&board);
			return;
		} else if (mode == 1){
			name = "#N Glider";
			rules = "#R B3/S23";
			Board board;
			board.set_field_with_glider();
			console_play(&board);
			get_after_game_command(&board);
			return;
		} else if (mode == 2) {
			life_list = read_file();
			Board board(width, height, rules);
			board.set_field(life_list);
			offline_game(&board);
			board.save_universe(outpath, name, rules);
			return;
		}
	}

	void get_after_game_command(Board* board) {
		bool flag = false;
		while (!flag) {
			std::cout << "Input your comand:\n";
			std::string comand;
			std::getline(std::cin, comand);
			if (comand == "help" || comand == "?") {
				std::cout << "\nList of comands:\n";
				std::cout << "help - print this info\n";
				std::cout << "dump - save universe\n";
				std::cout << "tick num_of_iter and t num_of_iter - print universe after num_of_iter iters (if no num_of_iter, 1 iter)\n";
				std::cout << "exit - mean exit\n\n";
			} else if (comand == "dump") {
				std::cout << "Output file name\n";
				std::string filename;
				std::getline(std::cin, filename);
				board->save_universe(filename, name, rules);
			} else if (comand == "t" || comand == "tick") {
				board->step();
				std::cout << name << '\n' << rules << '\n';
				board->print_field();
			} else if (comand.substr(0, 2) == "t ") {
				std::string temp = comand.substr(2);
				std::stringstream ss(temp);
				int loops;
				ss >> loops;
				for (unsigned int i = 0; i < loops; i++) {
					board->step();
				}
				std::cout << name << '\n' << rules << '\n';
				board->print_field();
			} else if (comand.substr(0, 5) == "tick ") {
				std::string temp = comand.substr(5);
				std::stringstream ss(temp);
				int loops;
				ss >> loops;
				for (unsigned int i = 0; i < loops; i++) {
					board->step();
				}
				std::cout << name << '\n' << rules << '\n';
				board->print_field();
			} else if (comand == "exit") {
				flag = true;
			}
			else {
				std::cout << "Uncnown comand. Write \"help\"\n";
			}
		}
	}
};

int main(int argc, char* argv[]) {
	ParametrSetter law;
	int mode = law.read_argv(argc, argv);
	law.start();
	return 0;
}