#include "game.h"

//template functions

game::game()
{

}

game::game(std::string word)
{
	answer = word;
	length = answer.length();
	std::random_device dev;
    std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist1(0, 1);
	gauntlet = dist1(rng);
	gameblock.setSize(sf::Vector2f(175.f, 75.f));
	gameblock.setPosition({810., (float)(150 + 100 * (length - 3))});
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

game::game(std::string word, std::vector<std::string> guessessofar, bool started, bool off, int turns)
{
	answer = word;
	length = answer.length();
	gameblock.setSize(sf::Vector2f(175.f, 75.f));
	gameblock.setPosition({810., float(150 + 100 * (length - 3))});
	begin = started;
	begintosave = started;
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
	switchedoff = off;
}

game::game(std::string word, std::vector<std::string> guessessofar, bool started, bool off, bool isGauntlet, int turns)
{
	answer = word;
	length = answer.length();
	gauntlet = isGauntlet;
	gameblock.setSize(sf::Vector2f(175.f, 75.f));
	gameblock.setPosition({810., float(150 + 100 * (length - 3))});
	begin = started;
	begintosave = started;
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
	switchedoff = off;
}

//add a charracter to active guess
void game::addcharacter(char ch)
{
	if (result() != 0) return;
	if (guesses[turn].length() < length) guesses[turn] += ch;
	if (!begin) begin = true;
}

//remove a char from current guess
void game::removecharacter()
{
	if (result() != 0) return;
	if (guesses[turn].length() != 0)
	{
		std::string temp = guesses[turn].substr(0, guesses[turn].length() - 1);
		guesses[turn] = temp;
	}
}

//check for valid guess
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

//invoked when press enter
void game::enterevent()
{
	if (turn == 6) return;
	if (result() != 0) return;
	if (guesses[turn].length() != length) return;
	for (int i = 0; i < turn; i++) if (guesses[turn] == guesses[i])
	{
		messagetype = 2;
		return;
	}
	if (!existcheck())
	{
		messagetype = 1;
		return;
	}
	checkguess();
}

//after valid check, this is called to check for the result of the guess
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

//flip state of game once game is over
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

//result fetch
const int game::result()
{
	if (resigned) return -1;
	if (!(begin || begintosave)) return 0;
	for (int i = 0; i < turn; i++)
	{
		if (guesses[i] == answer) return 1;
	}
	if (turn == 6)
	{
		return -1;
	}
	if (switchedoff) return -1;
	return 0;
}

void game::quit()
{
	return;
}

int game::getfinishedtime()
{
	return finishedtime;
}

//get list of guesses
std::vector<std::string> game::getanswers()
{
	return guesses;
}

//answer of game
std::string game::getanswer()
{
	return answer;
}

//state button check
const bool game::isHit(sf::RenderWindow& w)
{
	if (!accessible) return false;
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) return false;
	int x = sf::Mouse::getPosition(w).x, y = sf::Mouse::getPosition(w).y;
	return (x >= gameblock.getPosition().x && y >= gameblock.getPosition().y && x < gameblock.getPosition().x + gameblock.getSize().x && y < gameblock.getPosition().y + gameblock.getSize().y);
}

//resign from game
void game::resign()
{
	resigned = true;
	flipstate();
}

//drawer functions
drawer::drawer()
{
	wordblockinactive = new sf::Texture("Images/wordblockinactive.png");
	wordblocknotexisted = new sf::Texture("Images/wordblocknotexisted.png");
	wordblockwrongplace = new sf::Texture("Images/wordblockwrongplace.png");
	wordblockcorrect = new sf::Texture("Images/wordblockcorrect.png");
	characterblockinactive = new sf::Texture("Images/characterblockinactive.png");
	characterblocknotexisted = new sf::Texture("Images/characterblocknotexisted.png");
	characterblockwrongplace = new sf::Texture("Images/characterblockwrongplace.png");
	characterblockcorrect = new sf::Texture("Images/characterblockcorrect.png");
	gamenotstarted = new sf::Texture("Images/gamenotstarted.png");
	gameactive = new sf::Texture("Images/gameactive.png");
	gamefailed = new sf::Texture("Images/gamefailed.png");
	gamestall = new sf::Texture("Images/gamestall.png");
	notvalidguess = new sf::Texture("Images/invalidguess.png");
	alreadyguessed = new sf::Texture("Images/repetitiveguess.png");
	winmessage = new sf::Texture("Images/gamewinmessage.png");
	lostmessage = new sf::Texture("Images/gamelostmessage.png");
	gamewon = new sf::Texture("Images/gamewon.png");
	if (!font.openFromFile("Fonts/ebrimabd.ttf"));
	if (!notactivefont.openFromFile("Fonts/ebrima.ttf"));
	text = new sf::Text(font);
	text->setFillColor(sf::Color::Black);
	wordblock = new sf::Sprite(*wordblockinactive);
	characterblock = new sf::Sprite(*characterblockinactive);
	gameblock = new sf::Sprite(*gamenotstarted);
	message = new sf::Sprite(*notvalidguess);
}

void drawer::draw(game* g, sf::RenderWindow& w)
{
	drawer::getInstance()->drawkeyboard(g, w);
	drawer::getInstance()->drawresult(g, w);
	drawer::getInstance()->drawmessage(g, w);
}

void drawer::drawkeyboard(game* g, sf::RenderWindow& w)
{
	characterblock = new sf::Sprite(*characterblockinactive);
	int x;
	char temp;
	x = 0;
	for (char c : top)
	{
		c -= 97;
		c += g->getShift() + 26;
		c = c % 26;
		c += 97;
		switch (g->getcharacterstate(c))
		{
		case -1:
			characterblock->setTexture(*characterblockinactive);
			break;
		case 0:
			characterblock->setTexture(*characterblocknotexisted);
			break;
		case 1:
			characterblock->setTexture(*characterblockwrongplace);
			break;
		case 2:
			characterblock->setTexture(*characterblockcorrect);
			break;
		}
		characterblock->setPosition({(float)(150 + 50 * x), (float)590});
		text->setFont(font);
		temp = toupper(c);
		text->setString(temp);
		text->setCharacterSize(25);
		text->setOrigin(text->getLocalBounds().getCenter());
		text->setPosition({(float)(172 + 50 * x), (float)612});
		w.draw(*characterblock);
		w.draw(*text);
		x++;
	}
	x = 0;
	for (char c : mid)
	{
		c -= 97;
		c += g->getShift() + 26;
		c = c % 26;
		c += 97;
		switch (g->getcharacterstate(c))
		{
		case -1:
			characterblock->setTexture(*characterblockinactive);
			break;
		case 0:
			characterblock->setTexture(*characterblocknotexisted);
			break;
		case 1:
			characterblock->setTexture(*characterblockwrongplace);
			break;
		case 2:
			characterblock->setTexture(*characterblockcorrect);
			break;
		}
		characterblock->setPosition({(float)(165 + 50 * x), (float)640});
		text->setFont(font);
		
		temp = toupper(c);
		text->setString(temp);
		text->setCharacterSize(25);
		text->setOrigin(text->getLocalBounds().getCenter());
		text->setPosition({(float)(187 + 50 * x), (float)662});
		w.draw(*characterblock);
		w.draw(*text);
		x++;
	}
	x = 0;
	for (char c : bot)
	{
		c -= 97;
		c += g->getShift() + 26;
		c = c % 26;
		c += 97;
		switch (g->getcharacterstate(c))
		{
		case -1:
			characterblock->setTexture(*characterblockinactive);
			break;
		case 0:
			characterblock->setTexture(*characterblocknotexisted);
			break;
		case 1:
			characterblock->setTexture(*characterblockwrongplace);
			break;
		case 2:
			characterblock->setTexture(*characterblockcorrect);
			break;
		}
		characterblock->setPosition({(float)(195 + 50 * x), (float)690});
		text->setFont(font);
		
		temp = toupper(c);
		text->setString(temp);
		text->setCharacterSize(25);
		text->setOrigin(text->getLocalBounds().getCenter());
		text->setPosition({(float)(217 + 50 * x), (float)712});
		w.draw(*characterblock);
		w.draw(*text);
		x++;
	}
}

void drawer::drawstate(game* g, sf::RenderWindow& w, int atgame)
{
	if (!g->isaccessible()) return;
	std::string temp;
	// std::string temp = std::to_string(g->getremainingtime());
	// std::string temp = std::to_string(g->exitisdeath());
	if (g->result() == 0) temp = std::to_string(g->getlength()) + " letters";
	else temp = g->getanswer();
	if (atgame == g->getlength() - 3) text->setFont(font);
	else text->setFont(notactivefont);
	text->setString(temp);
	if (g->result() == 0) text->setCharacterSize(45);
	else text->setCharacterSize(40);
	text->setOrigin(text->getLocalBounds().getCenter());
	text->setPosition({(float)898, (float)(187 + 100 * (g->getlength() - 3))});
	if (g->isGauntlet())
	{
		gameblock->setTexture(*gamefailed);
		gameblock->setScale(sf::Vector2f(1.0, 1.0));
		gameblock->setPosition({(float)810, (float)(150 + 100 * (g->getlength() - 3))});
		w.draw(*gameblock);
	}
	if (g->getmaxtime() != 0)
	{
		gameblock->setTexture(*gamefailed);
		gameblock->setScale(sf::Vector2f(1.0, 1.0));
		gameblock->setPosition({(float)810, (float)(150 + 100 * (g->getlength() - 3))});
		w.draw(*gameblock);
	}
	
	if (atgame == g->getlength() - 3 && g->result() == 0) gameblock->setTexture(*gameactive);
	else if (!g->result()) gameblock->setTexture(*gamenotstarted);
	else if (g->result() == -1) gameblock->setTexture(*gamefailed);
	else gameblock->setTexture(*gamewon);
	if (g->getmaxtime() != 0 && g->result() == 0)
	{
		float scale = (float)g->getremainingtime() / g->getmaxtime();
		//gameblock.setScale(sf::Vector2f(scale, 1.0f));
		gameblock->setTextureRect(sf::IntRect({0, 0}, {float(scale * gameblock->getTexture().getSize().x), float(gameblock->getTexture().getSize().y)}));
		gameblock->setPosition({(float)810, float(150 + 100 * (g->getlength() - 3))});
	}
	else
	{
		// gameblock.setScale(sf::Vector2f(1.0, 1.0));
		gameblock->setTextureRect(sf::IntRect({0, 0}, {float(gameblock->getTexture().getSize().x), float(gameblock->getTexture().getSize().y)}));
		gameblock->setPosition({(float)810, (float)(150 + 100 * (g->getlength() - 3))});
	}
	w.draw(*gameblock);
	if (g->isDeadable() != 0)
	{
		float scale = (float)g->getremainingstall() / (15 * CLOCKS_PER_SEC);
		gameblock->setTexture(*gamestall);
		// gameblock.setScale(sf::Vector2f(scale, 1.0f));
		gameblock->setTextureRect(sf::IntRect({0, 0}, {scale * gameblock->getTexture().getSize().x, float(gameblock->getTexture().getSize().y)}));
		gameblock->setPosition({(float)810, float(150 + 100 * (g->getlength() - 3))});
		w.draw(*gameblock);
	}
	w.draw(*text);
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
		message->setTexture(*this->notvalidguess);
		message->setTextureRect(sf::IntRect({0, 0}, {float(notvalidguess->getSize().x), float(notvalidguess->getSize().y)}));
		message->setPosition({200., 265.});
		w.draw(*this->message);
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
		message->setTexture(*this->alreadyguessed);
		message->setTextureRect(sf::IntRect({0, 0}, {float(alreadyguessed->getSize().x), float(alreadyguessed->getSize().y)}));
		message->setPosition({200., 265.});
		w.draw(*this->message);
		break;
	case 3:
		if (g->messagestate())
		{
			message->setTexture(*this->winmessage);
			message->setTextureRect(sf::IntRect({0, 0}, {float(winmessage->getSize().x), float(winmessage->getSize().y)}));
			text->setFont(this->font);
			text->setString(g->getdisplaystring());
			text->setCharacterSize(45);
			text->setOrigin(text->getLocalBounds().getCenter());
			text->setPosition({500., 420.});
			message->setPosition({300., 300.});
			w.draw(*this->message);
			w.draw(*this->text);
		}
		break;
	case 4:
		if (g->messagestate())
		{
			message->setTexture(*this->lostmessage);
			message->setTextureRect(sf::IntRect({0, 0}, {float(lostmessage->getSize().x), float(lostmessage->getSize().y)}));
			text->setFont(this->font);
			text->setString(g->getdisplaystring());
			text->setCharacterSize(45);
			text->setOrigin(text->getLocalBounds().getCenter());
			text->setPosition({500., 420.});
			message->setPosition({300., 300.});
			w.draw(*this->message);
			w.draw(*this->text);
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
				wordblock->setTexture(*this->wordblockinactive);
				break;
			case 0:
				wordblock->setTexture(*this->wordblocknotexisted);
				break;
			case 1:
				wordblock->setTexture(*this->wordblockwrongplace);
				break;
			case 2:
				wordblock->setTexture(*this->wordblockcorrect);
				break;
			}
			wordblock->setPosition({float(beginposition + 85 * j), float(50 + 85 * i)});
			if (j < g->getguess(i).length()) displaystring = guess[j] - 32;
			else displaystring = "";
			text->setFont(this->font);
			text->setString(displaystring);
			text->setCharacterSize(45);
			text->setOrigin(text->getLocalBounds().getCenter());
			text->setPosition({(float)(beginposition + 37 + 85 * j), (float)(87 + 85 * i)});
			w.draw(*this->wordblock);
			w.draw(*this->text);
		}
	}
}

//normalGame
void normalGame::insertcharacter(char ch)
{
	addcharacter(ch);
}

//shiftedGame
void shiftedGame::insertcharacter(char ch)
{
	ch -= 97;
	ch += shift + 26;
	ch = ch % 26;
	ch += 97;
	addcharacter(ch);
}

//hardGame
bool hardGame::validcheck()
{
	notchecked.clear();
	if (turn == 0) return true;
	for (int i = 0; i < getlength(); i++)
	{
		if (fixedcharacters[i] && getguess(turn)[i] != getguess(turn - 1)[i]) return false;
		if (fixedcharacters[i]) continue;
		notchecked[getguess(turn)[i]]++;
	}
	std::map<char, int>::iterator it;
	for (it = neededcharacters.begin(); it != neededcharacters.end(); it++) if (it->second > notchecked[it->first]) return false;
	return true;
}

void hardGame::enterevent()
{
	if (turn == 6) return;
	if (result() != 0) return;
	if (getguess(turn).length() != getlength()) return;
	for (int i = 0; i < turn; i++) if (getguess(turn) == getguess(i))
	{
		setmessagestate(2);
		return;
	}
	if (!existcheck())
	{
		setmessagestate(1);
		return;
	}
	if (!validcheck())
	{
		setmessagestate(1);
		return;
	}
	checkguess();
	neededcharacters.clear();
	for (int i = 0; i < getlength(); i++)
	{
		if (fixedcharacters[i]) continue;
		if (getresultstate(turn - 1, i) == 2) fixedcharacters[i] = true;
		if (getresultstate(turn - 1, i) == 1) neededcharacters[getguess(turn - 1)[i]]++;
	}
}

void hardGame::insertcharacter(char ch)
{
	if (!begin) turnontimer();
	addcharacter(ch);
}

//timedGame
void timedGame::updateremainingtime()
{
	if (result() != 0) return;
	if (!begin)
	{
		// begin = true;
		// starttimer = clock();
		return;
	}
	if (remainingtime <= 0) return;
	int temp = clock();
	if (!starttimer) return;
	remainingtime -= temp - starttimer;
	if (remainingtime <= 0)
	{
		remainingtime = 0;
		flipstate();
	}
	starttimer = temp;
}

void timedGame::turnontimer()
{
	if (!active) return;
	if (!begin) return;
	if (!starttimer) starttimer = clock();
}

void timedGame::turnofftimer()
{
	starttimer = 0;
}

void timedGame::permanentturnoff()
{
	turnofftimer();
	active = false;
}

const int timedGame::result()
{
	int t = game::result();
	if (t != 0) return t;
	if (remainingtime <= 0)
	{
		return -1;
	}
	return t;
}

void timedGame::quit()
{
	if (begin && result() == 0)
	{
		remainingtime = (int)(remainingtime * .75);
		if (active) startstall = clock();
	}
}

void timedGame::turnoffstall()
{
	startstall = 0;
	stalltimer = 15 * CLOCKS_PER_SEC;
}

void timedGame::updatestall()
{
	if (!startstall) return;
	int temp = clock();
	stalltimer -= temp - startstall;
	if (stalltimer <= 0)
	{
		startstall = 0;
		stalltimer = 0;
		switchedoff = true;
		flipstate();
	}
	startstall = temp;
}

//hardshiftedGame
void hardshiftedGame::insertcharacter(char ch)
{
	ch -= 97;
	ch += shift + 26;
	ch = ch % 26;
	ch += 97;
	addcharacter(ch);
}

void hardshiftedGame::enterevent()
{
	if (turn == 6) return;
	if (result() != 0) return;
	if (getguess(turn).length() != getlength()) return;
	for (int i = 0; i < turn; i++) if (getguess(turn) == getguess(i))
	{
		setmessagestate(2);
		return;
	}
	if (!existcheck())
	{
		setmessagestate(1);
		return;
	}
	if (!validcheck())
	{
		setmessagestate(1);
		return;
	}
	checkguess();
	neededcharacters.clear();
	for (int i = 0; i < getlength(); i++)
	{
		if (fixedcharacters[i]) continue;
		if (getresultstate(turn - 1, i) == 2) fixedcharacters[i] = true;
		if (getresultstate(turn - 1, i) == 1) neededcharacters[getguess(turn - 1)[i]]++;
	}
}

bool hardshiftedGame::validcheck()
{
	notchecked.clear();
	if (turn == 0) return true;
	for (int i = 0; i < getlength(); i++)
	{
		if (fixedcharacters[i] && getguess(turn)[i] != getguess(turn - 1)[i]) return false;
		if (fixedcharacters[i]) continue;
		notchecked[getguess(turn)[i]]++;
	}
	std::map<char, int>::iterator it;
	for (it = neededcharacters.begin(); it != neededcharacters.end(); it++) if (it->second > notchecked[it->first]) return false;
	return true;
}

//hardtimedGame
bool hardtimedGame::validcheck()
{
	notchecked.clear();
	if (turn == 0) return true;
	for (int i = 0; i < getlength(); i++)
	{
		if (fixedcharacters[i] && getguess(turn)[i] != getguess(turn - 1)[i]) return false;
		if (fixedcharacters[i]) continue;
		notchecked[getguess(turn)[i]]++;
	}
	std::map<char, int>::iterator it;
	for (it = neededcharacters.begin(); it != neededcharacters.end(); it++) if (it->second > notchecked[it->first]) return false;
	return true;
}

void hardtimedGame::enterevent()
{
	if (!begin)
	{
		begin = true;
		candie = true;
		turnontimer();
	}
	if (turn == 6) return;
	if (result() != 0) return;
	if (getguess(turn).length() != getlength()) return;
	for (int i = 0; i < turn; i++) if (getguess(turn) == getguess(i))
	{
		setmessagestate(2);
		return;
	}
	if (!existcheck())
	{
		setmessagestate(1);
		return;
	}
	if (!validcheck())
	{
		setmessagestate(1);
		return;
	}
	checkguess();
	neededcharacters.clear();
	for (int i = 0; i < getlength(); i++)
	{
		if (fixedcharacters[i]) continue;
		if (getresultstate(turn - 1, i) == 2) fixedcharacters[i] = true;
		if (getresultstate(turn - 1, i) == 1) neededcharacters[getguess(turn - 1)[i]]++;
	}
}

void hardtimedGame::updateremainingtime()
{
	if (result() != 0) return;
	if (!begin)
	{
		// begin = true;
		// starttimer = clock();
		return;
	}
	if (remainingtime <= 0) return;
	int temp = clock();
	if (!starttimer) return;
	remainingtime -= temp - starttimer;
	if (remainingtime <= 0)
	{
		remainingtime = 0;
		flipstate();
	}
	starttimer = temp;
}

void hardtimedGame::turnontimer()
{
	if (!active) return;
	if (!begin) return;
	if (!starttimer) starttimer = clock();
}

void hardtimedGame::turnofftimer()
{
	starttimer = 0;
}

void hardtimedGame::permanentturnoff()
{
	turnofftimer();
	active = false;
}

const int hardtimedGame::result()
{
	int t = game::result();
	if (t != 0) return t;
	if (remainingtime <= 0)
	{
		return -1;
	}
	return t;
}

void hardtimedGame::quit()
{
	if (begin && result() == 0)
	{
		remainingtime = (int)(remainingtime * .75);
		if (active) startstall = clock();
	}
}

void hardtimedGame::turnoffstall()
{
	startstall = 0;
	stalltimer = 15 * CLOCKS_PER_SEC;
}

void hardtimedGame::updatestall()
{
	if (!startstall) return;
	int temp = clock();
	stalltimer -= temp - startstall;
	if (stalltimer <= 0)
	{
		startstall = 0;
		stalltimer = 0;
		switchedoff = true;
		flipstate();
	}
	startstall = temp;
}

//shiftedtimedGame
void shiftedtimedGame::insertcharacter(char ch)
{
	if (!begin)
	{
		begin = true;
		candie = true;
		turnontimer();
	}
	ch -= 97;
	ch += shift + 26;
	ch = ch % 26;
	ch += 97;
	addcharacter(ch);
}

void shiftedtimedGame::updateremainingtime()
{
	if (result() != 0) return;
	if (!begin)
	{
		// begin = true;
		// starttimer = clock();
		return;
	}
	if (remainingtime <= 0) return;
	int temp = clock();
	if (!starttimer) return;
	remainingtime -= temp - starttimer;
	if (remainingtime <= 0)
	{
		remainingtime = 0;
		flipstate();
	}
	starttimer = temp;
}

void shiftedtimedGame::turnontimer()
{
	if (!active) return;
	if (!begin) return;
	if (!starttimer) starttimer = clock();
}

void shiftedtimedGame::turnofftimer()
{
	starttimer = 0;
}

void shiftedtimedGame::permanentturnoff()
{
	turnofftimer();
	active = false;
}

const int shiftedtimedGame::result()
{
	int t = game::result();
	if (t != 0) return t;
	if (remainingtime <= 0)
	{
		return -1;
	}
	return t;
}

void shiftedtimedGame::quit()
{
	if (begin && result() == 0)
	{
		remainingtime = (int)(remainingtime * .75);
		if (active) startstall = clock();
	}
}

void shiftedtimedGame::turnoffstall()
{
	startstall = 0;
	stalltimer = 15 * CLOCKS_PER_SEC;
}

void shiftedtimedGame::updatestall()
{
	if (!startstall) return;
	int temp = clock();
	stalltimer -= temp - startstall;
	if (stalltimer <= 0)
	{
		startstall = 0;
		stalltimer = 0;
		switchedoff = true;
		flipstate();
	}
	startstall = temp;
}

//hardshiftedtimedGame
bool hardshiftedtimedGame::validcheck()
{
	notchecked.clear();
	if (turn == 0) return true;
	for (int i = 0; i < getlength(); i++)
	{
		if (fixedcharacters[i] && getguess(turn)[i] != getguess(turn - 1)[i]) return false;
		if (fixedcharacters[i]) continue;
		notchecked[getguess(turn)[i]]++;
	}
	std::map<char, int>::iterator it;
	for (it = neededcharacters.begin(); it != neededcharacters.end(); it++) if (it->second > notchecked[it->first]) return false;
	return true;
}

void hardshiftedtimedGame::enterevent()
{
	if (!begin)
	{
		begin = true;
		candie = true;
		turnontimer();
	}
	if (turn == 6) return;
	if (result() != 0) return;
	if (getguess(turn).length() != getlength()) return;
	for (int i = 0; i < turn; i++) if (getguess(turn) == getguess(i))
	{
		setmessagestate(2);
		return;
	}
	if (!existcheck())
	{
		setmessagestate(1);
		return;
	}
	if (!validcheck())
	{
		setmessagestate(1);
		return;
	}
	checkguess();
	neededcharacters.clear();
	for (int i = 0; i < getlength(); i++)
	{
		if (fixedcharacters[i]) continue;
		if (getresultstate(turn - 1, i) == 2) fixedcharacters[i] = true;
		if (getresultstate(turn - 1, i) == 1) neededcharacters[getguess(turn - 1)[i]]++;
	}

}

void hardshiftedtimedGame::insertcharacter(char ch)
{
	if (!begin)
	{
		begin = true;
		candie = true;
		turnontimer();
	}
	ch -= 97;
	ch += shift + 26;
	ch = ch % 26;
	ch += 97;
	addcharacter(ch);
}

void hardshiftedtimedGame::updateremainingtime()
{
	if (result() != 0) return;
	if (!begin)
	{
		// begin = true;
		// starttimer = clock();
		return;
	}
	if (remainingtime <= 0) return;
	int temp = clock();
	if (!starttimer) return;
	remainingtime -= temp - starttimer;
	if (remainingtime <= 0)
	{
		remainingtime = 0;
		flipstate();
	}
	starttimer = temp;	
}

void hardshiftedtimedGame::turnontimer()
{
	if (!active) return;
	if (!begin) return;
	if (!starttimer) starttimer = clock();
}

void hardshiftedtimedGame::turnofftimer()
{
	starttimer = 0;
}

void hardshiftedtimedGame::permanentturnoff()
{
	turnofftimer();
	active = false;
}

const int hardshiftedtimedGame::result()
{
	int t = game::result();
	if (t != 0) return t;
	if (remainingtime <= 0)
	{
		return -1;
	}
	return t;
}

void hardshiftedtimedGame::quit()
{
	if (begin && result() == 0)
	{
		remainingtime = (int)(remainingtime * .75);
		if (active) startstall = clock();
	}
}

void hardshiftedtimedGame::turnoffstall()
{
	startstall = 0;
	stalltimer = 15 * CLOCKS_PER_SEC;
}

void hardshiftedtimedGame::updatestall()
{
	if (!startstall) return;
	int temp = clock();
	stalltimer -= temp - startstall;
	if (stalltimer <= 0)
	{
		startstall = 0;
		stalltimer = 0;
		switchedoff = true;
		flipstate();
	}
	startstall = temp;
}
