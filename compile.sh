g++ -std=c++11 bubbles.cpp
g++ -std=c++11 -c main.cpp
echo "linking libraries"
g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
