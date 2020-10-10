#pragma once

#include <string.h>

/**
 * Ringbuffer structure
 *
 */
struct ringbuf {
  char *buf;              ///< Pointer to buffer memory
  unsigned bufsize;       ///< Size of buffer memory
  volatile unsigned pos;  ///< Current read position
  volatile unsigned len;  ///< Length of data in buffer
};

#define RINGBUF(size) \
  { .buf = (char[(size)]){0}, .bufsize = (size) }

/**
 * Read a single byte from a buffer
 *
 * \param   rb    pointer to ringbuffer struct
 * \param   data  pointer to data byte
 * \return  number of bytes read (0 if buffer was empty)
 */
int rb_getc(struct ringbuf *rb, char *data);


/**
 * Write a single byte to a buffer
 *
 * \param   rb    pointer to ringbuffer struct
 * \param   data  pointer to data byte
 * \return  number of bytes written (0 if buffer was full)
 */
int rb_putc(struct ringbuf *rb, const char data);
/**
 * Read from a buffer
 *
 */
int rb_read(struct ringbuf *rb, void *data, int len);


/**
 * Write to a buffer
 *
 * \param   rb    pointer to ringbuffer struct
 * \param   data  pointer to data byte
 * \return  number of bytes written (0 if buffer was full)
 *
 */
int rb_write(struct ringbuf *rb, const void *data, int len);

/**
 * undo rb_read
 *
 * \param   rb    pointer to ringbuffer struct
 * \param   len   length to reset read pointer
 * \return  0 on error
 *
 */
int rb_undo(struct ringbuf *rb, int len);

int rb_getline(struct ringbuf *rb, char *ptr, int len);
