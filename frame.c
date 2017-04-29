#include "frame.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>


Frame* mk_frame(
    int fr_pos_x, int fr_pos_y,
    int fr_width, int fr_height,
    int fr_buf_width, int fr_buf_height,
    int fr_scr_x, int fr_scr_y,
    char *fr_image){


  Frame *fr = malloc(sizeof(Frame));
  if(!fr){
    return 0;
  }


  WINDOW *win = newwin(fr_buf_height, fr_buf_width, fr_pos_y, fr_pos_x);
  if(!win){
    return 0;
  }

  char *buffer = malloc(fr_buf_height*fr_buf_width*sizeof(char));
  if(!buffer) return 0;


  *fr = (Frame) {
    fr_pos_x,
    fr_pos_y,
    fr_width,
    fr_height,
    fr_buf_width,
    fr_buf_height,
    fr_scr_x,
    fr_scr_y,
    0};

  fr -> fr_image = fr_image;
  fr -> fr_win = win;
  fr -> fr_buffer = buffer;

  return fr;
}

int update_buffer(Frame *fr){
  if(!fr) return 0;
  int buf_w = fr->fr_buf_width,
      buf_h = fr->fr_buf_height,
      img_w = fr->fr_width,
      scr_x = fr->fr_scr_x,
      scr_y = fr->fr_scr_y;
  char *buf = fr->fr_buffer,
       *img = fr->fr_image;

  for(int r = 0; r < buf_h; r++){
    for(int c = 0; c < buf_w; c++){
      // "real" coords
      int rr = scr_y+r,
          rc = scr_x+c;
      buf[r*buf_w+c] = img[rr*img_w+rc];
    }
  }
  return 1;
}

int draw_frame(Frame *fr){
  if(!update_buffer(fr)) return 0;
  int ls = fr->fr_buf_height,
      cs = fr->fr_buf_width;
  char *buf = fr->fr_buffer;
  for(int l = 0; l < ls; l++){
    mvwprintw(fr->fr_win, l, 0, "%.*s", cs, (char *) (buf+cs*l));
  }
  wrefresh(fr->fr_win);
  return 0;
}
