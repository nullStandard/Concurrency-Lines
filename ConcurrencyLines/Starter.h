#pragma once
#include <vector>
//#define BOOST_ASIO_HEADER_ONLY
//#define BOOST_ASIO_SEPARATE_COMPILATION
//#include <boost/asio/impl/src.hpp>
#include <utility>
#include <mutex>
#include "ProcessingData.h"

class Starter : public ProcessingData::InternalAbstraction
{
	std::vector<fs::path>& m_files;
	std::mutex mtx;

public:
	//маю тут мати поле з тотал результатами
	struct
	{
		long long total_blank = 0;
		long long total_comm = 0;
		long long total_phys = 0;
	};

	Starter(std::vector<fs::path>& files) : m_files(files) {}

	//отримує результати
	// приймає структуру з локальним підрахунком
	void did_recieved_results(ProcessingData::LocalCount& results) override;
	
	void start();
};