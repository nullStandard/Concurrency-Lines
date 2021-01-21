#include "SecondaryFunctions.h"

void folderTree(const fs::path& pathToShow, std::vector<fs::path>& q)
{
	if (fs::exists(pathToShow) && fs::is_directory(pathToShow))
	{
		for (const auto& entry : fs::directory_iterator(pathToShow))
		{
			if (fs::is_directory(entry.status()))
			{
				folderTree(entry, q);
			}
			else if (fs::is_regular_file(entry.status()))
			{
				static const char* ext[]{ ".cpp", ".c", ".h", ".hpp" };
				auto filename = entry.path().filename();
				if (std::find(std::begin(ext), std::end(ext), filename.extension()) != std::end(ext))
				{
					q.push_back(entry.path());
				}
			}
		}
	}
}

void printStatistic(std::vector<fs::path>& files, Starter& strt, Timer& t)
{
	std::cout << "Total count of files: " << files.size() << std::endl;
	std::cout << "Total count of blank lines: " << strt.total_blank << std::endl;
	std::cout << "Total count of comment lines: " << strt.total_comm << std::endl;
	std::cout << "Total count of physical lines: " << strt.total_phys << std::endl;
	std::cout << "Time of execution: " << t.elapsed() << std::endl;
}