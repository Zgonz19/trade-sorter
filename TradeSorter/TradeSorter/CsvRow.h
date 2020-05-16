#pragma once

#include <string>

class CsvRow
{
	public:
		long long TimeStamp = 0;
		std::string Symbol;
		int Quantity = 0;
		int Price = 0;
};

