    #ifndef INCLUDE_IO_H
    #define INCLUDE_IO_H

    #include <stddef.h>

    /** outb:
     *  Sends the given data to the given I/O port. Defined in io.s
     *
     *  @param port The I/O port to send the data to
     *  @param data The data to send to the I/O port
     */
    void outb(unsigned short port, unsigned char data);

    /** write:
     *  Write `len` bytes from `buf` to the screen framebuffer.
     *  Advances the cursor after each character and scrolls when needed.
     *
     *  @param buf Pointer to data to write
     *  @param len Number of bytes to write
     *  @return Number of bytes written
     */
    int write(char *buf, unsigned int len);

    #endif /* INCLUDE_IO_H */