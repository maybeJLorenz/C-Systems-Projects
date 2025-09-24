/*
 *
 *	dehuff.c file
 *
 */

#include "bitreader.h"
#include "node.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "i:o:h"
// stack push and pop functions

void stack_push(Node *array[], Node *node, int *top) {
    array[++(*top)] = node;
}

Node *stack_pop(Node *array[], int *top) {

    return array[(*top)--];
}

void dehuff_decompress_file(FILE *fout, const char *infile_name) {

    // opening input file
    FILE *infile = fopen(infile_name, "rb");
    if (!infile) {
        fprintf(stderr, "Error opening input file.\n");
        exit(1);
    }

    // creating bitreader
    BitReader *inbuf = bit_read_open(infile_name);
    if (!inbuf) {
        fprintf(stderr, "Error reading input file.\n");
        fclose(infile);
        exit(1);
    }

    // reading bits
    uint8_t type1 = bit_read_uint8(inbuf);
    uint8_t type2 = bit_read_uint8(inbuf);
    uint32_t filesize = bit_read_uint32(inbuf);
    uint16_t num_leaves = bit_read_uint16(inbuf);

    // asserting bits
    assert(type1 == 'H');
    assert(type2 == 'C');

    // calculating nodes
    int num_nodes = 2 * num_leaves - 1;

    //creating array of nodes
    Node *array[num_nodes];
    int top = -1;

    //Huffman tree
    for (int i = 0; i < num_nodes; i++) {
        uint8_t bit = bit_read_bit(inbuf);
        Node *node;
        if (bit == 1) {
            uint8_t symbol = bit_read_uint8(inbuf);
            node = node_create(symbol, 0);
        } else {
            node = node_create(0, 0);
            node->right = stack_pop(array, &top);
            node->left = stack_pop(array, &top);
        }
        stack_push(array, node, &top);
    }

    Node *code_tree = stack_pop(array, &top);

    // Decompressing the bits
    for (uint32_t i = 0; i < filesize; i++) {
        Node *node = code_tree;
        while (true) {
            uint8_t bit = bit_read_bit(inbuf);
            if (bit == 0) {
                node = node->left;
            } else {
                node = node->right;
            }

            // checks if node is a leaf
            if (node->left == NULL && node->right == NULL) {
                fwrite(&(node->symbol), sizeof(uint8_t), 1, fout);
                break;
            }
        }
    }

    // closing files
    bit_read_close(&inbuf);
    fclose(infile);

    //freeing memory
    node_free(&code_tree);
}

int main(int argc, char *argv[]) {

    bool iFile = false;
    bool oFile = false;

    FILE *outfile = stdout;

    char *infile_name;
    char *outfile_name;

    int opt;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            iFile = true;
            infile_name = optarg;
            break;

        case 'o':
            oFile = true;
            outfile_name = optarg;
            outfile = fopen(outfile_name, "wb");

            if (outfile == NULL) {
                fprintf(stderr, "Error opening outputfile.\n");
                exit(1);
            }
            break;
        case 'h':
            fprintf(stdout,
                "-i : Sets the name of the input file. Requires a filename as an argument.\n");
            fprintf(stdout,
                "-o : Sets the name of the output file. Requires a filename as an argument.\n");
            fprintf(stdout, "-h : Prints a help message to stdout.\n");

            return 0;

        default: fprintf(stderr, "dehuff: error in reading files.\b"); exit(1);
        }
    }

    if (iFile == false || oFile == false) {
        fprintf(stderr, "Files have no argument.\n");
        exit(1);
    }

    dehuff_decompress_file(outfile, infile_name);

    fclose(outfile);
    return 0;
}
