#include "Maze.hpp"

#include <stack>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include <iostream>

mg::Maze::Maze(sf::Vector2i dim, sf::Vector2f window_size) : mDim(dim) {

    this->mSize = sf::Vector2f(window_size.x / dim.x, window_size.y / dim.y);
    sf::Vector2f wall_width(mSize.x / 20, mSize.y / 20);

    for (int i = 0; i < dim.x; ++i) {
        for (int j = 0; j < dim.y; ++j) {
            sf::Vector2f position(i * mSize.x, j * mSize.y);
            mg::MazeCell *cell = new mg::MazeCell(i, j, mSize, position, wall_width);

            // CENTER CELL
            this->quads.push_back(sf::Vertex(sf::Vector2f(position.x + wall_width.x          , position.y + wall_width.y          ), sf::Color::Black));
            this->quads.push_back(sf::Vertex(sf::Vector2f(position.x + mSize.x - wall_width.x, position.y + wall_width.y          ), sf::Color::Black));
            this->quads.push_back(sf::Vertex(sf::Vector2f(position.x + mSize.x - wall_width.x, position.y + mSize.y - wall_width.y), sf::Color::Black));
            this->quads.push_back(sf::Vertex(sf::Vector2f(position.x + wall_width.x          , position.y + mSize.y - wall_width.y), sf::Color::Black));

            this->mMaze.push_back(cell);
        }
    }
}

mg::Maze::~Maze() {
    for (int i = 0; i < this->mDim.x; ++i) {
        for (int j = 0; j < this->mDim.y; ++j) {
            delete mMaze[i * this->mDim.y + j];
        }
    }
}

//.....................................................................................................................................................................................................................................................................
//.....................................................................................................................................................................................................................................................................
//.....................................................................................................................................................................................................................................................................

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

mg::MazeCell *mg::Maze::freeNeighbour(mg::MazeCell *cell, mazeMode mode = RECURSIVE) {
    bool possibleCells[] = {true, true, true, true};
    if (cell->getCol() == 0) possibleCells[UP] = false;
    else if (this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() - 1]->visited && mode == RECURSIVE)
        possibleCells[UP] = false;

    if (cell->getCol() == this->mDim.y - 1) possibleCells[DOWN] = false;
    else if (this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() + 1]->visited && mode == RECURSIVE)
        possibleCells[DOWN] = false;

    if (cell->getRow() == 0) possibleCells[LEFT] = false;
    else if (this->mMaze[(cell->getRow() - 1) * this->mDim.y + cell->getCol()]->visited && mode == RECURSIVE)
        possibleCells[LEFT] = false;

    if (cell->getRow() == this->mDim.x - 1) possibleCells[RIGHT] = false;
    else if (this->mMaze[(cell->getRow() + 1) * this->mDim.y + cell->getCol()]->visited && mode == RECURSIVE)
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

    if (dir == RIGHT) return this->mMaze[(cell->getRow() + 1) * this->mDim.y + cell->getCol()];
    else if (dir == LEFT) return this->mMaze[(cell->getRow() - 1) * this->mDim.y + cell->getCol()];
    else if (dir == DOWN) return this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() + 1];
    return this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() - 1];
}

void mg::Maze::__queueCells(std::vector<MazeCell *>& V, MazeCell *cell) {

    if (cell->getCol() > 0 && !this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() - 1]->visited && !this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() - 1]->isCurrent) {
        V.push_back(this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() - 1]);
        this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() - 1]->isCurrent = true;
    }
    if (cell->getCol() < this->mDim.y - 1 && !this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() + 1]->visited && !this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() + 1]->isCurrent) { 
        V.push_back(this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() + 1]);
        this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() + 1]->isCurrent = true;
    }
    if (cell->getRow() > 0 && !this->mMaze[(cell->getRow() - 1) * this->mDim.y + cell->getCol()]->visited && !this->mMaze[(cell->getRow() - 1) * this->mDim.y + cell->getCol()]->isCurrent) { 
        V.push_back(this->mMaze[(cell->getRow() - 1) * this->mDim.y + cell->getCol()]);
        this->mMaze[(cell->getRow() - 1) * this->mDim.y + cell->getCol()]->isCurrent = true;
    }
    if (cell->getRow() < this->mDim.x - 1 && !this->mMaze[(cell->getRow() + 1) * this->mDim.y + cell->getCol()]->visited && !this->mMaze[(cell->getRow() + 1) * this->mDim.y + cell->getCol()]->isCurrent) { 
        V.push_back(this->mMaze[(cell->getRow() + 1) * this->mDim.y + cell->getCol()]);
        this->mMaze[(cell->getRow() + 1) * this->mDim.y + cell->getCol()]->isCurrent = true;
    }
}

//.....................................................................................................................................................................................................................................................................
//.....................................................................................................................................................................................................................................................................
//.....................................................................................................................................................................................................................................................................

void mg::Maze::draw(sf::RenderWindow& window) {

    for (int i = 0; i < this->mDim.x; ++i) {
        for (int j = 0; j < this->mDim.y; ++j) {

            if (this->mMaze[i * mDim.y + j]->visited) {
                for (int k = 4 * (i * this->mDim.y + j); k < 4 * (i * this->mDim.y + j + 1); ++k) {
                    this->quads[k].color = sf::Color::White;
                }
            }

            if (this->mMaze[i * mDim.y + j]->isCurrent || this->mMaze[i * mDim.y + j]->visitedSolution) {
                for (int k = 4 * (i * this->mDim.y + j); k < 4 * (i * this->mDim.y + j + 1); ++k) {
                    this->quads[k].color = this->mMaze[i * mDim.y + j] ->mColor;
                }
            }

            if (!this->mMaze[i * mDim.y + j]->isCurrent && !this->mMaze[i * mDim.y + j]->visited) {
                for (int k = 4 * (i * this->mDim.y + j); k < 4 * (i * this->mDim.y + j + 1); ++k) {
                    this->quads[k].color = sf::Color::Black;
                }
            }

            if (!this->mMaze[i * mDim.y + j]->getDownWall()) {
                this->quads[4 * (i * mDim.y + j) + 3].position = sf::Vector2f(this->mMaze[i * this->mDim.y + j]->getPositon().x + this->mMaze[i * this->mDim.y + j]->Wall_width().x,
                                                                              this->mMaze[i * this->mDim.y + j]->getPositon().y + this->mSize.y + 2 * this->mMaze[i * this->mDim.y + j]->Wall_width().y);
                if (!this->mMaze[i * mDim.y + j]->getLeftWall()) {
                    this->quads[4 * (i * mDim.y + j) + 3].position = sf::Vector2f(this->mMaze[i * this->mDim.y + j]->getPositon().x,
                                                                                  this->mMaze[i * this->mDim.y + j]->getPositon().y + this->mSize.y + this->mMaze[i * this->mDim.y + j]->Wall_width().y);
                } 

                this->quads[4 * (i * mDim.y + j) + 2].position = sf::Vector2f(this->mMaze[i * this->mDim.y + j]->getPositon().x + this->mSize.x - this->mMaze[i * this->mDim.y + j]->Wall_width().x,
                                                                              this->mMaze[i * this->mDim.y + j]->getPositon().y + this->mSize.y + 2 * this->mMaze[i * this->mDim.y + j]->Wall_width().y);
                if (!this->mMaze[i * mDim.y + j]->getRightWall()) {
                    this->quads[4 * (i * mDim.y + j) + 2].position = sf::Vector2f(this->mMaze[i * this->mDim.y + j]->getPositon().x + this->mSize.x,
                                                                                  this->mMaze[i * this->mDim.y + j]->getPositon().y + this->mSize.y);
                }
            }
            if (!this->mMaze[i * mDim.y + j]->getUpWall()) {
                this->quads[4 * (i * mDim.y + j)].position = sf::Vector2f(this->mMaze[i * this->mDim.y + j]->getPositon().x + this->mMaze[i * this->mDim.y + j]->Wall_width().x,
                                                                          this->mMaze[i * this->mDim.y + j]->getPositon().y);
                if (!this->mMaze[i * mDim.y + j]->getLeftWall()) {
                    this->quads[4 * (i * mDim.y + j)].position = sf::Vector2f(this->mMaze[i * this->mDim.y + j]->getPositon().x,
                                                                              this->mMaze[i * this->mDim.y + j]->getPositon().y);
                }

                this->quads[4 * (i * mDim.y + j) + 1].position = sf::Vector2f(this->mMaze[i * this->mDim.y + j]->getPositon().x + this->mSize.x - this->mMaze[i * this->mDim.y + j]->Wall_width().x,
                                                                              this->mMaze[i * this->mDim.y + j]->getPositon().y);
                if (!this->mMaze[i * mDim.y + j]->getRightWall()) {
                    this->quads[4 * (i * mDim.y + j) + 1].position = sf::Vector2f(this->mMaze[i * this->mDim.y + j]->getPositon().x + this->mSize.x,
                                                                                  this->mMaze[i * this->mDim.y + j]->getPositon().y);
                }
            }
            if (!this->mMaze[i * mDim.y + j]->getLeftWall()) {
                if (this->mMaze[i * mDim.y + j]->getUpWall()) { 
                    this->quads[4 * (i * mDim.y + j)].position = sf::Vector2f(this->mMaze[i * this->mDim.y + j]->getPositon().x,
                                                                              this->mMaze[i * this->mDim.y + j]->getPositon().y + this->mMaze[i * this->mDim.y + j]->Wall_width().y);
                }

                if (this->mMaze[i * mDim.y + j]->getDownWall()) { 
                    this->quads[4 * (i * mDim.y + j) + 3].position = sf::Vector2f(this->mMaze[i * this->mDim.y + j]->getPositon().x,
                                                                                  this->mMaze[i * this->mDim.y + j]->getPositon().y + this->mSize.y - this->mMaze[i * this->mDim.y + j]->Wall_width().y);
                }
            }
            if (!this->mMaze[i * mDim.y + j]->getRightWall()) {
                if (this->mMaze[i * mDim.y + j]->getUpWall()) {
                    this->quads[4 * (i * mDim.y + j) + 1].position = sf::Vector2f(this->mMaze[i * this->mDim.y + j]->getPositon().x + this->mSize.x,
                                                                                this->mMaze[i * this->mDim.y + j]->getPositon().y + this->mMaze[i * this->mDim.y + j]->Wall_width().y);
                }

                if (this->mMaze[i * mDim.y + j]->getDownWall()) { 
                    this->quads[4 * (i * mDim.y + j) + 2].position = sf::Vector2f(this->mMaze[i * this->mDim.y + j]->getPositon().x + this->mSize.x,
                                                                                this->mMaze[i * this->mDim.y + j]->getPositon().y + this->mSize.y - this->mMaze[i * this->mDim.y + j]->Wall_width().y);
                }
            }
        }
    }

    window.draw(&quads[0], quads.size(), sf::Quads);
}

//.....................................................................................................................................................................................................................................................................
//.....................................................................................................................................................................................................................................................................
//.....................................................................................................................................................................................................................................................................

void mg::Maze::generateRecursive(sf::RenderWindow& window) {

    std::stack<MazeCell *> S;

    srand(time(NULL));

    S.push(this->mMaze[0]);
    this->mMaze[0]->visited = true;
    while (!S.empty()) {
        MazeCell *cell = S.top(), *nextCell;
        cell->visited = true;
        cell->isCurrent = true;

        nextCell = this->freeNeighbour(cell);
        while (nextCell == NULL && !S.empty()) {
            cell->isCurrent = false;
            cell = S.top();
            S.pop();
            nextCell = this->freeNeighbour(cell);
        }
        
        if (nextCell != NULL) {
            S.push(nextCell);
            this->removeWalls(cell, nextCell);
        }

        sf::Event event;
        window.pollEvent(event);
        if (event.type == sf::Event::Closed) {
            window.close();
            break;
        }

        window.clear();
        this->draw(window);
        cell->isCurrent = false;
        window.display();
    }

    this->isGenerated = true;
}

void mg::Maze::generatePrim(sf::RenderWindow& window) {
    std::vector<MazeCell *> V;

    srand(time(NULL));

    int randint = random() % (this->mDim.x * this->mDim.y);
    MazeCell *cell = this->mMaze[randint];
    cell->visited = true;

    this->__queueCells(V, cell);

    while (true) {
        int n = V.size();
        if (n == 0) break;
        int randint = random() % n;

        MazeCell *cell = V[randint];
        cell->visited = true;
        cell->isCurrent = false;

        std::vector<MazeCell *>::iterator it = V.begin();
        while (it != V.end() && randint) {
            randint--;
            it++;
        }
        V.erase(it);
        
        if (cell->getCol() > 0 && this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() - 1]->visited) {
            this->removeWalls(cell, this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() - 1]);
        } else if (cell->getRow() > 0 && this->mMaze[(cell->getRow() - 1) * this->mDim.y + cell->getCol()]->visited) {
            this->removeWalls(cell, this->mMaze[(cell->getRow() - 1) * this->mDim.y + cell->getCol()]);
        } else if (cell->getCol() < this->mDim.y - 1 && this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() + 1]->visited){
            this->removeWalls(cell, this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() + 1]);
        } else if (cell->getRow() < this->mDim.x - 1 && this->mMaze[(cell->getRow() + 1) * this->mDim.y + cell->getCol()]->visited){
            this->removeWalls(cell, this->mMaze[(cell->getRow() + 1) * this->mDim.y + cell->getCol()]);
        }

        this->__queueCells(V, cell);

        sf::Event event;
        window.pollEvent(event);
        if (event.type == sf::Event::Closed) {
            window.close();
            break;
        }

        window.clear();
        this->draw(window);
        window.display();
    
    }

    this->isGenerated = true;
}

void mg::Maze::generateWilson(sf::RenderWindow& window) {

    srand(time(NULL));

    std::stack<MazeCell *> S;
    int numOfVisited = 0;

    int randint = random() % (this->mDim.x * this->mDim.y);
    this->mMaze[randint]->visited = true;
    numOfVisited++;

    while (numOfVisited < this->mDim.y * this->mDim.x) {

        while (S.empty()) {
            randint = random() % (this->mDim.x * this->mDim.y);
            if (!this->mMaze[randint]->visited && !this->mMaze[randint]->isCurrent) {
                S.push(this->mMaze[randint]);
            }
        }

        MazeCell *cell = S.top();
        MazeCell *newCell = this->freeNeighbour(cell, WILSON);
    
        if (!newCell->isCurrent) {

            newCell->isCurrent = true;
            S.push(newCell);
            
        } else if (newCell->isCurrent) {

            while (cell->getRow() * this->mDim.y + cell->getCol() != newCell->getRow() * this->mDim.y + newCell->getCol()) {
                cell->isCurrent = false;
                S.pop();
                if (S.empty()) break;
                cell = S.top();
            }

        } else if (newCell->visited) {

            while (!S.empty()) {
                cell->visited = true;
                cell->isCurrent = false;
                numOfVisited++;
                this->removeWalls(newCell, cell);
                if (!S.empty()) { 
                    newCell = cell;
                    cell = S.top();
                }
            }

        }

        window.clear();
        this->draw(window);
        window.display();
    }

    this->isGenerated = true;
}

//.....................................................................................................................................................................................................................................................................
//.....................................................................................................................................................................................................................................................................
//.....................................................................................................................................................................................................................................................................

void mg::Maze::solveBFS(sf::RenderWindow& window) {
    std::queue<MazeCell *> Q;
    int *path = new int[this->mDim.x * this->mDim.y];

    for (int i = 0; i < this->mDim.x * this->mDim.y; ++i) {
        path[i] = -1;
    }

    Q.push(this->mMaze[START]);
    while(!Q.empty()) {
        MazeCell *cell = Q.front();
        Q.pop();
        cell->visitedSolution = true;
        cell->setColor(sf::Color::Red);
        if (cell->getCol() + cell->getRow() * this->mDim.y == END) break;

        if (!cell->getRightWall()) { 
            if (!this->mMaze[(cell->getRow() + 1) * this->mDim.y + cell->getCol()]->visitedSolution) {
                Q.push(this->mMaze[(cell->getRow() + 1) * this->mDim.y + cell->getCol()]);
                path[(cell->getRow() + 1) * this->mDim.y + cell->getCol()] = cell->getRow() * this->mDim.y + cell->getCol();
            }
        }
        if (!cell->getLeftWall()) { 
            if (!this->mMaze[(cell->getRow() - 1) * this->mDim.y + cell->getCol()]->visitedSolution) {
                Q.push(this->mMaze[(cell->getRow() - 1) * this->mDim.y + cell->getCol()]);
                path[(cell->getRow() - 1) * this->mDim.y + cell->getCol()] = cell->getRow() * this->mDim.y + cell->getCol();
            }
        }
        if (!cell->getDownWall()) { 
            if (!this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() + 1]->visitedSolution) {
                Q.push(this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() + 1]);
                path[cell->getRow() * this->mDim.y + cell->getCol() + 1] = cell->getRow() * this->mDim.y + cell->getCol();
            }
        }
        if (!cell->getUpWall()) { 
            if (!this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() - 1]->visitedSolution) {
                Q.push(this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() - 1]);
                path[cell->getRow() * this->mDim.y + cell->getCol() - 1] = cell->getRow() * this->mDim.y + cell->getCol();
            }
        }
        
        window.clear();
        this->draw(window);
        window.display();
    }

    int index = END;
    this->mMaze[index]->setColor(sf::Color::Green);
    while (index != START) {
        index = path[index];
        this->mMaze[index]->setColor(sf::Color::Green);
        window.clear();
        this->draw(window);
        window.display();
    }

    isSolved = true;
}

void mg::Maze::solveDFS(sf::RenderWindow& window) {
    std::stack<MazeCell *> S;int *path = new int[this->mDim.x * this->mDim.y];

    for (int i = 0; i < this->mDim.x * this->mDim.y; ++i) {
        path[i] = -1;
    }
    
    S.push(this->mMaze[START]);

    while (!S.empty()) {
        MazeCell *cell = S.top();
        S.pop();
        cell->setColor(sf::Color::Red);
        cell->visitedSolution = true;

        if (cell->getCol() + this->mDim.y * cell->getRow() == END) break;

        if (!cell->getRightWall()) { 
            if (!this->mMaze[(cell->getRow() + 1) * this->mDim.y + cell->getCol()]->visitedSolution) {
                S.push(this->mMaze[(cell->getRow() + 1) * this->mDim.y + cell->getCol()]);
                path[(cell->getRow() + 1) * this->mDim.y + cell->getCol()] = cell->getRow() * this->mDim.y + cell->getCol();
            }
        }
        if (!cell->getLeftWall()) { 
            if (!this->mMaze[(cell->getRow() - 1) * this->mDim.y + cell->getCol()]->visitedSolution) {
                S.push(this->mMaze[(cell->getRow() - 1) * this->mDim.y + cell->getCol()]);
                path[(cell->getRow() - 1) * this->mDim.y + cell->getCol()] = cell->getRow() * this->mDim.y + cell->getCol();
            }
        }
        if (!cell->getDownWall()) { 
            if (!this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() + 1]->visitedSolution) {
                S.push(this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() + 1]);
                path[cell->getRow() * this->mDim.y + cell->getCol() + 1] = cell->getRow() * this->mDim.y + cell->getCol();
            }
        }
        if (!cell->getUpWall()) { 
            if (!this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() - 1]->visitedSolution) {
                S.push(this->mMaze[cell->getRow() * this->mDim.y + cell->getCol() - 1]);
                path[cell->getRow() * this->mDim.y + cell->getCol() - 1] = cell->getRow() * this->mDim.y + cell->getCol();
            }
        }
        window.clear();
        this->draw(window);
        window.display();
    }

    int index = END;
    this->mMaze[index]->setColor(sf::Color::Green);
    while (index != START) {
        index = path[index];
        this->mMaze[index]->setColor(sf::Color::Green);
        window.clear();
        this->draw(window);
        window.display();
    }

    isSolved = true;
}
