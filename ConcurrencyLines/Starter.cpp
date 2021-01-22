#include "Starter.h"
#include <boost/asio.hpp>
#include "boost/bind.hpp"
#include <thread>

void Starter::did_recieved_results(ProcessingData::LocalCount& results)
{
	std::lock_guard<std::mutex> l_g(mtx);
	total_blank += results.count_blank;
	total_comm += results.count_comm;
	total_phys += results.count_phys;
}

void Starter::start()
{
	ProcessingData proc_obj(this);
	boost::asio::thread_pool m_pool(std::thread::hardware_concurrency());
	
	for (auto& file : m_files)
	{
		boost::asio::post(m_pool, boost::bind(&ProcessingData::processData, &proc_obj, std::ref(file)));
	}

	m_pool.join();
}
