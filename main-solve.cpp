#include "maze.h"
#include "solver.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    std::string alg = "right";

    if(argc == 3 && std::string(argv[1]) == "--alg"){
        alg = argv[2];
    }
    else if(argc != 1){
        std::cerr << "ERROR: usage ./maze-solve [--alg right | left]" << std::endl;
        return 1;
    }

    Maze* maze = Maze::parseAscii(std::cin);
    if(maze == nullptr){
        std::cerr << "ERROR: empty maze input" << std::endl;
        return 1;
    }

    Solver* solver = nullptr;
    if(alg == "right"){
        solver = new RightHandSolver(*maze);
    }
    else if(alg == "left"){
        solver = new LeftHandSolver(*maze);
    }
    else{
        std::cerr << "ERROR: unknown algorithm" << std::endl;
        delete maze;
        return 1;
    }

    bool solved = solver->solve();
    solver->printTrace(std::cerr);
    solver->printSolved(std::cout);

    delete solver;
    delete maze;

    return solved ? 0 : 1;
}