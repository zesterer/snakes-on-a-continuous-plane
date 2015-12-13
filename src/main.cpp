#include "stdio.h"
#include "vector"
#include "list"
#include "cstdlib"
#include "ctime"

#include "SFML/Graphics.hpp"

#include "snake.h"
#include "game.h"

struct MenuItem
{
	std::string text;
	sf::Color col;
};

int askMenu(sf::RenderWindow& window, WindowInfo& window_info, std::vector<MenuItem>& items)
{
	int item_select = 1;
	
	sf::Font menu_font;
	menu_font.loadFromFile("../data/LiquidCrystal-Normal.otf");
	
	while (true)
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
				exit(EXIT_SUCCESS);
			}
			
			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Key::Down)
					item_select = (item_select - 1 + items.size()) % (items.size() - 1) + 1;
				if (e.key.code == sf::Keyboard::Key::Up)
					item_select = (item_select - 3 + items.size()) % (items.size() - 1) + 1;
				if (e.key.code == sf::Keyboard::Key::Return)
					return item_select - 1;
			}
		}
		
		window.clear(sf::Color(100, 65, 25));
		
		for (int i = 0; i < (int)items.size(); i ++)
		{
			sf::Text text_item;
			text_item.setFont(menu_font);
			text_item.setString(items[i].text);
			
			if (i == item_select)
				text_item.setColor(sf::Color::White);
			else
				text_item.setColor(items[i].col);
			text_item.setCharacterSize(24);
			text_item.setPosition(64, 64 + 64 * i);
			window.draw(text_item);
		}
			
		window.display();
	}
}

int main(int argc, char* argv[])
{
	WindowInfo window_info;
	
	sf::RenderWindow window(sf::VideoMode(window_info.w, window_info.h), "Continuous Snake");
	window.setFramerateLimit(60);
	
	sf::View main_view(sf::FloatRect(0, 0, window_info.w, window_info.h));
	window.setView(main_view);
	
	Game game(4, window_info);

	while (true)
	{
		bool close = false;
		int blink_time = 0;
		
		sf::Texture texture;
		texture.loadFromFile("../data/title.png");
		sf::Sprite title;
		title.setTexture(texture);
		title.setPosition(120, 32);
		
		sf::Font menu_font;
		menu_font.loadFromFile("../data/LiquidCrystal-Normal.otf");
		
		sf::Text text_item;
		text_item.setFont(menu_font);
		text_item.setString("Press Any Key To Continue");
		
		text_item.setCharacterSize(40);
		text_item.setPosition(215, 500);
		
		while (true)
		{
			sf::Event e;
			while (window.pollEvent(e))
			{
				if (e.type == sf::Event::Closed)
				{
					window.close();
					exit(EXIT_SUCCESS);
				}
				
				if (e.type == sf::Event::KeyPressed)
				{
					close = true;
				}
			}
			
			text_item.setColor(sf::Color(sin(blink_time / 20.0f) * 128 + 128, 0, 0));
			
			window.clear(sf::Color(100, 65, 25));
			window.draw(title);
			window.draw(text_item);
			window.display();
			
			blink_time ++;
			
			if (close)
				break;
		}
		
		std::vector<MenuItem> items;
		items.push_back({"Choose The Number Of Players", sf::Color::Green});
		items.push_back({"1 player (LEFT/RIGHT)", sf::Color::Blue});
		items.push_back({"2 player (A/D)", sf::Color::Red});
		items.push_back({"3 player (J/L)", sf::Color::Blue});
		items.push_back({"4 player (C/B)", sf::Color::Red});
		items.push_back({"5 player (5/7)", sf::Color::Blue});
		items.push_back({"6 player (I/P)", sf::Color::Red});
		game.setPlayerNumber(askMenu(window, window_info, items) + 1, window_info);
		
		std::vector<MenuItem> items_points;
		int points[6] = {1, 3, 5, 10, 25, 100};
		items_points.push_back({"How many Points Needed To Win?", sf::Color::Green});
		items_points.push_back({"1 Point", sf::Color::Blue});
		items_points.push_back({"3 Points", sf::Color::Red});
		items_points.push_back({"5 Points", sf::Color::Blue});
		items_points.push_back({"10 Points", sf::Color::Red});
		items_points.push_back({"25 Points", sf::Color::Blue});
		items_points.push_back({"100 Points", sf::Color::Red});
		game.setMaxPoints(points[askMenu(window, window_info, items_points)]);
		
		while (window.isOpen())
		{
			if (game.tick(window, window_info))
				break;
		}
		
		for (int i = 0; i < (int)game.snakes.size(); i ++)
		{
			Snake& snake = game.snakes[i];
			if (snake.points >= game.max_points)
			{
				std::vector<MenuItem> won_items;
				won_items.push_back({"Player " + std::to_string(i + 1) + " won!", snake.col});
				won_items.push_back({"Continue", sf::Color::Blue});
				won_items.push_back({"Quit", sf::Color::Red});
				switch(askMenu(window, window_info, won_items))
				{
				case 0:
					break;
				case 1:
					exit(EXIT_SUCCESS);
				}
			}
		}
	}
	
	return 0;
}
