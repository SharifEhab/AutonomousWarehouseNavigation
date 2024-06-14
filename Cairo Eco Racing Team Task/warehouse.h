/*
    You shouldn't touch this file. You have all you need in the functions introduced in "main.cpp".
*/

#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

// warehouse building block (cell)
struct Point
{
    int row;
    int col;
};

namespace __privates
{
    // Map
    const int mapSize = 11;  // Map size in cells
    bool map[mapSize][mapSize]; // Map grid (isWalkable)

    // Sets every cell to walkable
    void clear()
    {
        for (int row = 0; row < mapSize; row++)
        {
            for (int col = 0; col < mapSize; col++)
                map[row][col] = true;
        }
    }

    // Robot
    Point robotPos; // robot position

    // Pickup
    Point pickupPos; // Item position

    // Goal
    Point goalPos; // delivery position

    // boolean indecates if robot is carrying an item or not
    bool hasItem = false;
};

// Initalizes warehouse with level file

void initMaze(const string &levelFile)
{
    __privates::clear();

    ifstream file(levelFile.c_str());
    if (file.is_open())
    {
        string line;
        for (int row = 0; row < __privates::mapSize; row++)
        {
            getline(file, line);
            for (int col = 0; col < __privates::mapSize; col++)
            {
                if (line[col] == '>')
                {
                    __privates::robotPos.row = row;
                    __privates::robotPos.col = col;
                }
                else if (line[col] == 'D')
                {
                    __privates::goalPos.row = row;
                    __privates::goalPos.col = col;
                }
                else if (line[col] == 'P')
                {
                    __privates::pickupPos.row = row;
                    __privates::pickupPos.col = col;
                }
                __privates::map[row][col] = (line[col] != '#'); //Location of obsticle is set to false in the 2d grid or array 
            }
        }
        file.close();
    }
    else
        throw new invalid_argument("Level file was not found.");
}

// Map

// Checks if cell around robot is walkable (right: [-1 0 1], down: [-1 0 1])
bool isWalkable(int right, int down)
{
    int row = __privates::robotPos.row + min(max(down, -1), 1); //ensure vertical is [-1 0 1](left, nochange,right), calculates current row
    int col = __privates::robotPos.col + min(max(right, -1), 1);//ensure horizontal movement is only one step

    // Boundries check 
    if (row < 0 || row >= __privates::mapSize || col < 0 || col >= __privates::mapSize)
        return false;

    return __privates::map[row][col]; //this is already handeled in initMaze Function
                                      //returns false when obsticle is present "#"
}

// Prints warehouse as a whole
void printMaze()
{
    for (int row = 0; row < __privates::mapSize; row++)
    {
        for (int col = 0; col < __privates::mapSize; col++)
        {
            if (__privates::robotPos.row == row && __privates::robotPos.col == col)
            {
                cout << ">";
            }
            else if (__privates::goalPos.row == row && __privates::goalPos.col == col)
            {
                cout << "D";
            }
            else if (__privates::pickupPos.row == row && __privates::pickupPos.col == col && !__privates::hasItem) //last condition check if robot doesn't have item
            {
                cout << "P";
            }
            else
            {
                cout << (__privates::map[row][col] ? "." : "#"); //if true/walkable print"." else "#"
            }
        }
        cout << endl;
    }
}

// Prints part of warehouse around the robot
void printAround()
{
    for (int down = -1; down <= 1; down++)
    {
        for (int right = -1; right <= 1; right++)
        {

            if (down == 0 && right == 0) //Corresponds to current robot position
            {
                cout << ">";
            }
            else if (__privates::goalPos.row == __privates::robotPos.row + down && __privates::goalPos.col == __privates::robotPos.col + right)
            {
                cout << "D";
            }
            else if (__privates::pickupPos.row == __privates::robotPos.row + down && __privates::pickupPos.col == __privates::robotPos.col + right && !__privates::hasItem)
            {
                cout << "P";
            }  //ensures that the robot doesn't have the item / didn't pick it up
            else
            {
                cout << (isWalkable(down, right) ? "." : "#");
            }
        }
        cout << endl;
    }
}

// Robot
 
// Gets robot poision in warehouse (Point: row,col)
Point getRobotPos() { return __privates::robotPos; }

 
/*
    Moves ROBOT one step (right: [-1 0 1], down: [-1 0 1]) 

      ↖ ↑ ↗
      ← R →
      ↙ ↓ ↘
*/
void moveRobot(int right, int down)
{
    int row = __privates::robotPos.row + min(max(down, -1), 1);
    int col = __privates::robotPos.col + min(max(right, -1), 1);

    if (row < 0 || row >= __privates::mapSize || col < 0 || col >= __privates::mapSize || !__privates::map[row][col]) //Check boundries and obsticles
        return;
    __privates::robotPos.row = row;
    __privates::robotPos.col = col;
}

// Goal

// Pick an item from the map 
bool pickItem() { 
    if(__privates::robotPos.row == __privates::pickupPos.row && __privates::robotPos.col == __privates::pickupPos.col){
        __privates::hasItem = true;  
        cout<< "Item Picked Successfully"<< endl;  //Item picked
    }
    return __privates::hasItem;
    }

// Gets Item position in warehouse
Point getItemPos() { return __privates::pickupPos; }  

// Gets goal poision in warehouse
Point getGoalPos() { return __privates::goalPos; }  

// Check if reached goal
bool hasReachedGoal()
{
    //Robot reached goal destination with item picked--->True Else False
    return __privates::robotPos.row == __privates::goalPos.row &&
           __privates::robotPos.col == __privates::goalPos.col &&
           __privates::hasItem;                                   
}

#endif
