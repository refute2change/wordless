#include <cstdlib>
#include <ctime>
#include "interface.h"

drawer* drawer::instance{nullptr};
std::mutex drawer::mtx;

int main()
{
    interface i;
    i.operate();
    // sf::RenderWindow w(sf::VideoMode(1000, 800), "Wordless", sf::Style::Close | sf::Style::Titlebar);
    // sf::Event ev;
    // std::vector<game*> t;
    // game* p = new game("app");
    // t.push_back(p);
    // drawer d;
    // bool focus = true;
    // while (w.isOpen())
    // {
    //     while (w.pollEvent(ev))
	// 	{
	// 		switch (ev.type)
	// 		{
	// 		case sf::Event::Closed:
	// 			w.close();
	// 			break;
	// 		case sf::Event::TextEntered:
	// 			if (ev.text.unicode == 13) t[0]->enterevent();
	// 			else if (ev.text.unicode == 8) t[0]->removecharacter();
	// 			else if ((ev.text.unicode >= 'A' && ev.text.unicode <= 'Z') || (ev.text.unicode >= 'a' && ev.text.unicode <= 'z'))
	// 			{
	// 				if (t[0]->result() != 0) break;
	// 				if (ev.text.unicode >= 'A' && ev.text.unicode <= 'Z') t[0]->insertcharacter(ev.text.unicode + 32);
	// 				else t[0]->insertcharacter(ev.text.unicode);
	// 			}
	// 			break;
	// 		case sf::Event::MouseButtonPressed:
	// 			break;
	// 		case sf::Event::GainedFocus:
	// 			focus = true;
	// 			break;
	// 		case sf::Event::LostFocus:
	// 			focus = false;
	// 			break;
	// 		}
	// 	}
    //     w.clear();
    //     d.draw(t[0], w);
    //     w.display();
    // }
    // delete t[0];
    return 0;
}
