#include "frame.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(){
  int width = 100,
      height = 100;
  char img[100*100];
#define IMG(x,y) (img[y*width+x])
  int colours[width*height];
#define COLOURS(x,y) (colours[y*width+x])



  int p_x = 1,
      p_y = 1;


  initscr();
  start_color();
  init_pair(1,COLOR_GREEN,COLOR_BLACK);
  init_pair(2,COLOR_BLUE,COLOR_BLACK);
  init_color(COLOR_BLACK, 0, 0, 0);
  for(int i = 0; i < 100; i++){
    for(int j = 0; j < 100; j++){
      img[i*100+j] = '"';
      COLOURS(j,i) = 1;
    }
  }
  noecho();
  cbreak();
  refresh();
  Frame *f = mk_frame(0, 0, 100, 100, 10, 10, 0, 0, img, NULL);
  if(!f){
    fprintf(stderr, "ERROR: f == null\n");
    exit(1);
  }
  Frame *f1 = mk_frame(7, 7, 100, 100, 10, 10, 0, 0, img, colours);
  if(!f){
    fprintf(stderr, "ERROR: f == null\n");
    exit(1);
  }
  //fprintf(stderr, "DRAW_FRAME(...)\n");
  //fprintf(stderr, "MK_FRAME(...)\n");

  int in = 0;
  do{
    IMG(p_x,p_y) = '"';
    COLOURS(p_x,p_y) = 1;
    switch(in){
      case 'i':
        mvprintw(30,30,"%d",scroll_frame(f,0,-1));
        break;
      case 'k':
        mvprintw(30,30,"%d",scroll_frame(f,0,1));
        break;
      case 'j':
        mvprintw(30,30,"%d",scroll_frame(f,-1,0));
        break;
      case 'l':
        mvprintw(30,30,"%d",scroll_frame(f,1,0));
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
      default:
        break;
    }
    IMG(p_x,p_y) = '@';
    COLOURS(p_x,p_y) = 2;
    draw_frame(f);
    draw_frame(f1);
  } while((in = getch()) != 'q');


  endwin();

  return 0;
}
