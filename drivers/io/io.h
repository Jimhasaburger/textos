#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/* 
 * Instead of <stddef.h>, define or use primitive types 
 * guaranteed to be the correct size across architectures.
 */
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;
typedef unsigned int   uint32_t;

/** outb:
 *  Sends the given data to the given I/O port. Defined in io.s
 *
 *  @param port The I/O port to send the data to
 *  @param data The data to send to the I/O port
 */
void outb(uint16_t port, uint8_t data);

/** write:
 *  Write `len` bytes from `buf` to the screen framebuffer.
 *  Advances the cursor after each character and scrolls when needed.
 *
 *  @param buf Pointer to data to write
 *  @param len Number of bytes to write
 *  @return Number of bytes written
 */
int write(const char *buf, uint32_t len);

#endif /* INCLUDE_IO_H */