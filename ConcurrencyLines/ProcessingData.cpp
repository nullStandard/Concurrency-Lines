#include "ProcessingData.h"

void ProcessingData::processData(const fs::path& FilePath)
{
	LocalCount counter;
	std::ifstream in(FilePath);
	std::string line;

	bool great_comment = false;
	while (std::getline(in, line))
	{
		if (!great_comment)
		{
			if (line.empty() || (line.find_first_not_of(' ') == std::string::npos))
			{
				counter.count_blank++;
			}
			else if (line.find("//") != std::string::npos)
			{
				counter.count_comm++;
			}
			else if (line.find("/*") != std::string::npos)
			{
				counter.count_comm++;
				great_comment = true;
			}
			else
			{
				counter.count_phys++;
			}
		}
		else
		{
			counter.count_comm++;
			if (line.find("*/") != std::string::npos)
			{
				great_comment = false;
			}
		}

	}
	in.close();

	if (InterAbstr_ptr)
	{
		this->InterAbstr_ptr->did_recieved_results(counter);
	}
}
