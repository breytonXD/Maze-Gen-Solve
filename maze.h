#ifndef MAZE_H
#define MAZE_H
#include <istream>
#include <iosfwd>

class Maze {
    public:
        void generate(int seed);
        void print() const;
        static Maze* parseAscii(std::istream& is);
        static bool validate(std::istream& is);
        Maze(int w, int h);
        ~Maze();

        // accessors added for solver to query the maze (Leo)
        int getWidth() const;
        int getHeight() const;
        bool isWall(int x, int y) const;
        void getStart(int& x, int& y) const;
        void getExit(int& x, int& y) const;
        char getCell(int x, int y) const;

    private:
        int height;
        int width;
        char** grid;
        void divide(int left, int right, int bottom, int top);
        int randomEven(int min, int max);
        int randomOdd(int min, int max);
        void startAndExit();
};

#endif
