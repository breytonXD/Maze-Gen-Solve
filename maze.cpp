#include "maze.h"
#include <cstdlib>
#include <iostream>
#include <iosfwd>
#include <string>

//Constructor & destructor
Maze::Maze(int w, int h){
    width = w;
    height = h;

    grid = new char*[height];

    for(int y = 0; y < height; ++y){
        grid[y] = new char[width];

        for(int x = 0; x < width; ++x){
            if(y == 0 || x == 0 || y == height - 1 || x == width - 1){
                grid[y][x] = 'X';
            }

            else{
                grid[y][x] = ' ';
            }
        }
    }
}

Maze::~Maze(){
    for(int y = 0; y < height; ++y){
        delete[] grid[y];
    }

    delete[] grid; 
}

//Maze generation functions
void Maze::generate(int seed){
    std::srand(seed);

    divide(1, width - 2, 1, height - 2);
    startAndExit();
}

void Maze::divide(int left, int right, int bottom, int top){
    int gridWidth = right - left + 1;
    int gridHeight = top - bottom + 1;

    if(gridWidth < 3 || gridHeight < 3){
        return;
    }

    bool horizontal;
    if(gridHeight > gridWidth){
        horizontal = true;
    }
    else if(gridWidth > gridHeight){
        horizontal = false;
    }
    else{
        horizontal = (std::rand() % 2 == 0);
    }

    if(horizontal){
        int wallY = randomEven(bottom + 1, top - 1);
        int passageX = randomOdd(left, right);

        for(int x = left; x <= right; ++x){
            grid[wallY][x] = 'X';
        }
        grid[wallY][passageX] = ' ';

        divide(left, right, bottom, wallY - 1);
        divide(left, right, wallY + 1, top);
    }
    else{
        int wallX = randomEven(left + 1, right - 1);
        int passageY = randomOdd(bottom, top);

        for(int y = bottom; y <= top; ++y){
            grid[y][wallX] = 'X';
        }
        grid[passageY][wallX] = ' ';

        divide(left, wallX - 1, bottom, top);
        divide(wallX + 1, right, bottom, top);
    }
}

int Maze::randomEven(int min, int max){
    if(min % 2 != 0){
        ++min;
    }
    if(max % 2 != 0){
        --max;
    }

    int count = ((max - min) / 2) + 1;
    return min + (std::rand() % count) * 2;
}

int Maze::randomOdd(int min, int max){
    if(min % 2 == 0){
        ++min;
    }
    if(max % 2 == 0){
        --max;
    }

    int count = ((max - min) / 2) + 1;
    return min + (std::rand() % count) * 2;
}

void Maze::startAndExit(){
    for(int y = 1; y < height - 1; ++y){
        if(grid[y][1] == ' '){
            grid[y][0] = 'S';
            break;
        }
    }

    for(int y = height - 2; y > 0; --y){
        if(grid[y][width - 2] == ' '){
            grid[y][width - 1] = 'E';
            break;
        }
    }
}

//Parse ascii
Maze* Maze::parseAscii(std::istream& is){
   std::string lines[256];
   int h = 0;
   std::string line;
   int w = -1;


   while(std::getline(is, line)){
       if(h >= 256){
           return nullptr;
       }

       if(line.empty()){
           return nullptr;
       }

       if(w == -1){
           w = line.size();
       }
       else if((int)line.size() != w){
           return nullptr;
       }

       for(int x = 0; x < (int)line.size(); ++x){
           char cell = line[x];
           if(cell != 'X' && cell != ' ' && cell != 'S' && cell != 'E' &&
              (cell < '1' || cell > '9')){
               return nullptr;
           }
       }

       lines[h] = line;
       ++h;
   }


   if(h == 0 || w <= 0){
       return nullptr;
   }


   Maze* m = new Maze(w, h);


   for(int y = 0; y < h; ++y){
       for(int x = 0; x < w; ++x){
           m->grid[y][x] = lines[y][x];
       }
   }


   return m;
}

//Accessors for solver (Leo)
int Maze::getWidth() const {
    return width;
}

int Maze::getHeight() const {
    return height;
}

bool Maze::isWall(int x, int y) const {
    if(x < 0 || y < 0 || x >= width || y >= height){
        return true;
    }

    return grid[y][x] == 'X';
}

void Maze::getStart(int& x, int& y) const {
    x = -1;
    y = -1;

    for(int row = 0; row < height; ++row){
        for(int col = 0; col < width; ++col){
            if(grid[row][col] == 'S'){
                x = col;
                y = row;
                return;
            }
        }
    }
}

void Maze::getExit(int& x, int& y) const {
    x = -1;
    y = -1;

    for(int row = 0; row < height; ++row){
        for(int col = 0; col < width; ++col){
            if(grid[row][col] == 'E'){
                x = col;
                y = row;
                return;
            }
        }
    }
}

char Maze::getCell(int x, int y) const {
    if(x < 0 || y < 0 || x >= width || y >= height){
        return 'X';
    }

    return grid[y][x];
}

//Print
void Maze::print() const{
    for(int y = 0; y < height; ++y){
        for(int x = 0; x < width; ++x){
                std::cout << grid[y][x];
        }

        std::cout << std::endl;
    }
}

//Validate
bool Maze::validate(std::istream& is){
    Maze* m = parseAscii(is);
    if(m == nullptr){
        std::cerr << "INVALID" << std::endl;
        return false;
    }

    int w = m->width;
    int h = m->height;
    int n = w * h;
    bool valid = true;
    int startCount = 0;
    int exitCount = 0;
    int startX = -1;
    int exitX = -1;

    for(int y = 0; y < h; ++y){
        for(int x = 0; x < w; ++x){
            if(m->grid[y][x] == 'S'){
                ++startCount;
                startX = x;
            }
            else if(m->grid[y][x] == 'E'){
                ++exitCount;
                exitX = x;
            }
        }
    }

    if(w < 3 || h < 3 || w % 2 == 0 || h % 2 == 0 ||
       startCount != 1 || exitCount != 1 || startX != 0 || exitX != w - 1){
        valid = false;
    }

    bool* filled = new bool[n];
    int* stack = new int[n];

    //Flood fill passages from S to find isolated open cells
    for(int i = 0; i < n; ++i){
        filled[i] = false;
    }

    int top = 0;
    if(valid){
        for(int y = 0; y < h; ++y){
            for(int x = 0; x < w; ++x){
                if(m->grid[y][x] == 'S'){
                    stack[top++] = y * w + x;
                    filled[y * w + x] = true;
                }
            }
        }
    }

    while(valid && top > 0){
        int cur = stack[--top];
        int cx = cur % w;
        int cy = cur / w;

        int dx[4] = {0, 1, 0, -1};
        int dy[4] = {-1, 0, 1, 0};
        for(int d = 0; d < 4; ++d){
            int nx = cx + dx[d];
            int ny = cy + dy[d];
            if(nx < 0 || ny < 0 || nx >= w || ny >= h){
                continue;
            }
            int ni = ny * w + nx;
            if(!filled[ni] && m->grid[ny][nx] != 'X'){
                filled[ni] = true;
                stack[top++] = ni;
            }
        }
    }

    for(int y = 0; y < h && valid; ++y){
        for(int x = 0; x < w && valid; ++x){
            if(m->grid[y][x] != 'X' && !filled[y * w + x]){
                valid = false;
            }
        }
    }

    //Flood fill walls from corner to find loops
    if(valid){
        for(int i = 0; i < n; ++i){
            filled[i] = false;
        }
        top = 0;
        stack[top++] = 0;
        filled[0] = true;

        while(top > 0){
            int cur = stack[--top];
            int cx = cur % w;
            int cy = cur / w;

            int dx[4] = {0, 1, 0, -1};
            int dy[4] = {-1, 0, 1, 0};
            for(int d = 0; d < 4; ++d){
                int nx = cx + dx[d];
                int ny = cy + dy[d];
                if(nx < 0 || ny < 0 || nx >= w || ny >= h){
                    continue;
                }
                int ni = ny * w + nx;
                if(!filled[ni] && (m->grid[ny][nx] == 'X' ||
                   m->grid[ny][nx] == 'S' || m->grid[ny][nx] == 'E')){
                    filled[ni] = true;
                    stack[top++] = ni;
                }
            }
        }

        for(int y = 0; y < h && valid; ++y){
            for(int x = 0; x < w && valid; ++x){
                if(m->grid[y][x] == 'X' && !filled[y * w + x]){
                    valid = false;
                }
            }
        }
    }

    delete[] filled;
    delete[] stack;
    delete m;

    std::cerr << (valid ? "VALID" : "INVALID") << std::endl;
    return valid;
}
