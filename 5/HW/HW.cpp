#include <tuple>
#include <utility>
#include <string>
#include <iostream>

int main() {
    std::tuple<int, std::string, bool> t(7, "C++", true);
    auto tt = std::make_tuple(7, "C++", true);
    std::cout << std::get<1>(t) << '\n'; //обращение к элементу tuple
    std::tuple tp(7, "C++", true); // c C++ 17 можно
    std::pair<int, std::string> p(7, "C++");
    std::pair pp(7, "C++");
    std::cout << p.first << " " << p.second << std::endl;
    std::cout << pp.first << " " << pp.second << std::endl;

    return 0;
}
