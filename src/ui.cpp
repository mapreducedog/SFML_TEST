#include "ui.h"
#include <SFML/Window.hpp>

namespace ui 
{
int sgn(int x)
{
    return (x > 0) - (x < 0);
}
int sgn(float x)
{
    return (x > 0) - (x < 0);
}
    
int cmp(float left, float right)
{
    return sgn(left - right);
}


void handle_events(sf::RenderWindow* window, float player_vecs[][2], int nr_players)
{
    float move_speed = 0.2;
    int controls[][2][2] = {{{sf::Keyboard::Left, sf::Keyboard::Right},{sf::Keyboard::Up, sf::Keyboard::Down}},
    {{sf::Keyboard::A,sf::Keyboard::D},{sf::Keyboard::W,sf::Keyboard::S}}}; 
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
        if ((event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) && nr_players)
        {
            bool is_keydown = event.type == sf::Event::KeyPressed;
            for (unsigned int i = 0; i < (2 << nr_players); i++) //rather than 3 inner loops, we use one for easy breaking
            {
                //we bitwise innerloop here, with direction being innermost, so in [player dimension direction]
                //we loop as [0 0 0], [0 0 1], [0 1 0], [0 1 1], [1 0 0], [1 0 1], [1 1 0], [1 1 1]
                unsigned int player    =   i >> 2; 
                unsigned int dimension =!!(i & 2);
                unsigned int direction =   i & 1;
                
                
                if (controls[player][dimension][direction] == event.key.code)
                {
                    float cur_speed =  player_vecs[player][dimension];
                    int key_direction = direction - !direction; //remap to -1, 1 (left/up, right/down)
                    bool diff_sgn = sgn(key_direction) != sgn(cur_speed); //key-event opposite direction of current direction
                    player_vecs[player][dimension] = is_keydown * key_direction * move_speed + 
                    !is_keydown * diff_sgn * cur_speed; //accelerate on keydown, or stop on keyup of the current direction (continue with current speed on keyup of opposite direction)
                    break;
                }
            }
        }
        if (event.type == sf::Event::KeyPressed)
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
    
void main(sf::RenderWindow* window, float player_vecs[][2], int nr_players)
{
    while (window->isOpen())
        handle_events(window, player_vecs, nr_players);
    
}
    
}



