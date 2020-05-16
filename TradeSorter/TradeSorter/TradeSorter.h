#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "CsvFile.h"

class TradeSorter
{
public:
	std::string InputFolder = "C:\\Users\\Gonzalo\\Dropbox\\QuantLab\\TradeSorterApp\\TradeSorter2\\Input";
	std::string OutputFolder = "C:\\Users\\Gonzalo\\Dropbox\\QuantLab\\TradeSorterApp\\TradeSorter2\\Output";
	std::list<CsvFile> ProcessedFiles;

public:
	void GenerateOutput();
	void ProcessInput();
	void ExecuteSorting(std::string csvInfo);
};

