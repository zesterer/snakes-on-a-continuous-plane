#ifndef GAME_H
#define GAME_H

#include "stdio.h"
#include "vector"
#include "list"
#include "cstdlib"
#include "ctime"

#include "SFML/Graphics.hpp"

class Game
{
	public:
		sf::Font scores_font;
		
		std::vector<Snake> snakes;
		
		std::vector<Particle> particles;
		
		std::vector<Food> foods;
		
		int time = 0;
		int reset_timer = 0;
		
		int player_count = 0;
		int max_points;
	
	Game(int snake_num, WindowInfo& window_info)
	{
		this->setPlayerNumber(snake_num, window_info);
		
		scores_font.loadFromFile("../data/LiquidCrystal-Normal.otf");
		
		srand(1234);
	}
	
	void setMaxPoints(int points)
	{
		max_points = points;
	}
	
	void setPlayerNumber(int num, WindowInfo& window_info)
	{
		this->snakes.clear();
		
		player_count = num;
		
		sf::Color cols[7] = {sf::Color::Green, sf::Color::Red, sf::Color::Yellow, sf::Color::Blue, sf::Color::Magenta, sf::Color::Cyan, sf::Color::White};
		
		for (int i = 0; i < num; i ++)
		{
			snakes.emplace_back();
			snakes.back().col = cols[i % 7];
			snakes.back().use_ai = false;
			
			snakes.back().pos = Position(rand() % window_info.w, rand() % window_info.h);
			snakes.back().reset();
		}
		
		while (snakes.size() < 2)
		{
			snakes.emplace_back();
			snakes.back().col = cols[(snakes.size() - 1) % 7];
			snakes.back().use_ai = true;
		}
	}
	
	void drawSnake(sf::RenderWindow& window, Snake& snake)
	{
		if (snake.dead)
			return;
		
		sf::CircleShape circle;
		
		for (int i = (int)snake.segments.size() - 1; i >= 0; i --)
		{
			circle.setFillColor(snake.getSegment(i).col);
			
			if (i == 0)
				circle.setFillColor(snake.head_col);
			
			circle.setRadius(snake.getSegmentSnakeWidth(i) * snake.getSegment(i).width);
			circle.setPosition(snake.getSegment(i).pos.x - circle.getRadius(), snake.getSegment(i).pos.y - circle.getRadius());
			window.draw(circle);
		}
		
		circle.setFillColor(sf::Color(0, 0, 0));
		circle.setRadius(3.0f);
		
		if (snake.powerup == Powerup::Teeth)
			circle.setFillColor(sf::Color(255, 0, 0));
		
		if (snake.powerup == Powerup::Speed)
			circle.setFillColor(sf::Color(0, 0, 255));
		
		//Eye 0
		circle.setPosition(snake.pos.x + cos(snake.angle - 0.5f) * 12.0f - 3.0f, snake.pos.y + sin(snake.angle - 0.5f) * 12.0f - 3.0f);
		window.draw(circle);
		
		//Eye 1
		circle.setPosition(snake.pos.x + cos(snake.angle + 0.5f) * 12.0f - 3.0f, snake.pos.y + sin(snake.angle + 0.5f) * 12.0f - 3.0f);
		window.draw(circle);
	}
	
	bool tick(sf::RenderWindow& window, WindowInfo& window_info)
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
				exit(EXIT_SUCCESS);
			}
		}
		
		if (player_count >= 1)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				snakes[0].angle -= 0.08f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				snakes[0].angle += 0.08f;
		}
		
		if (player_count >= 2)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				snakes[1].angle -= 0.08f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				snakes[1].angle += 0.08f;
		}
		
		if (player_count >= 3)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
				snakes[2].angle -= 0.08f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
				snakes[2].angle += 0.08f;
		}
		
		if (player_count >= 4)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
				snakes[3].angle -= 0.08f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
				snakes[3].angle += 0.08f;
		}
		
		if (player_count >= 5)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
				snakes[4].angle -= 0.08f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
				snakes[4].angle += 0.08f;
		}
		
		if (player_count >= 6)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
				snakes[5].angle -= 0.08f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
				snakes[5].angle += 0.08f;
		}
		
		for (Snake& snake : snakes)
		{
			if (!snake.dead)
				snake.tick(window_info);
			
			if (!snake.dead)
			{
				for (Snake& other : snakes)
				{
					if (!other.dead)
					{
						snake.collideWith(other);
						
						if (other.dead)
						{
							for (int i = 0; i < (int)other.segments.size(); i ++)
							{
								Position velocity = Position((rand() % 8 - 4) / 3.0f, (rand() % 8 - 4) / 3.0f);
								particles.push_back({other.segments[i].pos, velocity, other.col, 8.0f});
							}
						}
					}
				}
			}
			
			for (Food& other : foods)
			{
				sf::Color cols[4] = {sf::Color::Green, sf::Color::Red, sf::Color::Blue, sf::Color::Yellow};
				
				if (snake.collideWith(other))
				{
					for (int i = 0; i < 5; i ++)
					{
						Position velocity = Position((rand() % 8 - 4) / 3.0f, (rand() % 8 - 4) / 3.0f);
						particles.push_back({snake.pos, velocity, other.col, 8.0f});
					}
					
					other = Food(Position(rand() % window_info.w, rand() % window_info.h), cols[0]);
					
					if (rand() % 5 == 0)
					{
						int powerup = rand() % 2 + 1;
						other.col = cols[powerup];
						other.powerup = (Powerup)powerup;
					}
				}
			}
		}
		
		int i = 0;
		while (i < (int)particles.size())
		{
			if (particles[i].col.a <= 8)
			{
				particles[i] = particles.back();
				particles.pop_back();
			}
			else
				i ++;
		}
		
		while (foods.size() < 4)
		{
			sf::Color cols[4] = {sf::Color::Green, sf::Color::Red, sf::Color::Blue, sf::Color::Yellow};
			foods.push_back(Food({(float)(rand() % window_info.w), (float)(rand() % window_info.h)}, cols[rand() % 1]));
		}
		
		int dead_count = 0;
		for (Snake& snake : snakes)
		{
			if (!snake.dead)
				dead_count ++;
		}
		
		if (dead_count <= 1)
		{
			reset_timer ++;
			
			if (reset_timer >= 180)
			{
				reset_timer = 0;
				
				for (Snake& snake : snakes)
				{
					snake.pos = Position(rand() % window_info.w, rand() % window_info.h);
					snake.reset(true);
					
					if (snake.points >= max_points)
						return true;
				}
			}
		}
		
		//Drawing
		window.clear(sf::Color(100, 65, 25));
		//window.clear(sf::Color(0, 0, 0));indow.draw(circle);
		
		for (Particle& particle : particles)
		{
			sf::CircleShape circle;
			
			particle.col.a -= 8;
			particle.tick();
			
			circle.setRadius(particle.width);
			circle.setFillColor(particle.col);
			circle.setPosition(particle.pos.x - circle.getRadius(), particle.pos.y - circle.getRadius());
			
			window.draw(circle);
		}
		
		for (Food& food : foods)
		{
			sf::CircleShape circle;
			
			circle.setRadius(16);
			circle.setFillColor(food.col);
			circle.setPosition(food.pos.x - circle.getRadius(), food.pos.y - circle.getRadius());
			
			window.draw(circle);
		}
		
		for (Snake& snake : snakes)
			drawSnake(window, snake);
		
		for (int i = 0; i < (int)snakes.size(); i ++)
		{
			sf::Text text;
			text.setFont(scores_font);
			text.setString("Player " + std::to_string(i + 1) + " : " + std::to_string(snakes[i].points));
			text.setCharacterSize(24);
			text.setColor(snakes[i].col);
			text.setPosition(16.0f, 16.0f + 32.0f * i);
			window.draw(text);
		}
		
		window.display();
		
		//Posttick
		time ++;
		
		return false;
	}
};

#endif
