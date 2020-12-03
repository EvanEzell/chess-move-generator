/*
 * Eazy Engine
 * File: data.c
 * Author: Evan Ezell
 *
 * Data definitions.
 */

#include "defs.h"

int side;
int xside;
int fifty;
int ply;
char castle;
past_move last_move;
int ep;
int piece[64];
int color[64];
int piece_square[5][10];

move move_stack[400];
int possible_moves;

BOOL slide[6] = { FALSE, FALSE, TRUE, TRUE, TRUE, FALSE };
int n_dir[6] = { 0, 8, 4, 4, 8, 8 };
int dir_offsets[6][8] = { 
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { -21, -19, -12, -8, 8, 12, 19, 21 },
    { -11, -9, 9, 11, 0, 0, 0, 0 },
    { -10, -1, 1, 10, 0, 0, 0, 0 },
    { -11, -10, -9, -1, 1, 9, 10, 11 },
    { -11, -10, -9, -1, 1, 9, 10, 11 }
};

char piece_char[7] = {'P', 'N', 'B', 'R', 'Q', 'K', '.'};

int castle_mask[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15, 
    15, 15, 15, 15, 15, 15, 15, 15, 
    15, 15, 15, 15, 15, 15, 15, 15, 
    15, 15, 15, 15, 15, 15, 15, 15, 
    15, 15, 15, 15, 15, 15, 15, 15, 
    15, 15, 15, 15, 15, 15, 15, 15, 
    13, 15, 15, 15, 12, 15, 15, 14 
};

int board120[120] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
    -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
    -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
    -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
    -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
    -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
    -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
    -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

int board64[64] = {
   21, 22, 23, 24, 25, 26, 27, 28,
   31, 32, 33, 34, 35, 36, 37, 38,
   41, 42, 43, 44, 45, 46, 47, 48,
   51, 52, 53, 54, 55, 56, 57, 58,
   61, 62, 63, 64, 65, 66, 67, 68,
   71, 72, 73, 74, 75, 76, 77, 78,
   81, 82, 83, 84, 85, 86, 87, 88,
   91, 92, 93, 94, 95, 96, 97, 98
};

int init_piece[64] = {
    ROOK, KNIGHT, BISHOP, QUEEN,  KING, BISHOP, KNIGHT,  ROOK,
    PAWN,   PAWN,   PAWN,  PAWN,  PAWN,   PAWN,   PAWN,  PAWN,
    EMPTY, EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY,
    EMPTY, EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY,
    EMPTY, EMPTY,  EMPTY, EMPTY, EMPTY, EMPTY,   EMPTY, EMPTY,
    EMPTY, EMPTY,  EMPTY, EMPTY, EMPTY, EMPTY,   EMPTY, EMPTY,
    PAWN,   PAWN,   PAWN,  PAWN,  PAWN,  PAWN,    PAWN,  PAWN,
    ROOK, KNIGHT, BISHOP, QUEEN,  KING, BISHOP, KNIGHT,  ROOK
};

int init_color[64] = {
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE
};
