#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <time.h> /* for time */
#include "bubbles.h"
/*put some comment here*/




int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Enter a number";
		return 1;
	}
	srand(time(NULL));
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktop.width, desktop.height), "SFML works!");
    int nr_shapes = argv[1][0] - '0';
    bubbles::main(&window, nr_shapes); 
    return 0;
}
