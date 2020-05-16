#include "pch.h"
#include "OutputDataTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TradeSorterTest
{
	TEST_CLASS(OutputDataTest)
	{
	public:

		TEST_METHOD(GenerateRowTest)
		{
			auto pagedef = PageDefinitions();
			pagedef.MaxPrice = 1000;
			pagedef.MaxTimeGap = 2354734578;
			pagedef.Numerator = 30000;
			pagedef.Symbol = "aaa";
			pagedef.Volume = 3984;
			pagedef.RunningTimeStamps = std::set<long long>{ 42352345, 2345345, 2432356, 67543234, 567456352, 563534522 };
			std::list<CsvFile> processedFiles = std::list<CsvFile>();
			std::string outputFolder = std::string();
			auto outputData = OutputData(processedFiles, outputFolder);
			auto testString = pagedef.Symbol + "," + std::to_string(pagedef.MaxTimeGap) +
				"," + std::to_string(pagedef.Volume) +
				"," + std::to_string(pagedef.Numerator / pagedef.Volume) +
				"," + std::to_string(pagedef.MaxPrice) + "\n";
			auto rowString = outputData.GenerateRow(pagedef.Symbol, pagedef);
			Assert::AreEqual(testString, rowString);
		}
	};
}