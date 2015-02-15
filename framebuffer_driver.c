#include "io.h"
#include "framebuffer_driver.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Number of rows/columns on a framebuffer. */
#define COLUMNS                 80
#define ROWS                    25

char *fb = (char *) 0x000B8000;

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}


/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position.
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

/** clear_screen:
 *  Clear the buffer.  Clearing is looping through all the rows/columns and
 *  setting char to ' ' and bg and fg to black.
 **/
void clear_screen()
{
    int i;
    for (i = 0; i < (ROWS * COLUMNS); i++)
    {
        fb_write_cell(i*2, ' ', 0, 0);
    }
}

/** write:
 *  Write a char buffer to screen.
 *
 *  @param buf The char buffer
 *  @param len How long the buffer is
 **/
void write(char *buf, unsigned int len)
{
    for (unsigned int i = 0; i < len; i++)
    {
        fb_write_cell(i*2, buf[i], 0, 2);
    }
}
