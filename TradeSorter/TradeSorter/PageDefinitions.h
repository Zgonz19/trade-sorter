#pragma once

#include <string>
#include <list>
#include <set>

class PageDefinitions
{
    public:
        int Numerator = 0;
        std::string Symbol;
        int Volume = 0;
        int MaxPrice = 0;
        long MaxTimeGap = 0;
        std::set<long long> RunningTimeStamps = std::set<long long>();
};

