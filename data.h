/*
 * Eazy Engine
 * File: data.h
 * Author: Evan Ezell
 *
 * Data declarations.
 */

extern int DEPTH;

extern int side;
extern int xside;
extern int ep;
extern int fifty;
extern int ply;
extern char castle;
extern past_move last_move;

extern move move_stack[400];
extern int possible_moves;

extern int board64[64];
extern int board120[120];

extern int piece[64];
extern BOOL slide[6];
extern int piece_square[5][10];
extern char piece_char[7];

extern int n_dir[6];
extern int dir_offsets[6][8];

extern int color[64];
extern int castle_mask[64];

extern int init_piece[64];
extern int init_color[64];
