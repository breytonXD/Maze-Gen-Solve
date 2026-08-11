#include "maze.h"
#include "solver.h"
#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if(argc != 4 && argc != 6){
        std::cerr << "ERROR: usage ./maze-final <width> <height> <seed> [--alg right | left]" << std::endl;
        return 1;
    }

    std::string alg = "right";
    if(argc == 6){
        if(std::string(argv[4]) != "--alg"){
            std::cerr << "ERROR: usage ./maze-final <width> <height> <seed> [--alg right | left]" << std::endl;
            return 1;
        }
        alg = argv[5];
    }

    int width = 0, height = 0, seed = 0;
    try {
        width = std::stoi(argv[1]);
        height = std::stoi(argv[2]);
        seed = std::stoi(argv[3]);
    } catch (...) {
        std::cerr << "ERROR: width, height and seed must be numbers" << std::endl;
        return 1;
    }

    if(width < 3 || height < 3 || width % 2 == 0 || height % 2 == 0){
        std::cerr << "ERROR: width and height must be odd numbers >= 3" << std::endl;
        return 1;
    }

    Maze maze(width, height);
    maze.generate(seed);

    Solver* solver = nullptr;
    if(alg == "right"){
        solver = new RightHandSolver(maze);
    }
    else if(alg == "left"){
        solver = new LeftHandSolver(maze);
    }
    else{
        std::cerr << "ERROR: unknown algorithm" << std::endl;
        return 1;
    }

    bool solved = solver->solve();
    solver->printTrace(std::cerr);
    solver->printSolved(std::cout);

    delete solver;
    return solved ? 0 : 1;
}