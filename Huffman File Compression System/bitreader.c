/*
 *
 *	bitreader.c file
 *
 */

#include "bitreader.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct BitReader {

    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

// ( done ) - ready to be checked
BitReader *bit_read_open(const char *filename) {

    BitReader *br = (BitReader *) calloc(1, sizeof(BitReader));
    if (br == NULL) {
        return NULL; // error handling
    }

    //open filename for reading for binary
    br->underlying_stream = fopen(filename, "rb");
    if (br->underlying_stream == NULL) {
        free(br);
        return NULL; // error handling
    }

    // assign values
    br->byte = 0;
    br->bit_position = 8;

    // error handling for byte and bit position
    if (br->bit_position != 8 || br->byte != 0) {
        return NULL;
    }

    return br;
}

// ( done ) - ready to be checked
void bit_read_close(BitReader **pbuf) {

    if (*pbuf != NULL) {
        if ((*pbuf)->underlying_stream != NULL) {
            fclose((*pbuf)->underlying_stream);
        }
        free(*pbuf);
        *pbuf = NULL;
    }
}

// ( done ) - ready to be checked
uint8_t bit_read_bit(BitReader *buf) {

    if (buf->bit_position > 7) {
        int next_byte = fgetc(buf->underlying_stream);
        if (next_byte == EOF) {
            return 0xFF;
        }
        buf->byte = (uint8_t) next_byte;
        buf->bit_position = 0;
    }

    // get the bit numbered bit_position from byte
    uint8_t bit = (buf->byte >> (buf->bit_position)) & 1;
    buf->bit_position += 1;

    return bit;
}

// ( done ) - ready to be checked
uint32_t bit_read_uint32(BitReader *buf) {
    uint32_t word = 0x00000000;

    for (int i = 0; i < 32; i++) {
        //read a bit b from the underlying_stream
        uint8_t bit = bit_read_bit(buf);
        if (bit == 0xFF) {
            return 0xFF;
        }

        // set bit i of word to the value of b
        word |= ((uint32_t) bit << i);
    }

    return word;
}

// ( done ) - ready to be checked
uint16_t bit_read_uint16(BitReader *buf) {

    uint16_t word = 0x0000;

    for (int i = 0; i < 16; i++) {
        //read a bit b from the underlying_stream
        uint8_t bit = bit_read_bit(buf);
        if (bit == 0xFF) {
            return 0xFF;
        }

        // set bit i of word to the value of b
        word |= (bit << i);
    }

    return word;
}

// ( done ) - ready to be checked
uint8_t bit_read_uint8(BitReader *buf) {

    uint8_t byte = 0x00;

    for (int i = 0; i < 8; i++) {
        //read a bit b from the underlying_stream
        uint8_t bit = bit_read_bit(buf);
        if (bit == 0xFF) {
            return 0xFF; // error handling
        }

        // set bit i of word to the value of b
        byte |= (bit << i);
    }

    return byte;
}
