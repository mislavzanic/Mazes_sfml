#include "Maze.hpp"

#include <iostream>
#include <string>

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(1280, 760), "MazeGeneration");
    mg::Maze maze(sf::Vector2i(atoi(argv[1]), atoi(argv[2])), sf::Vector2f(1280.f, 760.f));
    while (window.isOpen()) {
        sf::Event event;
        window.pollEvent(event);
        if (event.type == sf::Event::Closed)
            window.close();

        if (!maze.isGenerated) {
            if (std::string(argv[3]) == std::string("r"))
                maze.generateRecursive(window);
            else if (std::string(argv[3]) == std::string("p"));
                maze.generatePrim(window);
            if (!window.isOpen()) break;
            std::cout << "Generated" << std::endl;
        }

        if (!maze.isSolved) {
            if (std::string(argv[4]) == std::string("BFS"))
                maze.solveBFS(window);
            else if (std::string(argv[4]) == std::string("DFS"))
                maze.solveDFS(window);
            std::cout << "Solved" << std::endl;
        }

        window.clear(sf::Color::White);
        maze.draw(window);
        window.display();
    }
    return 0;
}