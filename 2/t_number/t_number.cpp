#include <iostream>
#include <string>

/*std::string number_check(std::string t_number)
{
	return "";
}*/

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Wrong number of input parameters\n";
		exit(0);
	}

	std::string t_number = argv[1];

	//std::string t_number;
	//std::cin >> t_number;
	
	if (!((t_number[0] == '+' && t_number[1] == '7') || t_number[0] == '8'))
	{
		std::cout << "error";
		exit(0);
	}

	if (t_number[0] == '8')
		t_number.erase(0, 1);
	else
		t_number.erase(0, 2);

	bool line_check = false, bkt_open = false;
	int bkt_count = 0;

	std::string new_t_number = "+7";

	for (char num : t_number)
	{
		if (isspace(num))
			continue;

		if (num == '-')
		{
			if (line_check)
			{
				std::cout << "error";
				exit(0);
			}
			line_check = true;
		}
		else
		{
			line_check = false;
			if (num == '(')
			{
				if (bkt_count > 1)
				{
					std::cout << "error";
					exit(0);
				}
				bkt_open = true;
				bkt_count++;
				continue;
			}
			else if (num == ')')
			{
				if (!bkt_open)
				{
					std::cout << "error";
					exit(0);
				}
				bkt_open = false;
				continue;
			}
			else if (!isdigit(num))
			{
				std::cout << "error";
				exit(0);
			}
			new_t_number += num;
		}
	}
	
	if (new_t_number.size() != 12 || bkt_count > 1 || bkt_open)
		std::cout << "error";
	else
		std::cout << new_t_number;
}