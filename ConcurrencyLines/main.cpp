#include <string>  
#include <iostream>  
#include <filesystem>  
#include <vector>  
#include <algorithm>
#include <fstream>
#include <functional>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <chrono>
#include <map>
#include <utility>
#include <thread>
#include <mutex>
#include <utility>
namespace fs = std::filesystem;

class Timer
{
private:
	// Псевдонимы типов используются для удобного доступа к вложенным типам
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_t> m_beg;

public:
	Timer() : m_beg(clock_t::now())
	{
	}

	void reset()
	{
		m_beg = clock_t::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

/*void DisplayFileInfo(const fs::directory_entry& entry, std::string& lead, const fs::path& filename)
{
	std::cout << lead << " " << filename << ", "
		<< std::endl;
}*/

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
int level = 0;
//void DisplayTree(const fs::path& pathToShow)
//{
//	if (fs::exists(pathToShow) && fs::is_directory(pathToShow))
//	{
//		auto lead = std::string(level * 3, ' ') + "|__";
//		for (const auto& entry : fs::directory_iterator(pathToShow))
//		{
//			auto filename = entry.path().filename();
//			if (fs::is_directory(entry.status()))
//			{
//				std::cout << lead << "[+] " << filename << "\n";
//				level = level + 1;
//				DisplayTree(entry);
//			}
//			else if (fs::is_regular_file(entry.status()))
//			{
//
//				DisplayFileInfo(entry, lead, filename);
//			}
//			else
//				std::cout << lead << " [?]" << filename << "\n";
//		}
//	}
//}
/*std::tuple<int, int, int> processData(const fs::path& FilePath)
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
}*/

//std::mutex mtx;

class Container
{
	//std::map<const fs::path, std::array<unsigned long long, 3>> map;
	unsigned long long total_blank = 0;
	unsigned long long total_comm = 0;
	unsigned long long total_phys = 0;

public:
	
	class Lubiy
	{
	public:
		virtual void did_recieved_results(long long blank) = 0;
	};

	Lubiy* l_p;
	Container(Lubiy* p_L) : l_p(p_L) {}

	void processData(const fs::path& FilePath)
	{
		unsigned long long count_blank = 0;
		unsigned long long count_comm = 0;
		unsigned long long count_phys = 0;

		std::ifstream in(FilePath);
		std::string line;
		while (std::getline(in, line))
		{
			if (line.empty() || (line.find_first_not_of(' ') == std::string::npos))
			{
				++count_blank;
			}
			else
			{
				if (line.find("//") != std::string::npos)
				{
					++count_comm;
				}
				++count_phys;
			}
		}

		in.close();
		//ЗРОБИТИ СТРУКТУРУ Й ПЕРЕДАТИ В МЕТОД, ЗРОБИТИ ПЕРЕВІРКУ НА НУЛЛ
		this->l_p->did_recieved_results(count_blank);
		//std::lock_guard<std::mutex>l_g(mtx);
		//mtx.lock();
		//map[FilePath] = { count_blank, count_comm, count_phys };
		total_blank += count_blank;
		//std::cout << count_blank << std::endl;
		total_comm += count_comm;
		total_phys += count_phys;


		//mtx.unlock();
	}

	unsigned long long getTotal_blank()
	{
		return total_blank;
	}
	unsigned long long getTotal_comm()
	{
		return total_comm;
	}
	unsigned long long getTotal_phys()
	{
		return total_phys;
	}
};

class Starter : public Container::Lubiy
{
public:
	//маю тут мати поле з тотал результатами
	std::vector<fs::path>& m_files;
	Starter(std::vector<fs::path>& files) : m_files(files) {}
	std::mutex mtx;

	//отримує результати
	// приймає структуру з локальним підрахунком
	void did_recieved_results(long long blank) override
	{ 
		//мьютекс, зберігає результати в тотал
		std::lock_guard(mtx);
		std::cout << blank << " " << std::endl;
	}

	void start()
	{
		std::shared_ptr<Container> p(new Container(this));
		boost::asio::thread_pool m_pool(std::thread::hardware_concurrency());

		for (auto& file : m_files)
		{
			//cont.processData(file);
			//cont(file);
			//
			boost::asio::post(m_pool, boost::bind(&Container::processData, p, std::ref(file)));
			/*boost::asio::post(m_pool,
			[=]() {
				p->processData(file);
		});*/
		}

		m_pool.join();
	}

};

void printStatistic(std::vector<fs::path>& files, Container& cont, Timer& t)
{
	std::cout << "Total count of files: " << files.size() << std::endl;
	std::cout << "Total count of blank lines: " << cont.getTotal_blank() << std::endl;
	std::cout << "Total count of comment lines: " << cont.getTotal_comm() << std::endl;
	std::cout << "Total count of physical lines: " << cont.getTotal_phys() << std::endl;
	std::cout << "Time of execution: " << t.elapsed() << std::endl;
}

int main(int argc, char* argv[])
{
	Timer t;
	//fs::path path{ argc >= 2 ? argv[1] : fs::current_path() };

	std::vector<fs::path> files;
	//fs::path path("C:\\Program Files\\boost_1_75_0");
	//fs::path path("C:\\Users\\User\\source\\repos\\chernetka\\chernetka");
	fs::path path("C:\\Program Files\\boost_1_75_0\\boost");

	folderTree(path, files);
	Starter strt(files);
	strt.start();
	
	//printStatistic(files, cont, t);
	return 0;
}