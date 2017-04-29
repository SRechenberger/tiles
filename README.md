# tiles
Wrapper around ncurses, to render a character arrays as tile maps, supporting multiple scrollable frames.

## Example

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

## Result

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

