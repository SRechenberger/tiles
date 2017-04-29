#ifndef __FRAME_H__
#define __FRAME_H__

#include <ncurses.h>

/*******
 *
 *  +--------IMAGE---------------+
 *  |                            |
 *  |  +-----FRAME--------+      |
 *  |  |                  |      |
 *  |  |                  |      |
 *  |  |                  |      |
 *  |  |                  |      |
 *  |  |                  |      |
 *  |  |                  |      |
 *  |  +------------------+      |
 *  |                            |
 *  +----------------------------+
 *
 */

typedef struct FRAME {
  int fr_pos_x;     // X-Position of the Frame on the screen.
  int fr_pos_y;     // Y-Position of the Frame on the screen.
  int fr_width;     // Width of the Image.
  int fr_height;    // Height of the Image.
  int fr_buf_width; // Width of the Frame.
  int fr_buf_height;// Height of the Frame.
  int fr_scr_x;     // X-Scroll-Position.
  int fr_scr_y;     // Y-Scroll-Position.
  WINDOW *fr_win;   // The ncurses window, finally displaying the Frame.
  char *fr_image;   // The Image.
  char *fr_buffer;  // The Buffer of the Frame.
} Frame;

Frame* mk_frame(
    int fr_pos_x,
    int fr_pos_y,
    int fr_width,
    int fr_height,
    int fr_buf_width,
    int fr_buf_height,
    int fr_scr_x,
    int fr_scr_y,
    char *fr_image);

int draw_frame(Frame *fr);

#endif
