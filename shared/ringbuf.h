#pragma once



/**
 * Ringbuffer structure
 *
 */
struct ringbuf {
    char *buf;          ///< Pointer to buffer memory
    unsigned   bufsize;      ///< Size of buffer memory
    volatile unsigned pos;   ///< Current read position
    volatile unsigned len;   ///< Length of data in buffer
};


/**
 * Read a single byte from a buffer
 *
 * \param   rb    pointer to ringbuffer struct
 * \param   data  pointer to data byte
 * \return  number of bytes read (0 if buffer was empty)
 */
static inline int rb_getc(struct ringbuf *rb, char *data)
{
    if (!rb->len)
        return 0;

    *data = rb->buf[rb->pos++];
	if (rb->pos >= rb->bufsize)
		rb->pos -= rb->bufsize;
	rb->len--;

    return 1;
}


/**
 * Write a single byte to a buffer
 *
 * \param   rb    pointer to ringbuffer struct
 * \param   data  pointer to data byte
 * \return  number of bytes written (0 if buffer was full)
 */
static inline int rb_putc(struct ringbuf *rb, const char data)
{
    if (rb->len >= rb->bufsize)
        return 0;

	unsigned i = rb->pos + rb->len;
	if (i >= rb->bufsize)
		i -= rb->bufsize;

	rb->buf[i] = data;
	rb->len++;

    return 1;
}


#if 0
/**
 * Read from a buffer
 *
 */
int rb_read(struct ringbuf *rb, void *data, int len)
{
    if (len > rb->len)
        len = rb->len;

    int len1 = len;
    if (rb->pos + len1 >= rb->bufsize) {
        int len2 = (rb->pos + len1) - rb->bufsize;
        len1 -= len2;
        memcpy((char*)data + len1, rb->buf, len2);
    }
    memcpy(data, rb->buf + rb->pos, len1);

    rb->len -= len;
    rb->pos += len;
    if (rb->pos > rb->bufsize)
        rb->pos -= rb->bufsize;

    return len;
}


/**
 * Write to a buffer
 *
 * \param   rb    pointer to ringbuffer struct
 * \param   data  pointer to data byte
 * \return  number of bytes written (0 if buffer was full)
 *
 */
int rb_write(struct ringbuf *rb, const void *data, int len)
{
    if (len > rb->bufsize - rb->len)
        len = rb->bufsize - rb->len;

    int len1 = len;
    if (rb->pos + rb->len + len1 >= rb->bufsize) {
        int len2 = (rb->pos + rb->len + len1) - rb->bufsize;
        len1 -= len2;
        memcpy(rb->buf, (char*)data + len1, len2);
    }

    memcpy(rb->buf + rb->pos + rb->len, data, len1);

    rb->len += len;
    return len;
}
#endif

