#include <string>
export module module2;

export namespace Module2
{
	export std::string getMyName()
	{
		std::string name = "James";
		return name;
	}
}
