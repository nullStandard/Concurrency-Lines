#pragma once
#include <fstream>
#include <filesystem>  
#include <string>

namespace fs = std::filesystem;
class ProcessingData
{
public:
	struct LocalCount
	{
		long long count_blank = 0;
		long long count_comm = 0;
		long long count_phys = 0;
	};

	class InternalAbstraction
	{
	public:
		virtual void did_recieved_results(LocalCount& results) = 0;
	};

	InternalAbstraction* InterAbstr_ptr;
	ProcessingData(InternalAbstraction* starter) : InterAbstr_ptr(starter) {}

	void processData(const fs::path& FilePath);
};