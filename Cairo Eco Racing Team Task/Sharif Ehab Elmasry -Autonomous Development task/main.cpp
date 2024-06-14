/*
The goal of this task is to develop an algorithm to solve a warehouse (reach a goal) without knowing
the warehouse as a whole from the get-go, you will need to explore the warehouse step by step.


You have to include "warehouse.h".

Your code goes in the *initialize* and *solve* functions. Read the instructions there carefully.

Compile your code using any c++ compiler and pass level filename as an argument, for example:

g++ main.cpp -o main.exe
main.exe warehouse1.txt
*/

#include "warehouse.h"
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>                            
using namespace std;

struct AStarNode
{
    int row; //Row of current node
    int col; //Column of current node
    int g; //Cost from start to current node
    int h; //Heuristic/estimate cost from current node to goal 
    int f; //Total cost of current node
    AStarNode *parent; //Parent node

};

// Define a comparison operator for priority queue in A* search
struct CompareAStarNode {
    bool operator()(const AStarNode* lhs, const AStarNode* rhs) const {
        return lhs->f > rhs->f;
    }
};

//implement A* search to optimize robot's path for pickup and goal destination
vector<Point> astarSearch(Point start, Point goal)
{
    //Initialize open and closed list
    priority_queue<AStarNode*, vector<AStarNode*>, CompareAStarNode> openList; //Priority queue(min heap) for open list
                                                                               //ensure that nodes with lower f values (lower cost) are at the top.
                                                                               //CompareAStarNode is a comparison operator for priority queue

    map<pair<int, int>, AStarNode*> openlist;  //Open list to keep track of nodes to be visited
    map<pair<int, int>, AStarNode*> closedList; //Closed list to keep track of visited nodes

    //Initialize start node
    AStarNode* startNode = new AStarNode{start.row, start.col, 0, 0, 0, NULL};
    openList.push(startNode);
    //Add start node to open list
    openlist.insert(make_pair(make_pair(startNode->row, startNode->col), startNode));

    //Loop until open list is empty
    while (!openList.empty())
    {
		//Get node with lowest f value from open list
		AStarNode* currentNode = openList.top();  //Get node with lowest f value from open list
		openList.pop(); //Remove node from open list
        openlist.erase(make_pair(currentNode->row, currentNode->col)); //Remove node from open list

		//Check if current node is goal node
        if (currentNode->row == goal.row && currentNode->col == goal.col) //Check if current node is goal node
        {
			//Initialize vector to store path
			vector<Point> path;

			//Loop until start node is reached
            while (currentNode->parent != NULL) 
            {
				//Add node to path
				path.push_back(Point{currentNode->row, currentNode->col});

				//Move to parent node
				currentNode = currentNode->parent;
			}

			//Reverse path
			reverse(path.begin(), path.end());

			//Return path
			return path;
		}

		//Generate children nodes
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
				//Check if node is walkable
                if (isWalkable(i, j)) //neighbour node is walkable
                {
					//Initialize child node
					AStarNode* childNode = new AStarNode{currentNode->row + i, currentNode->col + j, 0, 0, 0, currentNode};

					//Check if child node is in closed list

                    if (closedList.find(make_pair(childNode->row, childNode->col)) != closedList.end())
                    {
						continue; //Skip child node if it is in closed list
					}

					//Calculate cost from start to child node
					childNode->g = currentNode->g + 1;

					//Calculate heuristic/estimate cost from child node to goal
                    //Manhattan distance
					childNode->h = abs(childNode->row - goal.row) + abs(childNode->col - goal.col);

					//Calculate total cost of child node
                    //check if child node is not open list or if it is in the openlist but has a lower f which indicates a shorter path
                    if (openlist.find(make_pair(childNode->row, childNode->col)) == openlist.end() || childNode->f < openlist[make_pair(childNode->row, childNode->col)]->f)
                    {
                        childNode->f = childNode->g + childNode->h;
                        childNode->parent = currentNode;
                        if (openlist.find(make_pair(childNode->row, childNode->col)) == openlist.end())
                        {
							openList.push(childNode); //Add child node to open list
							openlist[make_pair(childNode->row, childNode->col)] = childNode;
						}

                    }
               
					
				}
			}
		}

		//Add current node to closed list
		closedList[make_pair(currentNode->row, currentNode->col)] = currentNode;
	}

    //Return empty path if goal is unreachable
    return vector<Point>();


}






void initialize()
{
    /*
    This function will run after the warehouse is loaded.

    */
    printMaze();


}

void solve()
{
    /*
    This function will keep running forever until the goal is reached.


    You can access the warehouse API through below functions.


    Sense and Control:
        isWalkable(right, down): checks if cell around robot is walkable (right: [-1 0 1], down: [-1 0 1])
        moveRobot(right, down): moves player one step (right: [-1 0 1], down: [-1 0 1])


    Global Info:
        getRobotPos(): gets robot position in warehouse
        getGoalPos()  : gets goal position in warehouse
        getItemPos() : gets item position in warehouse

    Debugging:
        printMaze()  : prints warehouse as a whole
        printAround(): prints part of warehouse around robot
    */
    Point robotPos = getRobotPos();
    Point goalPos = getGoalPos();
    Point itemPos = getItemPos();

    // Check if the robot has reached the goal and delivered the item
    if (robotPos.row == goalPos.row && robotPos.col==goalPos.col && __privates::hasItem)
    {
        // Reset the hasItem flag
        __privates::hasItem = false;
        return;  // Return without any further action
    }


    // Use A* search to find path from robot to item
    vector<Point> pathToItem = astarSearch(robotPos, itemPos);
    if (pathToItem.empty())
    {
		cout << "No path to item!" << endl;
		return;
	}
    // Move robot to item
    for (const Point& nextPos : pathToItem)
    {
        // Get direction to move
		int right = nextPos.col - robotPos.col;
		int down = nextPos.row - robotPos.row;

		// Move robot
		moveRobot(right, down);

		// Update robot position
		robotPos = nextPos;

	}



        // Pick up item
   if (pickItem())
   {

            //Use A* search to find path from item to goal
            vector<Point> deliveryPath = astarSearch(itemPos, goalPos);
            if (deliveryPath.empty())
            {
				cout << "No path to goal!" << endl;
				return;
			}
            // Move robot to goal
            for (const Point& nextPos : deliveryPath)
            {
                // Get direction to move
                int right = nextPos.col - robotPos.col;
                int down = nextPos.row - robotPos.row;

                // Move robot
                moveRobot(right, down);

                // Update robot position
                robotPos = nextPos;
                itemPos = nextPos;
            }


   }

}

int main(int argc, char const *argv[])
{
    // Initalize level
    initMaze("C:/Cairo Eco Racing Team Task/warehouse1.txt");

    initialize();

    // Game loop
    while (true)
    {    
        // Check reached goal
        if (hasReachedGoal())
        {
            // Hurray!
            cout << "Reached Goal!" << endl;
            break;
        }

        // Solve maze
        solve();


    }


    return 0;
}