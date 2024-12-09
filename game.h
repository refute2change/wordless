#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <fstream>
#include <cmath>

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
protected:
	
public:
	int turn = 0;
	bool begin = false;
	int stalltimer = 15 * CLOCKS_PER_SEC, startstall = 0;
	bool switchedoff = false;
	bool candie = false;
	game();
	game(std::string);
	game(std::string, std::vector <std::string>, bool, bool, int);
	virtual ~game(){}
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
	virtual int getremainingtime()
	{
		return 0;
	}
	virtual int getmaxtime()
	{
		return 0;
	}
	virtual void turnontimer()
	{

	}
	virtual void permanentturnoff()
	{
		startstall = 0;
		if (result() == 0)
		{
			switchedoff = true;
		}
		
	}
	virtual void turnofftimer()
	{

	}
	virtual void insertcharacter(char) = 0;
	virtual void updateremainingtime(){}
	void addcharacter(char);
	virtual void removecharacter();
	virtual void enterevent();
	void checkguess();
	bool existcheck();
	virtual const int result();
	const void flipstate();
	int getfinishedtime();
	std::vector<std::string> getanswers();
	virtual void quit();
	void resetdeathtimer();
	void countdowntodeath();
	std::string getanswer();
	virtual void setShift(int)
	{

	}
	const bool isHit(sf::RenderWindow&);
};

class normalGame: virtual public game
{
public:
	normalGame(): game(){}
	normalGame(std::string answer): game(answer){}
	normalGame(std::string answer, std::vector <std::string> guesses, bool started, bool off, int turns): game(answer, guesses, started, off, turns){}
	~normalGame(){}
	void insertcharacter(char) override;
	int getShift() override
	{
		return 0;
	}
};

class hardGame: virtual public game
{
private:
	std::vector<bool> fixedcharacters;
	std::map<char, int> neededcharacters, notchecked;
public:
	hardGame(): game(){}
	hardGame(std::string answer): game(answer)
	{
		fixedcharacters.resize(getlength(), false);
	}
	hardGame(std::string answer, std::vector <std::string> guesses, bool started, bool off, int turns): game(answer, guesses, started, off, turns)
	{
		fixedcharacters.resize(getlength(), false);
		if (turn == 0) return;
		std::string lastanswered = getguess(turn - 1);
		for (int i = 0; i < getlength(); i++)
		{
			if (getresultstate(turn - 1, i) == 2) fixedcharacters[i] = true;
			else if (getresultstate(turn - 1, i) == 1) neededcharacters[lastanswered[i]]++;
		}
	}
	~hardGame(){}
	void insertcharacter(char) override;
	bool validcheck();
	void enterevent() override;
	int getShift() override
	{
		return 0;
	}
};

class shiftedGame: virtual public game
{
private:
	int shift = 0;
public:
	shiftedGame(): game(){}
	shiftedGame(std::string answer, int charShift): game(answer), shift(charShift){}
	shiftedGame(std::string answer, std::vector <std::string> guesses, bool started, bool off, int turns, int charShift): game(answer, guesses, started, off, turns), shift(charShift){}
	~shiftedGame(){}
	void insertcharacter(char) override;
	int getShift() override
	{
		return shift;
	}
	void setShift(int newShift)
	{
		shift = newShift;
	}
};

class timedGame: virtual public game
{
private:
	int remainingtime, starttimer = 0, maxtime;
	bool active = true;
public:
	timedGame(): game(){}
	timedGame(std::string answer, int allowedtime): game(answer), remainingtime(allowedtime), maxtime(allowedtime){}
	timedGame(std::string answer, std::vector <std::string> guesses, bool started, bool off, int turns, int allowedtime, int remainingtime): game(answer, guesses, started, off, turns), remainingtime(remainingtime), maxtime(allowedtime)
	{
		if (result() != 0)
		{
			flipstate();
			permanentturnoff();
		}

	}
	void updateremainingtime() override;
	int getremainingtime() override
	{
		return remainingtime;
	}
	int getmaxtime() override
	{
		return maxtime;
	}
	void turnontimer() override;
	void turnofftimer() override;
	void permanentturnoff() override;
	const int result() override;
	void insertcharacter(char ch) override
	{
		if (!begin)
		{
			begin = true;
			candie = true;
			turnontimer();
		}
		addcharacter(ch);
	}
	void removecharacter() override
	{
		if (!begin)
		{
			begin = true;
			turnontimer();
		}
		game::removecharacter();
	}
	int getShift() override
	{
		return 0;
	}
	void quit() override;
	~timedGame(){}
};

class hardshiftedGame: virtual public game
{
private:
	std::vector<bool> fixedcharacters;
	std::map<char, int> neededcharacters, notchecked;
	int shift = 0;
public:
	hardshiftedGame(): game(){}
	hardshiftedGame(std::string answer, int shift): game(answer)
	{
		this->shift = shift;
		fixedcharacters.resize(getlength(), false);
	}
	hardshiftedGame(std::string answer, std::vector <std::string> guesses, bool started, bool off, int turns, int charShift): game(answer, guesses, started, off, turns)
	{
		this->shift = charShift;
		fixedcharacters.resize(getlength(), false);
		if (turn == 0) return;
		std::string lastanswered = getguess(turn - 1);
		for (int i = 0; i < getlength(); i++)
		{
			if (getresultstate(turn - 1, i) == 2) fixedcharacters[i] = true;
			else if (getresultstate(turn - 1, i) == 1) neededcharacters[lastanswered[i]]++;
		}
	}
	~hardshiftedGame(){}
	//shiftedGame conponents
	int getShift() override
	{
		return shift;
	}
	void insertcharacter(char ch) override;
	//hardGame components
	void enterevent() override;
	bool validcheck();
	
};

class hardtimedGame: virtual public game
{
private:
	std::vector<bool> fixedcharacters;
	std::map<char, int> neededcharacters, notchecked;
	int remainingtime, starttimer = 0, maxtime;
	bool active = true;
public:
	hardtimedGame(): game(){}
	hardtimedGame(std::string answer, int allowedtime): game(answer), remainingtime(allowedtime), maxtime(allowedtime)
	{
		fixedcharacters.resize(getlength(), false);
	}
	hardtimedGame(std::string answer, std::vector <std::string> guesses, bool started, bool off, int turns, int allowedtime, int remainingtime): game(answer, guesses, started, off, turns), maxtime(allowedtime), remainingtime(allowedtime)
	{
		fixedcharacters.resize(getlength(), false);
		if (turn == 0) return;
		std::string lastanswered = getguess(turn - 1);
		for (int i = 0; i < getlength(); i++)
		{
			if (getresultstate(turn - 1, i) == 2) fixedcharacters[i] = true;
			else if (getresultstate(turn - 1, i) == 1) neededcharacters[lastanswered[i]]++;
		}
		if (result() != 0)
		{
			flipstate();
			permanentturnoff();
		}
	}
	~hardtimedGame(){}
	//hardGame components
	bool validcheck();
	void enterevent() override;
	int getShift() override
	{
		return 0;
	}
	//timedGame components
	void updateremainingtime() override;
	int getremainingtime() override
	{
		return remainingtime;
	}
	int getmaxtime() override
	{
		return maxtime;
	}
	void turnontimer() override;
	void turnofftimer() override;
	void permanentturnoff() override;
	const int result() override;
	void insertcharacter(char ch) override
	{
		if (!begin)
		{
			begin = true;
			turnontimer();
		}
		addcharacter(ch);
	}
	void removecharacter() override
	{
		if (!begin)
		{
			begin = true;
			turnontimer();
		}
		game::removecharacter();
	}	
	void quit() override;
};

class shiftedtimedGame: virtual public game
{
private:
	int shift = 0;
	int remainingtime, starttimer = 0, maxtime;
	bool active = true;
public:
	shiftedtimedGame(): game(){}
	shiftedtimedGame(std::string answer, int charShift, int allowedtime): game(answer), remainingtime(allowedtime), maxtime(allowedtime), shift(charShift){}
	shiftedtimedGame(std::string answer, std::vector <std::string> guesses, bool started, bool off, int turns, int charShift, int allowedtime, int remaintime): game(answer, guesses, started, off, turns), shift(charShift), maxtime(allowedtime), remainingtime(remaintime)
	{
		if (result() != 0)
		{
			flipstate();
			permanentturnoff();
		}
	}
	~shiftedtimedGame(){}
	//shiftedGame components
	void insertcharacter(char) override;
	int getShift() override
	{
		return shift;
	}
	void setShift(int newShift)
	{
		shift = newShift;
	}
	//timedGame components
	void updateremainingtime() override;
	int getremainingtime() override
	{
		return remainingtime;
	}
	int getmaxtime() override
	{
		return maxtime;
	}
	void turnontimer() override;
	void turnofftimer() override;
	void permanentturnoff() override;
	const int result() override;
	void removecharacter() override
	{
		if (!begin)
		{
			begin = true;
			turnontimer();
		}
		game::removecharacter();
	}
	void quit() override;
};

class hardshiftedtimedGame: virtual public game
{
private:
	std::vector<bool> fixedcharacters;
	std::map<char, int> neededcharacters, notchecked;
	int shift = 0;
	int remainingtime, starttimer = 0, maxtime;
	bool active = true;
public:
	hardshiftedtimedGame(): game(){}
	hardshiftedtimedGame(std::string answer, int charShift, int allowedtime): game(answer), shift(charShift), remainingtime(allowedtime), maxtime(allowedtime){fixedcharacters.resize(getlength());}
	hardshiftedtimedGame(std::string answer, std::vector <std::string> guesses, bool started, bool off, int turns, int charShift, int allowedtime, int remaintime): game(answer, guesses, started, off, turns), shift(charShift), maxtime(allowedtime), remainingtime(remaintime)
	{
		if (result() != 0)
		{
			flipstate();
			permanentturnoff();
		}
		fixedcharacters.resize(getlength(), false);
		if (turn == 0) return;
		std::string lastanswered = getguess(turn - 1);
		for (int i = 0; i < getlength(); i++)
		{
			if (getresultstate(turn - 1, i) == 2) fixedcharacters[i] = true;
			else if (getresultstate(turn - 1, i) == 1) neededcharacters[lastanswered[i]]++;
		}
	}
	//hardGame components
	bool validcheck();
	void enterevent() override;
	//shiftedGame components
	void insertcharacter(char) override;
	int getShift() override
	{
		return shift;
	}
	void setShift(int newShift)
	{
		shift = newShift;
	}
	//timedGame components
	void updateremainingtime() override;
	int getremainingtime() override
	{
		return remainingtime;
	}
	int getmaxtime() override
	{
		return maxtime;
	}
	void turnontimer() override;
	void turnofftimer() override;
	void permanentturnoff() override;
	const int result() override;
	void removecharacter() override
	{
		if (!begin)
		{
			begin = true;
			turnontimer();
		}
		game::removecharacter();
	}
	void quit() override;
	~hardshiftedtimedGame(){}
};

class drawer
{
private:
	std::string top = "qwertyuiop", mid = "asdfghjkl", bot = "zxcvbnm";
	sf::Texture wordblockinactive, wordblocknotexisted, wordblockwrongplace, wordblockcorrect, characterblockinactive, characterblocknotexisted, characterblockwrongplace, characterblockcorrect, gamenotstarted, gameactive, gamefailed, gamewon, gamestall, notvalidguess, alreadyguessed, winmessage, lostmessage;
	sf::Sprite wordblock, characterblock, gameblock, message;
	sf::Font font, notactivefont;
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