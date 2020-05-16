#include "TradeSorter.h"
#include <future>
#include <thread>
#include <algorithm>
#include "CsvFile.h"
#include "OutputData.h"

using namespace std;

void TradeSorter::GenerateOutput()
{
    auto outputData = OutputData(ProcessedFiles, OutputFolder);
    outputData.GenerateOutputFile();
}

void TradeSorter::ProcessInput()
{
    std::string holding; 
    std::list<std::string> filepaths = std::list<std::string>();
    std::list<future<void>> asyncTasks = list<future<void>>();
    for (const filesystem::directory_entry dirEntry : filesystem::directory_iterator(InputFolder)) {
        auto stringpath = std::string(dirEntry.path().u8string());
        filepaths.push_back(stringpath);
    }

    for (auto path : filepaths) {
        asyncTasks.push_back(std::async(launch::async, &TradeSorter::ExecuteSorting, this, path));
    }

    for (auto& task : asyncTasks) {
        task.get();
    }
    GenerateOutput();
}
void TradeSorter::ExecuteSorting(std::string csvInfo)
{
    auto csvFile = CsvFile(csvInfo);
    csvFile.ProcessData(csvInfo);
    ProcessedFiles.push_back(csvFile);
}

