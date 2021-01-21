#pragma once
#include <iostream>
#include "Starter.h"
#include "Timer.h"

void folderTree(const fs::path& pathToShow, std::vector<fs::path>& q);

void printStatistic(std::vector<fs::path>& files, Starter& strt, Timer& t);