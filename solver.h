#ifndef SOLVER_H
#define SOLVER_H

#include "maze.h"
#include "path.h"
#include <ostream>

// Solver — design and implement this class hierarchy yourself.
// Two constraints are fixed: the solver must be initialised with a `const Maze&`
// (read-only; allocate any mutable working state separately), and the hierarchy
// must use abstract class inheritance and polymorphism.

class Solver
{
public:
    Solver(const Maze &maze);
    virtual bool solve() = 0;
    virtual ~Solver();

    void printSolved(std::ostream &os) const;
    void printTrace(std::ostream &os) const;

protected:
    const Maze &maze_;
    int *visits_;
    Path path_;
    bool solved_;

    int idx(int x, int y) const;
};

// WallFollower — shared wall-following logic goes here; RightHandSolver and
// LeftHandSolver inject their directional bias through this class.
class WallFollower : public Solver
{
public:
    WallFollower(const Maze &maze) : Solver(maze) {}
    bool solve() override;

protected:
    virtual int firstTurn(int facing) const = 0;
    virtual int lastResort(int facing) const = 0;
};

class RightHandSolver : public WallFollower
{
public:
    RightHandSolver(const Maze &maze) : WallFollower(maze) {}

protected:
    int firstTurn(int facing) const override;
    int lastResort(int facing) const override;
};

class LeftHandSolver : public WallFollower
{
public:
    LeftHandSolver(const Maze &maze) : WallFollower(maze) {}

protected:
    int firstTurn(int facing) const override;
    int lastResort(int facing) const override;
};

#endif