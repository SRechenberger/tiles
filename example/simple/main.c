#include "../../src/frame.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(){
  srand(time(NULL));
  int width = 100,
      height = 100;
  char img[100*100],
       fg[100*100];
  char *fgs[1] = {fg};
#define FG(x,y) (fg[y*width+x])
#define IMG(x,y) (img[y*width+x])
  int colours[width*height];
#define COLOURS(x,y) (colours[y*width+x])

  Frame *frames[100];
  int fcnt = 0;


  int p_x = 1,
      p_y = 1;

  char borders[8] = "BOX";
  char b[] = "BOX";
  char tiles[8] = "+*~\".:,;";

  initscr();
  start_color();
  init_pair(1,COLOR_GREEN,COLOR_BLACK);
  init_pair(2,COLOR_BLUE,COLOR_BLACK);
  init_color(COLOR_BLACK, 0, 0, 0);
  for(int i = 0; i < 100; i++){
    for(int j = 0; j < 100; j++){
      img[i*100+j] = '"';
      FG(j,i) = 0;
      COLOURS(j,i) = 1;
    }
  }
  noecho();
  cbreak();
  refresh();
  Frame *f = mk_frame(0, 0, 100, 100, 80, 40, 0, 0, 1, 1, 1, fgs, img, colours, borders);
  if(!f){
    fprintf(stderr, "ERROR: f == null\n");
    exit(1);
  }
  //fprintf(stderr, "DRAW_FRAME(...)\n");
  //fprintf(stderr, "MK_FRAME(...)\n");

  int in = 0;
  do{
    FG(p_x,p_y) = 0;
    COLOURS(p_x,p_y) = 1;
    Frame *new;
    switch(in){
      case 'i':
        scroll_frame(f,0,-1);
        break;
      case 'k':
        scroll_frame(f,0,1);
        break;
      case 'j':
        scroll_frame(f,-1,0);
        break;
      case 'l':
        scroll_frame(f,1,0);
        break;
      case 'w':
        p_y--;
        break;
      case 's':
        p_y++;
        break;
      case 'a':
        p_x--;
        break;
      case 'd':
        p_x++;
        break;
      case 'v':
        new = splitv_frame(f, 60, PERCENT);
        if(new){
          frames[fcnt++] = new;
        }
        break;
      case 'h':
        new = splith_frame(f, 10, TILES);
        if(new){
          frames[fcnt++] = new;
        }
        break;
      case 'r':
        for(int i = 0; i < 100; i++){
          for(int j = 0; j < 100; j++){
            IMG(j,i) = tiles[rand()%8];
          }
        }
        break;
      default:
        break;
    }
    FG(p_x,p_y) = '@';
    COLOURS(p_x,p_y) = 2;
    draw_frame(f);
    for(int i = 0; i < fcnt; i++){
      draw_frame(frames[i]);
    }
  } while((in = getch()) != 'q');


  free_frame(f);
  for(int i = 0; i < fcnt; i++){
    free_frame(frames[i]);
  }
  endwin();

  return 0;
}
