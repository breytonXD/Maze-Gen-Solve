# Testing

---

## `maze-gen` tests

### Breyton Gooden | s4158350

#### test_gen_basic
- Behaviour: generates a valid 7x7 maze using seed 42.
- Command: ./bin/maze-gen 7 7 42 > out.txt 2> err.txt
- Expected stdout: contents of tests/test_gen_basic.expected_out
- Expected stderr: contents of tests/test_gen_basic.expected_err

#### test_invalid_dimensions_small
- Behaviour: rejects dimensions too small to form a proper maze.
- Command: ./bin/maze-gen 2 2 42 > out.txt 2> err.txt
- Expected stdout: contents of tests/test_invalid_dimensions_small.expected_out
- Expected stderr: contents of tests/test_invalid_dimensions_small.expected_err

#### test_gen_missing_args
- Behaviour: rejects missing command-line arguments.
- Command: ./bin/maze-gen > out.txt 2> err.txt
- Expected stdout: contents of tests/test_gen_missing_args.expected_out
- Expected stderr: contents of tests/test_gen_missing_args.expected_err

#### test_gen_non_numeric
- Behaviour: rejects non-numeric width input.
- Command: ./bin/maze-gen abc 7 42 > out.txt 2> err.txt
- Expected stdout: contents of tests/test_gen_non_numeric.expected_out
- Expected stderr: contents of tests/test_gen_non_numeric.expected_err

#### test_gen_even_width
- Behaviour: rejects even numerical input.
- Command: ./bin/maze-gen 4 4 21 > out.txt 2> err.txt
- Expected stdout: contents of tests/test_gen_even_width.expected_out
- Expected stderr: contents of tests/test_gen_even_width.expected_err

#### test_gen_minimum_3x3
- Behaviour: generates smallest possible grid size.
- Command: ./bin/maze-gen 3 3 19 > out.txt 2> err.txt
- Expected stdout: contents of tests/test_gen_minimum_3x3.expected_out
- Expected stderr: contents of tests/test_gen_minimum_3x3.expected_err

#### test_gen_11x11
- Behaviour: generates a larger valid 11x11 maze using seed 99.
- Command: ./bin/maze-gen 11 11 99 > out.txt 2> err.txt
- Expected stdout: contents of tests/test_gen_11x11.expected_out
- Expected stderr: contents of tests/test_gen_11x11.expected_err

#### test_validate_generated_maze
- Behaviour: confirms a freshly generated maze is reported as a valid perfect maze.
- Command: ./bin/maze-gen 11 11 99 | ./bin/maze-gen --validate > out.txt 2> err.txt
- Expected stdout: contents of tests/test_validate_generated_maze.expected_out 
- Expected stderr: contents of tests/test_validate_generated_maze.expected_err 

#### test_validate_invalid_maze
- Behaviour: confirms that the maze that user attempted to make was invalid.
- Command: ./bin/maze-gen 11 12 99 | ./bin/maze-gen --validate > out.txt 2> err.txt
- Expected stdout: contents of tests/test_validate_generated_maze.expected_out 
- Expected stderr: contents of tests/test_validate_generated_maze.expected_err 

---

## `maze-solve` tests

### Leo Ali | s4216434

#### test_solve_basic
- Behaviour: solver reaches the exit on a 7x7 maze (seed 99) using the right-hand rule, overlaying visit counts on the path.
- Command: ./bin/maze-solve --alg right < tests/test_solve_basic.in > out.txt 2> err.txt
- Expected stdout: contents of tests/test_solve_basic.expected_out
- Expected stderr: contents of tests/test_solve_basic.expected_err

#### test_solve_left
- Behaviour: solves the same maze with the left-hand rule, producing a different valid path — confirms both wall-follower directions work through the shared WallFollower algorithm.
- Command: ./bin/maze-solve --alg left < tests/test_solve_left.in > out.txt 2> err.txt
- Expected stdout: contents of tests/test_solve_left.expected_out
- Expected stderr: contents of tests/test_solve_left.expected_err

#### test_solve_trapped
- Behaviour: start cell is sealed by walls with no adjacent passage; solver reports failure immediately.
- Command: ./bin/maze-solve --alg right < tests/test_solve_trapped.in > out.txt 2> err.txt
- Expected stdout: contents of tests/test_solve_trapped.expected_out
- Expected stderr: contents of tests/test_solve_trapped.expected_err (NO PATH FOUND, exit 1)

#### test_solve_unsolvable
- Behaviour: open maze with an unreachable exit; exercises the iteration bound (width x height x 4) that prevents infinite looping.
- Command: ./bin/maze-solve --alg right < tests/test_solve_unsolvable.in > out.txt 2> err.txt
- Expected stdout: contents of tests/test_solve_unsolvable.expected_out
- Expected stderr: contents of tests/test_solve_unsolvable.expected_err (NO PATH FOUND, exit 1)

#### test_solve_empty
- Behaviour: empty stdin; solver rejects missing input cleanly.
- Command: ./bin/maze-solve --alg right < tests/test_solve_empty.in > out.txt 2> err.txt
- Expected stdout: empty
- Expected stderr: contents of tests/test_solve_empty.expected_err (ERROR: empty maze input, exit 1)

## Termination bound

The solver never loops forever. WallFollower::solve() caps iterations at width x height x 4. A maze of W x H has at most W*H cells, and a wall-follower steps on each cell only a small constant number of times while hugging a wall and backtracking, so 4*W*H safely exceeds any genuine solution length while cutting off any non-terminating case. When the bound is hit the solver prints NO PATH FOUND to stderr and exits 1. test_solve_unsolvable drives the solver into this bound; test_solve_trapped is the related case where failure is detected immediately because the start has no open neighbour.
