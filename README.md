# Autonomous Warehouse Navigation

## Introduction

In modern logistics, autonomous robots play a vital role in efficient warehouse management. These robots navigate through the warehouse environment, picking up items, and delivering them to designated locations using localization, mapping, and path planning algorithms.

## Goal

The goal of this project is to develop an algorithm that enables an autonomous robot to efficiently navigate through a warehouse, pick up items, and deliver them to specified drop-off points. The robot should dynamically map its environment, plan paths, and avoid obstacles.

## Task Description

### Warehouse Environment

You are provided with a warehouse represented as a grid with various elements:
- `.` represents empty spaces where the robot can move.
- `#` represents obstacles that the robot should avoid.
- `>` represents the robot's initial position.
- `P` represents pickup points for items.
- `D` represents drop-off points for items.

### API

You will be provided with a `warehouse.h` header file containing functions to interact with the environment:
- `isWalkable`: Checks if a cell around the robot is walkable.
- `moveRobot`: Moves the robot one step in different directions.
- `getRobotPos`: Retrieves the current position of the robot.
- `getItemPos`: Retrieves the item position.
- `getGoalPos`: Retrieves the delivery (goal) position.
- `pickItem`: Picks the item from the ground.

### Path Planning

Your algorithm should implement a path planning strategy to navigate the robot through the warehouse:
- Avoid obstacles and find optimal paths to pickup and drop-off points.
- Efficiently handle multiple items and their destinations (Bonus).

### Execution

Use the provided `main.cpp` file containing a simulation environment. Write your code in the `initialize` and `solve` functions. The `initialize` function sets up the environment, and the `solve` function implements the robot's behavior.

### Compilation

Compile your code using any C++ compiler. Example:g++ main.cpp -o main.exe main.exe warehouse1.txt


## Evaluation Criteria

You will be evaluated based on your algorithm's ability to:
- Navigate the robot through the warehouse while avoiding obstacles.
- Efficiently pick up items and deliver them to the correct drop-off points.
- Utilize path planning techniques to optimize the robot's movement.

## Bonus

Solve it for multiple items with different destinations. You can modify the API as needed to handle multiple items and destinations.

## Authors

CUERT Autonomous Dev. Team

## A* Algorithm Justification:
A* algorithm was used to solve this task, here is why:
- Optimality and Efficiency: A* guarantees the shortest path, balancing exploration and exploitation, making it both optimal and efficient for navigating complex environments.
- Heuristic Guidance: Utilizes heuristics to prioritize paths, reducing unnecessary exploration and speeding up the search.
- Obstacle Handling: Effectively navigates around obstacles, ensuring reliable pathfinding in dynamic warehouse environments.
- Versatility: Easily adapts to changes in the environment, accommodating dynamic item locations and multiple destinations.

These features make A* particularly suitable for the autonomous navigation and path planning required in a warehouse setting.

