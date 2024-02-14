#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <map>
#include <vector>
#include <algorithm>

bool cmp(std::pair<std::string, int>&a, std::pair<std::string, int>&b)
{
	return a.second > b.second;
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Wrong number of input parameters\n";
		exit(0);
	}

	//std::cout << input_file << " " << output_file;

	std::ifstream in;
	std::string input_file = argv[1];
	in.open(input_file);
	if (!in)
	{
		std::cout << "file doesn't exist";
		exit(0);
	}

	std::ofstream out;
	std::string output_file = argv[2];
	int out_len = output_file.length();
	if (output_file[out_len - 1] != 'v' || output_file[out_len - 2] != 's' || output_file[out_len - 3] != 'c' || output_file[out_len - 4] != '.')
	{
		std::cout << "wrong format for output file";
		exit(0);
	}
	out.open(output_file);

	/*std::ifstream in;
	std::ofstream out;

	in.open("war and peace.txt");
	out.open("war and peace.csv");*/

	std::list<std::string> text;
	std::string line;
	while (getline(in, line))
	{
		text.push_back(line);
	}

	std::map<std::string, long long> words;
	std::string word = "";
	int count_of_words = 0;
	for (std::string line : text)
	{
		for (char symbol : line)
		{
			if (symbol >= 0 && symbol <= 255)
			{
				if (!isalpha(symbol) && !isdigit(symbol))
				{
					if (word != "")
					{
						words[word]++;
						word = "";
						count_of_words++;
					}
					continue;
				}
				if (isupper(symbol))
					symbol = tolower(symbol);
				word += symbol;
			}
		}
		if (word != "")
		{
			words[word]++;
			word = "";
			count_of_words++;
		}
	}

	std::vector <std::pair<std::string, int>> words_vector;
	for (auto pair : words)
	{
		words_vector.push_back(pair);
	}

	std::sort(words_vector.begin(), words_vector.end(), 
[](std::pair<std::string, int> a, std::pair<std::string, int> b) {
		return (a.second > b.second);
	});

	for (auto elem : words_vector)
	{
		out << "\"" << elem.first + "\";" << elem.second << ";" << elem.second * 100 / count_of_words << "\n";
	}
}