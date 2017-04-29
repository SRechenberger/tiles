#include "frame.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(){
  char img[100*100];

  srand(time(NULL));
  for(int i = 0; i < 100; i++){
    for(int j = 0; j < 100; j++){
      img[i*100+j] = '.';
    }
  }

  int p_x = 1,
      p_y = 1;


  initscr();
  noecho();
  cbreak();
  refresh();
  Frame *f = mk_frame(0, 0, 100, 100, 10, 10, 0, 0, img);
  if(!f){
    fprintf(stderr, "ERROR: f == null\n");
    exit(1);
  }
  Frame *f1 = mk_frame(11, 11, 100, 100, 10, 10, 1, 1, img);
  if(!f){
    fprintf(stderr, "ERROR: f == null\n");
    exit(1);
  }
  //fprintf(stderr, "DRAW_FRAME(...)\n");
  //fprintf(stderr, "MK_FRAME(...)\n");

  int in;
  while((in = getch()) != 'q'){
    img[p_y*100+p_x]='.';
    switch(in){
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
    img[p_y*100+p_x] = '@';
    draw_frame(f);
    draw_frame(f1);
  }


  endwin();

  return 0;
}
