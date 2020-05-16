#include "OutputData.h"
#include <sstream>
#include <future>
#include <thread>
#include <fstream>

void OutputData::FilterDefinitions()
{
	for (auto fileSet : ProcessedFiles) {
		for (auto entry : fileSet.SymbolDefinitions) {

			auto it = Definitions.find(entry.second.Symbol);
			if (it != Definitions.end()) {
				auto symbolDef = Definitions.at(entry.second.Symbol);
				symbolDef.Volume += entry.second.Volume;
				symbolDef.Numerator += entry.second.Numerator;
				if (entry.second.MaxPrice > symbolDef.MaxPrice)
				{
					symbolDef.MaxPrice = entry.second.MaxPrice;
				}
				symbolDef.RunningTimeStamps.insert(entry.second.RunningTimeStamps.begin(), entry.second.RunningTimeStamps.end());
				Definitions[entry.second.Symbol] = symbolDef;
			}
			else {
				auto symbolDef = entry.second;
				Definitions.insert({ entry.second.Symbol, symbolDef });
			}
		}
	}
}

void OutputData::GenerateOutputFile()
{
	FilterDefinitions();
	FindTimeGaps();
	auto rowsToWrite = Definitions;
	std::ofstream outfile(OutputFolder + "\\output.csv");
	for (auto pair : rowsToWrite) {
		outfile << GenerateRow(pair.second.Symbol, pair.second);
	}
}

std::string OutputData::GenerateRow(std::string symbol, PageDefinitions definitions)
{
	auto rowstring = symbol + "," + std::to_string(definitions.MaxTimeGap) +
		"," + std::to_string(definitions.Volume) +
		"," + std::to_string(definitions.Numerator / definitions.Volume) +
		"," + std::to_string(definitions.MaxPrice) + "\n";
	return rowstring;
}

void OutputData::FindTimeGaps()
{
	std::vector<std::future<void>> asyncTasks = std::vector<std::future<void>>();
	for (auto pair : Definitions) {
		asyncTasks.push_back(std::async(std::launch::async, &OutputData::MaxTimeGapTask, this, pair.second.Symbol, pair.second));
	}

	for (auto& tasks : asyncTasks) {
		tasks.wait();
	}
}

void OutputData::MaxTimeGapTask(std::string symbol, PageDefinitions definition)
{
	Definitions[symbol].MaxTimeGap = FindMaxTimeGap(symbol, definition);
}

long long OutputData::FindMaxTimeGap(std::string symbol, PageDefinitions definition)
{
	long long currentStamp = (*definition.RunningTimeStamps.begin());
	long long MaxTimeGap = 0;
	for (auto row : definition.RunningTimeStamps) {
		if ((row - currentStamp) > MaxTimeGap) {
			MaxTimeGap = row - currentStamp;
		}
		currentStamp = row;
	}
	return MaxTimeGap;
}
