#include "square.h"
#include "graphic.h"

int main() {
    
    double a,b,c,d,x1,x2; // Объявляем переменные с плавающей точкой.
    std::cout << "Enter a\n"; // Вводим исходные данные
    std::cin >> a;
    std::cout << "Enter b\n";
    std::cin >> b;
    std::cout << "Enter c\n";
    std::cin >> c;
    if (a == 0 && b == 0 && c == 0) {
        return 0;
    } else if (a == 0) {
        func1(b, c, x1);
        plot_graph1(b, c, x1);
    } else {
        func2(a, b, c, x1, x2);
        plot_graph2(a, b, c, x1, x2);
    }

  return 0;
}