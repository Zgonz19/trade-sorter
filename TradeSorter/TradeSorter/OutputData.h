#pragma once
#include <list>
#include "CsvFile.h"

class OutputData
{
	public:
		OutputData(std::list<CsvFile> processedFiles, std::string outputFolder) : ProcessedFiles(processedFiles), OutputFolder(outputFolder){
		}

		std::list<CsvFile> ProcessedFiles;
		std::string OutputFolder;
		std::map<std::string, PageDefinitions> Definitions = std::map<std::string, PageDefinitions>();

	public:
		void FilterDefinitions();
		void GenerateOutputFile();
		std::string GenerateRow(std::string symbol, PageDefinitions definitions);
		void FindTimeGaps();
		void MaxTimeGapTask(std::string symbol, PageDefinitions definition);
		long long FindMaxTimeGap(std::string symbol, PageDefinitions definition);
};

