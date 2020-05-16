#include "CsvFile.h"
#include <sstream>
#include <future>
#include <thread>
#include <fstream>
#include <mutex>    
using namespace std;
std::mutex mtx;

void CsvFile::ProcessRow(CsvRow row, std::unordered_map<std::string, PageDefinitions>& pages)
{
	auto it = pages.find(row.Symbol);
	if (it != pages.end()) {
		auto pageDef = pages.at(row.Symbol);
		pageDef.Volume += row.Quantity;
		pageDef.Numerator += (row.Quantity * row.Price);
		if (row.Price > pageDef.MaxPrice)
		{
			pageDef.MaxPrice = row.Price;
		}
		pageDef.RunningTimeStamps.insert(row.TimeStamp);
		pages[row.Symbol] = pageDef;
	}
	else {
		auto pageDef = PageDefinitions();
		pageDef.Symbol = row.Symbol;
		pageDef.MaxPrice = row.Price;
		pageDef.Volume = row.Quantity;
		pageDef.Numerator = (row.Quantity * row.Price);
		pageDef.RunningTimeStamps = std::set<long long>();
		pageDef.RunningTimeStamps.insert(row.TimeStamp);
		pages.insert({ row.Symbol, pageDef });
	}
}

void CsvFile::ProcessPage(std::list<CsvRow> rows)
{
	auto pageDefinition = std::unordered_map<std::string, PageDefinitions>();
	for(auto row: rows) {
		ProcessRow(row, ref(pageDefinition));
	}
	PageDefinition.push_back(pageDefinition);
}

void CsvFile::ProcessData(std::string& filedir)
{
	auto filepath = CsvInfo;
	vector<future<void>> asyncTasks = vector<future<void>>();
	std::list<CsvRow> dataSet = std::list<CsvRow>();
	std::ifstream file (filepath);
	std::string line;
	while (std::getline(file, line)) {
		std::stringstream lineStream(line);
		auto csvrow = CsvRow();
		std::string cell;
		getline(lineStream, cell, ',');
		csvrow.TimeStamp = stoll(cell);
		getline(lineStream, cell, ',');
		csvrow.Symbol = cell;
		getline(lineStream, cell, ',');
		csvrow.Quantity = stoi(cell);
		getline(lineStream, cell, ',');
		csvrow.Price = stoi(cell);
		dataSet.push_back(csvrow);
		if (static_cast<int>(dataSet.size()) >= RowsPerTask) {
			asyncTasks.push_back(std::async(launch::async, &CsvFile::ProcessPage, this, dataSet));
			dataSet.clear();
		}
	}
	asyncTasks.push_back(std::async(launch::async, &CsvFile::ProcessPage, this, dataSet));
	for (auto& task : asyncTasks) {
		//task.wait();
		task.get();
	}
	GroupPages();
}

void CsvFile::GroupPages()
{
	vector<future<void>> asyncTasks = vector<future<void>>();
	for (auto pages : PageDefinition) {
		asyncTasks.push_back(std::async(launch::async, &CsvFile::ProcessPages, this, pages));
	}
	for (auto& task : asyncTasks) {
		task.wait();
		//task.get();
	}
	auto hold = SymbolDefinitions;
}

void CsvFile::ProcessPages(std::unordered_map<std::string, PageDefinitions> pages)
{
	for (auto page : pages) {
		mtx.lock();
		auto it = SymbolDefinitions.find(page.second.Symbol);
		if (it != SymbolDefinitions.end()) {

			auto symbolDef = SymbolDefinitions.at(page.second.Symbol);
			symbolDef.Volume += page.second.Volume;
			symbolDef.Numerator += page.second.Numerator;
			if (page.second.MaxPrice > symbolDef.MaxPrice)
			{
				symbolDef.MaxPrice = page.second.MaxPrice;
			}
			symbolDef.RunningTimeStamps.insert(page.second.RunningTimeStamps.begin(), page.second.RunningTimeStamps.end());
			SymbolDefinitions[page.second.Symbol] = symbolDef;
		}
		else {
			auto symbolDef = page.second;
			SymbolDefinitions.insert({ page.second.Symbol, symbolDef });
		}
		mtx.unlock();
	}
}
