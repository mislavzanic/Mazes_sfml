#include "Maze.hpp"
#include <iostream>

#include <stack>
#include <cstdlib>
#include <ctime>

mg::Maze::Maze(sf::Vector2i dim, sf::Vector2f window_size) : mDim(dim) {

    sf::Vector2f size(window_size.x / dim.x, window_size.y / dim.y);
    sf::Vector2f wall_width(size.x / 20, size.y / 20);

    for (int i = 0; i < dim.x; ++i) {
        for (int j = 0; j < dim.y; ++j) {
            sf::Vector2f position(i * size.x, j * size.y);
            mg::MazeCell *cell = new mg::MazeCell(i, j, sf::Color::White, sf::Color::Black, size, position, wall_width);
            mMaze.push_back(cell);
        }
    }
}

mg::Maze::~Maze() {
    // ..............
}

void mg::Maze::removeWalls(mg::MazeCell *cell1, mg::MazeCell *cell2) {
    if (cell1->getCol() == cell2->getCol()) {
        if (cell1->getRow() < cell2->getRow()) {
            cell1->removeWall(RIGHT);
            cell2->removeWall(LEFT);
        } else {
            cell1->removeWall(LEFT);
            cell2->removeWall(RIGHT);
        }
    } else {
        if (cell1->getCol() < cell2->getCol()) {
            cell1->removeWall(DOWN);
            cell2->removeWall(UP);
        } else {
            cell1->removeWall(UP);
            cell2->removeWall(DOWN);
        }
    }
}

mg::MazeCell *mg::Maze::freeNeighbour(mg::MazeCell *cell) {
    bool possibleCells[] = {true, true, true, true};
    if (cell->getCol() == 0) possibleCells[UP] = false;
    else if (this->mMaze[cell->getRow() * mDim.y + cell->getCol() - 1]->visited)
        possibleCells[UP] = false;

    if (cell->getCol() == mDim.y - 1) possibleCells[DOWN] = false;
    else if (this->mMaze[cell->getRow() * mDim.y + cell->getCol() + 1]->visited)
        possibleCells[DOWN] = false;

    if (cell->getRow() == 0) possibleCells[LEFT] = false;
    else if (this->mMaze[(cell->getRow() - 1) * mDim.y + cell->getCol()]->visited)
        possibleCells[LEFT] = false;

    if (cell->getRow() == mDim.x - 1) possibleCells[RIGHT] = false;
    else if (this->mMaze[(cell->getRow() + 1) * mDim.y + cell->getCol()]->visited)
        possibleCells[RIGHT] = false;

    int numOfAval = 0;
    for (int i = 0; i < 4; ++i) {
        if (possibleCells[i]) numOfAval++;
    }
    if (numOfAval == 0) return NULL;
    
    int randInt = (rand() % numOfAval) + 1;
    int dir;
    for (int i = 0; i < 4; ++i) {
        if (possibleCells[i]) {
            randInt--;
            if (randInt == 0) dir = i;
        }
    }

    if (dir == RIGHT) return this->mMaze[(cell->getRow() + 1) * mDim.y + cell->getCol()];
    else if (dir == LEFT) return this->mMaze[(cell->getRow() - 1) * mDim.y + cell->getCol()];
    else if (dir == DOWN) return this->mMaze[cell->getRow() * mDim.y + cell->getCol() + 1];
    return this->mMaze[cell->getRow() * mDim.y + cell->getCol() - 1];
}

// ...........................................................................................................................

void mg::Maze::draw(sf::RenderWindow& window) {

    for (int i = 0; i < mDim.x; ++i) {
        for (int j = 0; j < mDim.y; ++j) {
            mMaze[i * mDim.y + j]->draw(window);
        }
    }
}

void mg::Maze::generateRecursive(sf::RenderWindow& window) {

    srand(time(NULL));
    std::stack<MazeCell *> S;
    S.push(mMaze[0]);
    while (!S.empty()) {
        MazeCell *cell = S.top(), *nextCell;
        cell->visited = true;

        nextCell = this->freeNeighbour(cell);
        while (nextCell == NULL && !S.empty()) {
            cell = S.top();
            S.pop();
            nextCell = this->freeNeighbour(cell);
        }
        
        if (nextCell != NULL) {
            S.push(nextCell);
            this->removeWalls(cell, nextCell);
        }

        window.clear();
        this->draw(window);
        window.display();
    }

    is_generated = true;
}