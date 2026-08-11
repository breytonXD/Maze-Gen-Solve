# Report


## Video

YouTube: https://youtu.be/8vfKF13GB6E

Google Drive backup: https://drive.google.com/file/d/19JJNbaSPN6wIKs5x2H-xFbc0Nc28qhsk/view?usp=sharing


## C++ Interface (maze-final)

`maze-final` integrates generation and solving in a single binary with no text serialisation. A `Maze` object is generated in memory and passed directly to a `Solver` by `const` reference; the solver reads the maze through public accessors and never modifies it.

### Maze interface

The `Maze` class stores the grid as a char** 2D array with a destructor that frees every row. It is generated with the recursive division algorithm.

Public interface:

- `Maze(int w, int h)` — allocates the grid and fills it with the outer boundary walls.
- `void generate(int seed)` — runs recursive division from the given seed to carve the maze, then places the Start (`S`) on the left wall and Exit (`E`) on the right wall.
- `void print() const` — writes the maze as an ASCII grid to stdout.
- `static Maze* parseAscii(std::istream& is)` — builds a Maze from an existing ASCII grid read from a stream (used by maze-solve and maze-view).
- `static bool validate(std::istream& is)` — reads a maze from a stream and runs a flood fill to check it is a valid perfect maze, printing `VALID` or `INVALID` to stderr.
- `int getWidth() const` / `int getHeight() const` — return the grid dimensions.
- `bool isWall(int x, int y) const` — returns whether the cell at (x, y) is a wall. Used by the solver to navigate.
- `void getStart(int& x, int& y) const` / `void getExit(int& x, int& y) const` — return the coordinates of the Start and Exit cells.
- `char getCell(int x, int y) const` — returns the raw character at (x, y). Used by maze-view to choose which Minecraft block to place.

How recursive division works: the maze begins as an open rectangle bounded by outer walls. The algorithm picks a random line (preferring the longer axis), draws a wall across the region, then opens a single gap in that wall to keep both halves connected. It then recurses into each half, stopping when a region is too small to divide. This guarantees a perfect maze — exactly one path between any two cells, with no loops.

How --validate works: it flood-fills the open cells from the Start; any unreachable open cell means an isolated area. It then flood-fills the wall cells from a corner; any wall cell not reached means a wall region disconnected from the boundary, indicating a loop. Either failure makes the maze invalid.

### Solver interface

The solver is an abstract class hierarchy. `Solver` is the pure-virtual base; `WallFollower` holds the shared wall-following algorithm; `RightHandSolver` and `LeftHandSolver` extend it with their directional bias.

Public interface:

- `Solver(const Maze& maze)` — constructs the solver from a read-only reference to the maze. Because the maze is `const`, the solver cannot modify it. Any mutable working state (the per-cell visit counts) is allocated separately as an `int*` array inside the constructor.
- `virtual bool solve()` — pure virtual; runs the chosen algorithm and returns whether the exit was reached.
- `void printSolved(std::ostream&)` — writes the solved grid (visit-count digits overlaid on the maze) to the given stream.
- `void printTrace(std::ostream&)` — writes `PATH FOUND` / `NO PATH FOUND` followed by the coordinate trace.

Data flow: `maze-final` constructs a `Maze`, calls `generate(seed)`, then constructs a `RightHandSolver` or `LeftHandSolver` with the maze as a `const Maze&`. The solver queries the maze through `isWall(x, y)`, `getStart()`, `getExit()`, `getWidth()` and `getHeight()` — no ASCII is produced or parsed at any point. The visit trail is recorded in a custom `Path` linked list.

How code duplication is avoided: the entire wall-following loop lives once in `WallFollower::solve()`. The only difference between the two algorithms is two virtual hooks, `firstTurn()` and `lastResort()`, which return the preferred turn direction. `RightHandSolver` prefers right turns; `LeftHandSolver` prefers left. The shared loop calls these hooks without knowing which subclass it is running in.


## Benchmarks

Both approaches were timed over 200 iterations per maze size (seed 123). Dimensions must be odd, so sizes 11, 51, 101, 201 were used in place of 10/50/100/200. The metric reported is **user (CPU) time** — the time spent doing actual computation — rather than total wall-clock time, because wall-clock at these iteration counts is dominated by the fixed cost of launching the program 200 times, which is identical for both approaches and obscures the difference in the work itself.

| N (odd) | Pipeline CPU time | Integrated CPU time |
|---------|-------------------|---------------------|
| 11      | 0.32 s            | 0.01 s              |
| 51      | 0.75 s            | 0.01 s              |
| 101     | 2.81 s            | 0.02 s              |
| 201     | 6.23 s            | 0.04 s              |

### Why the integrated version is faster

The Unix pipeline pays a cost the integrated version does not. In Phase 1, `maze-gen` formats the entire maze as an ASCII grid, writes it through a pipe, and `maze-solve` reads that text back and re-parses it into a grid before solving. Every cell is converted to a character, transmitted, and parsed again. As the maze grows, this serialisation cost grows with the number of cells (N²).

`maze-final` removes all of it. The `Maze` object is handed to the `Solver` directly by `const` reference, so the solver works on the in-memory grid with no conversion to or from text. This is why the pipeline's CPU time climbs steeply with N (0.32 s → 6.23 s) while the integrated version stays almost flat (0.01 s → 0.04 s): the integrated version only does the essential generate-and-solve work, while the pipeline additionally formats, pipes, and re-parses the grid on every run.

A note on wall-clock time: measured as total elapsed time, the two approaches appear close, because both are dominated by per-process startup overhead at 200 iterations. CPU time isolates the actual computation and shows the true difference — the serialisation work the pipeline performs and the integrated version avoids.
