#include "maze.h"
#include <iostream>
#include <mcpp/mcpp.h>

int main() {
    Maze* maze = Maze::parseAscii(std::cin);

    if(maze == nullptr){
        return 1;
    }

    mcpp::MinecraftConnection world;
    mcpp::Coordinate startCoords = world.getPlayerPosition();

    //get player coordinates
    int startX = startCoords.x;
    int startY = startCoords.y;
    int startZ = startCoords.z;

    int width = maze->getWidth();
    int height = maze->getHeight();

    int startWorldX = -1;
    int startWorldZ = -1;

    //minecraft blocks
    mcpp::BlockType AIR(0);
    mcpp::BlockType STONE(1);
    mcpp::BlockType OAK_LOG(17);
    mcpp::BlockType OAK_LEAVES(18);
    mcpp::BlockType GOLD_BLOCK(41);
    mcpp::BlockType DIAMOND_BLOCK(57);
    mcpp::BlockType LIME_CARPET(171, 5);
    mcpp::BlockType YELLOW_CARPET(171, 4);
    mcpp::BlockType RED_CARPET(171, 14);

    //for loop
    for(int y = 0; y < height; ++y){
        for(int x = 0; x < width; ++x){
            char cell = maze->getCell(x, y);
            int worldX = startX + x;
            int worldY = startY;
            int worldZ = startZ + y;

            mcpp::Coordinate floorPos(worldX, worldY - 1, worldZ);
            mcpp::Coordinate groundPos(worldX, worldY, worldZ);
            mcpp::Coordinate midPos(worldX, worldY + 1, worldZ);
            mcpp::Coordinate topPos(worldX, worldY + 2, worldZ);

            //clear anything in the area
            world.setBlock(groundPos, AIR);
            world.setBlock(midPos, AIR);
            world.setBlock(topPos, AIR);

            world.setBlock(floorPos, STONE);

            //edit mc world based on ascii character
            if(cell == 'X'){
                world.setBlock(groundPos, OAK_LOG);
                world.setBlock(midPos, OAK_LOG);
                world.setBlock(topPos, OAK_LEAVES);
            }
                
            else if(cell == 'S'){
                world.setBlock(floorPos, GOLD_BLOCK);
                startWorldX = worldX;
                startWorldZ = worldZ;
            }

            else if(cell == 'E'){
                world.setBlock(floorPos, DIAMOND_BLOCK);
            }

            else if(cell == '1'){
                world.setBlock(groundPos, LIME_CARPET);
            }
            else if(cell == '2'){
                world.setBlock(groundPos, YELLOW_CARPET);
            }
            else if(cell >= '3' && cell <= '9'){
                world.setBlock(groundPos, RED_CARPET);
            }
        }
    }

    if(startWorldX != -1 && startWorldZ != -1){
        world.setPlayerPosition(mcpp::Coordinate(startWorldX, startY, startWorldZ));
    }
    
    delete maze;
    return 0;
}
