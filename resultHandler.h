#pragma once
#include <fstream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <sys/stat.h>

class resultHandler
{
private:
	time_t timestamp;
	time_t start;
	int detention[20] = { 10, 15, 20, 30, 60, 90, 120, 150, 200, 300, 450, 600, 750, 900, 1200, 1500, 1800, 2400, 3600, 7200 };
	std::ofstream f;
	int at;
public:
	resultHandler();
	int loses = 0;
	int remainingtime();
	void handleWingame();
	void handleLostgame(time_t stoptime);
	int minutes();
	int seconds();
	void savelostgames();
};

