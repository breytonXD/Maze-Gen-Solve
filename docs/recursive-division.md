# Recursive Division Algorithm

See also: [Walter Pullen's algorithm references](https://www.astrolog.org/labyrnth/algrithm.htm)

## How it works

1. **Initialise.** Create a rectangle with only the outer boundary walls; leave the entire interior open.
2. **Choose a direction.** Randomly pick horizontal or vertical. When the region is not square, prefer the axis along the longer dimension.
3. **Place a wall.** Draw the wall across the region along an **odd-numbered row or column** (1-indexed from the region boundary).
4. **Open a passage.** Remove one cell from that wall at an **even-numbered column or row** to ensure connectivity.
5. **Recurse.** Apply steps 2–4 to both sub-regions. Stop when a sub-region is only one cell wide or tall.
6. **Place Start and Exit.** Set `S` on the **left outer wall** adjacent to the first reachable interior passage (scanning top-to-bottom). Set `E` on the **right outer wall** adjacent to the first reachable interior passage (scanning bottom-to-top). Start and Exit must always be on **opposing sides**.

## Worked example (7×7)

Step 1 — initialise with outer walls only:
```text
xxxxxxx
x     x
x     x
x     x
x     x
x     x
xxxxxxx
```

Step 2 — choose Horizontal, select odd row 3:
```text
xxxxxxx
x     x
xxxxxxx
x     x
x     x
x     x
xxxxxxx
```

Step 3 — open a passage at even column 4:
```text
xxxxxxx
x     x
xxx xxx
x     x
x     x
x     x
xxxxxxx
```

Two sub-regions are now created. The upper region (one row tall) cannot be divided further. Recurse into the lower region.

Step 2–3 (lower region) — choose Vertical at column 3, open passage at even row 6:
```text
xxxxxxx
x     x
xxx xxx
x x   x
x x   x
x     x
xxxxxxx
```

Two sub-regions. The left cannot be divided further. Recurse into the right sub-region.

Step 2 (right sub-region) — choose Horizontal at row 5, place wall across right sub-region:
```text
xxxxxxx
x     x
xxx xxx
x x   x
x xxxxx
x     x
xxxxxxx
```

Step 3 (right sub-region) — open a passage at even column 6:
```text
xxxxxxx
x     x
xxx xxx
x x   x
x xxx x
x     x
xxxxxxx
```

Both remaining sub-regions are one cell wide — recursion ends.

Step 6 — place Start and Exit on opposing sides (left and right):
```text
xxxxxxx
S     x
xxx xxx
x x   x
x xxx x
x     E
xxxxxxx
```
