#include "frame.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>


// Constructor
Frame* mk_frame(
    int fr_pos_x, int fr_pos_y,
    int fr_width, int fr_height,
    int fr_buf_width, int fr_buf_height,
    int fr_scr_x, int fr_scr_y,
    char *fr_image, int *fr_colour_map){


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

  int *colour_buffer = 0;
  if(fr_colour_map){
    colour_buffer = malloc(fr_buf_height*fr_buf_width*sizeof(int));
    if(!colour_buffer) return 0;
  }

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

  fr -> fr_colour_map = fr_colour_map;
  fr -> fr_colour_buffer = colour_buffer;
  fr -> fr_image = fr_image;
  fr -> fr_win = win;
  fr -> fr_buffer = buffer;

  return fr;
}

// Update the visible buffer.
int update_buffer(Frame *fr){
  if(!fr) return 0;
  int buf_w = fr->fr_buf_width,
      buf_h = fr->fr_buf_height,
      img_w = fr->fr_width,
      scr_x = fr->fr_scr_x,
      scr_y = fr->fr_scr_y,
      *cm = fr->fr_colour_map,
      *cb = fr->fr_colour_buffer;
  char *buf = fr->fr_buffer,
       *img = fr->fr_image;

  for(int r = 0; r < buf_h; r++){
    for(int c = 0; c < buf_w; c++){
      // "real" coords
      int rr = scr_y+r,
          rc = scr_x+c;
      buf[r*buf_w+c] = img[rr*img_w+rc];
      if(cm) cb[r*buf_w+c] = cm[rr*img_w+rc];
    }
  }
  return 1;
}

// Draw the visible buffer in the window.
int draw_frame(Frame *fr){
  if(!update_buffer(fr)) return 0;
  int ls = fr->fr_buf_height,
      cs = fr->fr_buf_width;
  int *cb = fr->fr_colour_buffer;
  char *buf = fr->fr_buffer;
  for(int l = 0; l < ls; l++){
    for(int c = 0; c < cs; c++){
      if(cb) wattron(fr->fr_win, COLOR_PAIR(cb[cs*l+c]));
      mvwprintw(fr->fr_win, l, c, "%c", buf[cs*l+c]);
      if(cb) wattroff(fr->fr_win, COLOR_PAIR(cb[cs*l+c]));
    }
  }
  wrefresh(fr->fr_win);
  return 1;
}

int scroll_frame(Frame *fr, int x, int y){
  int max_x = fr->fr_width,
      max_y = fr->fr_height,
      h = fr->fr_buf_height,
      w = fr->fr_buf_width,
      p_x = fr->fr_scr_x,
      p_y = fr->fr_scr_y,

      new_x = p_x + x,
      new_y = p_y + y;

  if(new_x < 0 || max_x < new_x + w || new_y < 0 || max_y < new_y + h)
    return 0;

  fr->fr_scr_x = new_x;
  fr->fr_scr_y = new_y;
  return 1;
}
