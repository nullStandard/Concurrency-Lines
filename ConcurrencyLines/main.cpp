//#include "Timer.h"
//#include "Starter.h"
#include "SecondaryFunctions.h"

int main(int argc, char* argv[])
{
	fs::path path{ argc >= 2 ? argv[1] : fs::current_path() };

	Timer t;
	std::vector<fs::path> files;

	//searching for files
	folderTree(path, files);
	Starter strt(files);
	strt.start();
	
	saveInFile(files, strt, t);
	return 0;
}