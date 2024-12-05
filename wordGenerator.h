#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>


class wordGenerator
{
private:
	std::ifstream f3, f4, f5, f6, f7, f8;
	int length[6] = { 223, 653, 699, 770, 741, 585 };
public:
	wordGenerator();
	std::vector<std::string> generator();
};

