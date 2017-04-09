#include "bubbles.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace bubbles 
{

int cmp(float left, float right)
{
	if (left > right)
		return 1;
	if (right > left)
		return -1;
	if (right == left)
		return 0;
}

void move_shapes(sf::RenderWindow* window, sf::CircleShape* shapes, float move_vecs[][2], int nr_shapes)
{
	bool was_budged[nr_shapes] = {0};
	for (int shape_id = 0;  shape_id < nr_shapes; shape_id++)
	{
		sf::CircleShape shape = shapes[shape_id];
		float * move_vec = move_vecs[shape_id];
		sf::Vector2f position_ = shape.getPosition() + sf::Vector2f(shape.getRadius(), shape.getRadius());
		float position[] = {position_.x, position_.y};
		sf::Vector2u frame_ = window->getSize();
		unsigned int frame[] = {frame_.x,frame_.y};
        
		for (int neighbour_id = 0; neighbour_id < nr_shapes && !was_budged[shape_id]; neighbour_id++)
		{
			if (neighbour_id == shape_id)
				continue;
			sf::CircleShape neighbour = shapes[neighbour_id];
			sf::Vector2f neighbour_position_ = neighbour.getPosition() + sf::Vector2f(neighbour.getRadius(), 
                neighbour.getRadius());
            
			sf::Vector2f deltashapes_ = position_ - neighbour_position_;
            float totradius = neighbour.getRadius() + shape.getRadius();
            
			for (int dim = 0; dim < 2; dim++)
			{
				if (std::sqrt(std::pow(deltashapes_.x, 2) + std::pow(deltashapes_.y, 2)) < totradius)//these will collide
				{
                    std::swap(move_vecs[shape_id][dim], move_vecs[neighbour_id][dim]);
					was_budged[shape_id] = true;
					was_budged[neighbour_id] = true;
				}
			}
		}
		for (int dim = 0; dim < 2; dim++)
            //check hit bounds
			if ((position[dim] - shape.getRadius() < 0 && move_vec[dim] < 0) 
				|| 
				(position[dim] + shape.getRadius() > frame[dim] && move_vec[dim] > 0))
				move_vec[dim] *= -1;
		shapes[shape_id].move(move_vec[0], move_vec[1]);
	}
}

void handle_events(sf::RenderWindow* window, float move_vecs[][2])
{
    float move_speed = 0.3;
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
        if (event.type == sf::Event::KeyPressed)
            switch (event.key.code)
            {
                case sf::Keyboard::Up:
                    move_vecs[0][1] -= move_speed;
                    break;
                case sf::Keyboard::Down:
                    move_vecs[0][1] += move_speed;
                    break;
                case sf::Keyboard::Left:
                    move_vecs[0][0] -= move_speed;
                    break;
                case sf::Keyboard::Right:
                    move_vecs[0][0] += move_speed;
                    break;
                case sf::Keyboard::Q:
                    window->close();
                    break;
                default:
                    break;
            }
    }
}

void main(sf::RenderWindow* window, int nr_shapes)
{
    
    sf::Vector2u window_size = window->getSize();
    unsigned int x_size = window_size.x;
    unsigned int y_size = window_size.y;
	sf::Color colors[] = {sf::Color::Green, sf::Color::Red, sf::Color::Blue,sf::Color::Yellow, sf::Color::Magenta,
    };
    
	sf::CircleShape shapes[nr_shapes];
	float move_vecs[nr_shapes][2];
	for (int i = 0; i < nr_shapes; i++)
	{
		move_vecs[i][0] = (rand() % 4) - 2;
		move_vecs[i][1] = (rand() % 4) - 2;
		shapes[i] = sf::CircleShape(35.f);
		unsigned int x_max_spawn_pos = x_size  - floor(shapes[i].getRadius());
		unsigned int y_max_spawn_pos = y_size - floor(shapes[i].getRadius());
		shapes[i].setFillColor(colors[i % 5]);
		shapes[i].setPosition(rand() % x_max_spawn_pos,rand() % y_max_spawn_pos);
	}
    while (window->isOpen())
    {
        handle_events(window, move_vecs);
        window->clear();
		move_shapes(window, shapes,move_vecs, nr_shapes);
		for (auto shape : shapes)
			window->draw(shape);
        window->display();
    }
}
}
