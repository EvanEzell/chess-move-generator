#include <stdio.h>
#include "defs.h"
#include "data.h"

int topiecenumber(char c) {
    if (c == 'p' || c == 'P') return PAWN;
    if (c == 'n' || c == 'N') return KNIGHT;
    if (c == 'b' || c == 'B') return BISHOP;
    if (c == 'r' || c == 'R') return ROOK;
    if (c == 'q' || c == 'Q') return QUEEN;
    if (c == 'k' || c == 'K') return KING;
    return EMPTY;
}

int topiececolor(char c) {
    if (c == 'p' || c == 'n' || c == 'b' ||
        c == 'r' || c == 'q' || c == 'k')
        return BLACK;
    if (c == 'P' || c == 'N' || c == 'B' ||
        c == 'R' || c == 'Q' || c == 'K')
        return WHITE;
    return EMPTY;
}

int is_valid(char c) {

    if (c >= '1' && c <= '8') return 1;

    switch (c) {
        case 'p':
            return 1;
        case 'n':
            return 1;
        case 'b':
            return 1;
        case 'r':
            return 1;
        case 'q':
            return 1;
        case 'k':
            return 1;
        case 'P':
            return 1;
        case 'N':
            return 1;
        case 'B':
            return 1;
        case 'R':
            return 1;
        case 'Q':
            return 1;
        case 'K':
            return 1;
        default:
            return 0;
    }
}

BOOL parse_fen() {

    char c;
    int rank, file, i;

    /* parse piece placement string */
    for (rank = 7; rank >= 0; rank--) {
        for (file = 0; file <= 7; file++) {
            if(is_valid(c = getchar())) {
                if (c >= '1' && c <= '8') {
                    for(i = 0; i < c - '0'; i++) {
                        piece[(7-rank)*8+file] = EMPTY;
                        color[(7-rank)*8+file] = EMPTY;
                        file++;
                    }
                    file--;
                } else {
                    piece[(7-rank)*8+file] = topiecenumber(c); 
                    color[(7-rank)*8+file] = topiececolor(c); 
                }
            } else {
                fprintf(stderr, "Invalid piece (use [0-8pnbrqkPNBRQK]).\n");
                return FALSE;
            }
        }
        if (file != 8) {
            fprintf(stderr, "Too many pieces on rank.\n");
            return FALSE;
        }
        if (rank > 0 && getchar() != '/') {
            fprintf(stderr, "Invalid rank seperator (use '/').\n");
            return FALSE;
        }
    }

    if (getchar() != ' ') {
        fprintf(stderr, "Expecting space between piece ");
        fprintf(stderr, "placement string and active color.\n");
        return FALSE;
    }

    /* parse active color */
    if ((c = getchar()) == 'w') side = WHITE;
    else if (c == 'b') side = BLACK;
    else {
        fprintf(stderr, "Invalid active color (use [wb]).\n");
        return FALSE;
    }
    xside = 1 ^ side;

    if (getchar() != ' ') {
        fprintf(stderr, "Expecting space between active ");
        fprintf(stderr, "color and castling rights.\n");
        return FALSE;
    }

    c = getchar();

    /* parse castling availability */
    castle = 0;
    do {
        switch (c) {
            case 'K':
                castle |= 1;
                break;
            case 'Q':
                castle |= 2;
                break;
            case 'k':
                castle |= 4;
                break;
            case 'q':
                castle |= 8;
                break;
            case '-':
                if (!castle) break;
            default:
                fprintf(stderr, "Invalid castling rights string ");
                fprintf(stderr, "use '-' or one or more [KQkq]).\n");
                return FALSE;
            }
    } while ((c = getchar()) != ' ');

    /* parse en passant square */
    if ((c = getchar()) == '-') {
        ep = -1;
    } else {
        if (c < 'a' || c > 'h') {
            fprintf(stderr, "Invalid en passant file (must be a - h).\n");
            return FALSE;
        }

        file = c - 'a';
        c = getchar();

        if (c < '1' || c > '8') {
            fprintf(stderr, "Invalid en passant rank (must be 1 - 8).\n");
            return FALSE;
        }

        rank = c - '1';
        ep = (7-rank)*8+file;
    }

    if (getchar() != ' ') {
        fprintf(stderr, "Expecting space between en ");
        fprintf(stderr, "passant square and halfmove clock.\n");
        return FALSE;
    }

    /* parse halfmove clock */
    if (scanf("%d", &fifty) != 1 || fifty < 0) {
        fprintf(stderr, "Invalid halfmove clock. ");
        fprintf(stderr, "Must be a non-negative integer.\n");
        return FALSE;
    }
    
    if (getchar() != ' ') {
        fprintf(stderr, "Expecting space between halfmove clock ");
        fprintf(stderr, "and fullmove number.\n");
        return FALSE;
    }

    /* parse fullmove number */
    if (scanf("%d", &ply) != 1 || ply <= 0) {
        fprintf(stderr, "Invalid fullmove number. ");
        fprintf(stderr, "Must be a positive integer.\n");
        return FALSE;
    }
    
    ply *= 2;
    ply -= (side == BLACK) ? 1 : 2;

    getchar(); /* eat newline */

    if (getchar() != EOF) {
        fprintf(stderr, "EOF expected after fullmove number.\n");
        return FALSE;
    }

}
