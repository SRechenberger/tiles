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
  int  *fr_colour_map;
                    /* Colour map for the image; dimensions should match those of fr_image
                     * if this is null, default terminal colours are used.
                     * The value of each cell is an indexed colour pair, defined by
                     * init_pair()
                     * */
  int  *fr_colour_buffer;
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
    char *fr_image,
    int  *colour_map);

int draw_frame(Frame *fr);

int scroll_frame(Frame *fr, int x, int y);

#endif
