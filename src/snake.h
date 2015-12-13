#ifndef SNAKE_H
#define SNAKE_H

#include "SFML/Graphics.hpp"

#include "deque"
#include "string"

#define PI 3.141592

struct WindowInfo
{
	int w = 900;
	int h = 650;
};

struct Position
{
	float x;
	float y;
	
	Position() {}
	
	Position(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

struct Particle
{
	Position pos;
	Position velocity;
	sf::Color col;
	float width;
	
	void tick()
	{
		pos.x += velocity.x;
		pos.y += velocity.y;
	}
};

enum Powerup
{
	None = 0,
	Teeth = 1,
	Speed = 2,
};

struct Food
{
	Position pos;
	sf::Color col;
	Powerup powerup;
	
	Food(Position pos, sf::Color col, Powerup powerup = Powerup::None)
	{
		this->pos = pos;
		this->col = col;
		this->powerup = powerup;
	}
};

struct Segment
{
	Position pos;
	sf::Color col = sf::Color::Green;
	int col_fade = 255;
	float width = 1.0f;
	
	Segment(Position pos, sf::Color col, float width = 1.0f)
	{
		this->pos = pos;
		this->col = col;
		this->width = width;
	}
};

class Snake
{
	public:
	Position pos;
	float angle = 0.0f;
	float angle_momentum = 0.0f;
	float speed = 3.0f;
	
	bool use_ai = true;
	
	sf::Color col = sf::Color::Green;
	sf::Color head_col = sf::Color::Green;
	
	int max_segments = 10;
	
	Powerup powerup = Powerup::None;
	int powerup_time = 0;
	
	int time = 0;
	
	bool dead = false;
	
	int points = 0;
	
	std::deque<Segment> segments;
	
	void tick(WindowInfo& window_info)
	{
		this->speed = 3.0;
		head_col = this->col;
		
		if (this->powerup == Powerup::Speed)
			this->speed *= 1.4f;
		
		this->pos.x += this->speed * cos(angle);
		this->pos.y += this->speed * sin(angle);
		
		this->segments.push_front(Segment(this->pos, this->col, 1.0f));
		
		while ((int)this->segments.size() > this->max_segments)
			this->segments.pop_back();
		
		//Movement
		if (this->powerup != Powerup::Speed)
			this->angle += 0.03f * sin(this->time * 50.0f) + this->angle_momentum;
		this->angle_momentum = std::max(-0.05f, std::min(0.05f, this->angle_momentum));
		
		if (this->use_ai)
			this->angle_momentum += 0.001f * (rand() % 100 - 50);
		
		this->pos.x = fmod(this->pos.x + window_info.w, window_info.w);
		this->pos.y = fmod(this->pos.y + window_info.h, window_info.h);
		
		//Posttick
		this->time ++;
		
		if (powerup_time > 0)
			powerup_time --;
		else
			powerup = Powerup::None;
	}
	
	void collideWith(Snake& other)
	{
		for (int i = (int)(this->getSegmentSnakeWidth(0) / (this->speed / 2.0f)) + 1; i < (int)this->segments.size(); i ++)
		{
			float xx = pow(other.pos.x - this->getSegment(i).pos.x, 2);
			float yy = pow(other.pos.y - this->getSegment(i).pos.y, 2);
			
			if (sqrt(xx + yy) < other.getSegmentSnakeWidth(0))
			{
				if (other.powerup == Powerup::Teeth)
					this->cut(i);
				else
					other.dead = true;
			}
		}
	}
	
	bool collideWith(Food& other)
	{
		float xx = pow(other.pos.x - this->getSegment(0).pos.x, 2);
		float yy = pow(other.pos.y - this->getSegment(0).pos.y, 2);
		
		if (sqrt(xx + yy) < 16)
		{
			this->max_segments += 10;
			this->segments[0].width *= 1.6f;
			
			if (other.powerup != Powerup::None)
			{
				powerup = other.powerup;
				powerup_time = 500;
			}
			
			return true;
		}
		
		return false;
	}
	
	Segment& getSegment(int i)
	{
		return this->segments[i];
	}
	
	float getSegmentSnakeWidth(int i)
	{
		return 4.0f + 64.0f / ((20.0f * (float)(i) / this->max_segments) + 8.0f);
	}
	
	void cut(int segment)
	{
		this->max_segments = segment + 1;
		
		for (int i = segment; i < (int)this->segments.size(); i ++)
		{
			this->segments.pop_back();
		}
	}
	
	void reset(bool add_score = false)
	{
		if (!this->dead && add_score)
			this->points ++;
		
		this->dead = false;
		this->cut(0);
		this->max_segments = 10;
		powerup = Powerup::None;
	}
};

#endif
