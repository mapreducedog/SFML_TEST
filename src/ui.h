#ifndef UI_HEADER
#define UI_HEADER

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
namespace ui 
{
void handle_events(sf::RenderWindow* window, float player_vecs[][2], int nr_players);    
int sgn(float x);
int cmp(float left, float right);
void main(sf::RenderWindow* window, float player_vecs[][2], int nr_players);
}


#endif
