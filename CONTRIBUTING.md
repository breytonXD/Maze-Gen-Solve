# Team Contributions

## Members

| Student | Student ID | GitHub Username | Responsibilities |
|---|---|---|---|
| Breyton Gooden | s4158350 | breytonXD | maze-gen, Maze class, maze-view lead |
| Leo Ali | s4216434 | 105923814 | maze-solve, solver hierarchy, maze-final lead  |

Note: GitHub username 105923814 corresponds to Leo Ali.

## Individual Contributions

### Breyton Gooden | s4158350

I implemented:

- The maze generation: The maze-gen.cpp file handles the command line inputs, ensuring the inputs are correct. If something wrong is detected, an error message is printed in the command and a 1 is returned. If valid, a maze object is generated to the specifications of the width, height and seed that was input. The recursive algorithm then generates the maze within the walls. 
- The maze-view: The maze-view.cpp file is responsible for translating the ascii maze into our minecraft world, with every ascii character having a corresponding minecraft representation. The program interacts with the minecraft game via the mcpp library.  
- The maze class: I was responsible for implementing most of the maze class. This included the class values and the constructor, destructor, maze generation, print, parse-ascii and validation functions.

### Leo Ali | s4216434

I implemented the solver side of the project:

- The abstract `Solver` class hierarchy (`Solver` → `WallFollower` → `RightHandSolver` / `LeftHandSolver`), with the shared wall-following algorithm written once in `WallFollower` and the two solvers differing only in their `firstTurn`/`lastResort` directional hooks — avoiding code duplication.
- The custom `Path` linked list that records the solver's coordinate trace, with a destructor that frees the chain and zero memory leaks.
- `maze-solve` (reads a maze from stdin, selects the algorithm via `--alg`, routes the solved grid to stdout and the trace to stderr).
- Led the Phase 2 integration in `maze-final`, generating and solving in memory via the `const Maze&` contract, with output byte-identical to the pipeline.
- Added the read-only accessors to `Maze` that the solver needs, wrote the `maze-solve` black-box tests and their TESTING.md section, ran the Phase 1 vs Phase 2 benchmarks, and documented the Solver interface and benchmarks in REPORT.md.