/**
 * \file crc8.h
 * Functions and types for CRC checks.
 *
 * Generated on Wed Dec  2 02:28:13 2015,
 * by pycrc v0.8.3, https://pycrc.org
 * using the configuration:
 *    Width        = 8
 *    Poly         = 0x31
 *    XorIn        = 0x00
 *    ReflectIn    = True
 *    XorOut       = 0x00
 *    ReflectOut   = True
 *    Algorithm    = table-driven
 *****************************************************************************/
#ifndef __CRC8_H__
#define __CRC8_H__

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * The definition of the used algorithm.
 *
 * This is not used anywhere in the generated code, but it may be used by the
 * application code to call algoritm-specific code, is desired.
 *****************************************************************************/
#define CRC_ALGO_TABLE_DRIVEN 1


/**
 * The type of the CRC values.
 *
 * This type must be big enough to contain at least 8 bits.
 *****************************************************************************/
typedef uint_fast8_t crc8_t;


/**
 * Reflect all bits of a \a data word of \a data_len bytes.
 *
 * \param data         The data word to be reflected.
 * \param data_len     The width of \a data expressed in number of bits.
 * \return             The reflected data.
 *****************************************************************************/
crc8_t crc8_reflect(crc8_t data, size_t data_len);


/**
 * Calculate the initial crc value.
 *
 * \return     The initial crc value.
 *****************************************************************************/
static inline crc8_t crc8_init(void)
{
    return 0x00;
}


/**
 * Update the crc value with new data.
 *
 * \param crc      The current crc value.
 * \param data     Pointer to a buffer of \a data_len bytes.
 * \param data_len Number of bytes in the \a data buffer.
 * \return         The updated crc value.
 *****************************************************************************/
crc8_t crc8_update(crc8_t crc, const void *data, size_t data_len);


/**
 * Calculate the final crc value.
 *
 * \param crc  The current crc value.
 * \return     The final crc value.
 *****************************************************************************/
static inline crc8_t crc8_finalize(crc8_t crc)
{
    return crc ^ 0x00;
}


#ifdef __cplusplus
}           /* closing brace for extern "C" */
#endif

#endif      /* __CRC8_H__ */
