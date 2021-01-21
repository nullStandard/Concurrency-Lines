//#include "Timer.h"
//#include "Starter.h"
#include "SecondaryFunctions.h"

int main(int argc, char* argv[])
{
	Timer t;

	std::vector<fs::path> files;

	fs::path path("C:\\Program Files\\boost_1_75_0\\boost");

	folderTree(path, files);
	Starter strt(files);
	strt.start();
	
	printStatistic(files, strt, t);
	return 0;
}