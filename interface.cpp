#include "interface.h"

interface::interface()
{
	srand(time(NULL));
	totalwinmessage = new sf::Texture("Images/totalwinmessage.png");
	timerresetmessage = new sf::Texture("Images/timerresetmessage.png");
	newinform = new sf::Texture("Images/newinform.png");
	inform = new sf::Sprite(*newinform);
	inform->setPosition({50., 710.});
	resigntexture = new sf::Texture("Images/resignbutton.png");
	resetinactive = new sf::Texture("Images/resetinactive.png");
	resetactive = new sf::Texture("Images/resetactive.png");
	resignbutton = new sf::Sprite(*resigntexture);
	resignbutton->setPosition({920., 50.});
	resetbutton = new sf::Sprite(*resetinactive);
	resetbutton->setPosition({810., 50.});
	message = new sf::Sprite(*totalwinmessage);
	if (!font.openFromFile("Fonts/ebrimabd.ttf"));
	min = new sf::Text(font);
	sec = new sf::Text(font);
	typeable = false;
	resignavailable = false;
	resetavailable = false;
	resigned = false;
	finished = true;
	detention = true;
	// struct stat sb;
	// if (stat("C:/Wordless", &sb) != 0)
	// {
	// 	std::filesystem::create_directories("C:/Wordless");
	// 	SetFileAttributesA("C:/Wordless", FILE_ATTRIBUTE_HIDDEN);
	// 	std::ofstream fa, fb;
	// 	fa.open("C:/Wordless/gamedata.txt");
	// 	fb.open("C:/Wordless/unfinishedgame.txt");
	// 	fa << 0 << '\n' << 0;
	// 	fb << 0;
	// 	fa.close();
	// 	fb.close();
	// }
	readgame();
	res = 0;
	if (handler.remainingtime() == 0) resetavailable = true;
}

void interface::readgame()
{
	bool started, switchedoff, isGauntlet;
	int turns, mode, shift, allowedtime, remainingtime;
	game* gamePtr = nullptr;
	std::ifstream history("C:/Wordless/unfinishedgame.txt");
	history >> readavailable;
	std::string inputstrings;
	std::vector < std::string > previousanswers;
	std::getline(history, temp);
	if (!readavailable)
	{
		history.close();
		return;
	}
	for (int i = 0; i < 6; i++)
	{
		typeable = true;
		resignavailable = true;
		resetavailable = false;
		resigned = false;
		finished = false;
		detention = false;
		history >> mode;
		if (mode == 2 || mode == 3) history >> shift;
		else if (mode == 4 || mode == 5) history >> allowedtime >> remainingtime;
		else if (mode == 6 || mode == 7) history >> shift >> allowedtime >> remainingtime;
		modes.push_back(mode);
		history >> started >> switchedoff >> isGauntlet >> turns;
		std::getline(history, temp);
		std::getline(history, temp);
		previousanswers.clear();
		for (int j = 0; j < 6; j++)
		{
			std::getline(history, inputstrings);
			previousanswers.push_back(inputstrings);
		}
		if (modes[i] == 0) gamePtr = new normalGame(temp, previousanswers, started, switchedoff, isGauntlet, turns);
		else if (modes[i] == 1) gamePtr = new hardGame(temp, previousanswers, started, switchedoff, isGauntlet, turns);
		else if (modes[i] == 2) gamePtr = new shiftedGame(temp, previousanswers, started, switchedoff, isGauntlet, turns, shift);
		else if (modes[i] == 3) gamePtr = new hardshiftedGame(temp, previousanswers, started, switchedoff, isGauntlet, turns, shift);
		else if (modes[i] == 4) 
		{
			gamePtr = new timedGame(temp, previousanswers, started, switchedoff, isGauntlet, turns, allowedtime, remainingtime);
			gamePtr->begin = false;
		}
		else if (modes[i] == 5)
		{
			gamePtr = new hardtimedGame(temp, previousanswers, started, switchedoff, isGauntlet, turns, allowedtime, remainingtime);
			gamePtr->begin = false;
		}
		else if (modes[i] == 6)
		{
			gamePtr = new shiftedtimedGame(temp, previousanswers, started, switchedoff, isGauntlet, turns, shift, allowedtime, remainingtime);
			gamePtr->begin = false;
		}
		else if (modes[i] == 7)
		{
			gamePtr = new hardshiftedtimedGame(temp, previousanswers, started, switchedoff, isGauntlet, turns, shift, allowedtime, remainingtime);
			gamePtr->begin = false;
		}
		// gamePtr = new normalGame(temp, previousanswers, started, turns);
		gamez.push_back(gamePtr);
		// gamez.push_back(game(temp, previousanswers, started, turns));
	}
	// for (int i = 0; i < 6; i++) gamez.push_back(&gamez[i]);
	history.close();
}

void interface::generate()
{
	game* gamePtr = nullptr;
	int mode, shift;
	if (handler.remainingtime()) return;
	typeable = false;
	resignavailable = false;
	resetavailable = true;
	resigned = false;
	finished = true;
	detention = false;
	words = g.generator();
	std::random_device dev;
    std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist1(0, 7);
	for (int i = 0; i < 6; i++)
	{
		mode = dist1(rng);
		if (mode == 0) gamePtr = new normalGame(words[i]);
		else if (mode == 1) gamePtr = new hardGame(words[i]);
		else if (mode == 2)
		{
			shift = 0;
			while (shift == 0) shift = (rand() % 51) - 25;
			gamePtr = new shiftedGame(words[i], shift);
		}
		else if (mode == 3)
		{
			shift = 0;
			while (shift == 0) shift = (rand() % 51) - 25;
			gamePtr = new hardshiftedGame(words[i], shift);
		}
		else if (mode == 4)
		{
			gamePtr = new timedGame(words[i], 45 * 1000);
		}
		else if (mode == 5)
		{
			gamePtr = new hardtimedGame(words[i], 75 * 1000);
		}
		else if (mode == 6)
		{
			shift = 0;
			while (shift == 0) shift = (rand() % 51) - 25;
			gamePtr = new shiftedtimedGame(words[i], shift, 60 * 1000);
		}
		else if (mode == 7)
		{
			shift = 0;
			while (shift == 0) shift = (rand() % 51) - 25;
			gamePtr = new hardshiftedtimedGame(words[i], shift, 90 * 1000);
		}
		modes.push_back(mode);
		// gamePtr = new normalGame(words[i]);
		gamez.push_back(gamePtr);
		// gamez.push_back(game(words[i]));
		// gamePtr = &gamez[i];
	}
	// for (int i = 0; i < 6; i++) gamez.push_back(&gamez[i]);
}

void interface::operate()
{
	while (w.isOpen())
	{
		while (const std::optional event = w.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) w.close();
			else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
			{
				if (!typeable);
				else if (gamez.size() == 0);
				else if (textEntered->unicode == 13) gamez[active]->enterevent();
				else if (textEntered->unicode == 8) gamez[active]->removecharacter();
				else if ((textEntered->unicode >= 'A' && textEntered->unicode <= 'Z') || (textEntered->unicode >= 'a' && textEntered->unicode <= 'z'))
				{
					if (gamez[active]->isGauntlet() && !(gamez[active]->begin || gamez[active]->begintosave)) activategauntlet(gamez[active]);
					if (gamez[active]->result() != 0) break;
					if (textEntered->unicode >= 'A' && textEntered->unicode <= 'Z') gamez[active]->insertcharacter(textEntered->unicode + 32);
					else gamez[active]->insertcharacter(textEntered->unicode);
					
				}
			}
			else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (!focus) break;
				if (messageavailable) messageavailable = false;
				else if (informhit()) messageavailable = true;
				if (gamez.size())
				{
					if (gamez[active]->messagestate()) gamez[active]->hidemessage(); //***
					for (int i = 0; i < 6; i++)
					{
						if (gamez[i]->isHit(w))
						{
							if (active == i) continue;
							gamez[active]->turnofftimer();
							gamez[active]->quit();
							gamez[i]->turnontimer();
							gamez[i]->turnoffstall();
							active = i;
						}
					}
				}
			}
			else if (event->is<sf::Event::FocusLost>()) focus = false;
			else if (event->is<sf::Event::FocusGained>()) focus = true;
        }
		if (gamez.size() != 0) gamez[active]->updateremainingtime();
		for (int i = 0; i < gamez.size(); i++) gamez[i]->updatestall();
		if (isInGauntlet)
		{
			if (gamez[active]->result() != 0) deactivategauntlet();
		}
		w.clear();
		draw();
		resign();
		resetcheck();
		reset();
		if (gamez.size() == 6) handleendevent();
		w.display();
	}
	handler.savelostgames();
	for (int i = 0; i < gamez.size(); i++) if (gamez[i]->exitisdeath()) gamez[i]->switchedoff = true;
	savegame();
}

void interface::draw()
{
	if (resetavailable) resetbutton->setTexture(*resetactive);
	else resetbutton->setTexture(*resetinactive);
	if (resignavailable) w.draw(*resignbutton);
	w.draw(*resetbutton);
	this->drawclock();
	if (gamez.size() == 0) return;
	for (int i = 0; i < 6; i++) drawer::getInstance()->drawstate(gamez[i], w, active);
	drawer::getInstance()->draw(gamez[active], w);
	if (informavailable) w.draw(*inform);
	if (messageavailable) w.draw(*message);
}

int interface::finalresult()
{
	if (gamez.size() == 0) return 0;
	bool notcomplete = false, failed = false;
	int temp;
	if (resigned)
	{
		finished = true;
		return -1;
	}
	for (int i = 0; i < 6; i++)
	{
		temp = gamez[i]->result();
		if (temp == -1)
		{
			if (firstlost == 0) firstlost = gamez[i]->getfinishedtime();
			failed = true;
			continue;
		}
		if (temp == 0) notcomplete = true;
	}
	if (notcomplete) return 0;
	finished = true;
	if (failed) return -1;
	return 1;
}

bool interface::resignhit()
{
	if (!resignavailable) return false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		int x = sf::Mouse::getPosition(w).x, y = sf::Mouse::getPosition(w).y;
		if (x >= resignbutton->getPosition().x && y >= resignbutton->getPosition().y && x <= resignbutton->getPosition().x + 60 && y <= resignbutton->getPosition().y + 60)
		{
			resignavailable = false;
			count++;
			return true;
		}
	}
	return false;
}

void interface::resign()
{
	if (!focus) return;
	if (resignhit())
	{
		resigned = true;
		for (game* i: gamez)
		{
			if (i->result() == 0) i->resigned = true;
			i->permanentturnoff();
		}
		if (isInGauntlet) deactivategauntlet();
		std::cout << "RESIGNED\n";
	}
}

bool interface::resethit()
{
	if (!resetavailable) return false;
	if (handler.remainingtime() != 0) return false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		int x = sf::Mouse::getPosition(w).x, y = sf::Mouse::getPosition(w).y;
		if (x >= resetbutton->getPosition().x && y >= resetbutton->getPosition().y && x <= resetbutton->getPosition().x + 100 && y <= resetbutton->getPosition().y + 60)
		{
			resignavailable = false;
			return true;
		}
	}
	return false;
}

void interface::reset()
{
	if (!focus) return;
	if (resethit())
	{
		std::cout << "RESET\n";
		if (gamez.size() != 0) for (int i = 0; i < 6; i++)
		{
			delete gamez[i];
			gamez[i] = nullptr;
		}
		gamez.clear();
		modes.clear();
		generate();
		resigned = false;
		resetavailable = false;
		resignavailable = true;
		finished = false;
		typeable = true;
		detention = false;
		informavailable = false;
		messageavailable = false;
		active = 0;
		
		mode = 1;
	}
}

bool interface::informhit()
{
	if (!informavailable) return false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		int x = sf::Mouse::getPosition(w).x, y = sf::Mouse::getPosition(w).y;
		if ((x - 70) * (x - 70) + (y - 730) * (y - 730) <= 400)
		{
			return true;
		}
	}
	return false;
}

void interface::win()
{
	handler.handleWingame();
}

void interface::lose(time_t stamp)
{
	if (detention) return;
	detention = true;
	handler.handleLostgame(stamp);
}

void interface::handleendevent()
{
	time(&timestamp);
	switch (finalresult())
	{
	case -1:
		typeable = false;
		resignavailable = false;
		informavailable = true;
		message->setTexture(*timerresetmessage);
		message->setPosition({300., 300.});
		lose(timestamp);
		break;
	case 1:
		typeable = false;
		resignavailable = false;
		informavailable = true;
		message->setTexture(*totalwinmessage);
		message->setPosition({300., 300.});
		win();
		break;
	}
}

void interface::drawclock()
{
	if (!min) min = new sf::Text(font);
	if (!sec) sec = new sf::Text(font);
	if (resetavailable)
	{
		return;
	}
	if (handler.remainingtime() == 0 && finalresult() == 0)
	{
		mins = "-";
		secs = "--";
	}
	if (handler.remainingtime() != 0)
	{
		int minute = handler.minutes(), seconds = handler.seconds();
		mins = "";
		mins += std::to_string(minute);
		secs = std::to_string(seconds);
		if (seconds < 10) secs = "0" + secs;
	}
	min->setFont(font);
	min->setString(mins);
	min->setCharacterSize(30);
	min->setOrigin(min->getLocalBounds().getCenter());
	switch (mins.length())
	{
	case 1:
		min->setPosition({856., 80.});
		break;
	case 2:
		min->setPosition({850., 80.});
		break;
	case 3:
		min->setPosition({841., 80.});
		break;
	}
	
	min->setFillColor(sf::Color::Black);
	sec->setFont(font);
	sec->setString(secs);
	sec->setCharacterSize(30);
	sec->setOrigin(sec->getLocalBounds().getCenter());
	sec->setPosition({890., 80.});
	sec->setFillColor(sf::Color::Black);
	w.draw(*min);
	w.draw(*sec);
}

void interface::resetcheck()
{
	if (handler.remainingtime() != 0) resetavailable = false;
	else if (finished) resetavailable = true;
	else resetavailable = false;
}

void interface::savegame()
{
	std::ofstream history("C:/Wordless/unfinishedgame.txt");
	bool saved = false;
	if (gamez.size() == 0)
	{
		history << 0;
		return;
	}
	for (int i = 0; i < 6; i++) 
		if (gamez[i]->begin || gamez[i]->begintosave)
		{
			saved = true;
			break;
		}
	if (!saved || detention) history << 0;
	else if (finished) history << 0;
	else
	{
		history << 1 << '\n';
		for (int i = 0; i < 6; i++)
		{
			history << modes[i];
			if (modes[i] == 0) history << "\n";
			else if (modes[i] == 1) history << "\n";
			else if (modes[i] == 2) history << " " << gamez[i]->getShift() << "\n";
			else if (modes[i] == 3) history << " " << gamez[i]->getShift() << "\n";
			else if (modes[i] == 4) history << "\n" << gamez[i]->getmaxtime() << " " << gamez[i]->getremainingtime() << '\n';
			else if (modes[i] == 5) history << "\n" << gamez[i]->getmaxtime() << " " << gamez[i]->getremainingtime() << '\n';
			else if (modes[i] == 6) history << " " << gamez[i]->getShift() << "\n" << gamez[i]->getmaxtime() << " " << gamez[i]->getremainingtime() << '\n';
			else if (modes[i] == 7) history << " " << gamez[i]->getShift() << "\n" << gamez[i]->getmaxtime() << " " << gamez[i]->getremainingtime() << '\n';
			history << (gamez[i]->begin || gamez[i]->begintosave) << " " << gamez[i]->switchedoff << " " << gamez[i]->isGauntlet() << '\n';
			history << gamez[i]->turn << '\n';
			history << gamez[i]->getanswer() << '\n';
			std::vector<std::string> answers = gamez[i]->getanswers();
			for (int j = 0; j < 6; j++)
			{
				history << answers[j] << '\n';
			}
		}
	}
	history.close();
}

void interface::activategauntlet(game* gauntlet)
{
	for (game* i: gamez)
	{
		if (gauntlet == i) i->setaccessible();
		else i->setinaccessible();
	}
	isInGauntlet = true;
}

void interface::deactivategauntlet()
{
	for (game* i: gamez) i->setaccessible();
	isInGauntlet = false;
}