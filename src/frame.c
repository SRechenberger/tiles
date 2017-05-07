#include "frame.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>


// Constructor
Frame* mk_frame(
    int fr_pos_x, int fr_pos_y,
    int fr_width, int fr_height,
    int fr_buf_width, int fr_buf_height,
    int fr_scr_x, int fr_scr_y,
    int fr_pad_v, int fr_pad_h,
    int fr_fg_layers, char **fr_foreground,
    char *fr_background, int *fr_colour_map, char *fr_borders){


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
    fr_pad_v,
    fr_pad_h,
    0};

  fr -> fr_colour_map = fr_colour_map;
  fr -> fr_colour_buffer = colour_buffer;
  fr -> fr_background = fr_background;
  fr -> fr_win = win;
  fr -> fr_buffer = buffer;
  fr -> fr_borders = fr_borders;
  fr -> fr_fg_layers = fr_fg_layers;
  fr -> fr_foreground = fr_foreground;

  return fr;
}

// Update the visible buffer.
int update_buffer(Frame *fr, bool *diff){
  if(!fr) return 0;
  int buf_w = fr->fr_buf_width,
      buf_h = fr->fr_buf_height,
      img_w = fr->fr_width,
      scr_x = fr->fr_scr_x,
      scr_y = fr->fr_scr_y,
      *cm = fr->fr_colour_map,
      *cb = fr->fr_colour_buffer,
      pad_h = fr->fr_pad_h,
      pad_v = fr->fr_pad_v;

  char *buf = fr->fr_buffer;
  int cnt = 0;
  for(int r = pad_h; r < buf_h-pad_h; r++){
    for(int c = pad_v; c < buf_w-pad_v; c++){
      // "real" coords
      int rr = scr_y+r,
          rc = scr_x+c,
          layer = fr->fr_fg_layers-1;

      while(layer >= 0){
        if(fr->fr_foreground[layer][rr*img_w+rc]) break;
        layer--;
      }

      char *img = layer >= 0
        ? fr->fr_foreground[layer]
        : fr->fr_background;

      if(buf[r*buf_w+c] != img[rr*img_w+rc]){
        diff[r*buf_w+c] = true;
        buf[r*buf_w+c] = img[rr*img_w+rc];
        cnt++;
      } else {
        diff[r*buf_w+c] = false;
      }
      if(cm) cb[r*buf_w+c] = cm[rr*img_w+rc];
    }
  }
  return cnt;
}

// Draw the visible buffer in the window.
int draw_frame(Frame *fr){
  int size = fr->fr_buf_height*fr->fr_buf_width,
      changes;
  bool diff[fr->fr_buf_height*fr->fr_buf_width];

  if(0 > (changes = update_buffer(fr, diff))) return 0;
  int ls = fr->fr_buf_height,
      cs = fr->fr_buf_width,
      pad_h = fr->fr_pad_h,
      pad_v = fr->fr_pad_v;
  int *cb = fr->fr_colour_buffer;
  char *buf = fr->fr_buffer,
       *brds = fr->fr_borders;
  for(int l = pad_h; l < ls-pad_h; l++){
    /* If more than half of the visible area has changed
     * and no colours are used,
     * then print linewise to the window;
     * otherwise print only those chararcters changed.
     */
    if(changes > size / 2 && !cb){
      mvwprintw(fr->fr_win, l, pad_v, "%.*s", cs-2*pad_v, buf+cs*l+pad_v);
    } else {
      for(int c = pad_v; c < cs-pad_v; c++){
        if(diff[cs*l+c]){
          if(cb) wattron(fr->fr_win, COLOR_PAIR(cb[cs*l+c]));
          mvwprintw(fr->fr_win, l, c, "%c", buf[cs*l+c]);
          if(cb) wattroff(fr->fr_win, COLOR_PAIR(cb[cs*l+c]));
        }
      }
    }
  }
  if(brds){
    if(!strcmp(brds,"BOX")){
      box(fr->fr_win, 0, 0);
    } else {
      wborder(fr->fr_win,
          brds[0], brds[1], brds[2], brds[3],
          brds[4], brds[5], brds[6], brds[7]);
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

int free_frame(Frame *fr){
  int d = delwin(fr->fr_win);
  free(fr->fr_colour_buffer);
  free(fr->fr_buffer);
  free(fr);
  return d != ERR;
}

Frame *splitv_frame(Frame *fr, int size, int unit){
  int w = fr->fr_buf_width;
  if(w <= size && unit == TILES) return 0;
  if(w < 2) return 0; // One of the resulting frames would have width < 0
  int wnew1;
  switch(unit){
    case PERCENT:
      wnew1 = (w*size) / 100;
      break;
    case TILES:
      wnew1 = size;
      break;
    default:
      return 0;
      break;
  }
  int wnew2 = w - wnew1;
  if(wnew2 <= 0) return 0;
  fr->fr_buf_width = wnew1;
  wclear(fr->fr_win);
  wrefresh(fr->fr_win);
  if(wresize(fr->fr_win, fr->fr_buf_height, wnew1) == ERR){
    return 0;
  }
  Frame *right = mk_frame(
      fr->fr_pos_x+wnew1,
      fr->fr_pos_y,
      fr->fr_width,
      fr->fr_height,
      wnew2,
      fr->fr_buf_height,
      fr->fr_scr_x,
      fr->fr_scr_y,
      fr->fr_pad_v,
      fr->fr_pad_h,
      fr->fr_fg_layers,
      fr->fr_foreground,
      fr->fr_background,
      fr->fr_colour_map,
      fr->fr_borders);
  return right;
}

Frame *splith_frame(Frame *fr, int size, int unit){
  int h = fr->fr_buf_height;
  if(h <= size && unit == TILES) return 0;
  if(h < 2) return 0; // One of the resulting frames would have width < 0
  int hnew1;
  switch(unit){
    case PERCENT:
      hnew1 = (h*size) / 100;
      break;
    case TILES:
      hnew1 = size;
      break;
    default:
      return 0;
      break;
  }
  int hnew2 = h - hnew1;
  if(hnew2 <= 0) return 0;
  fr->fr_buf_height = hnew1;
  wclear(fr->fr_win);
  wrefresh(fr->fr_win);
  if(wresize(fr->fr_win, hnew2, fr->fr_buf_width) == ERR){
    return 0;
  }
  Frame *right = mk_frame(
      fr->fr_pos_x,
      fr->fr_pos_y+hnew1,
      fr->fr_width,
      fr->fr_height,
      fr->fr_buf_width,
      hnew2,
      fr->fr_scr_x,
      fr->fr_scr_y,
      fr->fr_pad_v,
      fr->fr_pad_h,
      fr->fr_fg_layers,
      fr->fr_foreground,
      fr->fr_background,
      fr->fr_colour_map,
      fr->fr_borders);
  return right;
}

