#include "ProcessingData.h"

void ProcessingData::processData(const fs::path& FilePath)

{
	LocalCount counter;
	std::ifstream in(FilePath);
	std::string line;
	while (std::getline(in, line))
	{
		if (line.empty() || (line.find_first_not_of(' ') == std::string::npos))
		{
			counter.count_blank++;
		}
		else
		{
			if (line.find("//") != std::string::npos)
			{
				counter.count_comm++;
			}
			counter.count_phys++;
		}
	}
	in.close();

	if (InterAbstr_ptr)
	{
		this->InterAbstr_ptr->did_recieved_results(counter);
	}
}
