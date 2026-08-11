#include "solver.h"

// Solver — design and implement the class hierarchy yourself.
// const Maze& is read-only; allocate mutable working state separately.
// Share the wall-following algorithm via WallFollower — do not duplicate it.

Solver::Solver(const Maze &maze) : maze_(maze), visits_(nullptr), solved_(false) {
    int n = maze_.getWidth() * maze_.getHeight();
    visits_ = new int[n];
    for (int i = 0; i < n; ++i) {
        visits_[i] = 0;
    }
}

Solver::~Solver() {
    delete[] visits_;
}

int Solver::idx(int x, int y) const {
    return y * maze_.getWidth() + x;
}

static const int DX[4] = {0, 1, 0, -1};
static const int DY[4] = {-1, 0, 1, 0};

bool WallFollower::solve() {
    int x = -1, y = -1, ex = -1, ey = -1;
    maze_.getStart(x, y);
    maze_.getExit(ex, ey);
    if (x < 0 || ex < 0) {
        return false;
    }

    int facing = 1;

    int maxSteps = maze_.getWidth() * maze_.getHeight() * 4;
    int steps = 0;

    visits_[idx(x, y)]++;
    path_.append(x, y);

    while (steps < maxSteps) {
        if (x == ex && y == ey) {
            solved_ = true;
            return true;
        }

        int tryOrder[4] = {
            firstTurn(facing),
            facing,
            lastResort(facing),
            (facing + 2) % 4
        };

        bool moved = false;
        for (int i = 0; i < 4; ++i) {
            int dir = tryOrder[i];
            int nx = x + DX[dir];
            int ny = y + DY[dir];
            if (nx < 0 || ny < 0 || nx >= maze_.getWidth() || ny >= maze_.getHeight()) {
                continue;
            }
            if (!maze_.isWall(nx, ny)) {
                x = nx;
                y = ny;
                facing = dir;
                visits_[idx(x, y)]++;
                path_.append(x, y);
                moved = true;
                break;
            }
        }

        if (!moved) {
            return false;
        }
        steps++;
    }

    return false;
}

int RightHandSolver::firstTurn(int facing) const {
    return (facing + 1) % 4;
}

int RightHandSolver::lastResort(int facing) const {
    return (facing + 3) % 4;
}

int LeftHandSolver::firstTurn(int facing) const {
    return (facing + 3) % 4;
}

int LeftHandSolver::lastResort(int facing) const {
    return (facing + 1) % 4;
}

void Solver::printSolved(std::ostream &os) const {
    int w = maze_.getWidth();
    int h = maze_.getHeight();
    int sx = -1, sy = -1, ex = -1, ey = -1;
    maze_.getStart(sx, sy);
    maze_.getExit(ex, ey);
    for (int row = 0; row < h; ++row) {
        for (int col = 0; col < w; ++col) {
            if (col == sx && row == sy) {
                os << 'S';
            } else if (col == ex && row == ey) {
                os << 'E';
            } else if (maze_.isWall(col, row)) {
                os << 'X';
            } else {
                int v = visits_[idx(col, row)];
                if (v > 0) {
                    os << (v > 9 ? 9 : v);
                } else {
                    os << ' ';
                }
            }
        }
        os << "\n";
    }
}

void Solver::printTrace(std::ostream &os) const {
    if (solved_) {
        os << "PATH FOUND\n";
    } else {
        os << "NO PATH FOUND\n";
    }
    path_.printTrace(os);
}