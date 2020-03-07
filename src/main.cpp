#include "Maze.hpp"

#include <iostream>

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(960, 540), "MazeGeneration");
    mg::Maze maze(sf::Vector2i(atoi(argv[1]), atoi(argv[2])), sf::Vector2f(960.f, 540.f));
    while (window.isOpen()) {
        sf::Event event;
        window.pollEvent(event);
        if (event.type == sf::Event::Closed)
            window.close();

        if (!maze.is_generated) {
            maze.generateRecursive(window);
            std::cout << "Generated" << std::endl;
        }

        window.clear();
        maze.draw(window);
        window.display();
    }
    return 0;
}