#include "../../src/frame.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct TILE{
  char tile_char;
  int tile_color;
} tile;

void generate_map(int begin_index, int w, int h, int x, int y, tile* tile_map[h][w], char *map, int *cmap, int tiles, tile tile_set[tiles]){
  if(0 > x || x >= w || 0 > y || y >= h || tile_map[y][x]) return;

  // printf("(%d,%d)\n", x,y);

  int idx;
  if(rand() % 100 >= 90){
    tile_map[y][x] = &tile_set[idx = rand() % tiles];
  } else {
    tile_map[y][x] = &tile_set[idx = begin_index];
  }

  generate_map(idx, w, h, x-1, y, tile_map, map, cmap, tiles, tile_set);
  generate_map(idx, w, h, x, y+1, tile_map, map, cmap, tiles, tile_set);
  generate_map(idx, w, h, x+1, y, tile_map, map, cmap, tiles, tile_set);
  generate_map(idx, w, h, x, y-1, tile_map, map, cmap, tiles, tile_set);

  cmap[y*w+x] = tile_map[y][x] -> tile_color;
  map[y*w+x] = tile_map[y][x] -> tile_char;

  return;
}

int main(){
  int w = 30, h = 30;
  char map[w*h];  
  int cmap[w*h];

  tile *tile_map[h][w];

  tile tile_set[3] =
    { {'~', 1}
    , {'"', 2}
    , {'.', 3}};


  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      tile_map[i][j] = 0;
    }
  }

  //printf("GEN\n");
  generate_map(0, w, h, w/2, h/2, tile_map, map, cmap, 3, tile_set);
  //printf("GEN'd\n");

  for(int i = 0; i < 10; i++){
    printf("%c %d\n", map[i], cmap[i]);
  }
  initscr();
  start_color();
  init_color(COLOR_BLACK,0,0,0);
  init_pair(1,COLOR_BLUE, COLOR_BLACK);
  init_pair(2,COLOR_GREEN,COLOR_BLACK);
  init_pair(3,COLOR_YELLOW, COLOR_BLACK);
  
  //printf("MK_FRAME\n");
  Frame* f = mk_frame(0,0,w,h,w,h,0,0,0,0,0,NULL,map,cmap,"BOX");
  //printf("DRAW_FRAME\n");
  refresh();
  draw_frame(f);

  refresh();
  
  getch();
  free_frame(f);
  endwin();
}
