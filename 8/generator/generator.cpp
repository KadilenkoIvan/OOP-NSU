#include <iostream>
#include <fstream>
#include <random>
int main()
{
	std::ofstream out("file.txt");
	if (out.is_open())
	{
		out << 100000 << '\n';
		for (int i = 0; i < 100000; ++i)
		{
			out << rand() % 100000 << ' ' << rand() % 100000 << '\n';
		}
	}
}