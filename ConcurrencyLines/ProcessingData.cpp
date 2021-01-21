#include "ProcessingData.h"

//struct ProcessingData::LocalCount
//{
//	unsigned long long count_blank = 0;
//	unsigned long long count_comm = 0;
//	unsigned long long count_phys = 0;
//};

//class ProcessingData::InternalAbstraction
//{
//public:
//	virtual void did_recieved_results(LocalCount& results) = 0;
//};

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

	//ÇĞÎÁÈÒÈ ÑÒĞÓÊÒÓĞÓ É ÏÅĞÅÄÀÒÈ Â ÌÅÒÎÄ, ÇĞÎÁÈÒÈ ÏÅĞÅÂ²ĞÊÓ ÍÀ ÍÓËË
	if (InterAbstr_ptr)
	{
		this->InterAbstr_ptr->did_recieved_results(counter);
	}
}
