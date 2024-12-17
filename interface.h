#pragma once
#include "game.h"
#include "wordGenerator.h"
#include "resultHandler.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class interface
{
private:
	time_t timestamp;
	int active = 0, mode = 1, i, res, count = 0;
	sf::RenderWindow w{sf::VideoMode(1000, 800), "Wordless", sf::Style::Close | sf::Style::Titlebar};
	sf::Event ev;
	std::vector<std::string> words;
	sf::Texture resigntexture, resetinactive, resetactive, totalwinmessage, timerresetmessage, newinform;
	sf::Sprite resignbutton, resetbutton, inform, message;
	wordGenerator g;
	sf::Font font;
	sf::Text min, sec;
	std::string mins, secs, temp;
	std::vector<int> modes;
	std::vector<game*> gamez;
	int firstlost = 0, readavailable;
	bool resignavailable = true, resigned = false, detention = false, messageavailable = false;
	bool resetavailable = false, finished = false, typeable = true, informavailable = false, focus = true;
	resultHandler handler;
public:
	void readgame();
	interface();
	void generate();
	void operate();
	void draw();
	int finalresult();
	bool resignhit();
	void resign();
	bool resethit();
	void reset();
	bool informhit();
	void lose(time_t stamp);
	void win();
	void handleendevent();
	void drawclock();
	void resetcheck();
	void savegame();
};