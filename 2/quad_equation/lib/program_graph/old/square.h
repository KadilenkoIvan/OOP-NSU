
#pragma once
#include <math.h>
#include <iostream>
void func2(const double &a, const double &b,const double &c, double &x1,  double &x2)
{
    double d = b * b - (4 * a * c);
    if (d > 0) {
        x1 = (-b + sqrt(d)) / (2 * a);
        x2 = (-b - sqrt(d)) / (2 * a);
        std::cout << "Answer: " << std::endl;
        std::cout << "   x1 = " << x1 << std::endl;
        std::cout << "   x2 = " << x2 << std::endl;
    } else if (d == 0) {
        x1 = (b * -1 / (2 * a));
        x2 = (b * -1 / (2 * a));
        std::cout << "Answer: " << std::endl;
        std::cout << "   x = " << x1 << std::endl;
    } else {
        std::cout << "Answer: " << std::endl;
        std::cout << "   no answer:";
    }
}
void func1(const double &b,const double &c, double &x1)
{
    x1 = (-c/b);
    std::cout << "Answer: " << std::endl;
    std::cout << "   x = " << x1 << std::endl;
}