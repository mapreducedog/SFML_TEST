#include "bubbles.h"
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


namespace bubbles 
{
int sgn(float x)
{
    return (x > 0) - (x < 0);
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

void merge_shapes(sf::CircleShape* shapes, float move_vecs[][2], int shape_id,int neighbour_id, int nr_shapes)
{
    int max_id = std::max(shape_id, neighbour_id);
    int min_id = std::min(shape_id, neighbour_id);
    sf::CircleShape& receptor = shapes[min_id];
    sf::CircleShape& donor = shapes[max_id];
    
    sf::Vector2f new_center = (get_center(receptor) + get_center(donor)) * 0.5f;
    float donor_rad = donor.getRadius();
    float rec_rad = receptor.getRadius();
    float new_radius = donor.getRadius() + receptor.getRadius();
    float contr_don = donor_rad / new_radius;
    float contr_rec = rec_rad / new_radius;
    
    for (int i = 0; i < 2; i++)
    {
        move_vecs[min_id][i] = (move_vecs[min_id][i] * contr_rec  + move_vecs[max_id][i] * contr_don);
        std::swap(move_vecs[max_id][i], move_vecs[nr_shapes - 1][i]);
    }
    std::swap(shapes[max_id], shapes[nr_shapes - 1]); //move the donor circle to the non-drawn part
    receptor.setRadius(new_radius);
    receptor.setPosition(new_center - sf::Vector2f(new_radius, new_radius));
}


void split_shape(sf::CircleShape* shapes, float move_vecs[][2], int shape_id, int nr_shapes)
{
    sf::CircleShape& shape = shapes[shape_id];
    int twin_id = nr_shapes;
    sf::Vector2f center = get_center(shape);
    sf::CircleShape& twin = shapes[twin_id];

    float* shape_move = move_vecs[shape_id];
    float* twin_move = move_vecs[twin_id];
    float parent_move[2] = {shape_move[0], shape_move[1]};
    float angle = 0.25f;
    float pi = 3.14159265358979f;
    twin_move[0] = parent_move[0] * std::cos(-angle * pi) - parent_move[1] * std::sin(-angle * pi);
    twin_move[1] = parent_move[0] * std::sin(-angle * pi) + parent_move[1] * std::cos(-angle * pi); 
    shape_move[0] = parent_move[0] * std::cos(angle * pi) - parent_move[1] * std::sin(angle * pi);
    shape_move[1] = parent_move[0] * std::sin(angle * pi) + parent_move[1] * std::cos(angle * pi); 

    float radius = shape.getRadius()/ 2;
    shape.setRadius(radius);
    twin.setRadius(shape.getRadius());
    twin.setPosition(shape.getPosition());
    twin.setFillColor(shape.getFillColor());
    while (check_collission(shapes, shape_id, twin_id))
    {
        shape.move(shape_move[0], shape_move[1]);
        twin.move(twin_move[0], twin_move[1]);
    }
    
}
inline bool check_future_collission(sf::CircleShape* shapes, float move_vecs[][2], int shape_id, int neighbour_id)
{
    return check_collission(shapes, move_vecs, shape_id, neighbour_id, 0.5);
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

int move_shapes(sf::RenderWindow* window, sf::CircleShape* shapes, float move_vecs[][2], int nr_shapes, float player_vecs[][2], int nr_players)
{
    sf::Vector2u frame_ = window->getSize();
    unsigned int frame[] = {frame_.x,frame_.y};
	bool was_budged[max_nr_shapes] = {0}; 
    for (int shape_id = 0; shape_id < nr_players; shape_id++)
        for (int dim = 0; dim < 2; dim++)
        {
            move_vecs[shape_id][dim] += player_vecs[shape_id][dim];
            float speed = move_vecs[shape_id][dim];
            if (std::abs(speed) > max_speed)
            {
                int sign = (speed > 0) - (speed < 0);
                move_vecs[shape_id][dim] = max_speed * sign;
            }
        }
	for (int shape_id = 0;  shape_id < nr_shapes ; shape_id++)
	{
		sf::CircleShape shape = shapes[shape_id];
        shape.setOri
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
                int largest_shape = (shapes[shape_id].getRadius() > shapes[neighbour_id].getRadius()) ? shape_id : neighbour_id;
                bool space_avalaible = nr_shapes < max_nr_shapes - 1;
                bool share_color = shapes[shape_id].getFillColor() == shapes[neighbour_id].getFillColor();
                bool large_enough = shapes[largest_shape].getRadius() > min_shape_size;
                if (share_color)
                {
                    merge_shapes(shapes, move_vecs,shape_id, neighbour_id, nr_shapes);
                    nr_shapes--;
                }
                else if (space_avalaible && large_enough)
                {
                    
                    split_shape(shapes, move_vecs, largest_shape, nr_shapes);
                    nr_shapes++;
                }
            }
        }
		keep_bounds(frame, shape, move_vec);//turn around when hitting borders
		shapes[shape_id].move(move_vec[0], move_vec[1]);
        
	}
	return nr_shapes;
}

void fill_board(sf::Vector2u window_size , sf::CircleShape * shapes, float move_vecs[][2], int nr_shapes)
{
    sf::Color colors[] = {sf::Color::Green, sf::Color::Red, sf::Color::Blue,
    sf::Color::Yellow, sf::Color::Magenta, };
    for (int i = 0; i < nr_shapes; i++)
	{
		move_vecs[i][0] = (rand() % 4) - 2;
		move_vecs[i][1] = (rand() % 4) - 2;
		shapes[i] = sf::CircleShape(rand() % 40 + 10.f);
		unsigned int x_max_spawn_pos = window_size.x  -  
            floor(shapes[i].getRadius());
		unsigned int y_max_spawn_pos = window_size.y - 
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
    
}



void main(sf::RenderWindow* window, int nr_shapes, float player_vecs[][2], int nr_players)
{
    
	sf::CircleShape shapes[max_nr_shapes];
	float move_vecs[max_nr_shapes][2];
    fill_board(window->getSize(), shapes, move_vecs, nr_shapes); 
    while (window->isOpen())
    {
        window->clear();
		nr_shapes = move_shapes(window, shapes,move_vecs, nr_shapes, player_vecs, nr_players);
		for (int i = 0; i < nr_shapes; i++)
			window->draw(shapes[i]);
        window->display();
    }
}
}
