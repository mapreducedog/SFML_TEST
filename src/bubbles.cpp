#include "bubbles.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace bubbles 
{
int sgn(float x)
{
    return (x > 0) - (x < 0);
}
    
    int cmp(float left, float right)
    {
        return sgn(left - right);
    }
void collide(sf::CircleShape* shapes, float move_vecs[][2], int shape_id, int neighbour_id)
{
    sf::CircleShape shape = shapes[shape_id];
    sf::CircleShape neighbour = shapes[neighbour_id];
    float shape_size = std::pow(shape.getRadius(), 2) * 3.14;
    float neighbour_size = std::pow(neighbour.getRadius(), 2) * 3.14;
    float own_contrib = (shape_size - neighbour_size) / (shape_size + neighbour_size);
    float other_contrib = 2 / (shape_size + neighbour_size);
    for (int dim = 0; dim < 2; dim++)
    {
        float shape_spd = move_vecs[shape_id][dim];
        float neighbour_spd = move_vecs[neighbour_id][dim];
        move_vecs[shape_id][dim] = own_contrib*shape_spd + other_contrib*neighbour_spd*neighbour_size;
        move_vecs[neighbour_id][dim] =  other_contrib*shape_spd*shape_size - own_contrib*neighbour_spd;
    }
}



inline sf::Vector2f get_center(sf::CircleShape shape)
{
    return shape.getPosition() + sf::Vector2f(shape.getRadius(), shape.getRadius());
}

inline sf::Vector2f get_future_center(sf::CircleShape shape, float move_vec[2], float steps)
{
    return get_center(shape) + sf::Vector2f(move_vec[0] * steps, move_vec[1] * steps);
}

bool check_collission(sf::CircleShape* shapes, float move_vecs[][2], int shape_id, int neighbour_id, float steps)
{
    sf::Vector2f delta_vec = get_future_center(shapes[shape_id], move_vecs[shape_id], steps) 
    - get_future_center(shapes[neighbour_id], move_vecs[neighbour_id], steps); //
    float distance = std::sqrt(std::pow(delta_vec.x, 2) + std::pow(delta_vec.y, 2));
    float totradius = shapes[shape_id].getRadius() + shapes[neighbour_id].getRadius();
    return  distance <= totradius;
}

bool check_collission(sf::CircleShape* shapes,int shape_id, int neighbour_id)
{
    sf::Vector2f delta_vec = get_center(shapes[shape_id]) - get_center(shapes[neighbour_id]);
    float distance = std::sqrt(std::pow(delta_vec.x, 2) + std::pow(delta_vec.y, 2));
    float totradius =  shapes[shape_id].getRadius() + shapes[neighbour_id].getRadius();
    return distance <= totradius;
}

inline bool check_future_collission(sf::CircleShape* shapes, float move_vecs[][2], int shape_id, int neighbour_id)
{
    return check_collission(shapes, move_vecs, shape_id, neighbour_id, 1);
}


bool keep_bounds(unsigned int frame[], sf::CircleShape shape,float move_vec[])
{
    
    sf::Vector2f position_ = get_center(shape);
    float position[] = {position_.x, position_.y};
    for (int dim = 0; dim < 2; dim++)
        if ((position[dim] - shape.getRadius() < 0 && move_vec[dim] < 0) 
            || 
            (position[dim] + shape.getRadius() > frame[dim] && move_vec[dim] > 0))
            move_vec[dim] *= -1;
}

void move_shapes(sf::RenderWindow* window, sf::CircleShape* shapes, float move_vecs[][2], int nr_shapes)
{
    sf::Vector2u frame_ = window->getSize();
    unsigned int frame[] = {frame_.x,frame_.y};
	bool was_budged[nr_shapes] = {0}; 
	for (int shape_id = 0;  shape_id < nr_shapes ; shape_id++)
	{
		sf::CircleShape shape = shapes[shape_id];
		float * move_vec = move_vecs[shape_id];
        for (int neighbour_id = 0; neighbour_id < nr_shapes && !was_budged[shape_id]; 
            neighbour_id++)
        {
            if (neighbour_id == shape_id)
                continue;
            bool will_collide = check_future_collission(shapes, move_vecs, shape_id, neighbour_id);
            if (will_collide)
            {
                was_budged[shape_id] = true;
                was_budged[neighbour_id] = true;
                collide(shapes, move_vecs,shape_id, neighbour_id);
            }
        }
		keep_bounds(frame, shape, move_vec);//turn around when hitting borders
		shapes[shape_id].move(move_vec[0], move_vec[1]);
	}
}

void handle_events(sf::RenderWindow* window, float move_vecs[][2])
{
    float move_speed = 0.3;
    int controls[][2][2] = {{{sf::Keyboard::Left, sf::Keyboard::Right},{sf::Keyboard::Up, sf::Keyboard::Down}},
    {{sf::Keyboard::A,sf::Keyboard::D},{sf::Keyboard::W,sf::Keyboard::S}}}; 
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
        if (event.type == sf::Event::KeyPressed)
        {
            for (unsigned int i = 0; i < 8; i++) //rather than 3 inner loops, we use one for easy breaking
            {
                //we bitwise innerloop here, with direction being innermost, so in [player dimension direction]
                //we loop as [0 0 0], [0 0 1], [0 1 0], [0 1 1], [1 0 0], [1 0 1], [1 1 0], [1 1 1]
                int player = !!(i & 4); 
                int dimension = !!(i & 2);
                int direction = (i & 1);
                
                if (controls[player][dimension][direction] == event.key.code)
                {
                    move_vecs[player][dimension] +=  (direction - !direction) * move_speed; //map direction 0 -> -1, direction 1 -> 1
                    break;
                }
            }
            switch (event.key.code)
                {
                    case sf::Keyboard::Q:
                        window->close();
                        break;
                    default:
                        break;
                }
        }
    }
}

void main(sf::RenderWindow* window, int nr_shapes)
{
    
    sf::Vector2u window_size = window->getSize();
    unsigned int x_size = window_size.x;
    unsigned int y_size = window_size.y;
	sf::Color colors[] = {sf::Color::Green, sf::Color::Red, sf::Color::Blue,
        sf::Color::Yellow, sf::Color::Magenta, };
    
	sf::CircleShape shapes[nr_shapes];
	float move_vecs[nr_shapes][2];
	for (int i = 0; i < nr_shapes; i++)
	{
		move_vecs[i][0] = (rand() % 4) - 2;
		move_vecs[i][1] = (rand() % 4) - 2;
		shapes[i] = sf::CircleShape(rand() % 40 + 10.f);
		unsigned int x_max_spawn_pos = x_size  -  
            floor(shapes[i].getRadius());
		unsigned int y_max_spawn_pos = y_size - 
            floor(shapes[i].getRadius());
		shapes[i].setFillColor(colors[i % 5]);
        //shapes[i].setPosition(rand() % x_max_spawn_pos,
        //         rand() % y_max_spawn_pos);
        bool is_placed = false;
        while (!is_placed) //find a non-overlapping starting position
        {
            is_placed = true;
            shapes[i].setPosition(rand() % x_max_spawn_pos,
                rand() % y_max_spawn_pos);
            for (int j = i - 1; j >= 0 && is_placed; j--)
                is_placed &= !check_collission(shapes, i, j);
        }
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
