#include "wordGenerator.h"

wordGenerator::wordGenerator()
{
	f3.open("answers/3letters.txt");
	f4.open("answers/4letters.txt");
	f5.open("answers/5letters.txt");
	f6.open("answers/6letters.txt");
	f7.open("answers/7letters.txt");
	f8.open("answers/8letters.txt");
}

std::vector<std::string> wordGenerator::generator()
{
	std::vector<std::string> data;
	data.resize(6);
	int places[6];
	for (int i = 0; i < 6; i++) places[i] = rand() % length[i];
	f3.clear();
	f4.clear();
	f5.clear();
	f6.clear();
	f7.clear();
	f8.clear();
	f3.seekg(0, f3.beg);
	f4.seekg(0, f4.beg);
	f5.seekg(0, f5.beg);
	f6.seekg(0, f6.beg);
	f7.seekg(0, f7.beg);
	f8.seekg(0, f8.beg);
	while (places[0]--)
	{
		std::getline(f3, data[0]);
	}
	while (places[1]--)
	{
		std::getline(f4, data[1]);
	}
	while (places[2]--)
	{
		std::getline(f5, data[2]);
	}
	while (places[3]--)
	{
		std::getline(f6, data[3]);
	}
	while (places[4]--)
	{
		std::getline(f7, data[4]);
	}
	while (places[5]--)
	{
		std::getline(f8, data[5]);
	}
	return data;
}