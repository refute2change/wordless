#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <fstream>

class game
{
private:
	time_t timestamp;
	std::ifstream availableanswers;
	int length, messageinitiated;
	int messagetime, messagetype = 0;
	bool informed = false;
	std::string answer, answerdisplay = "";
	std::map<char, int> targetmap, guessesmap;
	std::vector<int> letterstates;
	std::vector < std::vector<int> > guessresults;
	std::vector<std::string> guesses;
	int finishedtime = 0;
	sf::RectangleShape gameblock;
public:
	int turn = 0;
	bool begin = false;
	game();
	game(std::string);
	game(std::string, std::vector <std::string>, bool, int);
	std::string getdisplaystring()
	{
		return answerdisplay;
	}
	bool messagestate()
	{
		return informed;
	}
	void showmessage()
	{
		informed = true;
	}
	void hidemessage()
	{
		informed = false;
	}
	int getresultstate(int i, int j)
	{
		return guessresults[i][j];
	}
	int getcharacterstate(char c)
	{
		return letterstates[c - 97];
	}
	int getlength()
	{
		return length;
	}
	std::string getguess(int i)
	{
		return guesses[i];
	}
	int getmessagestate()
	{
		return messagetype;
	}
	void setmessagestate(int state)
	{
		messagetype = state;
	}
	void clearmessagestate()
	{
		messagetype = 0;
	}
	virtual int getShift() = 0;	
	virtual void insertcharacter(char) = 0;
	void addcharacter(char);
	void removecharacter();
	virtual void enterevent();
	void checkguess();
	bool existcheck();
	const int result();
	const void flipstate();
	int getfinishedtime();
	std::vector<std::string> getanswers();
	std::string getanswer();
	const bool isHit(sf::RenderWindow&);
};

class hardGame: public game
{
private:
	std::vector<bool> fixedcharacters;
	std::map<char, int> neededcharacters, notchecked;
public:
	hardGame(): game(){}
	hardGame(std::string answer): game(answer){fixedcharacters.resize(getlength(), false);}
	hardGame(std::string answer, std::vector <std::string> guesses, bool started, int turns): game(answer, guesses, started, turns){fixedcharacters.resize(getlength(), false);}
	void insertcharacter(char) override;
	bool validcheck();
	void enterevent() override;
	int getShift() override
	{
		return 0;
	}
};

class normalGame: public game
{
public:
	normalGame(): game(){}
	normalGame(std::string answer): game(answer){}
	normalGame(std::string answer, std::vector <std::string> guesses, bool started, int turns): game(answer, guesses, started, turns){}
	void insertcharacter(char) override;
	int getShift() override
	{
		return 0;
	}
};

class shiftedGame: public game
{
private:
	int shift = 0;
public:
	shiftedGame(): game(){}
	shiftedGame(std::string answer, int charShift): game(answer), shift(charShift){}
	shiftedGame(std::string answer, std::vector <std::string> guesses, bool started, int turns, int charShift): game(answer, guesses, started, turns), shift(charShift){}
	void insertcharacter(char) override;
	int getShift() override
	{
		return shift;
	}
};

class drawer
{
private:
	std::string top = "qwertyuiop", mid = "asdfghjkl", bot = "zxcvbnm";
	sf::Texture wordblockinactive, wordblocknotexisted, wordblockwrongplace, wordblockcorrect, characterblockinactive, characterblocknotexisted, characterblockwrongplace, characterblockcorrect, gamenotstarted, gameactive, gamefailed, gamewon, notvalidguess, alreadyguessed, winmessage, lostmessage;
	sf::Sprite wordblock, characterblock, gameblock, message;
	sf::Font font;
	sf::Text text;
	int messageinitiated = 0;
public:
	drawer();
	void drawresult(game*, sf::RenderWindow&);
	void drawkeyboard(game*, sf::RenderWindow&);
	void draw(game*, sf::RenderWindow&);
	void drawstate(game*, sf::RenderWindow&, int);
	void drawmessage(game*, sf::RenderWindow&);
};