# tiles
Wrapper around ncurses windows, to render character arrays as tile maps, supporting multiple scrollable frames.

## Example
### Image

    +------------ TILEMAP ----------+
    |                               |
    |  +---- FRAME 1 ----+          |
    |  | 1               |    4     |
    |  |                 |          |
    |  |            +-- FRAME 2 --+ |
    |  |            | 2           | |
    |  |            |             | |
    |  +------------|             | |
    |               |            3| |
    |      5        +-------------+ |
    |                               |
    +-------------------------------+

### Result

      +---- FRAME 1 ----+
      | 1               |
      |                 |
      |                 |
      |              2  |
      |                 |
      +-----------------+

      +-- FRAME 2 --+
      | 2           |
      |             |
      |             |
      |            3|
      +-------------+

## Current Version
0.0.3
