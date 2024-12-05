#include "game.h"

game::game(std::string word)
{
	answer = word;
	length = answer.length();
	gameblock.setSize(sf::Vector2f(175.f, 75.f));
	gameblock.setPosition(810, 150 + 100 * (length - 3));
	for (char c : answer) answerdisplay += toupper(c);
	switch (length)
	{
	case 3:
		availableanswers.open("legalanswers/3letters.txt");
		break;
	case 4:
		availableanswers.open("legalanswers/4letters.txt");
		break;
	case 5:
		availableanswers.open("legalanswers/5letters.txt");
		break;
	case 6:
		availableanswers.open("legalanswers/6letters.txt");
		break;
	case 7:
		availableanswers.open("legalanswers/7letters.txt");
		break;
	case 8:
		availableanswers.open("legalanswers/8letters.txt");
		break;
	}
	targetmap.clear();
	for (auto c : answer) targetmap[c]++;
	guesses.resize(6);
	for (std::string str : guesses) str = "";
	guessresults.resize(6);
	for (int i = 0; i < guessresults.size(); i++)
	{
		for (int j = 0; j < length; j++) guessresults[i].push_back(-1);
	}
	for (int i = 0; i < 26; i++) letterstates.push_back(-1);
}

game::game(std::string word, std::vector<std::string> guessessofar, bool started, int turns)
{
	answer = word;
	length = answer.length();
	gameblock.setSize(sf::Vector2f(175.f, 75.f));
	gameblock.setPosition(810, 150 + 100 * (length - 3));
	begin = started;
	for (char c : answer) answerdisplay += toupper(c);
	begin = started;
	switch (length)
	{
	case 3:
		availableanswers.open("legalanswers/3letters.txt");
		break;
	case 4:
		availableanswers.open("legalanswers/4letters.txt");
		break;
	case 5:
		availableanswers.open("legalanswers/5letters.txt");
		break;
	case 6:
		availableanswers.open("legalanswers/6letters.txt");
		break;
	case 7:
		availableanswers.open("legalanswers/7letters.txt");
		break;
	case 8:
		availableanswers.open("legalanswers/8letters.txt");
		break;
	}
	guessresults.resize(6);
	for (int i = 0; i < guessresults.size(); i++)
	{
		for (int j = 0; j < length; j++) guessresults[i].push_back(-1);
	}
	for (int i = 0; i < 26; i++) letterstates.push_back(-1);
	targetmap.clear();
	for (auto c : answer) targetmap[c]++;
	for (int i = 0; i < 6; i++) guesses.push_back("");
	for (int i = 0; i < 26; i++) letterstates.push_back(-1);
	for (int i = 0; i < 6; i++)
	{
		guesses[i] = guessessofar[i];
		if (guesses[turn].length() == length)
		{
			if (turn < turns) enterevent();
		}
	}
}

void game::addcharacter(char ch)
{
	if (guesses[turn].length() < length) guesses[turn] += ch;
	if (!begin) begin = true;
}

void game::removecharacter()
{
	if (guesses[turn].length() != 0)
	{
		std::string temp = guesses[turn].substr(0, guesses[turn].length() - 1);
		guesses[turn] = temp;
	}
}

bool game::existcheck()
{
	availableanswers.clear();
	availableanswers.seekg(0, availableanswers.beg);
	std::string str;
	while (std::getline(availableanswers, str))
	{
		if (str == guesses[turn]) return true;
	}
	return false;
}

void game::enterevent()
{
	if (turn == 6) return;
	if (result() != 0) return;
	if (guesses[turn].length() != length) return;
	for (int i = 0; i < turn; i++) if (guesses[turn] == guesses[i])
	{
		messagetime = clock();
		messagetype = 2;
		return;
	}
	if (!existcheck())
	{
		messagetime = clock();
		messagetype = 1;
		return;
	}
	checkguess();
}

void game::checkguess()
{
	guessesmap.clear();
	for (int i = 0; i < length; i++)
	{
		if (guesses[turn][i] == answer[i])
		{
			guessresults[turn][i] = 2;
			guessesmap[guesses[turn][i]]++;
		}
		else guessresults[turn][i] = 0;
	}
	for (int i = 0; i < length; i++)
	{
		if (guessresults[turn][i] == 2) continue;
		guessesmap[guesses[turn][i]]++;
		if (guessesmap[guesses[turn][i]] <= targetmap[guesses[turn][i]]) guessresults[turn][i] = 1;
	}
	for (int i = 0; i < length; i++) if (letterstates[guesses[turn][i] - 97] < guessresults[turn][i]) letterstates[guesses[turn][i] - 97] = guessresults[turn][i];
	turn++;
	flipstate();
}

const void game::flipstate()
{
	time(&timestamp);
	if (result() != 0)
	{
		informed = true;
		if (result() == 1) messagetype = 3;
		else messagetype = 4;
		if (finishedtime == 0) finishedtime = timestamp;
	}
}

const int game::result()
{
	if (turn == 0) return 0;
	for (int i = 0; i < turn; i++)
	{
		if (guesses[i] == answer) return 1;
	}
	if (turn == 6)
	{
		return -1;
	}
	return 0;
}

int game::getfinishedtime()
{
	return finishedtime;
}

std::vector<std::string> game::getanswers()
{
	return guesses;
}

std::string game::getanswer()
{
	return answer;
}

const bool game::isHit(sf::RenderWindow& w)
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) return false;
	int x = sf::Mouse::getPosition(w).x, y = sf::Mouse::getPosition(w).y;
	return (x >= gameblock.getPosition().x && y >= gameblock.getPosition().y && x < gameblock.getPosition().x + gameblock.getSize().x && y < gameblock.getPosition().y + gameblock.getSize().y);
}

drawer::drawer()
{
	wordblockinactive.loadFromFile("Images/wordblockinactive.png");
	wordblocknotexisted.loadFromFile("Images/wordblocknotexisted.png");
	wordblockwrongplace.loadFromFile("Images/wordblockwrongplace.png");
	wordblockcorrect.loadFromFile("Images/wordblockcorrect.png");
	characterblockinactive.loadFromFile("Images/characterblockinactive.png");
	characterblocknotexisted.loadFromFile("Images/characterblocknotexisted.png");
	characterblockwrongplace.loadFromFile("Images/characterblockwrongplace.png");
	characterblockcorrect.loadFromFile("Images/characterblockcorrect.png");
	gamenotstarted.loadFromFile("Images/gamenotstarted.png");
	gameactive.loadFromFile("Images/gameactive.png");
	gamefailed.loadFromFile("Images/gamefailed.png");
	notvalidguess.loadFromFile("Images/invalidguess.png");
	alreadyguessed.loadFromFile("Images/repetitiveguess.png");
	winmessage.loadFromFile("Images/gamewinmessage.png");
	lostmessage.loadFromFile("Images/gamelostmessage.png");
	gamewon.loadFromFile("Images/gamewon.png");
	font.loadFromFile("Fonts/ebrimabd.ttf");
	text.setFillColor(sf::Color::Black);
}

void drawer::draw(game* g, sf::RenderWindow& w)
{
	drawkeyboard(g, w);
	drawresult(g, w);
	drawmessage(g, w);
}

void drawer::drawkeyboard(game* g, sf::RenderWindow& w)
{
	int x;
	char temp;
	x = 0;
	for (char c : top)
	{
		switch (g->getcharacterstate(c))
		{
		case -1:
			characterblock.setTexture(characterblockinactive);
			break;
		case 0:
			characterblock.setTexture(characterblocknotexisted);
			break;
		case 1:
			characterblock.setTexture(characterblockwrongplace);
			break;
		case 2:
			characterblock.setTexture(characterblockcorrect);
			break;
		}
		characterblock.setPosition(178 + 50 * x, 600);
		text.setFont(font);
		temp = toupper(c);
		text.setString(temp);
		text.setCharacterSize(25);
		text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
		text.setPosition(200 + 50 * x, 622);
		w.draw(characterblock);
		w.draw(text);
		x++;
	}
	x = 0;
	for (char c : mid)
	{
		switch (g->getcharacterstate(c))
		{
		case -1:
			characterblock.setTexture(characterblockinactive);
			break;
		case 0:
			characterblock.setTexture(characterblocknotexisted);
			break;
		case 1:
			characterblock.setTexture(characterblockwrongplace);
			break;
		case 2:
			characterblock.setTexture(characterblockcorrect);
			break;
		}
		characterblock.setPosition(193 + 50 * x, 650);
		text.setFont(font);
		temp = toupper(c);
		text.setString(temp);
		text.setCharacterSize(25);
		text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
		text.setPosition(215 + 50 * x, 672);
		w.draw(characterblock);
		w.draw(text);
		x++;
	}
	x = 0;
	for (char c : bot)
	{
		switch (g->getcharacterstate(c))
		{
		case -1:
			characterblock.setTexture(characterblockinactive);
			break;
		case 0:
			characterblock.setTexture(characterblocknotexisted);
			break;
		case 1:
			characterblock.setTexture(characterblockwrongplace);
			break;
		case 2:
			characterblock.setTexture(characterblockcorrect);
			break;
		}
		characterblock.setPosition(223 + 50 * x, 700);
		text.setFont(font);
		temp = toupper(c);
		text.setString(temp);
		text.setCharacterSize(25);
		text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
		text.setPosition(245 + 50 * x, 722);
		w.draw(characterblock);
		w.draw(text);
		x++;
	}
}

void drawer::drawstate(game* g, sf::RenderWindow& w, int atgame)
{
	std::string temp = std::to_string(g->getlength()) + " letters";
	text.setFont(font);
	text.setString(temp);
	text.setCharacterSize(45);
	text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
	text.setPosition(898, 187 + 100 * (g->getlength() - 3));
	if (atgame == g->getlength() - 3) gameblock.setTexture(gameactive);
	else if (!g->result()) gameblock.setTexture(gamenotstarted);
	else if (g->result() == -1) gameblock.setTexture(gamefailed);
	else gameblock.setTexture(gamewon);
	gameblock.setPosition(810, 150 + 100 * (g->getlength() - 3));
	w.draw(gameblock);
	w.draw(text);
}

void drawer::drawmessage(game* g, sf::RenderWindow& w)
{
	switch (g->getmessagestate())
	{
	case 1:
		if (!g->messagestate())
		{
			g->showmessage();
			this->messageinitiated = clock();
		}
		else
		{
			if (clock() - this->messageinitiated > 1000)
			{
				g->hidemessage();
				this->messageinitiated = 0;
				g->clearmessagestate();
			}
		}
		if (!g->messagestate()) return;
		message.setTexture(this->notvalidguess);
		message.setTextureRect(sf::IntRect(0, 0, notvalidguess.getSize().x, notvalidguess.getSize().y));
		message.setPosition(200, 265);
		w.draw(this->message);
		break;
	
	case 2:
		if (!g->messagestate())
		{
			g->showmessage();
			this->messageinitiated = clock();
		}
		else
		{
			if (clock() - this->messageinitiated > 1000)
			{
				g->hidemessage();
				this->messageinitiated = 0;
				g->clearmessagestate();
			}
		}
		if (!g->messagestate()) return;
		message.setTexture(this->alreadyguessed);
		message.setTextureRect(sf::IntRect(0, 0, alreadyguessed.getSize().x, alreadyguessed.getSize().y));
		message.setPosition(200, 265);
		w.draw(this->message);
		break;
	case 3:
		if (g->messagestate())
		{
			message.setTexture(this->winmessage);
			message.setTextureRect(sf::IntRect(0, 0, winmessage.getSize().x, winmessage.getSize().y));
			text.setFont(this->font);
			text.setString(g->getdisplaystring());
			text.setCharacterSize(45);
			text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
			text.setPosition(500, 420);
			message.setPosition(300, 300);
			w.draw(this->message);
			w.draw(this->text);
		}
		break;
	case 4:
		if (g->messagestate())
		{
			message.setTexture(this->lostmessage);
			message.setTextureRect(sf::IntRect(0, 0, lostmessage.getSize().x, lostmessage.getSize().y));
			text.setFont(this->font);
			text.setString(g->getdisplaystring());
			text.setCharacterSize(45);
			text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
			text.setPosition(500, 420);
			message.setPosition(300, 300);
			w.draw(this->message);
			w.draw(this->text);
		}
		break;
	}
}

void drawer::drawresult(game* g, sf::RenderWindow& w)
{
	std::string displaystring, guess;
	int beginposition = 400 - (85 * g->getlength() - 10) / 2;
	for (int i = 0; i < 6; i++)
	{
		guess = g->getguess(i);
		for (int j = 0; j < g->getlength(); j++)
		{
			switch (g->getresultstate(i, j))
			{
			case -1:
				wordblock.setTexture(this->wordblockinactive);
				break;
			case 0:
				wordblock.setTexture(this->wordblocknotexisted);
				break;
			case 1:
				wordblock.setTexture(this->wordblockwrongplace);
				break;
			case 2:
				wordblock.setTexture(this->wordblockcorrect);
				break;
			}
			wordblock.setPosition(beginposition + 85 * j, 50 + 85 * i);
			if (j < g->getguess(i).length()) displaystring = guess[j] - 32;
			else displaystring = "";
			text.setFont(this->font);
			text.setString(displaystring);
			text.setCharacterSize(45);
			text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
			text.setPosition(beginposition + 37 + 85 * j, 87 + 85 * i);
			w.draw(this->wordblock);
			w.draw(this->text);
		}
	}
}

void normalGame::insertcharacter(char ch)
{
	addcharacter(ch);
}

void shiftedGame::insertcharacter(char ch)
{
	ch -= 97;
	ch += shift + 26;
	ch = ch % 26;
	ch += 97;
	addcharacter(ch);
}

void hardGame::insertcharacter(char ch)
{
	addcharacter(ch);
}