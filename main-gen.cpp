#include "maze.h"
#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
     // Validates Input
     if(argc == 2 && std::string(argv[1]) == "--validate"){
        return Maze::validate(std::cin) ? 0 : 1;
    }

    // Input needs 4 command line variables
    if(argc != 4){
        std::cerr << "ERROR: usage ./maze-gen <width> <height> <seed>" << std::endl;
        return 1;
    }

     // Check Variables are numbers
    int width = 0, height = 0, seed = 0;


    try {
        width = std::stoi(argv[1]);
        height = std::stoi(argv[2]);
        seed = std::stoi(argv[3]);
    }       
    
    catch (...) {
        std::cerr << "ERROR: width, height and seed must be numbers" << std::endl;
        return 1;
    }

    //All variables valid -> Generate Maze
    if(width >= 3 && height >= 3 && width % 2 != 0 && height % 2 != 0){
        Maze maze(width, height);
        maze.generate(seed);
        maze.print();
    }

    // Variables are even
    else{
        std::cerr << "ERROR: width and height must be odd numbers >= 3" << std::endl; 
        return 1; 
    }

    return 0;
}
