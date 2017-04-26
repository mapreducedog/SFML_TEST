#ifndef BUBBLES_HEADER
#define BUBBLES_HEADER
#define max_speed 5.0
#define max_nr_shapes 30
#define min_shape_size 10
#include "bubbles.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
namespace bubbles 
{
    int move_shapes(sf::RenderWindow* window, sf::CircleShape* shapes, float move_vecs[][2], int nr_shapes, float player_vecs[][2], int nr_players);
    int cmp(float left, float right);
    int sgn(float x);
    void collide(sf::CircleShape* shapes, float move_vecs[][2], int shape_id, int neighbour_id);
    bool check_collission(sf::CircleShape* shapes, float move_vecs[][2], int shape_id, int neighbour_id);
    bool check_collission(sf::CircleShape* shapes, float move_vecs[][2], int shape_id, int neighbour_id, float steps);
    inline bool check_future_collission(sf::CircleShape* shapes, float move_vecs[][2], int shape_id, int neighbour_id);
    //void handle_events(sf::RenderWindow* window, float move_vecs[][2]);
    inline sf::Vector2f get_future_center(sf::CircleShape shape, float move_vec[2], float steps);
    inline sf::Vector2f get_center(sf::CircleShape shape);

    void main(sf::RenderWindow* window, int nr_shapes, float player_vecs[][2], int nr_players);
    bool keep_bounds(unsigned int frame[], sf::CircleShape shape,float move_vec[]);

}

#endif
