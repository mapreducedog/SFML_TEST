#ifndef BUBBLES_HEADER
#define BUBBLES_HEADER
#include "bubbles.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
namespace bubbles 
{
    void move_shapes(sf::RenderWindow* window, sf::CircleShape* shapes, float move_vecs[][2], int nr_shapes);
    int cmp(float left, float right);
    void handle_events(sf::RenderWindow* window, float move_vecs[][2]);
    void main(sf::RenderWindow* window, int nr_shapes);
}

#endif
