#include <string>  
#include <iostream>  
#include <filesystem>  
#include <queue>  
#include <algorithm>
#include <fstream>
#include <tuple>

namespace fs = std::filesystem;

void DisplayFileInfo(const fs::directory_entry& entry, std::string& lead, fs::path& filename)
{
	std::cout << lead << " " << filename << ", "
		<< std::endl;
}

void folderTree(const fs::path& pathToShow, std::queue<fs::path>& q)
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
					q.push(entry.path());
				}
			}
		}
	}
}

void DisplayTree(const fs::path& pathToShow, int level = 0)
{
	if (fs::exists(pathToShow) && fs::is_directory(pathToShow))
	{
		auto lead = std::string(level * 3, ' ') + "|__";
		for (const auto& entry : fs::directory_iterator(pathToShow))
		{
			auto filename = entry.path().filename();
			if (fs::is_directory(entry.status()))
			{
				std::cout << lead << "[+] " << filename << "\n";
				DisplayTree(entry, level + 1);
			}
			else if (fs::is_regular_file(entry.status()))
			{

				DisplayFileInfo(entry, lead, filename);
			}
			else
				std::cout << lead << " [?]" << filename << "\n";
		}
	}
}

std::tuple<int, int, int> computeData(const fs::path& FilePath)
{
	int count_blank = 0;
	int count_comm = 0;
	int count_phys = 0;

	std::fstream in(FilePath);
	std::string line;
	while (std::getline(in, line))
	{
		if (line.empty() || line.find_first_not_of(' ') == std::string::npos)
		{
			++count_blank;
		}
		else
		{
			// here is a bug
			if (line.find("//") != std::string::npos)
			{
				++count_comm;
			}
			++count_phys;
		}
	}
	in.close();
	return std::make_tuple(count_blank, count_comm, count_phys);
}

int main(int argc, char* argv[])
{
	std::queue<fs::path> q;
	const fs::path pathToShow("C:\\Users\\User\\source\\repos\\ListWithPoiners\\ListWithPoiners");
	folderTree(pathToShow, q);
	DisplayTree(pathToShow);
	auto file = q.front().string();
	auto [blank, comm, phys] = computeData(file);

	std::cout << blank << " " << comm << " " << phys << std::endl;

	return 0;
}