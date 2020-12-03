/* move generator driver program */
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "data.h"
#include "protos.h"

int DEPTH;

int usage(char *msg) {

    if (msg != NULL) fprintf(stderr, "error: %s\n", msg);
    fprintf(stderr, "usage: perft DEPTH\n");
    fprintf(stderr, "       DEPTH should be a positive integer\n");

    exit(1);
}

int main (int argc, char **argv) {

    if (argc != 2) usage("program expects one argument");
    if (sscanf(argv[1], "%d", &DEPTH) == 0) usage("invalid DEPTH argument");
    if (DEPTH < 1) usage(NULL);

    if(!parse_fen()) usage("invalid FEN format");

    printf("%ld\n", gen(1));

    return 0;
}
