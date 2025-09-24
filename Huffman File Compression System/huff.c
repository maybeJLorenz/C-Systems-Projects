/*
 *
 *	huff.c file
 *
 */

#include "bitreader.h"
#include "bitwriter.h"
#include "node.h"
#include "pq.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

typedef struct Code {
    uint64_t code;
    uint8_t code_length;
} Code;

void huff_write_tree(BitWriter *outbuf, Node *node) {

    if (node->left == NULL) {
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, node->symbol);
    } else {
        huff_write_tree(outbuf, node->left);
        huff_write_tree(outbuf, node->right);
        bit_write_bit(outbuf, 0);
    }
}

uint32_t fill_histogram(FILE *fin, uint32_t *histogram) {

    uint32_t filesize = 0;
    int byte;

    //clearing all elements of hist. array
    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }

    //reading bytes with fgetc()
    while ((byte = fgetc(fin)) != EOF) {
        histogram[byte]++;
        ++filesize;
    }
    ++histogram[0x00];
    ++histogram[0xff];

    return filesize;
}

Node *create_tree(uint32_t *histogram, uint16_t *num_leaves) {

    PriorityQueue *pq = pq_create();

    *num_leaves = 0;

    for (int i = 0; i < 256; i++) {
        if (histogram[i] != 0) {
            // creating new node for each non-zeroi
            Node *new_node = node_create((uint8_t) i, histogram[i]);
            enqueue(pq, new_node);
            (*num_leaves)++;
        }
    }

    //Huffman coding algorithm
    while (pq_size_is_1(pq) == false) {
        Node *left = dequeue(pq);
        Node *right = dequeue(pq);

        // new node with combined weight
        Node *new_node = node_create(0, left->weight + right->weight);
        new_node->left = left;
        new_node->right = right;

        // enqueue new node
        enqueue(pq, new_node);
    }

    // dequeue the queue's only entry, return huffman tree
    Node *r = dequeue(pq);
    pq_free(&pq);
    return r;
}

void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) {

    //if (code_table == 0 || code_length == 0) {
    if (node->left != NULL && node->right != NULL) {
        // append a 0 to code and recurse
        fill_code_table(code_table, node->left, code, code_length + 1);

        // append a 1 to code and recurse
        code |= (uint64_t) 1 << code_length;
        fill_code_table(code_table, node->right, code, code_length + 1);

    } else {
        code_table[node->symbol].code = code;
        code_table[node->symbol].code_length = code_length;
    }
}

void huff_compress_file(BitWriter *outbuf, FILE *fin, uint32_t filesize, uint16_t num_leaves,
    Node *code_tree, Code *code_table) {

    uint64_t code;
    uint8_t code_length;

    bit_write_uint8(outbuf, 'H');
    bit_write_uint8(outbuf, 'C');
    bit_write_uint32(outbuf, filesize);
    bit_write_uint16(outbuf, num_leaves);

    huff_write_tree(outbuf, code_tree);
    int infile;
    while (true) {

        infile = fgetc(fin);

        if (infile == EOF) {
            break;
        }

        code = code_table[infile].code;
        code_length = code_table[infile].code_length;

        for (uint8_t i = 0; i < code_length; i++) {

            bit_write_bit(outbuf, (uint8_t) code & 1);
            code >>= 1;
        }
    }
}

#define OPTIONS "i:o:h"

int main(int argc, char *argv[]) {
    FILE *infile = stdin;
    FILE *outfile = stdout;

    bool iFile = false;
    bool oFile = false;

    char *infile_name;
    char *outfile_name;

    int opt;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            iFile = true;

            //sets name of input file
            infile_name = optarg;
            infile = fopen(infile_name, "rb");

            // error handling
            if (infile == NULL) {
                fprintf(stderr, "Error opening input file.\n");
                exit(1);
            }

            break;

        case 'o':
            oFile = true;

            //sets name of output file
            outfile_name = optarg;
            outfile = fopen(outfile_name, "wb");

            //error handling
            if (outfile == NULL) {
                fprintf(stderr, "Error opening output file.\n");
                exit(1);
            }
            break;

        case 'h':
            // prints a help message to stdout
            fprintf(stdout,
                "-i : Sets the name of the input file. Requires a filename as an argument.\n");
            fprintf(stdout,
                "-o : Sets the name of the output file. Requires a filename as an argument.\n");
            fprintf(stdout, "-h : Prints a help message to stdout.\n");
            return 0;

        default: fprintf(stderr, "huff : error in reading files.\n"); exit(1);
        }
    }

    if (iFile == false || oFile == false) {
        fprintf(stderr, "Files have no argument.\n");
        exit(1);
    }

    // ( 1 ) read file and count frequency of each symbol
    uint32_t histogram[256];
    uint32_t size = fill_histogram(infile, histogram);

    // ( 2 ) create a code tree from the histogram
    uint16_t num_leaves;
    Node *n = create_tree(histogram, &num_leaves);

    // ( 3 ) fill a 256-entry code table, one entry for each byte value
    Code code_table[256];
    fill_code_table(code_table, n, 0, 0);

    // ( 4 ) rewind the input file using fseek() in prep for the next step
    fseek(infile, 0L, SEEK_SET);

    // ( 5 ) Create a Huffman Coded output file from the input file
    BitWriter *output_file = bit_write_open(outfile_name);
    huff_compress_file(output_file, infile, size, num_leaves, n, code_table);
    node_free(&n);
    bit_write_close(&output_file);
    fclose(outfile);
    fclose(infile);
    return 0;
}
