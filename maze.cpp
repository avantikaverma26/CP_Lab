#include <iostream>
#include <ctime>
#include <string>
#include <stack>
#include <vector>



#define columnCell  "+---";
#define columnCellp "+   "
#define singleCell  "+";
#define rowCell     "|";
#define sbCell      " ";
#define visitedCell " . ";
#define blankCell   "   ";


class GridCell
{
    public:
        int posX;
        int posY;
        std::vector<GridCell> neighbourList;

        GridCell(int x, int y);
        bool check(GridCell cell);
        bool checkNearByCell(GridCell nextCell);
};

GridCell::GridCell(int pos_x, int pos_y)
{
    posX = pos_x;
    posY = pos_y;
}

bool GridCell::check(GridCell cell)
{
    return posX == cell.posX && posY == cell.posY;
}

bool GridCell::checkNearByCell(GridCell nextCell)
{
    for (int i = 0; i < neighbourList.size(); i++)
    {
        if (nextCell.check(neighbourList.at(i)))
        {
            return true;
        }
    }
    int i = 0;
    while (i < nextCell.neighbourList.size()){
    
        if (check(nextCell.neighbourList.at(i)))
        {
            return true;
        }
          i++;
    }

    return false;
}

class Grid
{
    public:
        std::vector<std::vector<GridCell>> Maze;
        void createGrid(int gridHeight, int gridWidth);
        bool getPath(GridCell beginCell, GridCell endCell, std::vector<GridCell> pathToOut);
        void displayMaze();
        void genrateMaze();
        
        

    private:
        std::stack<GridCell>  path;
        std::vector<GridCell> pathTaken;
        std::vector<GridCell> movesPossible();
        std::vector<GridCell> cellsArrived;
        
        GridCell randomNumber(std::vector<GridCell> steps);
        void backTrack();
        bool checkDone(std::vector<GridCell> pathToOut, GridCell cell);
        bool visited(GridCell possibleStep);
        void printMazeColumn();
        void printMazeRow();
     
};


bool Grid::visited(GridCell possibleStep)
{
    int i = 0;
    while ( i < cellsArrived.size())
    {
        if (possibleStep.check(cellsArrived.at(i)))
        {
            return true;
        }
         i++;
    }

    return false;
}

std::vector<GridCell> Grid::movesPossible()
{
    std::vector<int> xMoves = {1, 0, -1, 0};
    std::vector<int> yMoves = {0, 1, 0, -1};
    GridCell start = path.top();
    std::vector<GridCell> steps;

    for (int i = 0; i < 4; i++)
    {
        GridCell nextStep(xMoves.at(i) + start.posX, yMoves.at(i) + start.posY);
        if ((unsigned)nextStep.posX < Maze.size() && (unsigned)nextStep.posY < Maze.at(0).size() && !visited(nextStep))
        {
            steps.push_back(nextStep);
        }
    }

    return steps;
}

bool Grid::checkDone(std::vector<GridCell> pathToOut, GridCell cell)
{
    for (int j = 0; j < pathToOut.size(); j++)
    {
        if (cell.check(pathToOut.at(j)))
        {
            return true;
        }
    }

    return false;
}

bool Grid::getPath(GridCell beginCell, GridCell endCell, std::vector<GridCell> pathToOut)
{
    pathToOut.push_back( beginCell);

    if (beginCell.check(endCell))
    {
        pathTaken = pathToOut;
        return true;
    }

    std::vector<std::pair<unsigned int, unsigned int>> list_all_neighbours =
    {
        {beginCell.posX + 1, beginCell.posY},
        {beginCell.posX, beginCell.posY + 1},
        {beginCell.posX - 1, beginCell.posY},
        {beginCell.posX, beginCell.posY - 1}
    };

    for (int i = 0; i < list_all_neighbours.size(); i++)
    {
        if (list_all_neighbours.at(i).first < Maze.size() && list_all_neighbours.at(i).second < Maze.at(0).size())
        {
            if (beginCell.checkNearByCell(Maze.at(list_all_neighbours.at(i).first).at(list_all_neighbours.at(i).second)))
            {
                if (!checkDone(pathToOut, Maze.at(list_all_neighbours.at(i).first).at(list_all_neighbours.at(i).second)))
                {
                    if (getPath(Maze.at(list_all_neighbours.at(i).first).at(list_all_neighbours.at(i).second), endCell, pathToOut))
                    {
                        return true;
                    }
                }
            }
        }
    }

    pathToOut.pop_back();
    return false;
}

GridCell Grid::randomNumber(std::vector<GridCell> steps)
{
    int random = rand() % steps.size();
    return steps.at(random);
}

void Grid::backTrack()
{
    while(movesPossible().size() == 0)
    {
        path.pop();
    }
}

void Grid::createGrid(int gridHeight, int gridWidth)
{
    for (int i = 0; i < gridHeight; i++)
    {
        Maze.push_back(std::vector<GridCell>());
        for (int j = 0; j < gridWidth; j++)
        {
            Maze.at(i).push_back(GridCell(i, j));
        }
    }
}

void Grid::genrateMaze()
{
    path.push(Maze.at(0).at(0));
    cellsArrived.push_back(Maze.at(0).at(0));

    while(true)
    {
        GridCell nextMove = randomNumber(movesPossible());
        GridCell current = path.top();

        current.neighbourList.push_back(nextMove);
        Maze.at(current.posX).at(current.posY) = current;

        nextMove.neighbourList.push_back(current);
        Maze.at(nextMove.posX).at(nextMove.posY) = nextMove;

        path.push(nextMove);
        cellsArrived.push_back(nextMove);

        if (cellsArrived.size() != (Maze.size() * Maze.at(0).size()))
        {
            backTrack();
        }
        else
        {
            return;
        }
    }
}
void Grid::printMazeColumn()
{
   for (int i = 0; i < Maze.at(0).size(); i++)
    {
        std::cout << columnCell;
    }
    std::cout << '+' << std::endl;
    
}

void Grid::printMazeRow()
{
    for (int  i = 0; i < Maze.size(); i++)
    {
        std::cout << rowCell;
        for (int j = 0; j < Maze.at(0).size(); j++)
        {
            if (checkDone(pathTaken, Maze.at(i).at(j)))
            {
                std::cout << visitedCell;
            }
            else
            {
                std::cout << blankCell;
            }

            if (j + 1 < Maze.at(0).size())
            {
                if (Maze.at(i).at(j).checkNearByCell(Maze.at(i).at(j + 1)))
                {
                    std::cout << sbCell;
                }
                else {
                    std::cout << rowCell;
                }
            }
        }
        std::cout << '|' << std::endl;

        if (i + 1 < Maze.size())
        {
            for (int j = 0; j < Maze.at(0).size(); j++)
            {
                if (Maze.at(i).at(j).checkNearByCell(Maze.at(i + 1).at(j)))
                {
                    std::cout << columnCellp;
                }
                else
                {
                    std::cout << columnCell;
                }
            }
            std::cout << '+' << std::endl;
        }
    }

}

void Grid::displayMaze()
{
  printMazeColumn();
  printMazeRow();
  printMazeColumn();

}

void check_parameter_value(int gridHeight , int gridWidth)
 {
       if (gridHeight < 0 || gridWidth < 0)
        {
            throw std::runtime_error ("Invalid Parameter Values");
        }
 }


int main(int argc, char* argv[])
{
    int gridHeight;
    int gridWidth;

    try
    {
        if (argc < 3)
        {
            throw std::runtime_error ("Invalid Parameter Arguments");
        } 
        else if  (argc == 4)
        {
            std::srand(std::stoi(argv[3]));
        }
        else 
        {
            std::srand(std::time(NULL));
        }

        gridWidth  = std::stoi(argv[1]);
        gridHeight = std::stoi(argv[2]);

        check_parameter_value(gridWidth, gridHeight);
    }
    catch (std::invalid_argument &except)
    {
        std::cerr << "Invalid Argument Parameters" << std::endl;
        return 1;
    }
    catch (std::runtime_error  &except)
    {
        std::cerr << except.what() << std::endl;
        return 1;
    }

    Grid myGrid;
    myGrid.createGrid(gridWidth, gridHeight);
    myGrid.genrateMaze();
    myGrid.getPath(myGrid.Maze.at(0).at(0), myGrid.Maze.at(gridWidth - 1).at(gridHeight - 1), std::vector<GridCell>());
    myGrid.displayMaze();

    return 0;
}