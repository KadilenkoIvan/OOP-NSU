#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <numeric>
#include <algorithm>

#define DAY 86400

std::time_t from_str_to_time_t(std::string date)
{
    std::string D, M, Y;
    std::stringstream in(date);
    std::getline(in, Y, '-');
    std::getline(in, M, '-');
    std::getline(in, D, '-');
    int day_from = stoi(D), mounth_from = stoi(M), year_from = stoi(Y);
    std::tm a = { 0,0,0,day_from,mounth_from - 1,year_from - 1900 };
    return (std::mktime(&a));
}

double ComputeIncome(std::vector<double> dates, std::string from, std::string to)
{
    std::time_t date1 = from_str_to_time_t(from);
    std::time_t date2 = from_str_to_time_t(to);
    //std::cout << "------------" << ctime(&date1);
    //std::cout << "------------" << ctime(&date2);
    if (date1 > date2)
    {
        std::cout << "Wrong dates\n";
        return -1;
    }
    int ind1 = std::difftime(date1, from_str_to_time_t("2000-01-01")) / DAY;
    int ind2 = std::difftime(date2, from_str_to_time_t("2000-01-01")) / DAY;
    if (ind1 >= size(dates) || ind2 >= size(dates))
    {
        std::cout << "Dates out of range\n";
        return -1;
    }
    double res = std::accumulate(dates.begin() + ind1, dates.begin() + ind2 + 1, 0);
    return res;
}

void Earn(std::vector <double>& dates, std::string from, std::string to, double how_much)
{
    std::time_t date1 = from_str_to_time_t(from);
    std::time_t date2 = from_str_to_time_t(to);

    if (date1 > date2)
    {
        std::cout << "Wrong dates\n";
        return;
    }
    int ind1 = std::difftime(date1, from_str_to_time_t("2000-01-01")) / DAY;
    int ind2 = std::difftime(date2, from_str_to_time_t("2000-01-01")) / DAY;
    if (ind1 >= size(dates) || ind2 >= size(dates))
    {
        std::cout << "Dates out of range\n";
        return;
    }
    int dif = std::difftime(date2, date1) / DAY + 1;
    double how_much_in_day = how_much / dif;
    //std::generate(dates.begin() + ind1, dates.begin() + ind2 + 1, [&]() {return how_much_in_day; }); Не увеличивает, а заменяет
    for (int i = ind1; i <= ind2; ++i)
    {
        dates[i] += how_much_in_day;
    }
}

int main()
{
    std::cout.precision(25);

    std::vector<double> dates(36600);
    int Q = 0;
    std::cin >> Q;
    std::string command = "", from = "", to = "";
    double how_many = 0;
    for (int i = 0; i < Q; ++i)
    {
        std::cin >> command;
        if (command == "Earn")
        {
            std::cin >> from >> to >> how_many;
            Earn(dates, from, to, how_many);
        }
        else if (command == "ComputeIncome")
        {
            std::cin >> from >> to;
            std::cout << ComputeIncome(dates, from, to) << '\n';
        }
        else
        {
            std::cout << "Wrong command\n";
        }
    }
    return 0;
}