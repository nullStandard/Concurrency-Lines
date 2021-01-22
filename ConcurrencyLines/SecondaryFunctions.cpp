#include "SecondaryFunctions.h"


void folderTree(const fs::path& path, std::vector<fs::path>& q)
{
	//checking if path is really excists and it is a directory
	if (fs::exists(path) && fs::is_directory(path))
	{
		//iterating through directory and analysing files
		for (const auto& entry : fs::directory_iterator(path))
		{
			//if file in dir is dir
			if (fs::is_directory(entry.status()))
			{
				folderTree(entry, q);
			}
			else if (fs::is_regular_file(entry.status())) //else if iterator is ordinary file
			{
				static const char* ext[]{ ".cpp", ".c", ".h", ".hpp" };
				auto filename = entry.path().filename();

				//checking, if file is appropriates to our conditions
				if (std::find(std::begin(ext), std::end(ext), filename.extension()) != std::end(ext))
				{
					q.push_back(entry.path());
				}
			}
		}
	}
}

void saveInFile(std::vector<fs::path>& files, Starter& strt, Timer& t)
{
	std::ofstream out;
	out.open("output.txt");
	if (out.is_open())
	{
		out << "Total count of files: " << files.size() << std::endl;
		out << "Total count of blank lines: " << strt.total_blank << std::endl;
		out << "Total count of comment lines: " << strt.total_comm << std::endl;
		out << "Total count of physical lines: " << strt.total_phys << std::endl;
		out << "Time of execution: " << t.elapsed() << std::endl;
	}
	out.close();

	printStatistic();
}

void printStatistic()
{
	std::string line;

	std::ifstream in("output.txt");
	if (in.is_open())
	{
		while (getline(in, line))
		{
			std::cout << line << std::endl;
		}
	}
	in.close();
}