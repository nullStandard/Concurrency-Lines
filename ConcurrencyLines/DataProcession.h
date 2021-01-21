#pragma once
//#include <fstream>
//
//class DataProcession
//{
//	int count_blank = 0;
//	int count_comm = 0;
//	int count_phys = 0;
//
//	std::fstream in(FilePath);
//	std::string line;
//	while (std::getline(in, line))
//	{
//		if (line.empty() || line.find_first_not_of(' ') == std::string::npos)
//		{
//			++count_blank;
//		}
//		else
//		{
//			// here is a bug
//			if (line.find("//") != std::string::npos)
//			{
//				++count_comm;
//			}
//			++count_phys;
//		}
//	}
//	in.close();
//	return std::make_tuple(count_blank, count_comm, count_phys);
//};

