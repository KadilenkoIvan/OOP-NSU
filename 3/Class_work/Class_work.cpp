/*
Разобраться со словом friend
*/

#include <iostream>
#include <string>
#include <sstream>
#include <ostream>
#include <istream>

using namespace std;

class Rational {
public:
    Rational() {}
    Rational(int numerator, int denominator) {
        if (denominator == 0) {
            stringstream ss;
            ss << "Argument must not be zero";
            throw invalid_argument(ss.str());
        }

        num = numerator;
        denom = denominator;
    }

    int Numerator() const;
    int Denominator() const;

    friend Rational operator+(const Rational& a, const Rational& b)
    {
        int new_num1 = a.num * b.denom, new_num2 = b.num * a.denom, new_denom = a.denom * b.denom;
        return Rational(new_num1 + new_num2, new_denom);
    }

    friend Rational operator*(const Rational& a, const Rational& b)
    {
        return Rational(a.num * b.num, a.denom * b.denom);
    }

    friend Rational operator/(const Rational& a, const Rational& b)
    {
        return Rational(a.num * b.denom, a.denom * b.num);
    }

    friend Rational operator-(const Rational& a, const Rational& b)
    {
        int new_num1 = a.num * b.denom, new_num2 = b.num * a.denom, new_denom = a.denom * b.denom;
        return Rational(new_num1 - new_num2, new_denom);
    }

    friend bool operator==(const Rational& a, const Rational& b)
    {
        double num1 = a.num / a.denom, num2 = b.num / b.denom;
        return (num1 == num2);
    }

    friend ostream& operator<<(ostream& out, const Rational& a)
    {
        out << a.num << '/' << a.denom << endl;
        return out;
    }

    friend istream& operator>>(istream& in, Rational& a)
    {
        int num = 0, denom = 0;
        in >> a.num;
        in.ignore(1);
        in >> a.denom;
        return in;
    }

private:
    int num, denom;
};

int main() {
    std::string input;
    getline(std::cin, input);
    std::stringstream ss(input);
    Rational r1, r2;
    char operation;
    try {
        ss >> r1;
        ss.ignore(1);
        ss >> operation;
        ss.ignore(1);
        ss >> r2;
    }
    catch (exception& ex) {
        std::cout << "Invalid argument" << std::endl;
        return 1;
    }
    Rational res;
    try {
        switch (operation) {
            case '+': {
                res = r1 + r2;
                break;
            }
            case '-': {
                res = r1 - r2;
                break;
            }
            case '*': {
                res = r1 * r2;
                break;
            }
            case '/': {
                res = r1 / r2;
                break;
            }
            default: {
                std::cout << "Invalid operation" << std::endl;
            }
        }
    }
    catch (exception& ex) {
        std::cout << "Division by zero" << std::endl;
        return 2;
    }
    cout << res;
    return 0;
}
