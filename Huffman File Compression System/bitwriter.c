/*
 *
 *	bitwriter.c file
 *
 */

#include "bitwriter.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BitWriter {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

// ( done ) - ready to be checked
BitWriter *bit_write_open(const char *filename) {

    BitWriter *bw = (BitWriter *) calloc(1, sizeof(BitWriter));
    if (bw == NULL) {
        return NULL; // memory allocation fails
    }

    bw->underlying_stream = fopen(filename, "wb");

    if (bw->underlying_stream == NULL) {
        free(bw);
        return NULL; // failed to open file
    }

    // store <f>, byte = 0, and bit position = 0;
    bw->byte = 0;
    bw->bit_position = 0;

    return bw;
}

// ( done ) - ready to be checked
void bit_write_close(BitWriter **pbuf) {

    if (*pbuf != NULL) {
        if ((*pbuf)->bit_position > 0) {
            fputc((*pbuf)->byte, (*pbuf)->underlying_stream);
        }
        (*pbuf)->byte = 0;
        (*pbuf)->bit_position = 0;

        fclose((*pbuf)->underlying_stream);
        free(*pbuf);
        *pbuf = NULL;
    }
}

// ( done ) - ready to be checked
void bit_write_bit(BitWriter *buf, uint8_t bit) {

    if (buf->bit_position >= 8) {
        fputc(buf->byte, buf->underlying_stream);
        buf->byte = 0;
        buf->bit_position = 0;
    }

    // set the bit at bit_position of the byte to the
    // value of the bit

    buf->byte |= (bit << (buf->bit_position));
    buf->bit_position += 1;
}

// ( done ) - ready to be checked
void bit_write_uint16(BitWriter *buf, uint16_t x) {

    for (int i = 0; i < 16; i++) {
        bit_write_bit(buf, x & 1);
        x >>= 1;
    }
}

// ( done ) - ready to be checked
void bit_write_uint32(BitWriter *buf, uint32_t x) {

    for (int i = 0; i < 32; i++) {
        bit_write_bit(buf, x & 1);
        x >>= 1;
    }
}

// ( done ) - ready to be checked
void bit_write_uint8(BitWriter *buf, uint8_t x) {

    for (int i = 0; i < 8; i++) {
        bit_write_bit(buf, x & 1);
        x >>= 1;
    }
}
