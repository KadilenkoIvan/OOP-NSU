#include <iostream>
#include <matplot/matplot.h>
#include <thread>

void graph1(const double& b, const double& c, const double& x1) {
    using namespace matplot;

    fplot([&b, &c](double x) { return b * x + c; }, std::array<double, 2>{x1 - abs(b) - abs(c), x1 + abs(b) + abs(c)}, "b");

    grid(on);

    show();
}

void graph2(const double& a, const double& b, const double& c, const double& x1, const double& x2) {
    using namespace matplot;
    if (x1 < x2) {
        fplot([a, b, c](double x) { return a * x * x + b * x + c; }, std::array<double, 2>{x1 - abs(x1) - abs(x2), x2 + abs(x1) + abs(x2)}, "b");
    }
    else if (x1 > x2) {
        fplot([a, b, c](double x) { return a * x * x + b * x + c; }, std::array<double, 2>{x2 - abs(x1) - abs(x2), x1 + abs(x1) + abs(x2)}, "b");
    }
    else {
        fplot([a, b, c](double x) { return a * x * x + b * x + c; }, std::array<double, 2>{x1 - abs(a), x1 + abs(a)}, "b");
    }

    grid(on);

    show();
}


int main()
{
    std::cout << "Enter a, b, c\n";
    double a = 0, b = 0, c = 0;
    std::cin >> a >> b >> c;

    double x1 = 0, x2 = 0;

    if (a == 0 && b == 0)
    {
        std::cout << "no soutions";
        return 0;
    }

    if (a == 0)
    {
        x1 = -c / b;
        std::cout << "x = " << x1 << "\n";
        graph1(b, c, x1);
        return 0;
    }

    double D = (b * b) - (4 * a * c);
    if (D > 0)
    {
        x1 = (-b - sqrt(D)) / (2 * a);
        x2 = (-b + sqrt(D)) / (2 * a);
        std::cout << "x1 = " << x1 << "\nx2 = " << x2 << "\n";
    }
    else if (D == 0)
    {
        x1 = x2 = -b / (2 * a);
        std::cout << "x = " << x1 << "\n";
    }
    else
    {
        std::cout << "no solutions";
        return 0;
    }
    graph2(a, b, c, x1, x2);
    return 0;
}
