    #include "io.h"
    #include <stddef.h>

    /* VGA text-mode framebuffer */
    #define VGA_WIDTH   80
    #define VGA_HEIGHT  25
    #define VIDEO_MEM   ((volatile unsigned short*)0xB8000)
    #define DEFAULT_ATTR 0x07 /* light grey on black */

    /* The I/O ports */
    #define FB_COMMAND_PORT         0x3D4
    #define FB_DATA_PORT            0x3D5

    /* The I/O port commands */
    #define FB_HIGH_BYTE_COMMAND    14
    #define FB_LOW_BYTE_COMMAND     15

    /** fb_move_cursor:
     *  Moves the cursor of the framebuffer to the given position
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

    /* keep a software cursor to avoid needing inb; initialize to 0 */
    static unsigned short fb_cursor_pos = 0;

    static void fb_scroll_if_needed(void)
    {
        const unsigned int max_cells = VGA_WIDTH * VGA_HEIGHT;
        if (fb_cursor_pos < max_cells) return;

        /* scroll up one line */
        unsigned int i;
        for (i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; ++i) {
            VIDEO_MEM[i] = VIDEO_MEM[i + VGA_WIDTH];
        }

        /* clear last line */
        unsigned short blank = (DEFAULT_ATTR << 8) | ' ';
        for (i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < max_cells; ++i) {
            VIDEO_MEM[i] = blank;
        }

        fb_cursor_pos -= VGA_WIDTH;
    }

    /** write:
     *  Write len bytes from buf to VGA text-mode framebuffer. Handles '\n' and '\r'.
     *  Advances the cursor after each character and scrolls when needed.
     */
    int write(char *buf, unsigned int len)
    {
        unsigned int written = 0;
        unsigned int max_cells = VGA_WIDTH * VGA_HEIGHT;

        for (unsigned int i = 0; i < len; ++i) {
            unsigned char ch = (unsigned char)buf[i];

            if (ch == '\r') {
                /* carriage return -> move to start of line */
                fb_cursor_pos = (fb_cursor_pos / VGA_WIDTH) * VGA_WIDTH;
                continue; /* don't count as written output? still consumed from buffer */
            }

            if (ch == '\n') {
                /* newline -> move to start of next line */
                fb_cursor_pos = (fb_cursor_pos / VGA_WIDTH + 1) * VGA_WIDTH;
                fb_scroll_if_needed();
                fb_move_cursor(fb_cursor_pos);
                ++written;
                continue;
            }

            /* printable character: write and advance */
            if (fb_cursor_pos >= max_cells) {
                fb_scroll_if_needed();
            }

            VIDEO_MEM[fb_cursor_pos] = ((unsigned short)DEFAULT_ATTR << 8) | ch;
            ++fb_cursor_pos;
            fb_scroll_if_needed();
            fb_move_cursor(fb_cursor_pos);
            ++written;
        }

        return (int)written;
    }