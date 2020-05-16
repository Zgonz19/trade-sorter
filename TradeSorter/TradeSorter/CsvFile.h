#pragma once
#include <map>
#include <filesystem>
#include "PageDefinitions.h"
#include <unordered_map>
#include <iostream>
#include "CsvRow.h"

class CsvFile
{
	public:
		CsvFile(std::string csvInfo ) : CsvInfo(csvInfo){

		}	
	private:
		const std::string CsvInfo;
		std::list<std::unordered_map<std::string, PageDefinitions>> PageDefinition;
	public:
		int RowsPerTask = 3500;
		std::unordered_map<std::string, PageDefinitions> SymbolDefinitions;

		void ProcessRow(CsvRow row, std::unordered_map<std::string, PageDefinitions>& pages);
		void ProcessPage(std::list<CsvRow> rows);
		void ProcessData(std::string& filedir);
		void GroupPages();
		void ProcessPages(std::unordered_map<std::string, PageDefinitions> pages);
};

