#include "resultHandler.h"
#include <iostream>

resultHandler::resultHandler()
{
	timestamp = time(NULL);
	//create 2 files for history if they're missing
	struct stat sb;
	if (stat("C:/Wordless", &sb) != 0)
	{
		std::filesystem::create_directories("C:/Wordless");
		SetFileAttributesA("C:/Wordless", FILE_ATTRIBUTE_HIDDEN);
		std::ofstream fa, fb;
		fa.open("C:/Wordless/gamedata.txt");
		fb.open("C:/Wordless/unfinishedgame.txt");
		fa << 0 << '\n' << 0;
		fb << 0;
		fa.close();
		fb.close();
	}
	std::ifstream gamedata;
	gamedata.open("C:/Wordless/gamedata.txt");
	gamedata >> start >> loses;
	if (loses > 20) at = 19;
	else if (loses == 0) at = 0;
	else at = loses - 1;
	gamedata.close();
}

void resultHandler::handleWingame()
{
	loses = 0;
	at = 0;
}

void resultHandler::handleLostgame(time_t stoptime)
{
	if (start != 0) return;
	loses++;
	if (loses > 20) at = 19;
	else at = loses - 1;
	start = stoptime;
}

int resultHandler::minutes()
{
	return remainingtime() / 60;
}

int resultHandler::seconds()
{
	return remainingtime() % 60;
}

int resultHandler::remainingtime()
{
	timestamp = time(NULL);
	if (timestamp > start + detention[at])
	{
		start = 0;
	}
	if (start == 0) return 0;
	return (detention[at] - (timestamp - start));
}

void resultHandler::savelostgames()
{
	std::ofstream gamedata;
	gamedata.open("C:/Wordless/gamedata.txt");
	gamedata << start << '\n' << loses;
	gamedata.close();
}