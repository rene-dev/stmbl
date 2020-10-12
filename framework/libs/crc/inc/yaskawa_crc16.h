/**
 * \file
 * Functions and types for CRC checks.
 *
 * Generated on Thu Feb  8 00:22:27 2018
 * by pycrc v0.9.1, https://pycrc.org
 * using the configuration:
 *  - Width         = 16
 *  - Poly          = 0x1021
 *  - XorIn         = 0xffff
 *  - ReflectIn     = False
 *  - XorOut        = 0xffff
 *  - ReflectOut    = False
 *  - Algorithm     = table-driven
 *
 * This file defines the functions yaskawa_crc16_init(), yaskawa_crc16_update() and yaskawa_crc16_finalize().
 *
 * The yaskawa_crc16_init() function returns the inital \c crc value and must be called
 * before the first call to yaskawa_crc16_update().
 * Similarly, the yaskawa_crc16_finalize() function must be called after the last call
 * to yaskawa_crc16_update(), before the \c crc is being used.
 * is being used.
 *
 * The yaskawa_crc16_update() function can be called any number of times (including zero
 * times) in between the yaskawa_crc16_init() and yaskawa_crc16_finalize() calls.
 *
 * This pseudo-code shows an example usage of the API:
 * \code{.c}
 * yaskawa_crc16_t crc;
 * unsigned char data[MAX_DATA_LEN];
 * size_t data_len;
 *
 * crc = yaskawa_crc16_init();
 * while ((data_len = read_data(data, MAX_DATA_LEN)) > 0) {
 *     crc = yaskawa_crc16_update(crc, data, data_len);
 * }
 * crc = yaskawa_crc16_finalize(crc);
 * \endcode
 */
#ifndef YASKAWA_CRC16_H
#define YASKAWA_CRC16_H

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * The definition of the used algorithm.
 *
 * This is not used anywhere in the generated code, but it may be used by the
 * application code to call algorithm-specific code, if desired.
 */
#define CRC_ALGO_TABLE_DRIVEN 1


/**
 * The type of the CRC values.
 *
 * This type must be big enough to contain at least 16 bits.
 */
typedef uint_fast16_t yaskawa_crc16_t;


/**
 * Calculate the initial crc value.
 *
 * \return     The initial crc value.
 */
static inline yaskawa_crc16_t yaskawa_crc16_init(void) {
  return 0xffff;
}


/**
 * Update the crc value with new data.
 *
 * \param[in] crc      The current crc value.
 * \param[in] data     Pointer to a buffer of \a data_len bytes.
 * \param[in] data_len Number of bytes in the \a data buffer.
 * \return             The updated crc value.
 */
yaskawa_crc16_t yaskawa_crc16_update(yaskawa_crc16_t crc, const void *data, size_t data_len);


/**
 * Calculate the final crc value.
 *
 * \param[in] crc  The current crc value.
 * \return     The final crc value.
 */
static inline yaskawa_crc16_t yaskawa_crc16_finalize(yaskawa_crc16_t crc) {
  return crc ^ 0xffff;
}


#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /* YASKAWA_CRC16_H */
